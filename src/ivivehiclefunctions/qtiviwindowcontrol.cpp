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

#include "qtiviwindowcontrol.h"
#include "qtiviwindowcontrol_p.h"
#include "qtiviwindowcontrolbackendinterface.h"
#include <QtIVICore/QtIVIPropertyFactory>
#include <QtIVICore/QtIVIServiceObject>

QtIVIWindowControlPrivate::QtIVIWindowControlPrivate(const QString &interface, const QString &zone, QtIVIWindowControl *parent)
    : QtIVIAbstractZonedFeaturePrivate(interface, zone, parent)
    , m_heaterMode(QtIVIWindowControl::HeaterOff)
    , m_heater(false)
    , m_blindMode(QtIVIWindowControl::BlindClosed)
    , m_blindState(QtIVIWindowControl::Closed)
    , m_state(QtIVIWindowControl::Closed)
    , q_ptr(parent)
{}

void QtIVIWindowControlPrivate::init()
{
    Q_Q(QtIVIWindowControl);

    m_heaterModeProperty = QtIVIPropertyFactory<QtIVIWindowControl::HeaterMode>::create(q,
                                                                                        &QtIVIWindowControl::heaterModeAttribute,
                                                                                        &QtIVIWindowControl::heaterModeAttributeChanged,
                                                                                        &QtIVIWindowControl::heaterMode,
                                                                                        &QtIVIWindowControl::heaterModeChanged,
                                                                                        &QtIVIWindowControl::setHeaterMode);

    m_heaterProperty = QtIVIPropertyFactory<bool>::create(q,
                                                          &QtIVIWindowControl::heaterAttribute,
                                                          &QtIVIWindowControl::heaterAttributeChanged,
                                                          &QtIVIWindowControl::isHeaterEnabled,
                                                          &QtIVIWindowControl::heaterEnabledChanged);

    m_blindModeProperty = QtIVIPropertyFactory<QtIVIWindowControl::BlindMode>::create(q,
                                                                                      &QtIVIWindowControl::blindModeAttribute,
                                                                                      &QtIVIWindowControl::blindModeAttributeChanged,
                                                                                      &QtIVIWindowControl::blindMode,
                                                                                      &QtIVIWindowControl::blindModeChanged,
                                                                                      &QtIVIWindowControl::setBlindMode);

    m_blindStateProperty = QtIVIPropertyFactory<QtIVIWindowControl::State>::create(q,
                                                                                   &QtIVIWindowControl::blindStateAttribute,
                                                                                   &QtIVIWindowControl::blindStateAttributeChanged,
                                                                                   &QtIVIWindowControl::blindState,
                                                                                   &QtIVIWindowControl::blindStateChanged);

    m_stateProperty = QtIVIPropertyFactory<QtIVIWindowControl::State>::create(q,
                                                                              &QtIVIWindowControl::stateAttribute,
                                                                              &QtIVIWindowControl::stateAttributeChanged,
                                                                              &QtIVIWindowControl::state,
                                                                              &QtIVIWindowControl::stateChanged);

}

void QtIVIWindowControlPrivate::clearToDefaults()
{
    m_heaterMode = QtIVIWindowControl::HeaterOff;
    m_heater = false;
    m_blindMode = QtIVIWindowControl::BlindClosed;
    m_blindState = QtIVIWindowControl::Closed;
    m_state = QtIVIWindowControl::Closed;
}

void QtIVIWindowControlPrivate::onHeaterModeChanged(QtIVIWindowControl::HeaterMode value, const QString &zone)
{
    Q_Q(QtIVIWindowControl);
    QtIVIWindowControl *f = qobject_cast<QtIVIWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;

    f->d_func()->m_heaterMode = value;
    emit f->heaterModeChanged(value);
}

