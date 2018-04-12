/****************************************************************************
**
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
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

#include "qiviservicemanager.h"

#include "qiviproxyserviceobject_p.h"
#include "qiviservicemanager_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QJsonObject>
#include <QLibrary>
#include <QModelIndex>
#include <QStringList>

#define QIVI_PLUGIN_DIRECTORY "qtivi"

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(qLcIviServiceManagement, "qt.ivi.servicemanagement");

namespace qtivi_helper {
#ifdef QT_DEBUG
    static const bool loadDebug = true;
#else
    static const bool loadDebug = false;
#endif
    static const QString interfacesLiteral = QStringLiteral("interfaces");
    static const QString fileNameLiteral = QStringLiteral("fileName");
    static const QString metaDataLiteral = QStringLiteral("MetaData");
    static const QString classNameLiteral = QStringLiteral("className");
    static const QString simulationLiteral = QStringLiteral("simulation");
    static const QString debugLiteral = QStringLiteral("debug");
}

using namespace qtivi_helper;

QIviServiceManagerPrivate::QIviServiceManagerPrivate(QIviServiceManager *parent) : QObject(parent), q_ptr(parent)
{
}

QIviServiceManagerPrivate *QIviServiceManagerPrivate::get(QIviServiceManager *serviceManager)
{
    Q_ASSERT(serviceManager);
    return serviceManager->d_ptr;
}

bool QIviServiceManagerPrivate::isSimulation(const QVariantMap &metaData)
{
    QString fileName = metaData[fileNameLiteral].toString();
    return fileName.contains(QLatin1String("_simulation")) ||
            fileName.contains(QLatin1String("_simulator")) ||
            metaData[simulationLiteral].toBool();
}

QIviProxyServiceObject *QIviServiceManagerPrivate::createServiceObject(struct Backend *backend) const
{
    if (!backend)
        return nullptr;

    if (!backend->proxyServiceObject) {
        QIviServiceInterface *backendInterface = loadServiceBackendInterface(backend);
        if (backendInterface)
            backend->proxyServiceObject = new QIviProxyServiceObject(backendInterface);
    }

    if (backend->proxyServiceObject) {
        QString fileName = backend->metaData[fileNameLiteral].toString();
        if (fileName.isEmpty())
            fileName = QStringLiteral("static plugin");
        qCDebug(qLcIviServiceManagement) << "Found: " << backend->proxyServiceObject << "from: " << fileName;
        return backend->proxyServiceObject;
    }

    return nullptr;
}

QList<QIviServiceObject *> QIviServiceManagerPrivate::findServiceByInterface(const QString &interface, QIviServiceManager::SearchFlags searchFlags) const
{
    QList<QIviServiceObject*> list;
    qCDebug(qLcIviServiceManagement) << "Searching for a backend for:" << interface << "SearchFlags:" << searchFlags;

    for (Backend *backend : m_backends) {

        if (backend->metaData[interfacesLiteral].toStringList().contains(interface)) {
            bool isSimulation = QIviServiceManagerPrivate::isSimulation(backend->metaData);
            if ((searchFlags & QIviServiceManager::IncludeSimulationBackends && isSimulation) ||
                (searchFlags & QIviServiceManager::IncludeProductionBackends && !isSimulation)) {
                QIviServiceObject *serviceObject = createServiceObject(backend);
                if (serviceObject)
                    list.append(serviceObject);
            }
        }
    }

    return list;
}

void QIviServiceManagerPrivate::searchPlugins()
{
    bool found = false;
    const auto pluginDirs = QCoreApplication::libraryPaths();
    for (const QString &pluginDir : pluginDirs) {

        QDir dir(pluginDir);
        QString path = pluginDir + QDir::separator() + QLatin1String(QIVI_PLUGIN_DIRECTORY);
        //Check whether the directory exists
        if (!QDir(path).exists(QStringLiteral(".")))
            continue;

        const QStringList plugins = QDir(path).entryList(QDir::Files);
        for (const QString &pluginPath : plugins) {
            if (!QLibrary::isLibrary(pluginPath))
                continue;
            const QString fileName = QDir::cleanPath(path + QLatin1Char('/') + pluginPath);
            const QString absFileName = dir.absoluteFilePath(fileName);
            QPluginLoader loader(absFileName);

            registerBackend(loader.fileName(), loader.metaData());
            found = true;
        }
    }
    const auto staticPlugins = QPluginLoader::staticPlugins();
    for (const QStaticPlugin &plugin : staticPlugins)
        registerStaticBackend(plugin);

    if (Q_UNLIKELY(!found))
        qWarning() << "No plugins found in search path: " << QCoreApplication::libraryPaths().join(QLatin1String(":"));
}

void QIviServiceManagerPrivate::registerBackend(const QString &fileName, const QJsonObject &metaData)
{
    QVariantMap backendMetaData = metaData.value(metaDataLiteral).toVariant().toMap();

    if (Q_UNLIKELY(backendMetaData[interfacesLiteral].isNull() ||
                   backendMetaData[interfacesLiteral].toList().isEmpty())) {
        qCWarning(qLcIviServiceManagement, "PluginManager - Malformed metaData in '%s'. MetaData must contain a list of interfaces", qPrintable(fileName));
        return;
    }

    if (Q_UNLIKELY(metaData.value(debugLiteral).toBool() != qtivi_helper::loadDebug)) {
        qCWarning(qLcIviServiceManagement, "Skipping incompatible plugin %s. "
                                           "Expected build configuration '%s'",
                                            qPrintable(fileName), qtivi_helper::loadDebug ? "debug" : "release");
        return;
    }

    //TODO check for other metaData like name etc.

    backendMetaData.insert(fileNameLiteral, fileName);

    auto *backend = new Backend;
    backend->name =  metaData.value(classNameLiteral).toString();
    backend->metaData = backendMetaData;
    backend->interface = nullptr;
    backend->interfaceObject = nullptr;
    backend->loader = nullptr;
    backend->proxyServiceObject = nullptr;
    addBackend(backend);
}

void QIviServiceManagerPrivate::registerStaticBackend(QStaticPlugin plugin)
{
    QVariantMap backendMetaData = plugin.metaData().value(metaDataLiteral).toVariant().toMap();
    const char* pluginName = plugin.instance()->metaObject()->className();

    if (Q_UNLIKELY(backendMetaData[interfacesLiteral].isNull() ||
                   backendMetaData[interfacesLiteral].toList().isEmpty())) {
        qCWarning(qLcIviServiceManagement, "PluginManager - Malformed metaData in static plugin '%s'. MetaData must contain a list of interfaces", pluginName);
        return;
    }

    if (Q_UNLIKELY(plugin.metaData().value(debugLiteral).toBool() != qtivi_helper::loadDebug)) {
        qCWarning(qLcIviServiceManagement, "Skipping incompatible plugin %s. "
                                           "Expected build configuration '%s'",
                                            pluginName, qtivi_helper::loadDebug ? "debug" : "release");
        return;
    }

    QIviServiceInterface *backendInterface = qobject_cast<QIviServiceInterface*>(plugin.instance());
    if (Q_UNLIKELY(!backendInterface))
        qCWarning(qLcIviServiceManagement, "ServiceManager::serviceObjects - failed to cast to interface from '%s'", pluginName);

    //TODO check for other metaData like name etc.

    auto *backend = new Backend;
    backend->name = plugin.metaData().value(classNameLiteral).toString();
    backend->metaData = backendMetaData;
    backend->interface = backendInterface;
    backend->interfaceObject = nullptr;
    backend->loader = nullptr;
    backend->proxyServiceObject = nullptr;
    addBackend(backend);
}

bool QIviServiceManagerPrivate::registerBackend(QObject *serviceBackendInterface, const QStringList &interfaces, QIviServiceManager::BackendType backendType)
{
    if (interfaces.isEmpty()) {
        return false;
    }

    // Verify that the object implements the ServiceBackendInterface
    QIviServiceInterface *interface = qobject_cast<QIviServiceInterface*>(serviceBackendInterface);
    if (!interface) {
        return false;
    }

    QVariantMap metaData = QVariantMap();

    metaData.insert(interfacesLiteral, interfaces);
    if (backendType == QIviServiceManager::SimulationBackend)
        metaData.insert(simulationLiteral, true);

    auto *backend = new Backend;
    backend->name = QString::fromLocal8Bit(serviceBackendInterface->metaObject()->className());
    backend->metaData = metaData;
    backend->interface = interface;
    backend->interfaceObject = serviceBackendInterface;
    backend->loader = nullptr;
    backend->proxyServiceObject = nullptr;

    addBackend(backend);
    return true;
}

void QIviServiceManagerPrivate::unloadAllBackends()
{
    Q_Q(QIviServiceManager);

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

void QIviServiceManagerPrivate::addBackend(Backend *backend)
{
    Q_Q(QIviServiceManager);

    q->beginInsertRows(QModelIndex(), m_backends.count(), m_backends.count());
    m_backends.append(backend);
    q->endInsertRows();

    const auto interfaces = backend->metaData[interfacesLiteral].toStringList();
    for (const QString &interface : interfaces)
        m_interfaceNames.insert(interface);
}

namespace {
Q_NEVER_INLINE
static QIviServiceInterface *warn(const char *what, const QPluginLoader *loader)
{
    qWarning("ServiceManager::serviceObjects - failed to %s '%s'",
             what, qPrintable(loader->fileName()));
    delete loader;
    return nullptr;
}
} // unnamed namespace

QIviServiceInterface *QIviServiceManagerPrivate::loadServiceBackendInterface(struct Backend *backend) const
{
    if (backend->interface) {
        return backend->interface;
    }

    QPluginLoader *loader = new QPluginLoader(backend->metaData[fileNameLiteral].toString());
    QObject *plugin = loader->instance();
    if (Q_UNLIKELY(!plugin))
        return warn("load", loader);

    QIviServiceInterface *backendInterface = qobject_cast<QIviServiceInterface*>(plugin);
    if (Q_UNLIKELY(!backendInterface))
        return warn("cast to interface from", loader);

    backend->interface = backendInterface;
    backend->loader = loader;
    return backend->interface;
}

/*!
    \class QIviServiceManager
    \inmodule QtIviCore
    \brief QIviServiceManager provides the Backends to QIviAbstractFeature

    QIviServiceManager is the heart of QtIvi and provides you with an easy way to detect which
    backends and interfaces are available.

    By default QIviServiceManager reads the metaData of all plugins within the "qtivi" folder
    of your plugin path. The plugin itself will be loaded once it's explicitly requested by
    the developer by using findServiceByInterface().

    The manager can distinguish between \e Production and \e Simulation backends. For the recognition
    either the filename or the metaData is used.

    Simulation Backends can set the \e simulation key in their plugin metadata to \e true or use
    one of two supported plugin postfixes (_simulation, _simulator) in the plugin filename.

    The registerService() function can be used to add Backend classes without putting them into
    a plugin.

    The service manager is a process wide singleton and can be accessed through the \l instance method.

    For more detailed information on which plugins are recognized, enable the "qt.ivi.servicemanagement"
    logging category.

    See \l {Dynamic Backend System} for more information about how backend loading works.

    \note The QIviServiceManager will only accept plugins, which match the build configuration used
    for building qtivicore. This means qtivicore "release" build, doesn't accept plugins from a "debug" build.
*/

