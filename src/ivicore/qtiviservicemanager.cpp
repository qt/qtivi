/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

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

#define QTIVI_PLUGIN_DIRECTORY "qtivi"

QtIVIServiceManagerPrivate::QtIVIServiceManagerPrivate(QtIVIServiceManager *parent) : QObject(parent), q_ptr(parent)
{
}

QList<QtIVIServiceObject *> QtIVIServiceManagerPrivate::findServiceByInterface(const QString &interface)
{
    QList<QtIVIServiceObject*> list;

    foreach (Backend *backend, m_backends) {

        if (backend->metaData[QLatin1String("interfaces")].toStringList().contains(interface)) {
            QtIVIServiceInterface *backendInterface = loadServiceBackendInterface(backend);
            list.append(new QtIVIProxyServiceObject(backendInterface));
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

void QtIVIServiceManagerPrivate::registerBackend(const QString fileName, const QJsonObject metaData)
{
    QVariantMap backendMetaData = metaData.value(QLatin1String("MetaData")).toVariant().toMap();

    if (backendMetaData[QLatin1String("interfaces")].isNull() || backendMetaData[QLatin1String("interfaces")].toList().isEmpty()) {
        qDebug("PluginManager - Malformed metaData in '%s'. MetaData must contain a list of interfaces", qPrintable(fileName));
        return;
    }

    //TODO check for other metaData like name etc.

    backendMetaData.insert(QLatin1String("fileName"), fileName);

    Backend* backend = new Backend;
    backend->metaData = backendMetaData;
    backend->interface = 0;
    backend->interfaceObject = 0;
    backend->loader = 0;
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

    addBackend(backend);
    return true;
}

void QtIVIServiceManagerPrivate::unloadAllBackends()
{
    Q_Q(QtIVIServiceManager);

    q->beginResetModel();
    for(int i=0; i<m_backends.count(); i++) {
        Backend* backend = m_backends.takeAt(i);

        //If the Interface is from a Plugin, the Plugin owns it and it will be deleted when unloading.
        //Otherwise we own the Interface and delete the Pointer.
        if (backend->loader) {
            backend->loader->unload();
            delete backend->loader;
        } else if (backend->interfaceObject) {
            delete backend->interfaceObject;
        }

        delete backend;
    }
    m_backends.clear();
    q->endResetModel();
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
            qDebug("ServiceManager::serviceObjects - failed to cast to interface from '%s'", qPrintable(loader->fileName()));
        }

    } else {
        qDebug("ServiceManager::serviceObjects - failed to load '%s'", qPrintable(loader->fileName()));
    }

    //Only delete the Loader right away if we didn't succeeded with loading the interfaces.
    delete loader;

    return 0;
}

/*!
 * \class QtIVIServiceManager
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
 */
QtIVIServiceManager::QtIVIServiceManager()
    : QAbstractListModel(0)
    , d_ptr(new QtIVIServiceManagerPrivate(this))
{
    d_ptr->searchPlugins();
}

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
 * Register a backend. The provided \a serviceBackendInterface must implement the ServiceBackendInterface else
 * the registration will fail.
 *
 * \param serviceBackendInterface an QObject instance which implements the ServiceBackendInterface
 * \param interfaces a list with interfaces which the backend handles. At least one interface is required
 * \return true if the backend was successfully registered else false
 * \sa ServiceBackendInterface
 */
bool QtIVIServiceManager::registerService(QObject *serviceBackendInterface, const QStringList &interfaces)
{
    Q_D(QtIVIServiceManager);
    return d->registerBackend(serviceBackendInterface, interfaces);
}

void QtIVIServiceManager::unloadAllBackends()
{
    Q_D(QtIVIServiceManager);
    return d->unloadAllBackends();
}

/*!
 * Returns true if the specified \a interface has been registered.

 * \param interface the interface
 * \return true if the specified \a interface has been registered else false
 */
bool QtIVIServiceManager::hasInterface(const QString &interface) const
{
    Q_D(const QtIVIServiceManager);
    return d->m_interfaceNames.contains(interface);
}

/*!
 * Implements QAbstractListModel::rowCount()
 *
 * \param parent the model index, typically an empty model index
 * \return the number of interfaces available
 * \sa QAbstractListModel::data()
 */
int QtIVIServiceManager::rowCount(const QModelIndex &parent) const
{
    Q_D(const QtIVIServiceManager);
    return parent.isValid() ? 0 : d->m_backends.count();
}

/*!
 * Implements QAbstractListModel::data()
 *
 * \param index the index
 * \param role the role
 * \return the data for the spcified index and role
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
