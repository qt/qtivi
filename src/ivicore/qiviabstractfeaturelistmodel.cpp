/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

#include "qiviabstractfeaturelistmodel.h"
#include "qiviabstractfeaturelistmodel_p.h"

QT_BEGIN_NAMESPACE

QIviHelperFeature::QIviHelperFeature(const QString &interface, QIviAbstractFeatureListModel *model)
    : QIviAbstractFeature(interface)
    , m_model(model)
{
}

bool QIviHelperFeature::acceptServiceObject(QIviServiceObject *so)
{
    return m_model->acceptServiceObject(so);
}

bool QIviHelperFeature::acceptServiceObjectDefaultImpl(QIviServiceObject *so)
{
    return QIviAbstractFeature::acceptServiceObject(so);
}

void QIviHelperFeature::connectToServiceObject(QIviServiceObject *so)
{
    m_model->connectToServiceObject(so);
}

void QIviHelperFeature::connectToServiceObjectDefaultImpl(QIviServiceObject *so)
{
    QIviAbstractFeature::connectToServiceObject(so);
}

void QIviHelperFeature::disconnectFromServiceObject(QIviServiceObject *so)
{
    m_model->disconnectFromServiceObject(so);
}

void QIviHelperFeature::disconnectFromServiceObjectDefaultImpl(QIviServiceObject *so)
{
    QIviAbstractFeature::disconnectFromServiceObject(so);
}

void QIviHelperFeature::clearServiceObject()
{
    m_model->clearServiceObject();
}

QIviAbstractFeaturePrivate *QIviHelperFeature::iviPrivate()
{
    Q_D(QIviAbstractFeature);
    return d;
}

const QIviAbstractFeaturePrivate *QIviHelperFeature::iviPrivate() const
{
    Q_D(const QIviAbstractFeature);
    return d;
}

QIviAbstractFeatureListModelPrivate::QIviAbstractFeatureListModelPrivate(const QString &interface, QIviAbstractFeatureListModel *model)
    : QAbstractItemModelPrivate()
    , m_feature(new QIviHelperFeature(interface, model))
    , m_qmlCreation(false)
{

}

QIviAbstractFeatureListModelPrivate::~QIviAbstractFeatureListModelPrivate()
{
    delete m_feature;
}

void QIviAbstractFeatureListModelPrivate::initialize()
{
}

QIviFeatureInterface *QIviAbstractFeatureListModelPrivate::backend() const
{
    return m_feature->iviPrivate()->backend();
}

/*!
    \class QIviAbstractFeatureListModel
    \inmodule QtIviCore
    \brief The QIviAbstractFeatureListModel is the base class for QtIvi Features which should act as a model.

    See QIviAbstractFeature for more details on how a Feature works. This base class is needed to avoid a diamond
    inheritance from QAbstractListModel and QIviAbstractFeature.

    See the \l{Models} section for more information about all models in QtIvi.

    \section1 Subclassing

    //TODO add docs here.
*/

/*!
    \qmltype AbstractFeatureListModel
    \instantiates QIviAbstractFeatureListModel
    \inqmlmodule QtIvi

    \brief The QIviAbstractFeatureListModel is the base class for QtIvi Features which should act as a model.

    See QIviAbstractFeature for more details on how a Feature works. This base class is needed to avoid a diamond
    inheritance from QAbstractListModel and QIviAbstractFeature.

    This element is not directly accessible from QML. It provides the
    base QML properties for the feature, like autoDiscovery and isValid.

    See the \l{Models} section for more information about all models in QtIvi.

    \sa AbstractFeature
*/

/*!
    \fn void QIviAbstractFeatureListModel::clearServiceObject()

    This method is expected to be implemented by any class subclassing QIviAbstractFeatureListModel.

    Called when no service object is available. The implementation is expected to set all
    properties to safe defaults and forget all links to the previous service object.

    \note You must emit the corresponding change signals for these properties, so that the feature
    is informed about the state change. This makes it possible for the implemented class to connect
    to a new service object afterwards.

    There is no need to disconnect from the service object. If it still exists, it is guaranteed
    that \l disconnectFromServiceObject is called first.

    \sa acceptServiceObject(), connectToServiceObject(), disconnectFromServiceObject()
*/

