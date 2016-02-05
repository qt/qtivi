/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
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

#include "qiviabstractfeature.h"
#include "qiviabstractfeature_p.h"

#include "qiviserviceobject.h"
#include "qiviservicemanager.h"

#include <QMetaEnum>
#include <QDebug>

QIviAbstractFeaturePrivate::QIviAbstractFeaturePrivate(const QString &interface, QIviAbstractFeature *parent)
    : QObjectPrivate()
    , q_ptr(parent)
    , m_interface(interface)
    , m_serviceObject(0)
    , m_discoveryMode(QIviAbstractFeature::AutoDiscovery)
    , m_discoveryResult(QIviAbstractFeature::NoResult)
    , m_error(QIviAbstractFeature::NoError)
    , m_qmlCreation(false)
{
}

void QIviAbstractFeaturePrivate::setDiscoveryResult(QIviAbstractFeature::DiscoveryResult discoveryResult)
{
    if (m_discoveryResult == discoveryResult)
        return;

    m_discoveryResult = discoveryResult;
    Q_Q(QIviAbstractFeature);
    emit q->discoveryResultChanged(discoveryResult);
}

/*!
 * \class QIviAbstractFeature
 * \inmodule QtIviCore
 * \brief The QIviAbstractFeature is the base class for all QtIvi Features
 *
 * QIviAbstractFeature is the base class for the front facing API towards the developer.
 * The QIviAbstractFeature provides you with a way to automatically connect to a backend implementing
 * the interface needed. This discovery is started by using the startAutoDiscovery() function.
 *
 * Once the auto discovery is done, it can be checked whether a backend has been found by using the
 * isValid function.
 *
 * The auto discovery gives you an easy way to automatically connect to the right backend implementation.
 * If you don't want to use the auto discovery, it's also possible to use QIviServiceManager to retrieve
 * all Backends and search manually for the right one and connect it to the QIviAbstractFeature by calling
 * setServiceObject().
 *
 * The type of backend to be loaded can be controlled by setting the auto discovery mode. By default,
 * it is set to AutoDiscovery, which indicates that a production backend will be preferred over a simulation backend.
 *
 * QIviAbstractFeature is an abstract base class that needs to be subclassed to create an API for your
 * Feature.
 *
 * \chapter Subclassing
 *
 * When subclassing QIviAbstractFeature, you must provide implementations of the following functions:
 * \list
 * \li acceptServiceObject()
 * \li connectToServiceObject()
 * \li disconnectFromServiceObject()
 * \li clearServiceObject()
 * \endlist
 *
 * Once a QIviServiceObject has been set, either by auto discovery or by manually setting it, the acceptServiceObject()
 * function will be called to make sure the QIviServiceObject provides everything needed by the Feature.
 *
 * If the interface provides signals, you need to do all the connect statements in connectToServiceObject() and
 * disconnect them again in disconnectFromServiceObject().
 *
 * clearServiceObject() will be called once the Feature doesn't have a connection to a ServiceObject anymore and should
 * reset its state to sane defaults.
 */

/*!
   \enum QIviAbstractFeature::Error

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
   \enum QIviAbstractFeature::DiscoveryMode

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
   \enum QIviAbstractFeature::DiscoveryResult

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
    \instantiates QIviAbstractFeature
    \inqmlmodule QtIvi

    \brief The AbstractFeature is not directly accessible. QML type provides
    base QML properties for the feature, like autoDiscovery and isValid.

    Once the AbstracteFeature is instantiated by QML the autoDiscovery will be started automatically.
    To disable this behavior the discoveryMode needs to be set to \c NoDiscovery;
*/

/*!
 * \fn bool QIviAbstractFeature::acceptServiceObject(QIviServiceObject *so)
 *
 * This method is expected to be implemented by any class subclassing QIviAbstractFeature.
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
 * \fn void QIviAbstractFeature::connectToServiceObject(QIviServiceObject *so)
 *
 * This method is expected to be implemented by any class subclassing QIviAbstractFeature.
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
 * \fn void QIviAbstractFeature::disconnectFromServiceObject(QIviServiceObject *so)
 *
 * This method is expected to be implemented by any class subclassing QIviAbstractFeature.
 *
 * The implementation is expected to disconnect from the service object, \a so.
 *
 * It is not expected that the implementation goes to safe defaults. A call to this function is
 * always followed by a call to \l connectToServiceObject or \l clearServiceObject.
 *
 * \sa acceptServiceObject(), connectToServiceObject(), clearServiceObject()
 */