void QtIVIWindowControlPrivate::onHeaterModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode> &attribute, const QString &zone)
{
    Q_Q(QtIVIWindowControl);
    QtIVIWindowControl *f = qobject_cast<QtIVIWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_heaterModeAttribute = attribute;
    emit f->heaterModeAttributeChanged(attribute);
}


void QtIVIWindowControlPrivate::onHeaterEnabledChanged(bool enabled, const QString &zone)
{
    Q_Q(QtIVIWindowControl);
    QtIVIWindowControl *f = qobject_cast<QtIVIWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;

    f->d_func()->m_heater = enabled;
    emit f->heaterEnabledChanged(enabled);
}

void QtIVIWindowControlPrivate::onHeaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone)
{
    Q_Q(QtIVIWindowControl);
    QtIVIWindowControl *f = qobject_cast<QtIVIWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_heaterAttribute = attribute;
    emit f->heaterAttributeChanged(attribute);
}


void QtIVIWindowControlPrivate::onBlindModeChanged(QtIVIWindowControl::BlindMode value, const QString &zone)
{
    Q_Q(QtIVIWindowControl);
    QtIVIWindowControl *f = qobject_cast<QtIVIWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;

    f->d_func()->m_blindMode = value;
    emit f->blindModeChanged(value);
}

void QtIVIWindowControlPrivate::onBlindModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode> &attribute, const QString &zone)
{
    Q_Q(QtIVIWindowControl);
    QtIVIWindowControl *f = qobject_cast<QtIVIWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_blindModeAttribute = attribute;
    emit f->blindModeAttributeChanged(attribute);
}


void QtIVIWindowControlPrivate::onBlindStateChanged(QtIVIWindowControl::State value, const QString &zone)
{
    Q_Q(QtIVIWindowControl);
    QtIVIWindowControl *f = qobject_cast<QtIVIWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;

    f->d_func()->m_blindState = value;
    emit f->blindStateChanged(value);
}

void QtIVIWindowControlPrivate::onBlindStateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &attribute, const QString &zone)
{
    Q_Q(QtIVIWindowControl);
    QtIVIWindowControl *f = qobject_cast<QtIVIWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_blindStateAttribute = attribute;
    emit f->blindStateAttributeChanged(attribute);
}


void QtIVIWindowControlPrivate::onStateChanged(QtIVIWindowControl::State value, const QString &zone)
{
    Q_Q(QtIVIWindowControl);
    QtIVIWindowControl *f = qobject_cast<QtIVIWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;

    f->d_func()->m_state = value;
    emit f->stateChanged(value);
}

void QtIVIWindowControlPrivate::onStateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &attribute, const QString &zone)
{
    Q_Q(QtIVIWindowControl);
    QtIVIWindowControl *f = qobject_cast<QtIVIWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_stateAttribute = attribute;
    emit f->stateAttributeChanged(attribute);
}

QtIVIWindowControlBackendInterface *QtIVIWindowControlPrivate::windowControlBackend()
{
    Q_Q(QtIVIWindowControl);
    return qobject_cast<QtIVIWindowControlBackendInterface*>(q->backend());
}

/*!
   \class QtIVIWindowControl
   \inmodule QtIVIVehicleFunctions
   \brief Provides an interface to the window control.

   The QtIVIWindowControl provides an interface to control the physical windows of the vehicle.

   All properties are exposed through zone objects. The zones are retrieved using the \l zoneAt method.

   The QtIVIWindowControl expects a single backend to be available. It is recommended to use it
   with \l {QtIVIAbstractFeature::}{discoveryMode} set to \l AutoDiscovery.
*/

/*!
    \qmltype WindowControl
    \instantiates QtIVIWindowControl
    \inqmlmodule QtIVIVehicleFunctions
    \inherits AbstractZonedFeature
    \brief The WindowControl provides a QML interface to control the physical windows of the vehicle.
*/

/*!
   \enum QtIVIWindowControl::HeaterMode
   \value HeaterOn
          The window heater is turned on.
   \value HeaterOff
          The window heater is turned off.
   \value AutoHeater
          The window heater is turning off and on automatically.
*/

