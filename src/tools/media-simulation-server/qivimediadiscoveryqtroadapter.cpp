/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
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

#include "qivimediadiscoveryqtroadapter.h"
#include "qivisearchandbrowsemodelqtroadapter.h"
#include "core.h"

#include <QIviMediaDevice>
#include <QRemoteObjectRegistryHost>

QIviMediaDiscoveryModelQtRoAdapter::QIviMediaDiscoveryModelQtRoAdapter(MediaDiscoveryBackend *parent)
    : QIviMediaDiscoveryModelSource (parent)
    , m_backend(parent)
{
    QMap<QString, QIviServiceObject*> deviceMap = m_backend->deviceMap();

    for (auto it = deviceMap.cbegin(); it != deviceMap.cend(); it++) {
        QIviMediaDevice *mediaDevice = qobject_cast<QIviMediaDevice *>(it.value());
        if (mediaDevice)
            createDeviceAdapter(mediaDevice);
    }

    connect(m_backend, &MediaDiscoveryBackend::deviceAdded, this, &QIviMediaDiscoveryModelQtRoAdapter::onDeviceAdded);
    connect(m_backend, &MediaDiscoveryBackend::deviceRemoved, this, &QIviMediaDiscoveryModelQtRoAdapter::onDeviceRemoved);
}

QStringList QIviMediaDiscoveryModelQtRoAdapter::devices() const
{
    return QStringList(m_hostMap.keys());
}

void QIviMediaDiscoveryModelQtRoAdapter::onDeviceAdded(QIviServiceObject *device)
{
    QIviMediaDevice *mediaDevice = qobject_cast<QIviMediaDevice *>(device);
    if (!mediaDevice)
        return;

    createDeviceAdapter(mediaDevice);
    emit deviceAdded(mediaDevice->name());
}

void QIviMediaDiscoveryModelQtRoAdapter::onDeviceRemoved(QIviServiceObject *device)
{
    QIviMediaDevice *mediaDevice = qobject_cast<QIviMediaDevice *>(device);
    if (!mediaDevice)
        return;

    emit deviceRemoved(mediaDevice->name());
    auto instance =  m_hostMap.take(mediaDevice->name());

    qDebug() << "Removing USB Instance" << mediaDevice->name();
    Core::instance()->host()->disableRemoting(instance);
    delete instance;
}

void QIviMediaDiscoveryModelQtRoAdapter::createDeviceAdapter(QIviMediaDevice *device)
{
    qDebug() << "Adding USB Instance" << device->name();

    QIviSearchAndBrowseModelInterface *searchAndBrowseBackend = qivi_interface_cast<QIviSearchAndBrowseModelInterface *>(device->interfaceInstance(QStringLiteral(QIviSearchAndBrowseModel_iid)));

    searchAndBrowseBackend->initialize();
    auto instance = new QIviSearchAndBrowseModelQtRoAdapter(searchAndBrowseBackend, QStringLiteral("QIviSearchAndBrowseModel_") + device->name());
    Core::instance()->host()->enableRemoting<QIviSearchAndBrowseModelAddressWrapper>(instance);

    m_hostMap.insert(device->name(), instance);
}
