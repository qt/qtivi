/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#include "qivimediadevicediscoverymodel.h"
#include "qivimediadevicediscoverymodel_p.h"

#include <QtIviCore/QIviServiceObject>
#include <QtIviMedia/QIviMediaDevice>
#include <QtDebug>

QIviMediaDeviceDiscoveryModelPrivate::QIviMediaDeviceDiscoveryModelPrivate(const QString &interface, QIviMediaDeviceDiscoveryModel *parent)
    : QIviAbstractFeatureListModelPrivate(interface, parent)
    , q_ptr(parent)
{
}

void QIviMediaDeviceDiscoveryModelPrivate::init()
{
    Q_Q(QIviMediaDeviceDiscoveryModel);

    QObject::connect(q, &QAbstractListModel::rowsInserted,
                     q, &QIviMediaDeviceDiscoveryModel::countChanged);
    QObject::connect(q, &QAbstractListModel::rowsRemoved,
                     q, &QIviMediaDeviceDiscoveryModel::countChanged);
    QObject::connect(q, &QAbstractListModel::modelReset,
                     q, &QIviMediaDeviceDiscoveryModel::countChanged);
}

void QIviMediaDeviceDiscoveryModelPrivate::clearToDefaults()
{
    qDeleteAll(m_deviceList);
    m_deviceList.clear();
}

void QIviMediaDeviceDiscoveryModelPrivate::resetModel(const QList<QIviServiceObject*> deviceList)
{
    Q_Q(QIviMediaDeviceDiscoveryModel);
    q->beginResetModel();
    qDeleteAll(m_deviceList);
    m_deviceList.clear();
    m_deviceList = deviceList;
    q->endResetModel();
}

void QIviMediaDeviceDiscoveryModelPrivate::onDeviceAdded(QIviServiceObject *device)
{
    QIviMediaDevice *mdevice = qobject_cast<QIviMediaDevice*>(device);
    if (!mdevice) {
        qWarning() << "Couldn't cast added Device to QIviMediaDevice. Aborting here.";
        return;
    }

    Q_Q(QIviMediaDeviceDiscoveryModel);
    q->beginInsertRows(QModelIndex(), m_deviceList.count(), m_deviceList.count());
    m_deviceList += device;
    q->endInsertRows();

    q->deviceAdded(mdevice);
}

void QIviMediaDeviceDiscoveryModelPrivate::onDeviceRemoved(QIviServiceObject *device)
{
    QIviMediaDevice *mdevice = qobject_cast<QIviMediaDevice*>(device);
    if (!mdevice) {
        qWarning() << "Couldn't cast removed Device to QIviMediaDevice. Aborting here.";
        return;
    }

    Q_Q(QIviMediaDeviceDiscoveryModel);
    int index = m_deviceList.indexOf(device);
    if (index == -1) {
        qWarning() << "Failed to remove the Device. Couldn't find" << device << "in the list of available devices";
        return;
    }
    q->beginRemoveRows(QModelIndex(), index, index);
    m_deviceList.takeAt(index);
    q->endRemoveRows();

    q->deviceRemoved(mdevice);

    delete device;
}

QIviMediaDeviceDiscoveryModelBackendInterface *QIviMediaDeviceDiscoveryModelPrivate::discoveryBackend() const
{
    Q_Q(const QIviMediaDeviceDiscoveryModel);
    QIviServiceObject *so = q->serviceObject();
    if (so)
        return qobject_cast<QIviMediaDeviceDiscoveryModelBackendInterface*>(so->interfaceInstance(QIviStringMediaDeviceDiscoveryInterfaceName));

    return nullptr;
}

QIviMediaDeviceDiscoveryModel::QIviMediaDeviceDiscoveryModel(QObject *parent)
    : QIviAbstractFeatureListModel(*new QIviMediaDeviceDiscoveryModelPrivate(QIviStringMediaDeviceDiscoveryInterfaceName, this), parent)
{
    Q_D(QIviMediaDeviceDiscoveryModel);
    d->init();
}

int QIviMediaDeviceDiscoveryModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIviMediaDeviceDiscoveryModel);
    if (parent.isValid())
        return 0;

    return d->m_deviceList.count();
}

QVariant QIviMediaDeviceDiscoveryModel::data(const QModelIndex &index, int role) const
{
    Q_D(const QIviMediaDeviceDiscoveryModel);
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= d->m_deviceList.count() || row < 0)
        return QVariant();

    QIviMediaDevice *item = qobject_cast<QIviMediaDevice*>(d->m_deviceList.at(row));
    if (!item)
        return QVariant();

    switch (role) {
        case NameRole: return item->name();
        case TypeRole: return item->type();
        case ItemRole: return QVariant::fromValue(item);
    }

    return QVariant();
}

QVariantMap QIviMediaDeviceDiscoveryModel::get(int i) const
{
    QVariantMap map;
    map[QLatin1String("name")] = data(index(i,0), NameRole);
    map[QLatin1String("type")] = data(index(i,0), TypeRole);
    map[QLatin1String("item")] = data(index(i,0), ItemRole);

    return map;
}

QHash<int, QByteArray> QIviMediaDeviceDiscoveryModel::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[ItemRole] = "item";
    }
    return roles;
}

QIviMediaDeviceDiscoveryModel::QIviMediaDeviceDiscoveryModel(QIviMediaDeviceDiscoveryModelPrivate &dd, QObject *parent)
    : QIviAbstractFeatureListModel(dd, parent)
{
    Q_D(QIviMediaDeviceDiscoveryModel);
    d->init();
}

bool QIviMediaDeviceDiscoveryModel::acceptServiceObject(QIviServiceObject *serviceObject)
{
    return serviceObject->interfaces().contains(QIviStringMediaDeviceDiscoveryInterfaceName);
}

void QIviMediaDeviceDiscoveryModel::connectToServiceObject(QIviServiceObject *serviceObject)
{
    Q_UNUSED(serviceObject)
    Q_D(QIviMediaDeviceDiscoveryModel);

    QIviMediaDeviceDiscoveryModelBackendInterface *backend = d->discoveryBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIviMediaDeviceDiscoveryModelBackendInterface::availableDevices,
                            d, &QIviMediaDeviceDiscoveryModelPrivate::resetModel);
    QObjectPrivate::connect(backend, &QIviMediaDeviceDiscoveryModelBackendInterface::deviceAdded,
                            d, &QIviMediaDeviceDiscoveryModelPrivate::onDeviceAdded);
    QObjectPrivate::connect(backend, &QIviMediaDeviceDiscoveryModelBackendInterface::deviceRemoved,
                            d, &QIviMediaDeviceDiscoveryModelPrivate::onDeviceRemoved);

    backend->initialize();
}

void QIviMediaDeviceDiscoveryModel::disconnectFromServiceObject(QIviServiceObject *serviceObject)
{
    QIviMediaDeviceDiscoveryModelBackendInterface *backend = qobject_cast<QIviMediaDeviceDiscoveryModelBackendInterface*>(serviceObject->interfaceInstance(QIviStringMediaDeviceDiscoveryInterfaceName));

    if (backend)
        disconnect(backend, 0, this, 0);
}

void QIviMediaDeviceDiscoveryModel::clearServiceObject()
{
    Q_D(QIviMediaDeviceDiscoveryModel);
    d->clearToDefaults();
}

#include "moc_qivimediadevicediscoverymodel.cpp"
