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

/*!
 * \module QtIVICore
 *
 * \title Qt IVI Core Module
 *
 * \brief Classes for Qt IVI Extension modules.
 * 
 * Qt IVI provides a pattern for extending Qt with extendable APIs. It also comes with a set of 
 * demonstrator APIs that can be used as a starting point or off-the-shelf in your projects.
 *
 * The pattern is based around separation of the API facing the application developer, the so 
 * called \e {Feature}, and the code implementing the said API, the \e {Backend}. There can be 
 * multiple backends per feature and the Core module provides support for finding the corresponding
 * backend in an easy to use way.
 *
 * Common use cases driving this separation are:
 * 
 * \list
 * \li \e {Early development}, where the UI can rely on a feature with a very basic backend 
 *     implementation.
 * \li \e {Testing / Simulation}, where the backends can be tested separately from the app and the 
 *     app can be tested using a specialized testing backend.
 * \li \e {Targets with different hardware configurations}, where parts of the system is 
 *     represented by simulated backends while others use a real hardware integration.
 * \li \e {Dynamically appearing services}, when services are available sometimes, but not always, 
 *     meaning that backends come and go over time.
 * \endlist
 * 
 * \chapter Building Blocks
 * 
 * Qt IVI consists of a number of building blocks. Understanding the roles of these is key to using
 * Qt IVI.
 * 
 * \section1 Core
 * 
 * The core module provides base classes for writing features and backends as well as the 
 * mechanisms for finding the correct backend for each feature.
 *
 * The core also contains common classes and base classes for various models and shared types.
 * 
 * \section1 Feature
 * 
 * The feature is what an application developer uses. The feature contains the API facing the 
 * applications, but also the backend interface, i.e. the interface that the corresponding backend 
 * needs to implement. The object implementing the backend interface is called as service object.
 *
 * The backend interface defines an interface name. This is a unique string identifying the 
 * interface between the feature and the backend. It is commonly written using reverse domain name 
 * notation, e.g. com.example.FeatureName. This is what is used by Core to find service objects 
 * for a feature.
 *
 * The feature is derived from the QtIVIAbstractFeature class. The backend interface is commonly 
 * based on QObject.
 *
 * To make the API useable from QML, the feature needs to include a QML plugin registering types 
 * and classes. The QML plugin is derived from the QQmlExtensionPlugin class.
 *
 * It is common to include a simple stubbed backend and a testing backend with each feature.
 * 
 * \section1 Backend
 * 
 * A backend implements the backend interface specified by the feature. The backend is derived from
 * the backend interface class defined by the feature, which is a QObject derived class.
 *
 * The object implementing the backend is called as service object.
 *
 * The backends are loaded by Core when the features request them. Each backend has to provide a Qt
 * IVI plugin that exposes a factory to the Core. This is what is used to load and create backends.
 * The plugin interface is called QtIVIServiceInterface.
 * 
 * \section1 Simple vs Dynamic Features
 * 
 * Most features are simple. This means that each feature element needs a single service object. By
 * setting the autoDiscover property of QtIVIAbstractFeature to true, the Core module provides the 
 * first compatible service object to the feature and issues a warning if none or more than one 
 * compatible service object is found.
 * 
 * When auto discovery is used, Core looks for the backend once during the creation of the feature 
 * instance. When used from QML, this takes place when the feature element is completely 
 * initalized. From C++, it is made when a call to the startAutoDiscovery method is called.
 * 
 * For dynamic features, there can be multiple service objects for each feature. This means that 
 * the auto discovery mechanism does not work. Instead the application must take responsibility for
 * finding the right service object for each feature instance.
 * 
 * QtIVIServiceManager can be used in this case to manually search for plugins with a specific 
 * BackendInterface. All the discovery and loading of the backends takes place in this class.
 * 
 * The QtIVIServiceManager class can also be used to register backends which are part of the same 
 * application and shouldn’t go into a plugin. This is especially useful for autotest as you need 
 * to control the backend and the feature at the same time.
 * 
 * \part Extending Qt IVI
 * 
 * For easy deployment, Qt IVI extensions should be built as Qt modules. This makes it easy to 
 * install and find headers, shared libraries as well as plugin modules from app projects.
 * 
 * By using the module system the developer benefits from easily enable the inclusion of your 
 * module by using 
 * 
 * \code
 * QT += <module>
 * \endcode
 *
 * In addition your module is properly setup to also work with cmake and the qdoc environment is 
 * already setup, not to speak about the automatic test execution when using
 * 
 * \code
 * make tests
 * \endcode
 * 
 * When creating a new Qt IVI module, it is recommended that you pick a name such as 
 * \e {OemFeatureName}, where \e {Oem} is the name of the car-maker or platform owner, and 
 * \e {FeatureName} is the name of the feature(s) of the module. In addition to the name, a reverse
 * domain name prefix is needed for prefixing backend interface names, e.g. \e {com.example}.
 * 
 * Notice that it is possible to have multiple feature element classes in a single module. For 
 * instance, the media module of Qt IVI contains both source discovery, media search and media 
 * browse APIs.
 * 
 * Having decided on a name and a backend interface name prefix, the template module can be checked
 * out from git. Then the renamer script is used to create the module, e.g:
 * 
 * \code
 * ./renamer.sh Oem MyFeature com.pelagicore
 * \endcode
 * 
 * This will result in the \e {OemMyFeature} module to be created and the backend interfaces 
 * prefixed with \e {com.pelagicore}, e.g. \e {com.pelagicore.MyFeature}.
 * 
 * The resulting directory structure is described in the table below.
 * 
 * \table
 * \header
 *   \li Path
 *   \li Description
 * \row
 *   \li \c {examples}
 *   \li Examples top-level directory
 * \row
 *   \li \c {examples/feature}
 *   \li Feature specific example directory
 * \row
 *   \li \c {example/feature/feature_qml}
 *   \li Template QML-based feature example
 * \row
 *   \li \c {example/feature/feature_widgets}
 *   \li Template widgets-based feature example
 * \row
 *   \li \c {src}
 *   \li Source code top-level directory
 * \row
 *   \li \c {src/feature}
 *   \li Feature source code. Includes feature APIs as well as backend interfaces
 * \row
 *   \li \c {src/feature/doc}
 *   \li Feature documentation configuration
 * \row
 *   \li \c {src/imports}
 *   \li QML import modules top-level directory
 * \row
 *   \li \c {src/imports/feature}
 *   \li Template feature QML import module
 * \row
 *   \li \c {src/plugins}
 *   \li Qt IVI backend plugins directory
 * \row
 *   \li \c {src/plugins/feature}
 *   \li Feature Qt IVI backends directory
 * \row
 *   \li \c {src/plugin/feature/feature_stub}
 *   \li Stubbed template feature Qt IVI backend
 * \row
 *   \li \c {tests}
 *   \li Unit tests top-level directory
 * \row
 *   \li \c {tests/auto}
 *   \li Automatic tests directory
 * \row
 *   \li \c {tests/auto/feature}
 *   \li Template feature unit test
 * \row
 *   \li \c {sync.profile}
 *   \li Qt module control script
 * \endtable
 * 
 * To add more feature APIs, simply add them into \c {src/feature} and 
 * \c {src/imports/feature}. To add more backends, add them to \c {src/plugins/feature}.
 *
 * If another OEM specific feature is needed there is no need to create the whole folder structure 
 * again. Simply add your feature’s code into the example \c {src} and \c {tests} folders and add 
 * it to the \c {sync.profile} file.
 * 
 * To create a backend for an existing feature, simply create a new Qt project based on the feature
 * module in question and build a Qt IVI backend plugin.
 *
 * \chapter Creating a Qt Module
 * \chapter Creating a Feature
 * \chapter Creating a Backend
 * 
 */ 


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
