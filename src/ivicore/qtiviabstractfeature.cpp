/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "qtiviabstractfeature.h"

#include "qtiviserviceobject.h"
#include "qtiviservicemanager.h"

#include <QDebug>

/*!
 * \class QtIVIAbstractFeature
 * \inmodule QtIVICore
 * \brief The QtIVIAbstractFeature is the base class for all QtIVI Features
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

/*!
    \qmltype AbstractFeature
    \instantiates QtIVIAbstractFeature
    \inqmlmodule QtIVICore

    \brief The AbstractFeature is not directly accessible. QML type provides
    base QML properties for the feature, like autoDiscovery and isValid.
*/

/*!
 * \fn bool QtIVIAbstractFeature::acceptServiceObject(QtIVIServiceObject *so)
 *
 * This method is expected to be implemented by any class subclassing QtIVIAbstractFeature.
 *
 * This method is expected to return \c true if the given service object, \a so, is accepted and
 * can be used, otherwise \c false.
 *
 * If the object is accepted, \l connectToServiceObject is called to actually connect to the
 * service object.
 *
 * \sa connectToServiceObject(), disconnectFromServiceObject(), clearServiceObject()
 */

/*!
 * \fn void QtIVIAbstractFeature::connectToServiceObject(QtIVIServiceObject *so)
 *
 * This method is expected to be implemented by any class subclassing QtIVIAbstractFeature.
 *
 * The implementation is expected to connect to the service object, \a so, and to set all
 * properties to reflect the state of the service object.
 *
 * There is no previous service object connected, as this call always is preceeded by a call to
 * \l disconnectFromServiceObject or \l clearServiceObject.
 *
 * It is safe to assume that the service object, \a so, has always been accepted through the
 * \l acceptServiceObject method prior to being passed to this method.
 *
 * \sa acceptServiceObject(), disconnectFromServiceObject(), clearServiceObject()
 */

/*!
 * \fn void QtIVIAbstractFeature::disconnectFromServiceObject(QtIVIServiceObject *so)
 *
 * This method is expected to be implemented by any class subclassing QtIVIAbstractFeature.
 *
 * The implementation is expected to disconnect from the service object, \a so.
 *
 * It is not expected that the implementation goes to safe defaults. A call to this function is
 * always followed by a call to \l connectToServiceObject or \l clearServiceObject.
 *
 * \sa acceptServiceObject(), connectToServiceObject(), clearServiceObject()
 */

/*!
 * \fn void QtIVIAbstractFeature::clearServiceObject()
 *
 * This method is expected to be implemented by any class subclassing QtIVIAbstractFeature.
 *
 * Called when no service object is available. The implementation is expected to set all
 * properties to safe defaults and forget all links to the previous service object.
 *
 * There is no need to disconnect from the service object. If it still exists, it is guaranteed
 * that \l disconnectFromServiceObject is called first.
 *
 * \sa acceptServiceObject(), connectToServiceObject(), disconnectFromServiceObject()
 */

/*!
 * Constructs an abstract feature.
 *
 * The \a parent argument is passed on to the \l QObject constructor.
 *
 * The \a interface argument is used to locate suitable service objects.
 */
QtIVIAbstractFeature::QtIVIAbstractFeature(const QString &interface, QObject *parent)
    : QObject(parent)
    , m_interface(interface)
    , m_serviceObject(0)
    , m_autoDiscovery(true)
{
}

/*!
 * Destructor.
 */
QtIVIAbstractFeature::~QtIVIAbstractFeature()
{

}

/*!
 * \property QtIVIAbstractFeature::serviceObject
 * \brief Sets the service object for the feature.
 *
 * As features only expose the front API facing the developer, a service object implementing the
 * actual function is required. This is usually retrieved through the \l autoDiscovery mechanism.
 *
 * \sa autoDiscovery
 */
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

/*!
 * \qmlproperty bool AbstractFeature::autoDiscovery
 * \brief True if service objects are located automatically.
 *
 * If automatic discovery is enabled, the feature will search for a suitable backend when
 * \l {QQmlParserStatus::}{componentComplete} is invoked from QML.
 *
 */

/*!
 * \property QtIVIAbstractFeature::autoDiscovery
 * \brief True if service objects are located automatically.
 *
 * If automatic discovery is enabled, the feature will search for a suitable backend when
 * \l startAutoDiscovery is called from C++.
 *
 * \sa startAutoDiscovery()
 */
void QtIVIAbstractFeature::setAutoDiscovery(bool autoDiscovery)
{
    if (m_autoDiscovery == autoDiscovery)
        return;

    m_autoDiscovery = autoDiscovery;
    emit autoDiscoveryChanged(autoDiscovery);
}

/*!
 * \internal
 * \overload
 */
void QtIVIAbstractFeature::classBegin()
{
}

/*!
 * Invoked automatically when used from QML. Calls \l startAutoDiscovery() if \l autoDiscovery is \c true.
 */
void QtIVIAbstractFeature::componentComplete()
{
    if (m_autoDiscovery) {
        startAutoDiscovery();
    }
}

/*!
 * Returns the interface name.
 */
QString QtIVIAbstractFeature::interfaceName() const
{
    return m_interface;
}

QtIVIServiceObject *QtIVIAbstractFeature::serviceObject() const
{
    return m_serviceObject;
}

bool QtIVIAbstractFeature::autoDiscovery() const
{
    return m_autoDiscovery;
}

/*!
 * \brief Performs an automatic discovery attempt.
 *
 * The feature will try to locate a single service object implementing the required interface.
 *
 * If no service object is found, the feature will stay invalid. If more than one service object
 * is found, the first instance is used.
 *
 * This function sets the \l autoDiscovery property to \c true.
 *
 * \sa autoDiscovery()
 */
void QtIVIAbstractFeature::startAutoDiscovery()
{
    setAutoDiscovery(true);

    if (m_serviceObject) // No need to discover a new backend when we already have one
        return;

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

/*!
 * \qmlproperty bool AbstractFeature::isValid
 * \brief Holds whether the feature is ready to use.
 *
 * The property holds \c true if the feature is ready to be used, otherwise \c false. Not being
 * ready usually indicates that no suitable service object could be found, or that automatic
 * discovery has not been triggered.
 *
 * \sa QtIVIServiceObject, autoDiscovery
 */
/*!
 * \property QtIVIAbstractFeature::isValid
 * \brief Holds whether the feature is ready to use.
 *
 * The property holds \c true if the feature is ready to be used, otherwise \c false. Not being
 * ready usually indicates that no suitable service object could be found, or that automatic
 * discovery has not been triggered.
 *
 * \sa QtIVIServiceObject, autoDiscovery()
 */
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