/*!
   \enum QtIVIWindowControl::BlindMode
   \value BlindOpen
          The blind will be opened.
   \value BlindClosed
          The blind will be closed.
   \value AutoBlind
          The blind is opened or closed automatically.
*/

/*!
   \enum QtIVIWindowControl::State
   \value FullyOpen
          The object is fully open.
   \value Open
          The object is open, but not fully open yet.
   \value Closed
          The object is closed.
*/

/*!
   Constructs a window control object to the given \a zone.

   The \a parent argument is passed on to the \l QtIVIAbstractZonedFeature base class.
*/
QtIVIWindowControl::QtIVIWindowControl(const QString &zone, QObject *parent)
    : QtIVIAbstractZonedFeature(*new QtIVIWindowControlPrivate(QtIVIStringWindowControlInterfaceName, zone, this), parent)
{
    Q_D(QtIVIWindowControl);
    d->init();

    qRegisterMetaType<QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>>();
    qRegisterMetaType<QtIVIPropertyAttribute<bool>>();
    qRegisterMetaType<QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>>();
    qRegisterMetaType<QtIVIPropertyAttribute<QtIVIWindowControl::State>>();
}

QtIVIWindowControl::~QtIVIWindowControl()
{
}

/*!
   \reimp
*/
void QtIVIWindowControl::connectToServiceObject(QtIVIServiceObject *serviceObject)
{
    Q_D(QtIVIWindowControl);
    QtIVIAbstractZonedFeature::connectToServiceObject(serviceObject);

    QtIVIWindowControlBackendInterface* backend = d->windowControlBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QtIVIWindowControlBackendInterface::heaterModeChanged,
                            d, &QtIVIWindowControlPrivate::onHeaterModeChanged);
    QObjectPrivate::connect(backend, &QtIVIWindowControlBackendInterface::heaterModeAttributeChanged,
                            d, &QtIVIWindowControlPrivate::onHeaterModeAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIWindowControlBackendInterface::heaterEnabledChanged,
                            d, &QtIVIWindowControlPrivate::onHeaterEnabledChanged);
    QObjectPrivate::connect(backend, &QtIVIWindowControlBackendInterface::heaterAttributeChanged,
                            d, &QtIVIWindowControlPrivate::onHeaterAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIWindowControlBackendInterface::blindModeChanged,
                            d, &QtIVIWindowControlPrivate::onBlindModeChanged);
    QObjectPrivate::connect(backend, &QtIVIWindowControlBackendInterface::blindModeAttributeChanged,
                            d, &QtIVIWindowControlPrivate::onBlindModeAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIWindowControlBackendInterface::blindStateChanged,
                            d, &QtIVIWindowControlPrivate::onBlindStateChanged);
    QObjectPrivate::connect(backend, &QtIVIWindowControlBackendInterface::blindStateAttributeChanged,
                            d, &QtIVIWindowControlPrivate::onBlindStateAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIWindowControlBackendInterface::stateChanged,
                            d, &QtIVIWindowControlPrivate::onStateChanged);
    QObjectPrivate::connect(backend, &QtIVIWindowControlBackendInterface::stateAttributeChanged,
                            d, &QtIVIWindowControlPrivate::onStateAttributeChanged);

    backend->initializeAttributes();
}

/*!
   \reimp
*/
void QtIVIWindowControl::disconnectFromServiceObject(QtIVIServiceObject *serviceObject)
{
    QtIVIWindowControlBackendInterface *backend = qobject_cast<QtIVIWindowControlBackendInterface*>(serviceObject->interfaceInstance(QtIVIStringWindowControlInterfaceName));

    if (backend)
        disconnect(backend, 0, this, 0);
}

/*!
   \reimp
*/
void QtIVIWindowControl::clearServiceObject()
{
    Q_D(QtIVIWindowControl);
    d->clearToDefaults();
}

