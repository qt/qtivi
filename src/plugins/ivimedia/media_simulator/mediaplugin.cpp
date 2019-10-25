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
#include "mediaindexerbackend.h"
#include "mediaplayerbackend.h"
#include "mediaplugin.h"
#include "searchandbrowsebackend.h"
#include "usbdevice.h"
#include "database_helper.h"

#include <QtIviCore/QIviSearchAndBrowseModel>
#include <QtIviMedia/QIviMediaPlayer>

#include <QCoreApplication>
#include <QStringList>
#include <QtDebug>

MediaSimulatorPlugin::MediaSimulatorPlugin(QObject *parent)
    : QObject(parent)
    , m_discovery(new MediaDiscoveryBackend(this))
{
    QString dbFile = mediaDatabaseFile();

    createMediaDatabase(dbFile);

    m_player = new MediaPlayerBackend(createDatabaseConnection(QStringLiteral("player"), dbFile), this);
    m_browse = new SearchAndBrowseBackend(createDatabaseConnection(QStringLiteral("model"), dbFile), this);
    m_indexer = new MediaIndexerBackend(createDatabaseConnection(QStringLiteral("indexer"), dbFile), this);

    auto deviceMap = m_discovery->deviceMap();
    for (auto it = deviceMap.cbegin(); it != deviceMap.cend(); it++) {
        USBDevice *device = qobject_cast<USBDevice*>(it.value());
        if (!device)
            continue;
        m_indexer->addMediaFolder(device->folder());
    }

    QObject::connect(m_indexer, &MediaIndexerBackend::removeFromQueue,
                     m_player, &MediaPlayerBackend::remove);
    QObject::connect(m_discovery, &MediaDiscoveryBackend::mediaDirectoryAdded,
                     m_indexer, &MediaIndexerBackend::addMediaFolder);
    QObject::connect(m_discovery, &MediaDiscoveryBackend::mediaDirectoryRemoved,
                     m_indexer, &MediaIndexerBackend::removeMediaFolder);

}

QStringList MediaSimulatorPlugin::interfaces() const
{
    QStringList list;
    list << QStringLiteral(QIviMediaPlayer_iid);
    list << QStringLiteral(QIviSearchAndBrowseModel_iid);
    list << QStringLiteral(QIviMediaDeviceDiscovery_iid);
    list << QStringLiteral(QIviMediaIndexer_iid);
    return list;
}

QIviFeatureInterface *MediaSimulatorPlugin::interfaceInstance(const QString &interface) const
{
    if (interface == QStringLiteral(QIviMediaPlayer_iid))
        return m_player;
    else if (interface == QStringLiteral(QIviSearchAndBrowseModel_iid))
        return m_browse;
    else if (interface == QStringLiteral(QIviMediaDeviceDiscovery_iid))
        return m_discovery;
    else if (interface == QStringLiteral(QIviMediaIndexer_iid))
        return m_indexer;

    return nullptr;
}
