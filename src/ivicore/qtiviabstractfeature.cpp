/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
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

QtIVIAbstractFeaturePrivate::QtIVIAbstractFeaturePrivate(const QString &interface, QtIVIAbstractFeature *parent)
    : QObjectPrivate()
    , q_ptr(parent)
    , m_interface(interface)
    , m_serviceObject(0)
    , m_discoveryMode(QtIVIAbstractFeature::AutoDiscovery)
    , m_discoveryResult(QtIVIAbstractFeature::NoResult)
    , m_error(QtIVIAbstractFeature::NoError)
    , m_qmlCreation(false)
{
}

void QtIVIAbstractFeaturePrivate::setDiscoveryResult(QtIVIAbstractFeature::DiscoveryResult discoveryResult)
{
    if (m_discoveryResult == discoveryResult)
        return;

    m_discoveryResult = discoveryResult;
    Q_Q(QtIVIAbstractFeature);
    emit q->discoveryResultChanged(discoveryResult);
}

/*!
 * \class QtIVIAbstractFeature
 * \inmodule QtIVICore
 * \brief The QtIVIAbstractFeature is the base class for all QtIVI Features
 *
 * QtIVIAbstractFeature is the base class for the front facing API towards the developer.
 * The QtIVIAbstractFeature provides you with a way to automatically connect to a backend implementing
 * the interface needed. This discovery is started by using the startAutoDiscovery() function.
 *
 * Once the auto discovery is done, it can be checked whether a backend has been found by using the
 * isValid function.
 *
 * The auto discovery gives you an easy way to automatically connect to the right backend implementation.
 * If you don't want to use the auto discovery, it's also possible to use QtIVIServiceManager to retrieve
 * all Backends and search manually for the right one and connect it to the QtIVIAbstractFeature by calling
 * setServiceObject().
 *
 * The type of backend to be loaded can be controlled by setting the auto discovery mode. By default,
 * it is set to AutoDiscovery, which indicates that a production backend will be preferred over a simulation backend.
 *
 * QtIVIAbstractFeature is an abstract base class that needs to be subclassed to create an API for your
 * Feature.
 *
 * \chapter Subclassing
 *
 * When subclassing QtIVIAbstractFeature, you must provide implementations of the following functions:
 * \list
 * \li acceptServiceObject()
 * \li connectToServiceObject()
 * \li disconnectFromServiceObject()
 * \li clearServiceObject()
 * \endlist
 *
 * Once a QtIVIServiceObject has been set, either by auto discovery or by manually setting it, the acceptServiceObject()
 * function will be called to make sure the QtIVIServiceObject provides everything needed by the Feature.
 *
 * If the interface provides signals, you need to do all the connect statements in connectToServiceObject() and
 * disconnect them again in disconnectFromServiceObject().
 *
 * clearServiceObject() will be called once the Feature doesn't have a connection to a ServiceObject anymore and should
 * reset its state to sane defaults.
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
   \enum QtIVIAbstractFeature::DiscoveryMode

   \value NoAutoDiscovery
          No Auto Discovery is done and the ServiceObject needs to be set manually
   \value AutoDiscovery
          Tries to first find a production backend with a matching interface and falls back to a simulation backend if not found
   \value LoadOnlyProductionBackends
          Only tries to load a production backend with a matching interface
   \value LoadOnlySimulationBackends
          Only tries to load a simulation backend with a matching interface
 */

/*!
   \enum QtIVIAbstractFeature::DiscoveryResult

   \value NoResult
          Indicates that no auto discovery was started because the feature has already assigned a valid ServiceObject
   \value ErrorWhileLoading
          An error has happened while searching for a backend with a matching interface
   \value ProductionBackendLoaded
          As a result of the auto discovery a production backend was loaded
   \value SimulationBackendLoaded
          As a result of the auto discovery a simulation backend was loaded
 */

