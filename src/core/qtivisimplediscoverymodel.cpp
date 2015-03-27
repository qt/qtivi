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

template <class T> QtIVISimpleDiscoveryModel<T>::QtIVISimpleDiscoveryModel(QString interface, QObject *parent)
    : QAbstractListModel(parent)
    , m_interface(interface)
{
    m_roles.insert(ServiceObject, "serviceObject");

    // Initialize the list of properties this instance of the model knows about
    for(int propertyIndex = 0; propertyIndex < T::staticMetaObject.propertyCount(); propertyIndex++)
    {
        QMetaProperty property = T::staticMetaObject.property(propertyIndex);
        const int roleNumber = Qt::UserRole + 1 + propertyIndex;
        if(property.isEnumType())
        {
            m_enumProperties.insert(roleNumber);
        }
        m_roles.insert(roleNumber, property.name());
    }

    connect(this, &QtIVISimpleDiscoveryModel::rowsInserted, this, &QtIVISimpleDiscoveryModel::countChanged);
    connect(this, &QtIVISimpleDiscoveryModel::rowsRemoved, this, &QtIVISimpleDiscoveryModel::countChanged);
    connect(this, &QtIVISimpleDiscoveryModel::layoutChanged, this, &QtIVISimpleDiscoveryModel::countChanged);
    connect(this, &QtIVISimpleDiscoveryModel::modelReset, this, &QtIVISimpleDiscoveryModel::countChanged);
}

template <class T> QtIVISimpleDiscoveryModel<T>::~QtIVISimpleDiscoveryModel()
{

}

template <class T> QHash<int, QByteArray> QtIVISimpleDiscoveryModel<T>::roleNames() const
{
    return m_roles;
}

template <class T> int QtIVISimpleDiscoveryModel<T>::rowCount(const QModelIndex &parent) const
{
     return parent.isValid() ? 0 : m_serviceObjects.count();
}

template <class T> QVariant QtIVISimpleDiscoveryModel<T>::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= 0 && row < m_serviceObjects.count()) {
        QtIVIServiceObject* so= serviceObjectAt(row);

        if (role == ServiceObject)
            return QVariant::fromValue<QtIVIServiceObject*>(so);

        QObject* instance = so->interfaceInstance(m_interface);
        if (!instance)
            return QVariant();

        T* obj = qobject_cast<T*>(instance);
        if(!obj)
            return QVariant();

        QVariant ret = obj->property(m_roles.value(role));
        if(m_enumProperties.contains(role))
        {
            ret = ret.toInt();
        }
        return ret;
    }

    return QVariant();
}

template <class T> T *QtIVISimpleDiscoveryModel<T>::at(int index)
{
    QtIVIServiceObject* so = serviceObjectAt(index);
    if (!so)
        return 0;

    QObject* instance = so->interfaceInstance(m_interface);
    if (!instance)
        return 0;

    T* obj = qobject_cast<T*>(instance);
    if(!obj)
        return 0;

    return obj;
}

template <class T> QtIVIServiceObject *QtIVISimpleDiscoveryModel<T>::serviceObjectAt(int index)
{
    if (index >= 0 && index < m_serviceObjects.count()) {
        return m_serviceObjects.at(index);
    }

    return 0;
}

template <class T> void QtIVISimpleDiscoveryModel<T>::updateServiceObjects(QList<QtIVIServiceObject *> objects)
{
    //Do a logic here to call update for as many buckets as possible
}

template <class T> void QtIVISimpleDiscoveryModel<T>::addServiceObjects(QList<QtIVIServiceObject *> objects)
{
    beginInsertRows(QModelIndex(), m_serviceObjects.count(), m_serviceObjects.count());
    m_serviceObjects += objects;
    endInsertRows();
}

template <class T> void QtIVISimpleDiscoveryModel<T>::removeServiceObjects(QList<QtIVIServiceObject *> objects)
{
    //FIXME
    beginRemoveRows(QModelIndex(), m_serviceObjects.count(), m_serviceObjects.count());
    m_serviceObjects -= objects;
    endRemoveRows();
}