/*!
    \enum QIviServiceManager::SearchFlag

    \value IncludeProductionBackends
          Include production backends in the search result. \sa ProductionBackend
    \value IncludeSimulationBackends
          Include simulation backends in the search result. \sa SimulationBackend
    \value IncludeAll
          Include production and simulation backends in the search result
*/

/*!
    \enum QIviServiceManager::BackendType

    \value ProductionBackend
          A backend controlling a real automotive interface (e.g. a climate control connected over the CAN bus)
    \value SimulationBackend
          A backend used for development as it's only returning simulation values and won't be deployed to the final hardware
*/

QIviServiceManager::QIviServiceManager()
    : QAbstractListModel(nullptr)
    , d_ptr(new QIviServiceManagerPrivate(this))
{
    d_ptr->searchPlugins();
}

/*!
    Returns the global service manager instance.
*/
QIviServiceManager *QIviServiceManager::instance()
{
    static auto *instance = new QIviServiceManager();
    return instance;
}

/*!
    Returns a list of backends implementing the specified \a interface.

    The \a searchFlags argument can be used to control which type of backends are included in the search result.
*/
QList<QIviServiceObject *> QIviServiceManager::findServiceByInterface(const QString &interface, SearchFlags searchFlags)
{
    Q_D(QIviServiceManager);
    return d->findServiceByInterface(interface, searchFlags);
}

