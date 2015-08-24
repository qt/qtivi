/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#include "qtivisimplediscoverymodel.h"

#include "qtiviserviceobject.h"

QtIVISimpleDiscoveryModelBase::QtIVISimpleDiscoveryModelBase(QString interface, QObject *parent)
    : QAbstractListModel(parent)
    , m_interface(interface)
{    
    connect(this, &QtIVISimpleDiscoveryModelBase::rowsInserted, this, &QtIVISimpleDiscoveryModelBase::countChanged);
    connect(this, &QtIVISimpleDiscoveryModelBase::rowsRemoved, this, &QtIVISimpleDiscoveryModelBase::countChanged);
    connect(this, &QtIVISimpleDiscoveryModelBase::layoutChanged, this, &QtIVISimpleDiscoveryModelBase::countChanged);
    connect(this, &QtIVISimpleDiscoveryModelBase::modelReset, this, &QtIVISimpleDiscoveryModelBase::countChanged);
}

QtIVISimpleDiscoveryModelBase::~QtIVISimpleDiscoveryModelBase()
{
}

QHash<int, QByteArray> QtIVISimpleDiscoveryModelBase::roleNames() const
{
    return m_roles;
}

int QtIVISimpleDiscoveryModelBase::rowCount(const QModelIndex &parent) const
{
     return parent.isValid() ? 0 : m_serviceObjects.count();
}

QVariant QtIVISimpleDiscoveryModelBase::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= 0 && row < m_serviceObjects.count()) {
        QtIVIServiceObject* so = serviceObjectAt(row);

        if (role == ServiceObject)
            return QVariant::fromValue<QtIVIServiceObject*>(so);

        QObject* instance = so->interfaceInstance(m_interface);
        if (!instance)
            return QVariant();

        /* TODO is this casting really needed? The QObject property system works anyway, doesn't it?
        T* obj = qobject_cast<T*>(instance);
        if(!obj)
            return QVariant();
        */

        QVariant ret = /*obj*/ instance->property(m_roles.value(role));
        if(m_enumProperties.contains(role))
        {
            ret = ret.toInt();
        }
        return ret;
    }

    return QVariant();
}

QtIVIServiceObject *QtIVISimpleDiscoveryModelBase::serviceObjectAt(int index) const
{
    if (index >= 0 && index < m_serviceObjects.count()) {
        return m_serviceObjects.at(index);
    }

    return 0;
}

void QtIVISimpleDiscoveryModelBase::updateServiceObjects(QList<QtIVIServiceObject *> objects)
{
    Q_UNUSED(objects)
    //Do a logic here to call update for as many buckets as possible
}

void QtIVISimpleDiscoveryModelBase::addServiceObjects(QList<QtIVIServiceObject *> objects)
{
    beginInsertRows(QModelIndex(), m_serviceObjects.count(), m_serviceObjects.count());
    m_serviceObjects += objects;
    endInsertRows();
}

void QtIVISimpleDiscoveryModelBase::removeServiceObjects(QList<QtIVIServiceObject *> objects)
{
    //FIXME
    beginRemoveRows(QModelIndex(), m_serviceObjects.count(), m_serviceObjects.count());
    foreach(QtIVIServiceObject *obj, objects)
        m_serviceObjects.removeAll(obj);
    endRemoveRows();
}

