#ifndef QTIVISERVICEMANAGER_H
#define QTIVISERVICEMANAGER_H

#include <QtCore/QObject>

class QtIVIServiceManager : public QObject
{
    Q_OBJECT
public:
    QtIVIServiceManager *instance();
    virtual ~QtIVIServiceManager();

signals:

private:
    explicit QtIVIServiceManager(QObject *parent = 0);
};

#endif // QTIVISERVICEMANAGER_H