/*!
    Register a backend. The provided \a serviceBackendInterface must implement the
    QIviServiceInterface else the registration will fail. \a interfaces is a list
    with interfaces (at least one) supported by the backend. The \a backendType
    indicates the type of the backend and has influence on whether the backend is
    found by the auto discovery of the Feature.

    Returns true if the backend was successfully registered else false.

    \sa QIviServiceInterface
*/
bool QIviServiceManager::registerService(QObject *serviceBackendInterface, const QStringList &interfaces, BackendType backendType)
{
    Q_D(QIviServiceManager);
    return d->registerBackend(serviceBackendInterface, interfaces, backendType);
}

/*!
    \internal

    Unloads all currently loaded backends. Commonly only used for unit testing.
*/
void QIviServiceManager::unloadAllBackends()
{
    Q_D(QIviServiceManager);
    d->unloadAllBackends();
}

/*!
    Returns true if the specified \a interface has been registered.
*/
bool QIviServiceManager::hasInterface(const QString &interface) const
{
    Q_D(const QIviServiceManager);
    return d->m_interfaceNames.contains(interface);
}

/*!
    Returns the number of rows for the given \a parent. Typically \a parent is an empty model index.

    \sa QAbstractListModel::data()
*/
int QIviServiceManager::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIviServiceManager);
    return parent.isValid() ? 0 : d->m_backends.count();
}

/*!
    Returns the data for \a index and \a role.

    \sa QAbstractListModel::data()
*/
QVariant QIviServiceManager::data(const QModelIndex &index, int role) const
{
    Q_D(const QIviServiceManager);

    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row < 0 || row >= d->m_backends.count())
        return QVariant();

    Backend *backend = d->m_backends.at(row);

    switch (role) {
    case NameRole: return backend->name;
    case ServiceObjectRole: return QVariant::fromValue(d->createServiceObject(backend));
    case InterfacesRole: return backend->metaData[interfacesLiteral];
    }

    return QVariant();
}


/*!
    \reimp
*/
QHash<int, QByteArray> QIviServiceManager::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[NameRole] = "name";
        roles[ServiceObjectRole] = "serviceObject";
        roles[InterfacesRole] = "interfaces";
    }
    return roles;
}

QT_END_NAMESPACE
