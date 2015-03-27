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

class QtIVIServiceObject;

template <class T>
class QtIVISimpleDiscoveryModel : public QAbstractListModel
{
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum ModelRoles {
        ServiceObject = Qt::UserRole + 1
    };

    QtIVISimpleDiscoveryModel(QString interface, QObject* parent = 0);
    virtual ~QtIVISimpleDiscoveryModel();

    QHash<int, QByteArray> roleNames() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE T* at(int index);
    Q_INVOKABLE QtIVIServiceObject* serviceObjectAt(int index);

signals:
    void countChanged();

protected:
     virtual void updateServiceObjects(QList<QtIVIServiceObject*>);
     virtual void addServiceObjects(QList<QtIVIServiceObject*>);
     virtual void removeServiceObjects(QList<QtIVIServiceObject*>);

private:

    QString m_interface;
    QList<QtIVIServiceObject*> m_serviceObjects;
    QHash< QByteArray, int > m_reverseRoles;
    QHash< int, QByteArray > m_roles;
    QSet< int > m_enumProperties;
};

#endif // QTIVISIMPLEDISCOVERYMODEL_H