QtIVIWindowControl::QtIVIWindowControl(QtIVIWindowControlPrivate &dd, QObject *parent)
    : QtIVIAbstractZonedFeature(dd, parent)
{
    Q_D(QtIVIWindowControl);
    d->init();
}

/*!
   \reimp
*/
QtIVIAbstractZonedFeature *QtIVIWindowControl::createZoneFeature(const QString &zone)
{
    return new QtIVIWindowControl(zone, this);
}

/*!
   \qmlqtivipropertyEnum {QtIVIVehicleFunctions::WindowControl::heaterMode}

   \e value holds the heater mode.
   Available values are:
   \value HeaterOn
          The window heater is turned on.
   \value HeaterOff
          The window heater is turned off.
   \value AutoHeater
          The window heater is turning off and on automatically.
 */
/*!
   \property QtIVIWindowControl::heaterMode

   Holds a QtIVIProperty of type \e QtIVIWindowControl::HeaterMode where the QtIVIProperty::value() function returns the current heaterMode.

   \sa AttributeSystem
   \sa heaterMode() setHeaterMode() heaterModeAttribute()
 */
/*!
 * Returns the current heaterMode.
 *
 * \sa setHeaterMode() heaterModeChanged() heaterModeAttribute()
 */
QtIVIWindowControl::HeaterMode QtIVIWindowControl::heaterMode() const

{
    Q_D(const QtIVIWindowControl);
    return d->m_heaterMode;
}
/*!
 * Returns the attribute defining the boundaries and availability of the heaterMode property.
 *
 * \sa setHeaterMode() heaterMode() heaterModeChanged()
 */
QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode> QtIVIWindowControl::heaterModeAttribute() const
{
    Q_D(const QtIVIWindowControl);
    return d->m_heaterModeAttribute;
}

QtIVIProperty *QtIVIWindowControl::heaterModeProperty() const
{
    Q_D(const QtIVIWindowControl);
    return d->m_heaterModeProperty;
}

/*!
   \qmlqtivipropertyBool {QtIVIVehicleFunctions::WindowControl::heater}

   \e value is true if the heater is currently running.
 */
/*!
   \property QtIVIWindowControl::heater

   Holds a QtIVIProperty of type \e bool where the QtIVIProperty::value() function indicates if the heater is running.

   \sa AttributeSystem
   \sa isHeaterEnabled() heaterAttribute()
 */
/*!
 * Returns true if heater is currently enabled(running).
 *
 * \sa heaterEnabledChanged() heaterAttribute()
 */
bool QtIVIWindowControl::isHeaterEnabled() const

{
    Q_D(const QtIVIWindowControl);
    return d->m_heater;
}
/*!
 * Returns the attribute defining the boundaries and availability of the heater property.
 *
 * \sa isHeaterEnabled() heaterEnabledChanged()
 */
QtIVIPropertyAttribute<bool> QtIVIWindowControl::heaterAttribute() const
{
    Q_D(const QtIVIWindowControl);
    return d->m_heaterAttribute;
}

QtIVIProperty *QtIVIWindowControl::heaterProperty() const
{
    Q_D(const QtIVIWindowControl);
    return d->m_heaterProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIVIVehicleFunctions::WindowControl::blindMode}

   \e value holds the blind mode.
   Available values are:
   \value BlindOpen
          The blind will be opened.
   \value BlindClosed
          The blind will be closed.
   \value AutoBlind
          The blind is opened or closed automatically.
 */
/*!
   \property QtIVIWindowControl::blindMode

   Holds a QtIVIProperty of type \e QtIVIWindowControl::BlindMode where the QtIVIProperty::value() function returns the current blind mode.

   \sa AttributeSystem
   \sa blindMode() setBlindMode() blindModeAttribute()
 */
