/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "logging.h"
#include "mediadiscoverybackend.h"
#include "usbdevice.h"

#include <QDir>
#include <QTimer>
#include <QtDebug>

MediaDiscoveryBackend::MediaDiscoveryBackend(QObject *parent)
    : QIviMediaDeviceDiscoveryModelBackendInterface(parent)
{
    m_deviceFolder = QDir::homePath() + "/usb-simulation";
    const QByteArray customDeviceFolder = qgetenv("QTIVIMEDIA_SIMULATOR_DEVICEFOLDER");
    if (customDeviceFolder.isEmpty())
        qCCritical(media) << "QTIVIMEDIA_SIMULATOR_DEVICEFOLDER environment variable is not set, falling back to:" << m_deviceFolder;
    else
        m_deviceFolder = customDeviceFolder;

    const QDir deviceFolder(m_deviceFolder);
    const QStringList folders = deviceFolder.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folder : folders) {
        qCDebug(media) << "Adding USB Device for: " << folder;
        m_deviceMap.insert(folder, new USBDevice(deviceFolder.absoluteFilePath(folder)));
    }
}

void MediaDiscoveryBackend::initialize()
{
#ifndef QT_NO_FILESYSTEMWATCHER
    m_watcher.addPath(m_deviceFolder);
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &MediaDiscoveryBackend::onDirectoryChanged);
#endif

    emit availableDevices(m_deviceMap.values());
    emit initializationDone();
}

QMap<QString, QIviServiceObject*> MediaDiscoveryBackend::deviceMap() const
{
    return m_deviceMap;
}

void MediaDiscoveryBackend::onDirectoryChanged(const QString &path)
{
    Q_UNUSED(path)
    QDir deviceFolder(m_deviceFolder);

    //Check for removed Devices
    for (auto i = m_deviceMap.cbegin(), end = m_deviceMap.cend(); i != end; ++i) {
        const QString &folder = i.key();
        if (!deviceFolder.exists(folder)) {
            qCDebug(media) << "Removing USB Device for: " << folder;
            QIviServiceObject *device = m_deviceMap.take(folder);
            emit deviceRemoved(device);
            emit mediaDirectoryRemoved(deviceFolder.absoluteFilePath(folder));
        }
    }

    //Check for newly added Devices
    const QStringList folders = deviceFolder.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folder : folders) {
        if (m_deviceMap.contains(folder))
            continue;

        qCDebug(media) << "Adding USB Device for: " << folder;
        USBDevice *device = new USBDevice(deviceFolder.absoluteFilePath(folder));
        m_deviceMap.insert(folder, device);
        emit deviceAdded(device);
        const QString absFolder = deviceFolder.absoluteFilePath(folder);
        // If we point the simulation to a real mount location, give the mount some time to actually make
        // the files accessible
        QTimer::singleShot(2000, this, [this, absFolder](){emit mediaDirectoryAdded(absFolder);});
    }
}