/*!
    Constructs a QIviAbstractFeatureListModel.

    The \a parent argument is passed on to the \l QAbstractListModel base class.

    The \a interface argument is used to locate suitable service objects.
*/
QIviAbstractFeatureListModel::QIviAbstractFeatureListModel(const QString &interface, QObject *parent)
    : QAbstractListModel(*new QIviAbstractFeatureListModelPrivate(interface, this), parent)
{
    Q_D(QIviAbstractFeatureListModel);
    d->initialize();
    connect(d->m_feature, &QIviAbstractFeature::serviceObjectChanged, this, &QIviAbstractFeatureListModel::serviceObjectChanged);
    connect(d->m_feature, &QIviAbstractFeature::discoveryModeChanged, this, &QIviAbstractFeatureListModel::discoveryModeChanged);
    connect(d->m_feature, &QIviAbstractFeature::discoveryResultChanged, this, &QIviAbstractFeatureListModel::discoveryResultChanged);
    connect(d->m_feature, &QIviAbstractFeature::isValidChanged, this, &QIviAbstractFeatureListModel::isValidChanged);
    connect(d->m_feature, &QIviAbstractFeature::isInitializedChanged, this, &QIviAbstractFeatureListModel::isInitializedChanged);
    connect(d->m_feature, &QIviAbstractFeature::errorChanged, this, &QIviAbstractFeatureListModel::errorChanged);
}

/*!
    \qmlproperty ServiceObject AbstractFeatureListModel::serviceObject
    \brief Sets the service object for the feature.

    As features only expose the front API facing the developer, a service object implementing the
    actual function is required. This is usually retrieved through the auto discovery mechanism.

    The setter for this property returns false if the \e {Service Object} is already set to exactly this instance
    or the \e {Service Object} doesn't get accepted by the feature.

    \sa discoveryMode
*/

/*!
    \property QIviAbstractFeatureListModel::serviceObject
    \brief Sets the service object for the feature.

    As features only expose the front API facing the developer, a service object implementing the
    actual function is required. This is usually retrieved through the auto discovery mechanism.

    The setter for this property returns false if the \e {Service Object} is already set to exactly this instance
    or the \e {Service Object} doesn't get accepted by the feature.

    \sa discoveryMode
*/
QIviServiceObject *QIviAbstractFeatureListModel::serviceObject() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->serviceObject();
}

/*!
    \qmlproperty enumeration AbstractFeatureListModel::discoveryMode
    \brief Holds the mode that is used for the autoDiscovery

    Available values are:
    \value NoAutoDiscovery
           No auto discovery is done and the ServiceObject needs to be set manually.
    \value AutoDiscovery
           Tries to find a production backend with a matching interface and falls back to a simulation backend if not found.
    \value LoadOnlyProductionBackends
           Only tries to load a production backend with a matching interface.
    \value LoadOnlySimulationBackends
           Only tries to load a simulation backend with a matching interface.

    If needed the auto discovery will be started once the Feature creation is completed.

    \note If you change this property after the Feature is instantiated you need to call startAutoDiscovery() to search for
    a new Service Object
*/

/*!
    \property QIviAbstractFeatureListModel::discoveryMode
    \brief Holds the mode that is used for the autoDiscovery

    \note If you change this property after the Feature is instantiated you need to call startAutoDiscovery() to search for
    a new Service Object
*/
QIviAbstractFeature::DiscoveryMode QIviAbstractFeatureListModel::discoveryMode() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->discoveryMode();
}

/*!
    \qmlproperty enumeration AbstractFeatureListModel::discoveryResult
    \brief The result of the last autoDiscovery attempt

    Available values are:
    \value NoResult
           Indicates that no auto discovery was started because the feature has already assigned a valid ServiceObject.
    \value ErrorWhileLoading
           An error has happened while searching for a backend with a matching interface.
    \value ProductionBackendLoaded
           As a result of the auto discovery a production backend was loaded.
    \value SimulationBackendLoaded
           As a result of the auto discovery a simulation backend was loaded.
*/

/*!
    \property QIviAbstractFeatureListModel::discoveryResult
    \brief The result of the last autoDiscovery attempt

    \sa startAutoDiscovery()
*/
QIviAbstractFeature::DiscoveryResult QIviAbstractFeatureListModel::discoveryResult() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->discoveryResult();
}

