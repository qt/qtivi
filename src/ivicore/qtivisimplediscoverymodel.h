/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.pelagicore.com/
**
** This file is part of Qt IVI.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions see http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QTIVISIMPLEDISCOVERYMODEL_H
#define QTIVISIMPLEDISCOVERYMODEL_H

#include <QAbstractListModel>
#include <QQmlParserStatus>

#include <QtIVICore/qtiviglobal.h>
#include <QtIVICore/qtiviserviceobject.h>

class Q_QTIVICORE_EXPORT QtIVISimpleDiscoveryModelBase: public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(bool autoDiscovery READ autoDiscovery WRITE setAutoDiscovery NOTIFY autoDiscoveryChanged)

public:
    enum ModelRoles {
        ServiceObject = Qt::UserRole + 1
    };

    QtIVISimpleDiscoveryModelBase(QString interface, bool autoDiscovery = true, QObject *parent = 0);
    ~QtIVISimpleDiscoveryModelBase();

    QHash<int, QByteArray> roleNames() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE QtIVIServiceObject* serviceObjectAt(int index) const;

    void startAutoDiscovery();

    bool autoDiscovery() const;

public slots:
    void setAutoDiscovery(bool autoDiscovery);

signals:
    void countChanged();
    void autoDiscoveryChanged(bool autoDiscovery);

protected:
    virtual void updateServiceObjects(QList<QtIVIServiceObject*>);
    virtual void addServiceObjects(QList<QtIVIServiceObject*>);
    virtual void removeServiceObjects(QList<QtIVIServiceObject*>);

    //TODO This doesn't work for the C++ usecases we should use the constructor there instead
    // Also this means a qml dependency in the core, do we want that ?
    virtual void classBegin();
    virtual void componentComplete();

    void setRoleNames(QHash<int, QByteArray> roles, QSet<int> enumProperties);

private slots:
    void onModelReset();
    void onRowsInserted(const QModelIndex &, int, int);
    void onRowsRemoved(const QModelIndex &, int, int);

private:
    QString m_interface;
    bool m_autoDiscovery;
    QList<QtIVIServiceObject*> m_serviceObjects;
    QHash< QByteArray, int > m_reverseRoles;
    QHash< int, QByteArray > m_roles;
    QSet< int > m_enumProperties;
};

template <class T>
class Q_QTIVICORE_EXPORT QtIVISimpleDiscoveryModel : public QtIVISimpleDiscoveryModelBase
{
public:
    QtIVISimpleDiscoveryModel(QString interface, QObject* parent = 0)
        : QtIVISimpleDiscoveryModelBase(interface, parent)
    {
        QHash<int, QByteArray> roles;
        QSet<int> enumProperties;

        // Initialize the list of properties this instance of the model knows about
        for(int propertyIndex = 0; propertyIndex < T::staticMetaObject.propertyCount(); propertyIndex++)
        {
            QMetaProperty property = T::staticMetaObject.property(propertyIndex);
            const int roleNumber = Qt::UserRole + 1 + propertyIndex;
            if(property.isEnumType())
            {
                enumProperties.insert(roleNumber);
            }
            roles.insert(roleNumber, property.name());
        }

        setRoleNames(roles, enumProperties);
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
