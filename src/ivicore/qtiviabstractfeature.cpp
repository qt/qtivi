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
#include "qtiviabstractfeature_p.h"

#include "qtiviserviceobject.h"
#include "qtiviservicemanager.h"

#include <QMetaEnum>
#include <QDebug>

QtIVIAbstractFeaturePrivate::QtIVIAbstractFeaturePrivate(const QString &interface, QObject *parent)
    : QObject(parent)
    , m_interface(interface)
    , m_serviceObject(0)
    , m_autoDiscovery(true)
    , m_qmlCreation(false)
{
}

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
   \enum QtIVIAbstractFeature::Error

   \value NoError
          No error
   \value PermissionDenied
          Permission for the operation is denied
   \value InvalidOperation
          Operation is invalid
   \value Timeout
          Operation timeout
   \value InvalidZone
          Zone is not available for the operation
   \value Unknown
          Unknown error
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
    , d_ptr(new QtIVIAbstractFeaturePrivate(interface, this))
{
    qRegisterMetaType<QtIVIAbstractFeature::Error>();
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
    Q_D(QtIVIAbstractFeature);
    if (d->m_serviceObject == so)
        return;

    if (d->m_serviceObject) {
        disconnectFromServiceObject(d->m_serviceObject);
        disconnect(d->m_serviceObject, SIGNAL(destroyed()), this, SLOT(serviceObjectDestroyed()));
    }

    d->m_serviceObject = 0;

    //We only want to call clearServiceObject if we are sure that the serviceObject changes
    if (!so) {
        clearServiceObject();
    } if (so && !acceptServiceObject(so)) {
        qWarning("ServiceObject is not accepted");
        clearServiceObject();
    }

    d->m_serviceObject = so;
    emit serviceObjectChanged();
    emit isValidChanged(isValid());

    if (so) {
        connectToServiceObject(d->m_serviceObject);
        connect(so, SIGNAL(destroyed()), this, SLOT(serviceObjectDestroyed()));
    }
}

/*!
 * \qmlproperty bool AbstractFeature::autoDiscovery
 * \brief True if service objects are located automatically.
 *
 * If automatic discovery is enabled, the feature will search for a suitable backend when
 * \l {QQmlParserStatus::}{componentComplete} is invoked from QML.
 *
 * After component creation the property can be used to start the automatic discsovery later.
 */

/*!
 * \property QtIVIAbstractFeature::autoDiscovery
 * \brief True if service objects are located automatically.
 *
 * If automatic discovery is enabled, the feature will search for a suitable backend.
 *
 * \sa startAutoDiscovery()
 */
void QtIVIAbstractFeature::setAutoDiscovery(bool autoDiscovery)
{
    Q_D(QtIVIAbstractFeature);
    if (d->m_autoDiscovery == autoDiscovery)
        return;

    d->m_autoDiscovery = autoDiscovery;
    emit autoDiscoveryChanged(autoDiscovery);

    if (!d->m_qmlCreation && autoDiscovery)
        startAutoDiscovery();
}

/*!
 * \internal
 * \overload
 */
void QtIVIAbstractFeature::classBegin()
{
    Q_D(QtIVIAbstractFeature);
    d->m_qmlCreation = true;
}

/*!
 * Invoked automatically when used from QML. Calls \l startAutoDiscovery() if \l autoDiscovery is \c true.
 */
void QtIVIAbstractFeature::componentComplete()
{
    Q_D(QtIVIAbstractFeature);
    d->m_qmlCreation = false;
    if (d->m_autoDiscovery) {
        startAutoDiscovery();
    }
}

/*!
 * Returns the interface name.
 */
QString QtIVIAbstractFeature::interfaceName() const
{
    Q_D(const QtIVIAbstractFeature);
    return d->m_interface;
}

QtIVIServiceObject *QtIVIAbstractFeature::serviceObject() const
{
    Q_D(const QtIVIAbstractFeature);
    return d->m_serviceObject;
}

bool QtIVIAbstractFeature::autoDiscovery() const
{
    Q_D(const QtIVIAbstractFeature);
    return d->m_autoDiscovery;
}

/*!
   Sets \a error with the \a message.

   Emits errorChanged() signal.

   \sa QtIVIAbstractZonedFeature::Error
 */
void QtIVIAbstractFeature::setError(QtIVIAbstractFeature::Error error, const QString &message)
{
    Q_D(QtIVIAbstractFeature);
    d->m_error = error;
    d->m_errorMessage = errorText() + QStringLiteral(" ") + message;
    if (d->m_error == QtIVIAbstractFeature::NoError)
        d->m_errorMessage.clear();
    emit errorChanged(d->m_error, d->m_errorMessage);
}

/*!
   Returns the last error code.

   \sa QtIVIAbstractFeature::Error
 */
QtIVIAbstractFeature::Error QtIVIAbstractFeature::error() const
{
    Q_D(const QtIVIAbstractFeature);
    return d->m_error;
}


/*!
   \qmlproperty QString QtIVIAbstractFeature::error

   Last error message of the feature. Empty if no error.
 */
/*!
   \property QtIVIAbstractFeature::error

   Last error message of the feature. Empty if no error.
 */
QString QtIVIAbstractFeature::errorMessage() const
{
    Q_D(const QtIVIAbstractFeature);
    return d->m_errorMessage;
}

/*!
   Returns a string containing the error code.

   Empty if no error.
*/
QString QtIVIAbstractFeature::errorText() const
{
    Q_D(const QtIVIAbstractFeature);
    if (d->m_error == QtIVIAbstractFeature::NoError)
        return QString();
    QMetaEnum metaEnum = QMetaEnum::fromType<QtIVIAbstractFeature::Error>();
    return QLatin1String(metaEnum.valueToKey(d->m_error));
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
    Q_D(QtIVIAbstractFeature);
    if (!d->m_autoDiscovery) {
        d->m_autoDiscovery = true;
        emit autoDiscoveryChanged(true);
    }

    if (d->m_serviceObject) // No need to discover a new backend when we already have one
        return;

    QtIVIServiceManager* serviceManager = QtIVIServiceManager::instance();
    QList<QtIVIServiceObject*> serviceObjects = serviceManager->findServiceByInterface(d->m_interface);

    if (serviceObjects.isEmpty()) {
        qWarning() << "There is no backend implementing" << d->m_interface << ".";
        return;
    }

    if (serviceObjects.count() > 1)
        qWarning() << "There is more than one backend implementing" << d->m_interface << ".";

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
    Q_D(const QtIVIAbstractFeature);
    return d->m_serviceObject != 0;
}

/*!
   Updates \a error and \a message from the backend.

   This slot can be used when implementing a new Feature to report generic errors.
*/
void QtIVIAbstractFeature::onErrorChanged(QtIVIAbstractFeature::Error error, const QString &message)
{
    setError(error, message);
}

void QtIVIAbstractFeature::serviceObjectDestroyed()
{
    Q_D(QtIVIAbstractFeature);
    d->m_serviceObject = 0;
    clearServiceObject();
    emit serviceObjectChanged();
}
