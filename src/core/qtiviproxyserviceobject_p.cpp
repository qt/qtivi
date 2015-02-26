#include "qtiviproxyserviceobject_p.h"

QtIVIProxyServiceObject::QtIVIProxyServiceObject(QtIVIServiceInterface *interface)
    : QtIVIServiceObject()
    , m_interface(interface)
{

}

QtIVIProxyServiceObject::~QtIVIProxyServiceObject()
{

}

QStringList QtIVIProxyServiceObject::interfaces() const
{
    return m_interface->interfaces();
}

QObject *QtIVIProxyServiceObject::interfaceInstance(const QString &interface) const
{
    return m_interface->interfaceInstance(interface);
}

