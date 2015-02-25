#ifndef QTIVISERVICEINTERFACE_H
#define QTIVISERVICEINTERFACE_H

#include <QtPlugin>

class QtIVIServiceInterface {

public:

    virtual QStringList interfaces() const = 0;
    virtual QObject* interfaceInstance(const QString& interface) const = 0;
};

Q_DECLARE_INTERFACE(QtIVIServiceInterface, "com.pelagicore.QtIVIServiceInterface")
Q_DECLARE_METATYPE(QtIVIServiceInterface*)

#endif // QTIVISERVICEINTERFACE_H