/*!
 * Returns the current blind mode.
 *
 * \sa setBlindMode() blindModeChanged() blindModeAttribute()
 */
QtIVIWindowControl::BlindMode QtIVIWindowControl::blindMode() const

{
    Q_D(const QtIVIWindowControl);
    return d->m_blindMode;
}
/*!
 * Returns the attribute defining the boundaries and availability of the blindMode property.
 *
 * \sa setBlindMode() blindMode() blindModeChanged()
 */
QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode> QtIVIWindowControl::blindModeAttribute() const
{
    Q_D(const QtIVIWindowControl);
    return d->m_blindModeAttribute;
}

QtIVIProperty *QtIVIWindowControl::blindModeProperty() const
{
    Q_D(const QtIVIWindowControl);
    return d->m_blindModeProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIVIVehicleFunctions::WindowControl::blindState}

   \e value holds the current blind state.
   Available values are:
   \value FullyOpen
          The object is fully open.
   \value Open
          The object is open, but not fully open yet.
   \value Closed
          The object is closed.
 */
/*!
   \property QtIVIWindowControl::blindState

   Holds a QtIVIProperty of type \e QtIVIWindowControl::State where the QtIVIProperty::value() function returns the current blind state.

   \sa AttributeSystem
   \sa blindState() blindStateAttribute()
 */
/*!
 * Returns the current blind state.
 *
 * \sa blindStateChanged() blindStateAttribute()
 */
QtIVIWindowControl::State QtIVIWindowControl::blindState() const

{
    Q_D(const QtIVIWindowControl);
    return d->m_blindState;
}
/*!
 * Returns the attribute defining the boundaries and availability of the blindState property.
 *
 * \sa blindState() blindStateChanged()
 */
QtIVIPropertyAttribute<QtIVIWindowControl::State> QtIVIWindowControl::blindStateAttribute() const
{
    Q_D(const QtIVIWindowControl);
    return d->m_blindStateAttribute;
}

QtIVIProperty *QtIVIWindowControl::blindStateProperty() const
{
    Q_D(const QtIVIWindowControl);
    return d->m_blindStateProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIVIVehicleFunctions::WindowControl::state}

   \e value holds the current window state.
   Available values are:
   \value FullyOpen
          The object is fully open.
   \value Open
          The object is open, but not fully open yet.
   \value Closed
          The object is closed.

   \sa open() close()
 */
/*!
   \property QtIVIWindowControl::state

   Holds a QtIVIProperty of type \e QtIVIWindowControl::State where the QtIVIProperty::value() function returns the current window state.

   \sa AttributeSystem
   \sa state() stateAttribute() open() close()
 */
/*!
 * Returns the current window state.
 *
 * \sa stateChanged() stateAttribute()
 */
QtIVIWindowControl::State QtIVIWindowControl::state() const

{
    Q_D(const QtIVIWindowControl);
    return d->m_state;
}
/*!
 * Returns the attribute defining the boundaries and availability of the state property.
 *
 * \sa state() stateChanged()
 */
QtIVIPropertyAttribute<QtIVIWindowControl::State> QtIVIWindowControl::stateAttribute() const
{
    Q_D(const QtIVIWindowControl);
    return d->m_stateAttribute;
}

QtIVIProperty *QtIVIWindowControl::stateProperty() const
{
    Q_D(const QtIVIWindowControl);
    return d->m_stateProperty;
}

/*!
 * Sets the current heater mode to \a value.
 *
 * \sa heaterMode() heaterModeChanged() heaterModeAttribute()
 */
void QtIVIWindowControl::setHeaterMode(QtIVIWindowControl::HeaterMode value)
{
    Q_D(QtIVIWindowControl);
    if (QtIVIWindowControlBackendInterface* backend = d->windowControlBackend())
        backend->setHeaterMode(value, zone());
}

/*!
 * Sets the current blind mode to \a value.
 *
 * \sa blindMode() blindModeChanged() blindModeAttribute()
 */
