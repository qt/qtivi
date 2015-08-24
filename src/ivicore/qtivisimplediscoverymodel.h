/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#ifndef QTIVISIMPLEDISCOVERYMODEL_H
#define QTIVISIMPLEDISCOVERYMODEL_H

#include <QAbstractListModel>

#include <QtIVICore/qtiviglobal.h>
#include <QtIVICore/qtiviserviceobject.h>

class Q_QTIVICORE_EXPORT QtIVISimpleDiscoveryModelBase: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum ModelRoles {
        ServiceObject = Qt::UserRole + 1
    };

    QtIVISimpleDiscoveryModelBase(QString interface, QObject *parent = 0);
    ~QtIVISimpleDiscoveryModelBase();

    QHash<int, QByteArray> roleNames() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE QtIVIServiceObject* serviceObjectAt(int index) const;

signals:
    void countChanged();

protected:
     virtual void updateServiceObjects(QList<QtIVIServiceObject*>);
     virtual void addServiceObjects(QList<QtIVIServiceObject*>);
     virtual void removeServiceObjects(QList<QtIVIServiceObject*>);

    // TODO add set methods and make these private
    QHash< int, QByteArray > m_roles;
    QSet< int > m_enumProperties;

private:
    QString m_interface;
    QList<QtIVIServiceObject*> m_serviceObjects;
    QHash< QByteArray, int > m_reverseRoles;
};

template <class T>
class Q_QTIVICORE_EXPORT QtIVISimpleDiscoveryModel : public QtIVISimpleDiscoveryModelBase
{
public:
    QtIVISimpleDiscoveryModel(QString interface, QObject* parent = 0)
        : QtIVISimpleDiscoveryModelBase(interface, parent)
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
    }
    virtual ~QtIVISimpleDiscoveryModel() {}

    Q_INVOKABLE T* at(int index)
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
};

#endif // QTIVISIMPLEDISCOVERYMODEL_H
