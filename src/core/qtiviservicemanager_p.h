#ifndef QTIVISERVICEMANAGER_P_H
#define QTIVISERVICEMANAGER_P_H

#include <QAbstractListModel>
#include <QVariantMap>
#include <QStringList>
#include <QMap>

class QPluginLoader;
class QtIVIServiceInterface;
class QtIVIServiceManager;

struct Backend{
    QVariantMap metaData;
    QtIVIServiceInterface *interface;
};

class QtIVIServiceManagerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit QtIVIServiceManagerPrivate(QtIVIServiceManager *parent);

    QList<QtIVIServiceInterface*> findServiceByInterface(const QString &interface);

    void searchPlugins();
    void registerBackend(const QString fileName, const QJsonObject metaData);
    bool registerBackend(QObject *serviceBackendInterface, const QStringList &interfaces);
    void addBackend(struct Backend *backend);

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

