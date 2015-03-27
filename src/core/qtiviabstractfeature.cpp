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
