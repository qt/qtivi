/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
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
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "qtiviservicemanager.h"

#include "qtiviproxyserviceobject_p.h"
#include "qtiviservicemanager_p.h"

#include <QStringList>
#include <QPluginLoader>
#include <QJsonObject>
#include <QCoreApplication>
#include <QDir>
#include <QModelIndex>
#include <QDebug>
#include <QLibrary>

#define QTIVI_PLUGIN_DIRECTORY "qtivi"

QtIVIServiceManagerPrivate::QtIVIServiceManagerPrivate(QtIVIServiceManager *parent) : QObject(parent), q_ptr(parent)
{
}

QtIVIServiceManagerPrivate *QtIVIServiceManagerPrivate::get(QtIVIServiceManager *serviceManager)
{
    Q_ASSERT(serviceManager);
    return serviceManager->d_ptr;
}

QList<QtIVIServiceObject *> QtIVIServiceManagerPrivate::findServiceByInterface(const QString &interface)
{
    QList<QtIVIServiceObject*> list;

    foreach (Backend *backend, m_backends) {

        if (backend->metaData[QLatin1String("interfaces")].toStringList().contains(interface)) {
            if (!backend->proxyServiceObject) {
                QtIVIServiceInterface *backendInterface = loadServiceBackendInterface(backend);
                if (backendInterface)
                    backend->proxyServiceObject = new QtIVIProxyServiceObject(backendInterface);
            }
            if (backend->proxyServiceObject)
                list.append(backend->proxyServiceObject);
        }
    }

    return list;
}

void QtIVIServiceManagerPrivate::searchPlugins()
{
    bool found = false;
    foreach (const QString &pluginDir, QCoreApplication::libraryPaths()) {

        QDir dir(pluginDir);
        QString path = pluginDir + QDir::separator() + QLatin1Literal(QTIVI_PLUGIN_DIRECTORY);
        //Check whether the directory exists
        if (!QDir(path).exists(QStringLiteral(".")))
            continue;

        QStringList plugins = QDir(path).entryList(QDir::Files);
        foreach (const QString &pluginPath, plugins) {
            if (!QLibrary::isLibrary(pluginPath))
                continue;
            QString fileName = QDir::cleanPath(path + QLatin1Char('/') + pluginPath);
            QPluginLoader loader(dir.absoluteFilePath(fileName));
            registerBackend(loader.fileName(), loader.metaData());
            found = true;
        }
    }
    if (!found)
    {
        qWarning() << "No plugins found in search path: " << QCoreApplication::libraryPaths().join(QLatin1String(":"));
    }
}

void QtIVIServiceManagerPrivate::registerBackend(const QString &fileName, const QJsonObject &metaData)
{
    QVariantMap backendMetaData = metaData.value(QLatin1String("MetaData")).toVariant().toMap();

    if (backendMetaData[QLatin1String("interfaces")].isNull() || backendMetaData[QLatin1String("interfaces")].toList().isEmpty()) {
        qWarning("PluginManager - Malformed metaData in '%s'. MetaData must contain a list of interfaces", qPrintable(fileName));
        return;
    }

    //TODO check for other metaData like name etc.

    backendMetaData.insert(QLatin1String("fileName"), fileName);

    Backend* backend = new Backend;
    backend->metaData = backendMetaData;
    backend->interface = 0;
    backend->interfaceObject = 0;
    backend->loader = 0;
    backend->proxyServiceObject = 0;
    addBackend(backend);
}

bool QtIVIServiceManagerPrivate::registerBackend(QObject *serviceBackendInterface, const QStringList &interfaces)
{
    if (interfaces.isEmpty()) {
        return false;
    }

    // Verify that the object implements the ServiceBackendInterface
    QtIVIServiceInterface *interface = qobject_cast<QtIVIServiceInterface*>(serviceBackendInterface);
    if (!interface) {
        return false;
    }

    QVariantMap metaData = QVariantMap();

    metaData.insert(QLatin1String("interfaces"), interfaces);

    Backend *backend = new Backend;
    backend->metaData = metaData;
    backend->interface = interface;
    backend->interfaceObject = serviceBackendInterface;
    backend->loader = 0;
    backend->proxyServiceObject = 0;

    addBackend(backend);
    return true;
}

void QtIVIServiceManagerPrivate::unloadAllBackends()
{
    Q_Q(QtIVIServiceManager);

    q->beginResetModel();
    QMutableListIterator<Backend*> i(m_backends);
    while (i.hasNext()) {
        Backend* backend = i.next();

        //If the Interface is from a Plugin, the Plugin owns it and it will be deleted when unloading.
        //Otherwise we own the Interface and delete the Pointer.
        if (backend->loader) {
            backend->loader->unload();
            delete backend->loader;
        } else if (backend->interfaceObject) {
            delete backend->interfaceObject;
        }
        delete backend->proxyServiceObject;

        i.remove();
        delete backend;
    }
    m_backends.clear();
    q->endResetModel();

    m_interfaceNames.clear();
}