/*!
    \qmltype AbstractFeature
    \instantiates QtIVIAbstractFeature
    \inqmlmodule QtIVICore

    \brief The AbstractFeature is not directly accessible. QML type provides
    base QML properties for the feature, like autoDiscovery and isValid.

    Once the AbstracteFeature is instantiated by QML the autoDiscovery will be started automatically.
    To disable this behavior the discoveryMode needs to be set to \c NoDiscovery;
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
 * There is no previous service object connected, as this function call is always preceded by a call to
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
    : QObject(*new QtIVIAbstractFeaturePrivate(interface, this), parent)
{
    qRegisterMetaType<QtIVIAbstractFeature::Error>();
    qRegisterMetaType<QtIVIAbstractFeature::DiscoveryMode>();
    qRegisterMetaType<QtIVIAbstractFeature::DiscoveryResult>();
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
 * actual function is required. This is usually retrieved through the auto discovery mechanism.
 *
 * \sa discoveryMode
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
 * \qmlproperty enumeration AbstractFeature::discoveryMode
 * \brief Holds the mode that is used for the autoDiscovery
 *
 *  Available values are:
 *  \value NoAutoDiscovery
 *         No Auto Discovery is done and the ServiceObject needs to be set manually.
 *  \value AutoDiscovery
 *         Tries to find a production backend with a matching interface and falls back to a simulation backend if not found.
 *  \value LoadOnlyProductionBackends
 *         Only tries to load a production backend with a matching interface.
 *  \value LoadOnlySimulationBackends
 *         Only tries to load a simulation backend with a matching interface.
 *
 *  If needed the auto discovery will be started once the Item is created.
 *
 *  \note If you change this property after the Item is instantiated you need to call startAutoDiscovery() to search for
 *  a new Service Object
 */

/*!
 * \property QtIVIAbstractFeature::discoveryMode
 * \brief Holds the mode that is used for the autoDiscovery
 *
 * \sa startAutoDiscovery()
 */