/*!
    \qmlproperty bool AbstractFeatureListModel::isValid
    \brief Indicates whether the feature is ready for use.

    The property is \c true if the feature is ready to be used, otherwise \c false. Not being
    ready usually indicates that no suitable service object could be found, or that automatic
    discovery has not been triggered.

    The backend still might not have sent all properties yet and is not fully initialized.
    Use isInitialized instead to know when the feature holds all correct values.

    \sa QIviServiceObject, discoveryMode, isInitialized
*/
/*!
    \property QIviAbstractFeatureListModel::isValid
    \brief Indicates whether the feature is ready to use.

    The property is \c true if the feature is ready to be used, otherwise \c false. Not being
    ready usually indicates that no suitable service object could be found, or that automatic
    discovery has not been triggered.

    The backend still might not have sent all properties yet and is not fully initialized.
    Use isInitialized instead to know when the feature holds all correct values.

    \sa QIviServiceObject, discoveryMode, isInitialized
*/
bool QIviAbstractFeatureListModel::isValid() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->isValid();
}

/*!
    \qmlproperty bool AbstractFeatureListModel::isInitialized
    \brief Indicates whether the feature has been initialized with all the values from the backend.

    The property is \c true once the backend sends the QIviFeatureInterface::initializationDone signal
    to indicate that all values have now been initialized with values from the backend.

    \sa isValid, QIviFeatureInterface::initializationDone
*/
/*!
    \property QIviAbstractFeatureListModel::isInitialized
    \brief Indicates whether the feature has been initialized with all the values from the backend.

    The property is \c true once the backend sends the QIviFeatureInterface::initializationDone signal
    to indicate that all values have now been initialized with values from the backend.

    \sa isValid, QIviFeatureInterface::initializationDone
*/
bool QIviAbstractFeatureListModel::isInitialized() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->isInitialized();
}

/*!
    Returns the last error code.

    \sa QIviAbstractFeature::Error
*/
QIviAbstractFeature::Error QIviAbstractFeatureListModel::error() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->error();
}

/*!
    \qmlproperty string AbstractFeatureListModel::error

    Last error message of the feature. Empty if no error.
*/
/*!
    \property QIviAbstractFeatureListModel::error

    Last error message of the feature. Empty if no error.
*/
QString QIviAbstractFeatureListModel::errorMessage() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->errorMessage();
}

bool QIviAbstractFeatureListModel::setServiceObject(QIviServiceObject *so)
{
    Q_D(QIviAbstractFeatureListModel);
    return d->m_feature->setServiceObject(so);
}

void QIviAbstractFeatureListModel::setDiscoveryMode(QIviAbstractFeature::DiscoveryMode discoveryMode)
{
    Q_D(QIviAbstractFeatureListModel);
    d->m_feature->setDiscoveryMode(discoveryMode);
}

/*!
    \qmlmethod enumeration AbstractFeatureListModel::startAutoDiscovery()

    Performs an automatic discovery attempt.

    See AbstractFeature::startAutoDiscovery() for more information
*/

/*!
    \brief Performs an automatic discovery attempt.
*/
QIviAbstractFeature::DiscoveryResult QIviAbstractFeatureListModel::startAutoDiscovery()
{
    Q_D(QIviAbstractFeatureListModel);
    return d->m_feature->startAutoDiscovery();
}

/*!
    \internal
*/
QIviAbstractFeatureListModel::QIviAbstractFeatureListModel(QIviAbstractFeatureListModelPrivate &dd, QObject *parent)
    : QAbstractListModel(dd, parent)
{
    Q_D(QIviAbstractFeatureListModel);
    d->initialize();
    connect(d->m_feature, &QIviAbstractFeature::serviceObjectChanged, this, &QIviAbstractFeatureListModel::serviceObjectChanged);
    connect(d->m_feature, &QIviAbstractFeature::discoveryModeChanged, this, &QIviAbstractFeatureListModel::discoveryModeChanged);
    connect(d->m_feature, &QIviAbstractFeature::discoveryResultChanged, this, &QIviAbstractFeatureListModel::discoveryResultChanged);
    connect(d->m_feature, &QIviAbstractFeature::isValidChanged, this, &QIviAbstractFeatureListModel::isValidChanged);
    connect(d->m_feature, &QIviAbstractFeature::isInitializedChanged, this, &QIviAbstractFeatureListModel::isInitializedChanged);
    connect(d->m_feature, &QIviAbstractFeature::errorChanged, this, &QIviAbstractFeatureListModel::errorChanged);
}