void QtIVIServiceManagerPrivate::addBackend(Backend *backend)
{
    Q_Q(QtIVIServiceManager);

    q->beginInsertRows(QModelIndex(), m_backends.count(), m_backends.count());
    m_backends.append(backend);
    q->endInsertRows();

    foreach (const QString &interface, backend->metaData[QLatin1String("interfaces")].toStringList()) {
        m_interfaceNames.insert(interface);
    }
}

QtIVIServiceInterface *QtIVIServiceManagerPrivate::loadServiceBackendInterface(struct Backend *backend)
{
    if (backend->interface) {
        return backend->interface;
    }

    QPluginLoader *loader = new QPluginLoader(backend->metaData[QLatin1String("fileName")].toString());
    QObject *plugin = loader->instance();
    if (plugin) {

        QtIVIServiceInterface *backendInterface = qobject_cast<QtIVIServiceInterface*>(plugin);
        if (backendInterface) {
            backend->interface = backendInterface;
            backend->loader = loader;
            return backend->interface;
        } else {
            qWarning("ServiceManager::serviceObjects - failed to cast to interface from '%s'", qPrintable(loader->fileName()));
        }

    } else {
        qWarning("ServiceManager::serviceObjects - failed to load '%s'", qPrintable(loader->fileName()));
    }

    //Only delete the Loader right away if we didn't succeeded with loading the interfaces.
    delete loader;

    return 0;
}

/*!
 * \class QtIVIServiceManager
 * \inmodule QtIVICore
 * \brief QtIVIServiceManager provides the Backends to QtIVIAbstractFeature
 *
 * QtIVIServiceManager is the heart of QtIVI and provides you a easy way to detect what
 * Backends and interfaces are available.
 *
 * By default QtIVIServiceManager reads the metaData of all plugins within the "qtivi" folder
 * of your plugin path. The plugin itself will be loaded once it's explictly requested by
 * the developer by using findServiceByInterface()
 *
 * The registerService() function can be used to add Backend classes without putting them into
 * a plugin.
 *
 * The service manager is a process wide singleton and can be accessed through the \l instance method.
 */
QtIVIServiceManager::QtIVIServiceManager()
    : QAbstractListModel(0)
    , d_ptr(new QtIVIServiceManagerPrivate(this))
{
    d_ptr->searchPlugins();
}

/*!
 * Returns the global service manager instance.
 */
QtIVIServiceManager *QtIVIServiceManager::instance()
{
    static QtIVIServiceManager* instance = new QtIVIServiceManager();
    return instance;
}

/*!
 * Destructor.
 */
QtIVIServiceManager::~QtIVIServiceManager()
{

}

/*!
 * Returns a List of Backends which implementing the specified \a interface.
 */
QList<QtIVIServiceObject *> QtIVIServiceManager::findServiceByInterface(const QString &interface)
{
    Q_D(QtIVIServiceManager);
    return d->findServiceByInterface(interface);
}

/*!
 * Register a backend. The provided \a serviceBackendInterface must implement the QtIVIServiceInterface else
 * the registration will fail. \a interfaces is list with interfaces which the backend handles. At least one
 * interface is required.
 *
 * Returns true if the backend was successfully registered else false.
 *
 * \sa QtIVIServiceInterface
 */
bool QtIVIServiceManager::registerService(QObject *serviceBackendInterface, const QStringList &interfaces)
{
    Q_D(QtIVIServiceManager);
    return d->registerBackend(serviceBackendInterface, interfaces);
}

/*!
 * \internal
 *
 * Unloads all currently loaded backends. Commonly only used for unit testing.
 */
void QtIVIServiceManager::unloadAllBackends()
{
    Q_D(QtIVIServiceManager);
    return d->unloadAllBackends();
}

/*!
 * Returns true if the specified \a interface has been registered.
 */
bool QtIVIServiceManager::hasInterface(const QString &interface) const
{
    Q_D(const QtIVIServiceManager);
    return d->m_interfaceNames.contains(interface);
}

/*!
 * Returns the number of rows for the given \a parent. Typically \a parent is an empty model index.
 *
 * \sa QAbstractListModel::data()
 */
int QtIVIServiceManager::rowCount(const QModelIndex &parent) const
{
    Q_D(const QtIVIServiceManager);
    return parent.isValid() ? 0 : d->m_backends.count();
}

/*!
 * Returns the data for \a index and \a role.
 *
 * \sa QAbstractListModel::data()
 */
QVariant QtIVIServiceManager::data(const QModelIndex &index, int role) const
{
    Q_D(const QtIVIServiceManager);

    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= 0 && row < d->m_backends.count() && role == Qt::DisplayRole) {
        return QVariant::fromValue<QtIVIServiceInterface*>(d->m_backends.at(index.row())->interface);
    }

    return QVariant();
}
