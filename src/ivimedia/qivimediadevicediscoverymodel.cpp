/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

QT_BEGIN_NAMESPACE

QIviMediaDeviceDiscoveryModelPrivate::QIviMediaDeviceDiscoveryModelPrivate(const QString &interface, QIviMediaDeviceDiscoveryModel *parent)
    : QIviAbstractFeatureListModelPrivate(interface, parent)
    , q_ptr(parent)
{
}

void QIviMediaDeviceDiscoveryModelPrivate::initialize()
{
    QIviAbstractFeatureListModelPrivate::initialize();
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

void QIviMediaDeviceDiscoveryModelPrivate::resetModel(const QList<QIviServiceObject*> &deviceList)
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

    emit q->deviceAdded(mdevice);
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

    emit q->deviceRemoved(mdevice);

    delete device;
}

QIviMediaDeviceDiscoveryModelBackendInterface *QIviMediaDeviceDiscoveryModelPrivate::discoveryBackend() const
{
    return backend<QIviMediaDeviceDiscoveryModelBackendInterface*>();
}

/*!
    \class QIviMediaDeviceDiscoveryModel
    \inmodule QtIviMedia
    \brief Provides a model for discovering media devices.

    The QIviMediaDeviceDiscoveryModel provides a way to query for available media devices and
    to get notified when new media devices get added or are removed.

    All devices listed here need to be a subclass of QIviMediaDevice.

    The QIviMediaDeviceDiscoveryModel expects a single backend to be available. It is recommended to use it
    with \l {QIviAbstractFeatureListModel::}{discoveryMode} set to \l QIviAbstractFeature::AutoDiscovery.
*/

/*!
    \qmltype MediaDeviceDiscoveryModel
    \instantiates QIviMediaDeviceDiscoveryModel
    \inqmlmodule QtIvi.Media
    \inherits AbstractFeatureListModel
    \brief Provides a model for discovering media devices.

    The MediaDeviceDiscoveryModel provides a way to query for available media devices and
    to get notified when new media devices get added or are removed.

    All devices listed here need to be a subclass of MediaDevice.

    The following roles are available in this model:

    \table
    \header
        \li Role name
        \li Type
        \li Description
    \row
        \li \c name
        \li string
        \li The name of the media device. E.g. The name of the connected USB-Thumbdrive/SDCard or a connected Ipod.
    \row
        \li \c type
        \li string
        \li The type of the media device. See \l SupportedMediaDevices for a detailed listing.
    \row
        \li \c serviceObject
        \li QIviMediaDevice
        \li The Media Device. This object be used as ServiceObject for other Features. E.g. The SearchAndBrowseModel.
    \endtable


    The MediaDeviceDiscoveryModel expects a single backend to be available. It is recommended to use it
    with \l {AbstractFeatureListModel::}{discoveryMode} set to AbstractFeature.AutoDiscovery.
*/

/*!
    \enum QIviMediaDeviceDiscoveryModel::Roles
    \value NameRole
          The name of the media device. E.g. The name of the connected USB-Thumbdrive/SDCard or a connected Ipod.
    \value TypeRole
          The type of the media device. See \l SupportedMediaDevices for a detailed listing.
    \value ServiceObjectRole
          A pointer to the media device itself. This pointer can be used as the ServiceObject for other Features. E.g. The QIviSearchAndBrowseModel.
*/

/*!
    Constructs a QIviMediaDeviceDiscoveryModel.

    The \a parent argument is passed on to the \l QIviAbstractFeatureListModel base class.
*/
QIviMediaDeviceDiscoveryModel::QIviMediaDeviceDiscoveryModel(QObject *parent)
    : QIviAbstractFeatureListModel(*new QIviMediaDeviceDiscoveryModelPrivate(QLatin1String(QIviMediaDeviceDiscovery_iid), this), parent)
{
}

/*!
    \qmlproperty int MediaDeviceDiscoveryModel::count
    \brief Holds the current number of rows in this model.
*/
/*!
    \property QIviMediaDeviceDiscoveryModel::count
    \brief Holds the current number of rows in this model.
*/
int QIviMediaDeviceDiscoveryModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIviMediaDeviceDiscoveryModel);
    if (parent.isValid())
        return 0;

    return d->m_deviceList.count();
}

/*!
    \reimp
*/
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
        case ServiceObjectRole: return QVariant::fromValue(item);
    }

    return QVariant();
}

/*!
    \qmlmethod MediaDevice MediaDeviceDiscoveryModel::get(i)

    Returns the media devices at index \a i.

    \note The returned device is owned by the model and can be deleted at any time.
    If stored in a property or a var, this could lead to a dangling pointer.
*/
/*!
    Returns the media device at index \a i.

    \note The returned device is owned by the model and can be deleted at any time.
*/
QIviMediaDevice *QIviMediaDeviceDiscoveryModel::get(int i) const
{
    Q_D(const QIviMediaDeviceDiscoveryModel);
    if (i >= d->m_deviceList.count() || i < 0)
        return nullptr;

    QIviMediaDevice *item = qobject_cast<QIviMediaDevice*>(d->m_deviceList.at(i));
    return item;
}

/*!
    Returns the media device at index \a i.

    \note The returned device is owned by the model and can be deleted at any time.
*/
QIviMediaDevice *QIviMediaDeviceDiscoveryModel::at(int i) const
{
    return get(i);
}

/*!
    \reimp
*/
QHash<int, QByteArray> QIviMediaDeviceDiscoveryModel::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[ServiceObjectRole] = "serviceObject";
    }
    return roles;
}

/*!
    \internal
*/
QIviMediaDeviceDiscoveryModel::QIviMediaDeviceDiscoveryModel(QIviMediaDeviceDiscoveryModelPrivate &dd, QObject *parent)
    : QIviAbstractFeatureListModel(dd, parent)
{
}

/*!
    \reimp
*/
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

    QIviAbstractFeatureListModel::connectToServiceObject(serviceObject);

    backend->initialize();
}

/*!
    \reimp
*/
void QIviMediaDeviceDiscoveryModel::clearServiceObject()
{
    Q_D(QIviMediaDeviceDiscoveryModel);
    d->clearToDefaults();
}

/*!
    \fn void QIviMediaDeviceDiscoveryModel::deviceAdded(QIviMediaDevice *device)

    This signal is emitted whenever a new media device got added. The new media device is passed as \a device.
*/

/*!
    \qmlsignal MediaDeviceDiscoveryModel::deviceAdded(MediaDevice device)

    This signal is emitted whenever a new media device got added. The new media device is passed as \a device.
*/

/*!
    \fn void QIviMediaDeviceDiscoveryModel::deviceRemoved(QIviMediaDevice *device)

    This signal is emitted whenever a media device got removed. The device which got removed is passed as \a device.
    Afterwards the device will be deleted.
*/

/*!
    \qmlsignal MediaDeviceDiscoveryModel::deviceRemoved(MediaDevice device)

    This signal is emitted whenever a media device got removed. The device which got removed is passed as \a device.
    Afterwards the device will be deleted.
*/

QT_END_NAMESPACE

#include "moc_qivimediadevicediscoverymodel.cpp"
