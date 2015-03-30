/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#ifndef QTIVISERVICEMANAGER_P_H
#define QTIVISERVICEMANAGER_P_H

#include <QAbstractListModel>
#include <QVariantMap>
#include <QStringList>
#include <QMap>

class QPluginLoader;
class QtIVIServiceInterface;
class QtIVIServiceObject;
class QtIVIServiceManager;

struct Backend{
    QVariantMap metaData;
    QtIVIServiceInterface *interface;
    QObject *interfaceObject;
    QPluginLoader *loader;
};

class QtIVIServiceManagerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit QtIVIServiceManagerPrivate(QtIVIServiceManager *parent);

    QList<QtIVIServiceObject*> findServiceByInterface(const QString &interface);

    void searchPlugins();
    void registerBackend(const QString fileName, const QJsonObject metaData);
    bool registerBackend(QObject *serviceBackendInterface, const QStringList &interfaces);
    void addBackend(struct Backend *backend);

    void unloadAllBackends();

    QtIVIServiceInterface *loadServiceBackendInterface(struct Backend *backend);

    QList<Backend*> m_backends;
    QSet<QString> m_interfaceNames;


    QtIVIServiceManager * const q_ptr;
    Q_DECLARE_PUBLIC(QtIVIServiceManager)

signals:
    void beginInsertRows(const QModelIndex &index, int start, int end);
    void endInsertRows();

};

#endif // QTIVISERVICEMANAGER_P_H