/*!
 * \fn void QIviAbstractFeature::clearServiceObject()
 *
 * This method is expected to be implemented by any class subclassing QIviAbstractFeature.
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
QIviAbstractFeature::QIviAbstractFeature(const QString &interface, QObject *parent)
    : QObject(*new QIviAbstractFeaturePrivate(interface, this), parent)
{
    qRegisterMetaType<QIviAbstractFeature::Error>();
    qRegisterMetaType<QIviAbstractFeature::DiscoveryMode>();
    qRegisterMetaType<QIviAbstractFeature::DiscoveryResult>();
}

/*!
 * Destructor.
 */
QIviAbstractFeature::~QIviAbstractFeature()
{

}

/*!
 * \property QIviAbstractFeature::serviceObject
 * \brief Sets the service object for the feature.
 *
 * As features only expose the front API facing the developer, a service object implementing the
 * actual function is required. This is usually retrieved through the auto discovery mechanism.
 *
 * Returns false if so is already set or the so doesn't get accepted by the feature.
 *
 * \sa discoveryMode
 */
bool QIviAbstractFeature::setServiceObject(QIviServiceObject *so)
{
    Q_D(QIviAbstractFeature);
    if (d->m_serviceObject == so)
        return false;

    bool serviceObjectIsSet = d->m_serviceObject;
    if (d->m_serviceObject) {
        disconnectFromServiceObject(d->m_serviceObject);
        disconnect(d->m_serviceObject, SIGNAL(destroyed()), this, SLOT(serviceObjectDestroyed()));
    }

    d->m_serviceObject = 0;

    //We only want to call clearServiceObject if we are sure that the serviceObject changes
    if (!so) {
        clearServiceObject();
    } else if (Q_UNLIKELY(so && !acceptServiceObject(so))) {
        qWarning("ServiceObject is not accepted");
        clearServiceObject();

        if (serviceObjectIsSet) {
            emit serviceObjectChanged();
            emit isValidChanged(isValid());
        }
        return false;
    }

    d->m_serviceObject = so;
    emit serviceObjectChanged();
    emit isValidChanged(isValid());

    if (so) {
        connectToServiceObject(d->m_serviceObject);
        connect(so, SIGNAL(destroyed()), this, SLOT(serviceObjectDestroyed()));
    }

    return true;
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
 * \property QIviAbstractFeature::discoveryMode
 * \brief Holds the mode that is used for the autoDiscovery
 *
 * \sa startAutoDiscovery()
 */
void QIviAbstractFeature::setDiscoveryMode(QIviAbstractFeature::DiscoveryMode discoveryMode)
{
    Q_D(QIviAbstractFeature);
    if (d->m_discoveryMode == discoveryMode)
        return;

    d->m_discoveryMode = discoveryMode;
    emit discoveryModeChanged(discoveryMode);
}

/*!
 * \internal
 * \overload
 */
void QIviAbstractFeature::classBegin()
{
    Q_D(QIviAbstractFeature);
    d->m_qmlCreation = true;
}

/*!
 * Invoked automatically when used from QML. Calls \l startAutoDiscovery().
 */
void QIviAbstractFeature::componentComplete()
{
    Q_D(QIviAbstractFeature);
    d->m_qmlCreation = false;
    startAutoDiscovery();
}

/*!
 * Returns the interface name.
 */
QString QIviAbstractFeature::interfaceName() const
{
    Q_D(const QIviAbstractFeature);
    return d->m_interface;
}

QIviServiceObject *QIviAbstractFeature::serviceObject() const
{
    Q_D(const QIviAbstractFeature);
    return d->m_serviceObject;
}

QIviAbstractFeature::DiscoveryMode QIviAbstractFeature::discoveryMode() const
{
    Q_D(const QIviAbstractFeature);
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
 *  \property QIviAbstractFeature::discoveryResult
 *  \brief Holds the result of the last autoDiscovery
 *
 *  \sa startAutoDiscovery()
 */
QIviAbstractFeature::DiscoveryResult QIviAbstractFeature::discoveryResult() const
{
    Q_D(const QIviAbstractFeature);
    return d->m_discoveryResult;
}

/*!
   Sets \a error with the \a message.

   Emits errorChanged() signal.

   \sa QIviAbstractZonedFeature::Error
 */
void QIviAbstractFeature::setError(QIviAbstractFeature::Error error, const QString &message)
{
    Q_D(QIviAbstractFeature);
    d->m_error = error;
    d->m_errorMessage = errorText() + QStringLiteral(" ") + message;
    if (d->m_error == QIviAbstractFeature::NoError)
        d->m_errorMessage.clear();
    emit errorChanged(d->m_error, d->m_errorMessage);
}

/*!
   Returns the last error code.

   \sa QIviAbstractFeature::Error
 */
QIviAbstractFeature::Error QIviAbstractFeature::error() const
{
    Q_D(const QIviAbstractFeature);
    return d->m_error;
}


/*!
   \qmlproperty QString QIviAbstractFeature::error

   Last error message of the feature. Empty if no error.
 */
/*!
   \property QIviAbstractFeature::error

   Last error message of the feature. Empty if no error.
 */
QString QIviAbstractFeature::errorMessage() const
{
    Q_D(const QIviAbstractFeature);
    return d->m_errorMessage;
}

/*!
   Returns a string containing the error code.

   Empty if no error.
*/
QString QIviAbstractFeature::errorText() const
{
    Q_D(const QIviAbstractFeature);
    if (d->m_error == QIviAbstractFeature::NoError)
        return QString();
    QMetaEnum metaEnum = QMetaEnum::fromType<QIviAbstractFeature::Error>();
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
 * If the discoveryMode is set to QIviAbstractFeature::NoAutoDiscovery this function will
 * do nothing and return QIviAbstractFeature::NoResult.
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
 * If the discoveryMode is set to QIviAbstractFeature::NoAutoDiscovery this function will
 * do nothing and return QIviAbstractFeature::NoResult.
 *
 * \sa discoveryMode()
 */
QIviAbstractFeature::DiscoveryResult QIviAbstractFeature::startAutoDiscovery()
{
    Q_D(QIviAbstractFeature);

     // No need to discover a new backend when we already have one
    if (d->m_serviceObject || d->m_discoveryMode == QIviAbstractFeature::NoAutoDiscovery) {
        d->setDiscoveryResult(NoResult);
        return NoResult;
    }

    QIviServiceManager* serviceManager = QIviServiceManager::instance();
    QList<QIviServiceObject*> serviceObjects;
    DiscoveryResult result = NoResult;
    if (d->m_discoveryMode == AutoDiscovery || d->m_discoveryMode == LoadOnlyProductionBackends) {
        serviceObjects = serviceManager->findServiceByInterface(d->m_interface, QIviServiceManager::IncludeProductionBackends);
        result = ProductionBackendLoaded;
    }

    if (serviceObjects.isEmpty()) {

        if (Q_UNLIKELY(d->m_discoveryMode == AutoDiscovery || d->m_discoveryMode == LoadOnlyProductionBackends))
            qWarning() << "There is no production backend implementing" << d->m_interface << ".";

        if (d->m_discoveryMode == AutoDiscovery || d->m_discoveryMode == LoadOnlySimulationBackends) {
            serviceObjects = serviceManager->findServiceByInterface(d->m_interface, QIviServiceManager::IncludeSimulationBackends);
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

    bool serviceObjectSet = false;
    foreach (QIviServiceObject* object, serviceObjects) {
        if (setServiceObject(object)) {
            serviceObjectSet = true;
            break;
        }
    }

    if (Q_UNLIKELY(!serviceObjectSet)) {
        qWarning() << "No ServiceObject got accepted.";
        d->setDiscoveryResult(ErrorWhileLoading);
        return ErrorWhileLoading;
    }

    d->setDiscoveryResult(result);
    return result;
}

QIviAbstractFeature::QIviAbstractFeature(QIviAbstractFeaturePrivate &dd, QObject *parent)
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
 * \sa QIviServiceObject, discoveryMode
 */
/*!
 * \property QIviAbstractFeature::isValid
 * \brief Holds whether the feature is ready to use.
 *
 * The property holds \c true if the feature is ready to be used, otherwise \c false. Not being
 * ready usually indicates that no suitable service object could be found, or that automatic
 * discovery has not been triggered.
 *
 * \sa QIviServiceObject, discoveryMode
 */
bool QIviAbstractFeature::isValid() const
{
    Q_D(const QIviAbstractFeature);
    return d->m_serviceObject != 0;
}

/*!
   Updates \a error and \a message from the backend.

   This slot can be used when implementing a new Feature to report generic errors.
*/
void QIviAbstractFeature::onErrorChanged(QIviAbstractFeature::Error error, const QString &message)
{
    setError(error, message);
}

void QIviAbstractFeature::serviceObjectDestroyed()
{
    Q_D(QIviAbstractFeature);
    d->m_serviceObject = 0;
    clearServiceObject();
    emit serviceObjectChanged();
}
