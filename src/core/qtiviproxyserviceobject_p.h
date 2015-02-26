#ifndef QTIVIPROXYSERVICEOBJECT_H
#define QTIVIPROXYSERVICEOBJECT_H

#include "qtiviserviceobject.h"

class QtIVIProxyServiceObject : public QtIVIServiceObject
{
public:
    QtIVIProxyServiceObject(QtIVIServiceInterface* interface);
    virtual ~QtIVIProxyServiceObject();

    virtual QStringList interfaces() const;
    virtual QObject* interfaceInstance(const QString& interface) const;

private:

    QtIVIServiceInterface* m_interface;
};

#endif // QTIVIPROXYSERVICEOBJECT_H