/*!
    This method is expected to be implemented by any class subclassing QIviAbstractFeature.

    The method should return \c true if the given \a serviceObject is accepted and
    can be used, otherwise \c false.

    If the object is accepted, \l connectToServiceObject is called to actually connect to the
    service object.

    The default implementation accepts the \a serviceObject if it implements the interface
    returned by interfaceName();

    \sa connectToServiceObject(), disconnectFromServiceObject(), clearServiceObject()
*/
bool QIviAbstractFeatureListModel::acceptServiceObject(QIviServiceObject *serviceObject)
{
    Q_D(QIviAbstractFeatureListModel);
    return d->m_feature->acceptServiceObjectDefaultImpl(serviceObject);
}

/*!
    This method is expected to be implemented by any class subclassing QIviAbstractFeature.

    The implementation should connect to the \a serviceObject, and set up all
    properties to reflect the state of the service object.

    There is no previous service object connected, as this function call is always preceded by a call to
    \l disconnectFromServiceObject or \l clearServiceObject.

    It is safe to assume that the \a serviceObject, has always been accepted through the
    \l acceptServiceObject method prior to being passed to this method.

    The default implementation connects to the signals offered by QIviFeatureInterface and calls
    QIviFeatureInterface::initialize() afterwards.

    When reimplementing please keep in mind to connect all signals before calling this function. e.g.

    /code
    void SimpleFeature::connectToServiceObject(QIviServiceObject *serviceObject)
    {
         SimpleFeatureBackendInterface *backend = backend(serviceObject);
         if (!backend)
             return;

         // connect your signals
         connect(backend, &SimpleFeatureBackendInterface::propertyChanged,
                 this, &SimpleFeature::onPropertyChanged);

         // connects the base signals and call initialize()
         QIviAbstractFeature::connectToServiceObject(serviceObject);

         // Additional initialization functions can be added here
    }
    /endcode

    \sa acceptServiceObject(), disconnectFromServiceObject(), clearServiceObject()
*/
void QIviAbstractFeatureListModel::connectToServiceObject(QIviServiceObject *serviceObject)
{
    Q_D(QIviAbstractFeatureListModel);
    d->m_feature->connectToServiceObjectDefaultImpl(serviceObject);
}

/*!
    This method is expected to be implemented by any class subclassing QIviAbstractFeature.

    The implementation should disconnect all connections to the \a serviceObject.

    There is no need to reset internal variables to safe defaults. A call to this function is
    always followed by a call to \l connectToServiceObject or \l clearServiceObject.

    The default implementation disconnects all signals from the serviceObject to this instance.

    \sa acceptServiceObject(), connectToServiceObject(), clearServiceObject()
*/
void QIviAbstractFeatureListModel::disconnectFromServiceObject(QIviServiceObject *serviceObject)
{
    Q_D(QIviAbstractFeatureListModel);
    d->m_feature->disconnectFromServiceObjectDefaultImpl(serviceObject);
}

/*!
    \internal
*/
void QIviAbstractFeatureListModel::classBegin()
{
    Q_D(QIviAbstractFeatureListModel);
    d->m_qmlCreation = true;
}

/*!
    \internal
*/
void QIviAbstractFeatureListModel::componentComplete()
{
    Q_D(QIviAbstractFeatureListModel);
    d->m_qmlCreation = false;
    startAutoDiscovery();
}

/*!
    Returns the interface name this Feature expect to be available from the Service Object and this Feature is implementing.

    See \l {Extending Qt IVI} for more information.
*/
QString QIviAbstractFeatureListModel::interfaceName() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->interfaceName();
}

/*!
    Returns the current error code converted from QIviAbstractFeature::Error to QString

    \sa error
*/
QString QIviAbstractFeatureListModel::errorText() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->errorText();
}

/*!
    Sets \a error with the \a message.

    Emits errorChanged() signal.

    \sa QIviAbstractFeature::Error
*/
void QIviAbstractFeatureListModel::setError(QIviAbstractFeature::Error error, const QString &message)
{
    Q_D(QIviAbstractFeatureListModel);
    d->m_feature->setError(error, message);
}

/*!
    Updates \a error and \a message from the backend.

    This slot can be used when implementing a new Feature to report generic errors.
*/
void QIviAbstractFeatureListModel::onErrorChanged(QIviAbstractFeature::Error error, const QString &message)
{
    setError(error, message);
}

QT_END_NAMESPACE
