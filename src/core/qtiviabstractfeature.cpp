/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#include "qtiviabstractfeature.h"

#include "qtiviserviceobject.h"
#include "qtiviservicemanager.h"

/*!
 * \class QtIVIAbstractFeature
 * \brief The QtIVIAbstractFeature is the base class for all QtIVI Featues
 *
 * QtIVIAbstractFeature is the base class for the front facing API towards the Developer.
 * The QtIVIAbstractFeature provides you with a way to automatically connect to a Backend implementing
 * your needed interface. This discovery is started by using the startAutoDiscovery() function.
 *
 * Once the auto discovery is done, it can be checked whether a backend has been found by using the
 * isValid function.
 *
 * The auto discovery gives you a easy way to automatically connect to the right backend implementation.
 * If you don't want to use the auto discovery, it's also possible to use QtIVIServiceManager to retrieve
 * all Backends and search manually for the right one and connect it to the QtIVIAbstractFeature by calling
 * setServiceObject()
 *
 * QtIVIAbstractFeature is a abstract base class which needs to be subclassed to create a API for your
 * Feature.
 *
 * \chapter Subclassing
 *
 * When subclassing QtIVIAbstractFeature you must provide implementations of the following functions:
 * \list
 * \li acceptServiceObject()
 * \li connectToServiceObject()
 * \li disconnectFromServiceObject()
 * \li clearServiceObject()
 * \endlist
 *
 * Once a QtIVIServiceObject has be set, either by auto discovery or by manually setting it the acceptServiceObject()
 * function will be called to make sure the QtIVIServiceObject provides everything which is needed by the Feature.
 *
 * If the interface provides signals you need to do all the connect statements in connectToServiceObject() and
 * disconnect them again in disconnectFromServiceObject().
 *
 * clearServiceObject() will be called once the Feature doesn't have a connection to a ServiceObject anymore and should
 * reset it's state to sane defaults
 */

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
    emit isValidChanged(isValid());
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
        startAutoDiscovery();
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

void QtIVIAbstractFeature::startAutoDiscovery()
{
    setAutoDiscovery(true);

    QtIVIServiceManager* serviceManager = QtIVIServiceManager::instance();
    QList<QtIVIServiceObject*> serviceObjects = serviceManager->findServiceByInterface(m_interface);

    if (serviceObjects.isEmpty()) {
        qWarning() << "There is no backend implementing" << m_interface << ".";
        return;
    }

    if (serviceObjects.count() > 1)
        qWarning() << "There is more than one backend implementing" << m_interface << ".";

    setServiceObject(serviceObjects.at(0));
}

bool QtIVIAbstractFeature::isValid() const
{
    return m_serviceObject != 0;
}

void QtIVIAbstractFeature::serviceObjectDestroyed()
{
    m_serviceObject = 0;
    clearServiceObject();
    emit serviceObjectChanged();
}
