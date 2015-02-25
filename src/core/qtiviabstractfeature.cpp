#include "qtiviabstractfeature.h"

#include "qtiviserviceobject.h"
#include "qtiviservicemanager.h"

QtIVIAbstractFeature::QtIVIAbstractFeature(const QString &interface, bool autoDiscovery, QObject *parent)
    : QObject(parent)
    , m_interface(interface)
    , m_serviceObject(0)
    , m_autoDiscovery(autoDiscovery)
{
    //Detect whether we got created from within QML
    //If not call the autoDiscovery
}

QtIVIAbstractFeature::~QtIVIAbstractFeature()
{

}

void QtIVIAbstractFeature::setServiceObject(QtIVIServiceObject *so)
{
    if (m_serviceObject) {
        disconnectFromServiceObject(m_serviceObject);
        disconnect(so, SIGNAL(destroyed()), this, SLOT(serviceObjectDestroyed()));
    }

    m_serviceObject = 0;

    if (!acceptServiceObject(so)) {
        clearServiceObject();
        qWarning("ServiceObject is not accepted");
    }

    m_serviceObject = so;
    emit serviceObjectChanged();
    connectToServiceObject(m_serviceObject);
    connect(so, SIGNAL(destroyed()), this, SLOT(serviceObjectDestroyed()));
}

void QtIVIAbstractFeature::setAutoDiscovery(bool autoDiscovery)
{
    if (m_autoDiscovery == autoDiscovery)
        return;

    m_autoDiscovery = autoDiscovery;
    emit autoDiscoveryChanged(autoDiscovery);
}

void QtIVIAbstractFeature::classBegin()
{

}

void QtIVIAbstractFeature::componentComplete()
{
    if (m_autoDiscovery) {
        QtIVIServiceManager* serviceManager = QtIVIServiceManager::instance();
        QList<QtIVIServiceInterface*> serviceObjects = serviceManager->findServiceByInterface(m_interface);

        if (serviceObjects.isEmpty()) {
            qWarning() << "There is no backend implementing";
            return;
        }

        if (serviceObjects.count() > 1)
            qWarning() << "There is more than one backend implementing" << m_interface << ".";

        //ServiceObject != QtIVIServiceInterface
        //setServiceObject(serviceObjects.at(0));
    }
}

QtIVIServiceObject *QtIVIAbstractFeature::serviceObject() const
{
    return m_serviceObject;
}

bool QtIVIAbstractFeature::autoDiscovery() const
{
    return m_autoDiscovery;
}

void QtIVIAbstractFeature::serviceObjectDestroyed()
{
    m_serviceObject = 0;
    clearServiceObject();
    emit serviceObjectChanged();
}
