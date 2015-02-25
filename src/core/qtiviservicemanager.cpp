#include "qtiviservicemanager.h"

#include "qtiviserviceinterface.h"
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

QList<QtIVIServiceInterface*> QtIVIServiceManagerPrivate::findServiceByInterface(const QString &interface)
{
    QList<QtIVIServiceInterface*> list;

    foreach (Backend *backend, m_backends) {

        if (backend->metaData["interfaces"].toStringList().contains(interface)) {
            QtIVIServiceInterface *backendInterface = loadServiceBackendInterface(backend);
            list.append(backendInterface);
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
        qWarning() << "No plugins found in search path: " << QCoreApplication::libraryPaths().join(":");
    }
}

void QtIVIServiceManagerPrivate::registerBackend(const QString fileName, const QJsonObject metaData)
{
    QVariantMap backendMetaData = metaData.value(QLatin1String("MetaData")).toVariant().toMap();

    if (backendMetaData["interfaces"].isNull() || backendMetaData["interfaces"].toList().isEmpty()) {
        qDebug("PluginManager - Malformed metaData in '%s'. MetaData must contain a list of interfaces", qPrintable(fileName));
        return;
    }

    //TODO check for other metaData like name etc.

    backendMetaData.insert("fileName", fileName);

    Backend* backend = new Backend;
    backend->metaData = backendMetaData;
    backend->interface = 0;
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

    metaData.insert("interfaces", interfaces);

    Backend *backend = new Backend;
    backend->metaData = metaData;
    backend->interface = interface;

    addBackend(backend);
    return true;
}

void QtIVIServiceManagerPrivate::addBackend(Backend *backend)
{
    Q_Q(QtIVIServiceManager);

    q->beginInsertRows(QModelIndex(), m_backends.count(), m_backends.count());
    m_backends.append(backend);
    q->endInsertRows();

    foreach (const QString &interface, backend->metaData["interfaces"].toStringList()) {
        m_interfaceNames.insert(interface);
    }
}

QtIVIServiceInterface *QtIVIServiceManagerPrivate::loadServiceBackendInterface(struct Backend *backend)
{
    if (backend->interface) {
        return backend->interface;
    }

    QPluginLoader loader(backend->metaData["fileName"].toString());
    QObject *plugin = loader.instance();
    if (plugin) {

        QtIVIServiceInterface *backendInterface = qobject_cast<QtIVIServiceInterface*>(plugin);
        if (backendInterface) {
            backend->interface = backendInterface;
            return backend->interface;
        } else {
            qDebug("ServiceManager::serviceObjects - failed to cast to interface from '%s'", qPrintable(loader.fileName()));
        }

    } else {
        qDebug("ServiceManager::serviceObjects - failed to load '%s'", qPrintable(loader.fileName()));
    }

    return 0;
}


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

QtIVIServiceManager::~QtIVIServiceManager()
{

}

/*!
 * Returns a List of Backends which implementing the specified \a interface.
 */
QList<QtIVIServiceInterface *> QtIVIServiceManager::findServiceByInterface(const QString &interface)
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
