#ifndef QTIVISERVICEMANAGER_H
#define QTIVISERVICEMANAGER_H

#include <QtCore/QAbstractListModel>

class QtIVIServiceObject;
class QtIVIServiceManagerPrivate;

//TODO Detect simulation plugins
//TODO make it possible to only search there e.g. by a SearchFlag BitField
class QtIVIServiceManager : public QAbstractListModel
{
    Q_OBJECT
public:
    static QtIVIServiceManager *instance();
    virtual ~QtIVIServiceManager();

    QList<QtIVIServiceObject*> findServiceByInterface(const QString &interface);
    bool hasInterface(const QString &interface) const;

    bool registerService(QObject *serviceBackendInterface, const QStringList &interfaces);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
signals:

private:
    explicit QtIVIServiceManager();
    QtIVIServiceManagerPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QtIVIServiceManager)
};

#endif // QTIVISERVICEMANAGER_H