void QtIVIWindowControl::setBlindMode(QtIVIWindowControl::BlindMode value)
{
    Q_D(QtIVIWindowControl);
    if (QtIVIWindowControlBackendInterface* backend = d->windowControlBackend())
        backend->setBlindMode(value, zone());
}

/*!
 * \qmlmethod WindowControl::open()
 *
 * Opens the window, if not already in the QtIVIWindowControl::FullyOpen state.
 *
 * \sa WindowControl::state
 */

/*!
 * Opens the window, if not already in the QtIVIWindowControl::FullyOpen state.
 *
 * \sa state() close()
 */
void QtIVIWindowControl::open()
{
    Q_D(QtIVIWindowControl);
    if (QtIVIWindowControlBackendInterface* backend = d->windowControlBackend())
        backend->open(zone());
}

/*!
 * \qmlmethod WindowControl::close()
 *
 * Closes the window, if not already in the QtIVIWindowControl::Closed state.
 *
 * \sa WindowControl::state
 */

/*!
 * Closes the window, if not already in the QtIVIWindowControl::Closed state.
 *
 * \sa state() open()
 */
void QtIVIWindowControl::close()
{
    Q_D(QtIVIWindowControl);
    if (QtIVIWindowControlBackendInterface* backend = d->windowControlBackend())
        backend->close(zone());
}

/*!
 * \fn void QtIVIWindowControl::heaterModeChanged(QtIVIWindowControl::HeaterMode value);
 *
 * This signal is emitted whenever the heater mode changed. The new value is passed as \a value.
 *
 * \sa heaterMode() setHeaterMode() heaterModeAttribute()
 */

/*!
 * \fn void QtIVIWindowControl::heaterModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode> &attribute);
 *
 * This signal is emitted whenever the attribute for the heaterMode property changes. The new attribute is passed as \a attribute.
 *
 * \sa heaterModeAttribute() heaterMode()
 */

/*!
 * \fn void QtIVIWindowControl::heaterEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the heater is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isHeaterEnabled() heaterAttribute()
 */

/*!
 * \fn void QtIVIWindowControl::heaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the heater property changes. The new attribute is passed as \a attribute.
 *
 * \sa heaterAttribute() isHeaterEnabled()
 */

/*!
 * \fn void QtIVIWindowControl::blindModeChanged(QtIVIWindowControl::BlindMode value);
 *
 * This signal is emitted whenever the blind mode changed. The new value is passed as \a value.
 *
 * \sa blindMode() setBlindMode() blindModeAttribute()
 */

/*!
 * \fn void QtIVIWindowControl::blindModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode> &attribute);
 *
 * This signal is emitted whenever the attribute for the blindMode property changes. The new attribute is passed as \a attribute.
 *
 * \sa blindModeAttribute() blindMode()
 */

/*!
 * \fn void QtIVIWindowControl::blindStateChanged(QtIVIWindowControl::State value);
 *
 * This signal is emitted whenever the blind state changed. The new value is passed as \a value.
 *
 * \sa blindState() blindStateAttribute()
 */

/*!
 * \fn void QtIVIWindowControl::blindStateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &attribute);
 *
 * This signal is emitted whenever the attribute for the blindState property changes. The new attribute is passed as \a attribute.
 *
 * \sa blindStateAttribute() blindState()
 */

/*!
 * \fn void QtIVIWindowControl::stateChanged(QtIVIWindowControl::State value);
 *
 * This signal is emitted whenever the state changed. The new value is passed as \a value.
 *
 * \sa state() stateAttribute()
 */

/*!
 * \fn void QtIVIWindowControl::stateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &attribute);
 *
 * This signal is emitted whenever the attribute for the state property changes. The new attribute is passed as \a attribute.
 *
 * \sa stateAttribute() state()
 */



#include "moc_qtiviwindowcontrol.cpp"