void QtIVIAbstractFeature::setDiscoveryMode(QtIVIAbstractFeature::DiscoveryMode discoveryMode)
{
    Q_D(QtIVIAbstractFeature);
    if (d->m_discoveryMode == discoveryMode)
        return;

    d->m_discoveryMode = discoveryMode;
    emit discoveryModeChanged(discoveryMode);
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
 * Invoked automatically when used from QML. Calls \l startAutoDiscovery().
 */
void QtIVIAbstractFeature::componentComplete()
{
    Q_D(QtIVIAbstractFeature);
    d->m_qmlCreation = false;
    startAutoDiscovery();
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

QtIVIAbstractFeature::DiscoveryMode QtIVIAbstractFeature::discoveryMode() const
{
    Q_D(const QtIVIAbstractFeature);
    return d->m_discoveryMode;
}

/*!
 *  \qmlproperty enumeration AbstractFeature::autoDiscoveryResult
 *  \brief Holds the result of the last autoDiscovery
 *
 *  Available values are:
 *  \value NoResult
 *         Indicates that no auto discovery was started because the feature has already assigned a valid ServiceObject.
 *  \value ErrorWhileLoading
 *         An error has happened while searching for a backend with a matching interface.
 *  \value ProductionBackendLoaded
 *         As a result of the auto discovery a production backend was loaded.
 *  \value SimulationBackendLoaded
 *         As a result of the auto discovery a simulation backend was loaded.
 */

/*!
 *  \property QtIVIAbstractFeature::discoveryResult
 *  \brief Holds the result of the last autoDiscovery
 *
 *  \sa startAutoDiscovery()
 */
QtIVIAbstractFeature::DiscoveryResult QtIVIAbstractFeature::discoveryResult() const
{
    Q_D(const QtIVIAbstractFeature);
    return d->m_discoveryResult;
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
 *  \qmlmethod enumeration AbstractFeature::startAutoDiscovery()
 *
 * Performs an automatic discovery attempt.
 *
 * The feature will try to locate a single service object implementing the required interface.
 *
 * If no service object is found, the feature will stay invalid. If more than one service object
 * is found, the first instance is used.
 *
 * Either the type of the backend which was loaded or an error is returned.
 *
 * If the discoveryMode is set to QtIVIAbstractFeature::NoAutoDiscovery this function will
 * do nothing and return QtIVIAbstractFeature::NoResult.
 *
 *  Return values are:
 *  \value NoResult
 *         Indicates that no auto discovery was started because the feature has already assigned a valid ServiceObject.
 *  \value ErrorWhileLoading
 *         An error has happened while searching for a backend with a matching interface.
 *  \value ProductionBackendLoaded
 *         As a result of the auto discovery a production backend was loaded.
 *  \value SimulationBackendLoaded
 *         As a result of the auto discovery a simulation backend was loaded.
 */

/*!
 * \brief Performs an automatic discovery attempt.
 *
 * The feature will try to locate a single service object implementing the required interface.
 *
 * If no service object is found, the feature will stay invalid. If more than one service object
 * is found, the first instance is used.
 *
 * Either the type of the backend which was loaded or an error is returned.
 *
 * If the discoveryMode is set to QtIVIAbstractFeature::NoAutoDiscovery this function will
 * do nothing and return QtIVIAbstractFeature::NoResult.
 *
 * \sa discoveryMode()
 */
QtIVIAbstractFeature::DiscoveryResult QtIVIAbstractFeature::startAutoDiscovery()
{
    Q_D(QtIVIAbstractFeature);

     // No need to discover a new backend when we already have one
    if (d->m_serviceObject || d->m_discoveryMode == QtIVIAbstractFeature::NoAutoDiscovery) {
        d->setDiscoveryResult(NoResult);
        return NoResult;
    }

    QtIVIServiceManager* serviceManager = QtIVIServiceManager::instance();
    QList<QtIVIServiceObject*> serviceObjects;
    DiscoveryResult result = NoResult;
    if (d->m_discoveryMode == AutoDiscovery || d->m_discoveryMode == LoadOnlyProductionBackends) {
        serviceObjects = serviceManager->findServiceByInterface(d->m_interface, QtIVIServiceManager::IncludeProductionBackends);
        result = ProductionBackendLoaded;
    }

    if (serviceObjects.isEmpty()) {

        if (Q_UNLIKELY(d->m_discoveryMode == AutoDiscovery || d->m_discoveryMode == LoadOnlyProductionBackends))
            qWarning() << "There is no production backend implementing" << d->m_interface << ".";

        if (d->m_discoveryMode == AutoDiscovery || d->m_discoveryMode == LoadOnlySimulationBackends) {
            serviceObjects = serviceManager->findServiceByInterface(d->m_interface, QtIVIServiceManager::IncludeSimulationBackends);
            result = SimulationBackendLoaded;
            if (Q_UNLIKELY(serviceObjects.isEmpty())) {
                qWarning() << "There is no simulation backend implementing" << d->m_interface << ".";
                d->setDiscoveryResult(ErrorWhileLoading);
                return ErrorWhileLoading;
            }
        } else {
            d->setDiscoveryResult(ErrorWhileLoading);
            return ErrorWhileLoading;
        }
    }

    if (Q_UNLIKELY(serviceObjects.count() > 1))
        qWarning() << "There is more than one backend implementing" << d->m_interface << ".";

    setServiceObject(serviceObjects.at(0));

    d->setDiscoveryResult(result);
    return result;
}

QtIVIAbstractFeature::QtIVIAbstractFeature(QtIVIAbstractFeaturePrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
}

/*!
 * \qmlproperty bool AbstractFeature::isValid
 * \brief Holds whether the feature is ready for use.
 *
 * The property holds \c true if the feature is ready to be used, otherwise \c false. Not being
 * ready usually indicates that no suitable service object could be found, or that automatic
 * discovery has not been triggered.
 *
 * \sa QtIVIServiceObject, discoveryMode
 */
/*!
 * \property QtIVIAbstractFeature::isValid
 * \brief Holds whether the feature is ready to use.
 *
 * The property holds \c true if the feature is ready to be used, otherwise \c false. Not being
 * ready usually indicates that no suitable service object could be found, or that automatic
 * discovery has not been triggered.
 *
 * \sa QtIVIServiceObject, discoveryMode
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
