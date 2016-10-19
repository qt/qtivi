/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#include <QStringList>
#include <QPluginLoader>
#include <QJsonObject>
#include <QCoreApplication>
#include <QDir>
#include <QModelIndex>
#include <QDebug>
#include <QLibrary>

#define QIVI_PLUGIN_DIRECTORY "qtivi"

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(qLcIviServiceManagement, "qt.ivi.servicemanagement");

QT_END_NAMESPACE

QIviServiceManagerPrivate::QIviServiceManagerPrivate(QIviServiceManager *parent) : QObject(parent), q_ptr(parent)
{
}

QIviServiceManagerPrivate *QIviServiceManagerPrivate::get(QIviServiceManager *serviceManager)
{
    Q_ASSERT(serviceManager);
    return serviceManager->d_ptr;
}

QList<QIviServiceObject *> QIviServiceManagerPrivate::findServiceByInterface(const QString &interface, QIviServiceManager::SearchFlags searchFlags) const
{
    QList<QIviServiceObject*> list;
    qCDebug(qLcIviServiceManagement) << "Searching for a backend for:" << interface << "SearchFlags:" << searchFlags;

    for (Backend *backend : m_backends) {

        if (backend->metaData[QLatin1String("interfaces")].toStringList().contains(interface)) {
            const QString& fileName = backend->metaData[QLatin1String("fileName")].toString();
            bool isSimulation = fileName.contains(QLatin1String("_simulation.")) || fileName.contains(QLatin1String("_simulator."));
            if ((searchFlags & QIviServiceManager::IncludeSimulationBackends && isSimulation) ||
                (searchFlags & QIviServiceManager::IncludeProductionBackends && !isSimulation)) {
                if (!backend->proxyServiceObject) {
                    QIviServiceInterface *backendInterface = loadServiceBackendInterface(backend);
                    if (backendInterface)
                        backend->proxyServiceObject = new QIviProxyServiceObject(backendInterface);
                }
                if (backend->proxyServiceObject) {
                    qCDebug(qLcIviServiceManagement) << "Found: " << backend->proxyServiceObject << "from: " << fileName;
                    list.append(backend->proxyServiceObject);
                }
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
        QString path = pluginDir + QDir::separator() + QLatin1Literal(QIVI_PLUGIN_DIRECTORY);
        //Check whether the directory exists
        if (!QDir(path).exists(QStringLiteral(".")))
            continue;

        const QStringList plugins = QDir(path).entryList(QDir::Files);
        for (const QString &pluginPath : plugins) {
            if (!QLibrary::isLibrary(pluginPath))
                continue;
            QString fileName = QDir::cleanPath(path + QLatin1Char('/') + pluginPath);
            QPluginLoader loader(dir.absoluteFilePath(fileName));
            registerBackend(loader.fileName(), loader.metaData());
            found = true;
        }
    }
    if (Q_UNLIKELY(!found))
        qWarning() << "No plugins found in search path: " << QCoreApplication::libraryPaths().join(QLatin1String(":"));
}

void QIviServiceManagerPrivate::registerBackend(const QString &fileName, const QJsonObject &metaData)
{
    QVariantMap backendMetaData = metaData.value(QLatin1String("MetaData")).toVariant().toMap();

    if (Q_UNLIKELY(backendMetaData[QLatin1String("interfaces")].isNull() ||
                   backendMetaData[QLatin1String("interfaces")].toList().isEmpty())) {
        qCWarning(qLcIviServiceManagement, "PluginManager - Malformed metaData in '%s'. MetaData must contain a list of interfaces", qPrintable(fileName));
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

    metaData.insert(QLatin1String("interfaces"), interfaces);
    if (backendType == QIviServiceManager::SimulationBackend) //fake a simulation filename
        metaData.insert(QLatin1String("fileName"), QLatin1String("fake_simulation."));

    Backend *backend = new Backend;
    backend->metaData = metaData;
    backend->interface = interface;
    backend->interfaceObject = serviceBackendInterface;
    backend->loader = 0;
    backend->proxyServiceObject = 0;

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

    const auto interfaces = backend->metaData[QLatin1String("interfaces")].toStringList();
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
    return Q_NULLPTR;
}
} // unnamed namespace

QIviServiceInterface *QIviServiceManagerPrivate::loadServiceBackendInterface(struct Backend *backend) const
{
    if (backend->interface) {
        return backend->interface;
    }

    QPluginLoader *loader = new QPluginLoader(backend->metaData[QLatin1String("fileName")].toString());
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
 * \class QIviServiceManager
 * \inmodule QtIviCore
 * \brief QIviServiceManager provides the Backends to QIviAbstractFeature
 *
 * QIviServiceManager is the heart of QtIvi and provides you with an easy way to detect which
 * backends and interfaces are available.
 *
 * By default QIviServiceManager reads the metaData of all plugins within the "qtivi" folder
 * of your plugin path. The plugin itself will be loaded once it's explictly requested by
 * the developer by using findServiceByInterface().
 *
 * The registerService() function can be used to add Backend classes without putting them into
 * a plugin.
 *
 * The service manager is a process wide singleton and can be accessed through the \l instance method.
 */

/*!
 * \enum QIviServiceManager::SearchFlag
 *
 * \value IncludeProductionBackends
 *        Include production backends in the search result. \sa ProductionBackend
 * \value IncludeSimulationBackends
 *        Include simulation backends in the search result. \sa SimulationBackend
 * \value IncludeAll
 *        Include production and simulation backends in the search result
 */

/*!
 * \enum QIviServiceManager::BackendType
 *
 * \value ProductionBackend
 *        A backend controlling a real automotive interface (e.g. a climate control connected over the CAN bus)
 * \value SimulationBackend
 *        A backend used for development as it's only returning simulation values and won't be deployed to the final hardware
 */

QIviServiceManager::QIviServiceManager()
    : QAbstractListModel(0)
    , d_ptr(new QIviServiceManagerPrivate(this))
{
    d_ptr->searchPlugins();
}

/*!
 * Returns the global service manager instance.
 */
QIviServiceManager *QIviServiceManager::instance()
{
    static QIviServiceManager *instance = new QIviServiceManager();
    return instance;
}

/*!
 * Destructor.
 */
QIviServiceManager::~QIviServiceManager()
{

}

/*!
 * Returns a list of backends implementing the specified \a interface.
 *
 * The \a searchFlags argument can be used to control which type of backends are included in the search result.
 */
QList<QIviServiceObject *> QIviServiceManager::findServiceByInterface(const QString &interface, SearchFlags searchFlags)
{
    Q_D(QIviServiceManager);
    return d->findServiceByInterface(interface, searchFlags);
}

/*!
 * Register a backend. The provided \a serviceBackendInterface must implement the
 * QIviServiceInterface else the registration will fail. \a interfaces is a list
 * with interfaces (at least one) supported by the backend. The \a backendType
 * indicates the type of the backend and has influence on whether the backend is
 * found by the auto discovery of the Feature.
 *
 * Returns true if the backend was successfully registered else false.
 *
 * \sa QIviServiceInterface
 */
bool QIviServiceManager::registerService(QObject *serviceBackendInterface, const QStringList &interfaces, BackendType backendType)
{
    Q_D(QIviServiceManager);
    return d->registerBackend(serviceBackendInterface, interfaces, backendType);
}

/*!
 * \internal
 *
 * Unloads all currently loaded backends. Commonly only used for unit testing.
 */
void QIviServiceManager::unloadAllBackends()
{
    Q_D(QIviServiceManager);
    return d->unloadAllBackends();
}

/*!
 * Returns true if the specified \a interface has been registered.
 */
bool QIviServiceManager::hasInterface(const QString &interface) const
{
    Q_D(const QIviServiceManager);
    return d->m_interfaceNames.contains(interface);
}

/*!
 * Returns the number of rows for the given \a parent. Typically \a parent is an empty model index.
 *
 * \sa QAbstractListModel::data()
 */
int QIviServiceManager::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIviServiceManager);
    return parent.isValid() ? 0 : d->m_backends.count();
}

/*!
 * Returns the data for \a index and \a role.
 *
 * \sa QAbstractListModel::data()
 */
QVariant QIviServiceManager::data(const QModelIndex &index, int role) const
{
    Q_D(const QIviServiceManager);

    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= 0 && row < d->m_backends.count() && role == Qt::DisplayRole) {
        return QVariant::fromValue<QIviServiceInterface*>(d->m_backends.at(index.row())->interface);
    }

    return QVariant();
}
