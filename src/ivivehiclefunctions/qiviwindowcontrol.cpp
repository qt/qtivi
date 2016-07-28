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

#include "qiviwindowcontrol.h"
#include "qiviwindowcontrol_p.h"
#include "qiviwindowcontrolbackendinterface.h"
#include <QtIviCore/QIviPropertyFactory>
#include <QtIviCore/QIviServiceObject>

QIviWindowControlPrivate::QIviWindowControlPrivate(const QString &interface, const QString &zone, QIviWindowControl *parent)
    : QIviAbstractZonedFeaturePrivate(interface, zone, parent)
    , m_heaterMode(QIviWindowControl::HeaterOff)
    , m_heater(false)
    , m_blindMode(QIviWindowControl::BlindClosed)
    , m_blindState(QIviWindowControl::Closed)
    , m_state(QIviWindowControl::Closed)
    , q_ptr(parent)
{}

void QIviWindowControlPrivate::init()
{
    Q_Q(QIviWindowControl);

    m_heaterModeProperty = QIviPropertyFactory<QIviWindowControl::HeaterMode>::create(q,
                                                                                        &QIviWindowControl::heaterModeAttribute,
                                                                                        &QIviWindowControl::heaterModeAttributeChanged,
                                                                                        &QIviWindowControl::heaterMode,
                                                                                        &QIviWindowControl::heaterModeChanged,
                                                                                        &QIviWindowControl::setHeaterMode);

    m_heaterProperty = QIviPropertyFactory<bool>::create(q,
                                                          &QIviWindowControl::heaterAttribute,
                                                          &QIviWindowControl::heaterAttributeChanged,
                                                          &QIviWindowControl::isHeaterEnabled,
                                                          &QIviWindowControl::heaterEnabledChanged);

    m_blindModeProperty = QIviPropertyFactory<QIviWindowControl::BlindMode>::create(q,
                                                                                      &QIviWindowControl::blindModeAttribute,
                                                                                      &QIviWindowControl::blindModeAttributeChanged,
                                                                                      &QIviWindowControl::blindMode,
                                                                                      &QIviWindowControl::blindModeChanged,
                                                                                      &QIviWindowControl::setBlindMode);

    m_blindStateProperty = QIviPropertyFactory<QIviWindowControl::State>::create(q,
                                                                                   &QIviWindowControl::blindStateAttribute,
                                                                                   &QIviWindowControl::blindStateAttributeChanged,
                                                                                   &QIviWindowControl::blindState,
                                                                                   &QIviWindowControl::blindStateChanged);

    m_stateProperty = QIviPropertyFactory<QIviWindowControl::State>::create(q,
                                                                              &QIviWindowControl::stateAttribute,
                                                                              &QIviWindowControl::stateAttributeChanged,
                                                                              &QIviWindowControl::state,
                                                                              &QIviWindowControl::stateChanged);

}

void QIviWindowControlPrivate::clearToDefaults()
{
    m_heaterMode = QIviWindowControl::HeaterOff;
    m_heater = false;
    m_blindMode = QIviWindowControl::BlindClosed;
    m_blindState = QIviWindowControl::Closed;
    m_state = QIviWindowControl::Closed;
}

void QIviWindowControlPrivate::onHeaterModeChanged(QIviWindowControl::HeaterMode value, const QString &zone)
{
    Q_Q(QIviWindowControl);
    QIviWindowControl *f = qobject_cast<QIviWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;

    f->d_func()->m_heaterMode = value;
    emit f->heaterModeChanged(value);
}

void QIviWindowControlPrivate::onHeaterModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::HeaterMode> &attribute, const QString &zone)
{
    Q_Q(QIviWindowControl);
    QIviWindowControl *f = qobject_cast<QIviWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_heaterModeAttribute = attribute;
    emit f->heaterModeAttributeChanged(attribute);
}


void QIviWindowControlPrivate::onHeaterEnabledChanged(bool enabled, const QString &zone)
{
    Q_Q(QIviWindowControl);
    QIviWindowControl *f = qobject_cast<QIviWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;

    f->d_func()->m_heater = enabled;
    emit f->heaterEnabledChanged(enabled);
}

void QIviWindowControlPrivate::onHeaterAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone)
{
    Q_Q(QIviWindowControl);
    QIviWindowControl *f = qobject_cast<QIviWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_heaterAttribute = attribute;
    emit f->heaterAttributeChanged(attribute);
}


void QIviWindowControlPrivate::onBlindModeChanged(QIviWindowControl::BlindMode value, const QString &zone)
{
    Q_Q(QIviWindowControl);
    QIviWindowControl *f = qobject_cast<QIviWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;

    f->d_func()->m_blindMode = value;
    emit f->blindModeChanged(value);
}

void QIviWindowControlPrivate::onBlindModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::BlindMode> &attribute, const QString &zone)
{
    Q_Q(QIviWindowControl);
    QIviWindowControl *f = qobject_cast<QIviWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_blindModeAttribute = attribute;
    emit f->blindModeAttributeChanged(attribute);
}


void QIviWindowControlPrivate::onBlindStateChanged(QIviWindowControl::State value, const QString &zone)
{
    Q_Q(QIviWindowControl);
    QIviWindowControl *f = qobject_cast<QIviWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;

    f->d_func()->m_blindState = value;
    emit f->blindStateChanged(value);
}

void QIviWindowControlPrivate::onBlindStateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute, const QString &zone)
{
    Q_Q(QIviWindowControl);
    QIviWindowControl *f = qobject_cast<QIviWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_blindStateAttribute = attribute;
    emit f->blindStateAttributeChanged(attribute);
}


void QIviWindowControlPrivate::onStateChanged(QIviWindowControl::State value, const QString &zone)
{
    Q_Q(QIviWindowControl);
    QIviWindowControl *f = qobject_cast<QIviWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;

    f->d_func()->m_state = value;
    emit f->stateChanged(value);
}

void QIviWindowControlPrivate::onStateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute, const QString &zone)
{
    Q_Q(QIviWindowControl);
    QIviWindowControl *f = qobject_cast<QIviWindowControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_stateAttribute = attribute;
    emit f->stateAttributeChanged(attribute);
}

QIviWindowControlBackendInterface *QIviWindowControlPrivate::windowControlBackend()
{
    Q_Q(QIviWindowControl);
    return qobject_cast<QIviWindowControlBackendInterface*>(q->backend());
}

/*!
   \class QIviWindowControl
   \inmodule QtIviVehicleFunctions
   \brief Provides an interface to the window control.

   The QIviWindowControl provides an interface to control the physical windows of the vehicle.

   All properties are exposed through zone objects. The zones are retrieved using the \l zoneAt method.

   The QIviWindowControl expects a single backend to be available. It is recommended to use it
   with \l {QIviAbstractFeature::}{discoveryMode} set to \l AutoDiscovery.
*/

/*!
    \qmltype WindowControl
    \instantiates QIviWindowControl
    \inqmlmodule QtIvi.VehicleFunctions
    \inherits AbstractZonedFeature
    \brief The WindowControl provides a QML interface to control the physical windows of the vehicle.
*/

/*!
   \enum QIviWindowControl::HeaterMode
   \value HeaterOn
          The window heater is turned on.
   \value HeaterOff
          The window heater is turned off.
   \value AutoHeater
          The window heater is turning off and on automatically.
*/

/*!
   \enum QIviWindowControl::BlindMode
   \value BlindOpen
          The blind will be opened.
   \value BlindClosed
          The blind will be closed.
   \value AutoBlind
          The blind is opened or closed automatically.
*/

/*!
   \enum QIviWindowControl::State
   \value FullyOpen
          The object is fully open.
   \value Open
          The object is open, but not fully open yet.
   \value Closed
          The object is closed.
*/

/*!
   Constructs a window control object to the given \a zone.

   The \a parent argument is passed on to the \l QIviAbstractZonedFeature base class.
*/
QIviWindowControl::QIviWindowControl(const QString &zone, QObject *parent)
    : QIviAbstractZonedFeature(*new QIviWindowControlPrivate(QLatin1String(QIviWindowControl_iid), zone, this), parent)
{
    Q_D(QIviWindowControl);
    d->init();

    qRegisterMetaType<QIviPropertyAttribute<QIviWindowControl::HeaterMode>>();
    qRegisterMetaType<QIviPropertyAttribute<QIviWindowControl::BlindMode>>();
    qRegisterMetaType<QIviPropertyAttribute<QIviWindowControl::State>>();
}

QIviWindowControl::~QIviWindowControl()
{
}

/*!
   \reimp
*/
void QIviWindowControl::connectToServiceObject(QIviServiceObject *serviceObject)
{
    Q_D(QIviWindowControl);
    QIviAbstractZonedFeature::connectToServiceObject(serviceObject);

    QIviWindowControlBackendInterface *backend = d->windowControlBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIviWindowControlBackendInterface::heaterModeChanged,
                            d, &QIviWindowControlPrivate::onHeaterModeChanged);
    QObjectPrivate::connect(backend, &QIviWindowControlBackendInterface::heaterModeAttributeChanged,
                            d, &QIviWindowControlPrivate::onHeaterModeAttributeChanged);
    QObjectPrivate::connect(backend, &QIviWindowControlBackendInterface::heaterEnabledChanged,
                            d, &QIviWindowControlPrivate::onHeaterEnabledChanged);
    QObjectPrivate::connect(backend, &QIviWindowControlBackendInterface::heaterAttributeChanged,
                            d, &QIviWindowControlPrivate::onHeaterAttributeChanged);
    QObjectPrivate::connect(backend, &QIviWindowControlBackendInterface::blindModeChanged,
                            d, &QIviWindowControlPrivate::onBlindModeChanged);
    QObjectPrivate::connect(backend, &QIviWindowControlBackendInterface::blindModeAttributeChanged,
                            d, &QIviWindowControlPrivate::onBlindModeAttributeChanged);
    QObjectPrivate::connect(backend, &QIviWindowControlBackendInterface::blindStateChanged,
                            d, &QIviWindowControlPrivate::onBlindStateChanged);
    QObjectPrivate::connect(backend, &QIviWindowControlBackendInterface::blindStateAttributeChanged,
                            d, &QIviWindowControlPrivate::onBlindStateAttributeChanged);
    QObjectPrivate::connect(backend, &QIviWindowControlBackendInterface::stateChanged,
                            d, &QIviWindowControlPrivate::onStateChanged);
    QObjectPrivate::connect(backend, &QIviWindowControlBackendInterface::stateAttributeChanged,
                            d, &QIviWindowControlPrivate::onStateAttributeChanged);

    backend->initializeAttributes();
}

/*!
   \reimp
*/
void QIviWindowControl::disconnectFromServiceObject(QIviServiceObject *serviceObject)
{
    QIviWindowControlBackendInterface *backend = qobject_cast<QIviWindowControlBackendInterface*>(serviceObject->interfaceInstance(QLatin1String(QIviWindowControl_iid)));

    if (backend)
        disconnect(backend, 0, this, 0);
}

/*!
   \reimp
*/
void QIviWindowControl::clearServiceObject()
{
    Q_D(QIviWindowControl);
    d->clearToDefaults();
    QIviAbstractZonedFeature::clearServiceObject();
}

QIviWindowControl::QIviWindowControl(QIviWindowControlPrivate &dd, QObject *parent)
    : QIviAbstractZonedFeature(dd, parent)
{
    Q_D(QIviWindowControl);
    d->init();
}

/*!
   \reimp
*/
QIviAbstractZonedFeature *QIviWindowControl::createZoneFeature(const QString &zone)
{
    return new QIviWindowControl(zone, this);
}

/*!
   \qmlqtivipropertyEnum {QtIvi.VehicleFunctions::WindowControl::heaterMode}

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
   \property QIviWindowControl::heaterMode

   Holds a QIviProperty of type \e QIviWindowControl::HeaterMode where the QIviProperty::value() function returns the current heaterMode.

   \sa AttributeSystem
   \sa heaterMode() setHeaterMode() heaterModeAttribute()
 */
/*!
 * Returns the current heaterMode.
 *
 * \sa setHeaterMode() heaterModeChanged() heaterModeAttribute()
 */
QIviWindowControl::HeaterMode QIviWindowControl::heaterMode() const

{
    Q_D(const QIviWindowControl);
    return d->m_heaterMode;
}
/*!
 * Returns the attribute defining the boundaries and availability of the heaterMode property.
 *
 * \sa setHeaterMode() heaterMode() heaterModeChanged()
 */
QIviPropertyAttribute<QIviWindowControl::HeaterMode> QIviWindowControl::heaterModeAttribute() const
{
    Q_D(const QIviWindowControl);
    return d->m_heaterModeAttribute;
}

QIviProperty *QIviWindowControl::heaterModeProperty() const
{
    Q_D(const QIviWindowControl);
    return d->m_heaterModeProperty;
}

/*!
   \qmlqtivipropertyBool {QtIvi.VehicleFunctions::WindowControl::heater}

   \e value is true if the heater is currently running.
 */
/*!
   \property QIviWindowControl::heater

   Holds a QIviProperty of type \e bool where the QIviProperty::value() function indicates if the heater is running.

   \sa AttributeSystem
   \sa isHeaterEnabled() heaterAttribute()
 */
/*!
 * Returns true if heater is currently enabled(running).
 *
 * \sa heaterEnabledChanged() heaterAttribute()
 */
bool QIviWindowControl::isHeaterEnabled() const

{
    Q_D(const QIviWindowControl);
    return d->m_heater;
}
/*!
 * Returns the attribute defining the boundaries and availability of the heater property.
 *
 * \sa isHeaterEnabled() heaterEnabledChanged()
 */
QIviPropertyAttribute<bool> QIviWindowControl::heaterAttribute() const
{
    Q_D(const QIviWindowControl);
    return d->m_heaterAttribute;
}

QIviProperty *QIviWindowControl::heaterProperty() const
{
    Q_D(const QIviWindowControl);
    return d->m_heaterProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIvi.VehicleFunctions::WindowControl::blindMode}

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
   \property QIviWindowControl::blindMode

   Holds a QIviProperty of type \e QIviWindowControl::BlindMode where the QIviProperty::value() function returns the current blind mode.

   \sa AttributeSystem
   \sa blindMode() setBlindMode() blindModeAttribute()
 */
/*!
 * Returns the current blind mode.
 *
 * \sa setBlindMode() blindModeChanged() blindModeAttribute()
 */
QIviWindowControl::BlindMode QIviWindowControl::blindMode() const

{
    Q_D(const QIviWindowControl);
    return d->m_blindMode;
}
/*!
 * Returns the attribute defining the boundaries and availability of the blindMode property.
 *
 * \sa setBlindMode() blindMode() blindModeChanged()
 */
QIviPropertyAttribute<QIviWindowControl::BlindMode> QIviWindowControl::blindModeAttribute() const
{
    Q_D(const QIviWindowControl);
    return d->m_blindModeAttribute;
}

QIviProperty *QIviWindowControl::blindModeProperty() const
{
    Q_D(const QIviWindowControl);
    return d->m_blindModeProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIvi.VehicleFunctions::WindowControl::blindState}

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
   \property QIviWindowControl::blindState

   Holds a QIviProperty of type \e QIviWindowControl::State where the QIviProperty::value() function returns the current blind state.

   \sa AttributeSystem
   \sa blindState() blindStateAttribute()
 */
/*!
 * Returns the current blind state.
 *
 * \sa blindStateChanged() blindStateAttribute()
 */
QIviWindowControl::State QIviWindowControl::blindState() const

{
    Q_D(const QIviWindowControl);
    return d->m_blindState;
}
/*!
 * Returns the attribute defining the boundaries and availability of the blindState property.
 *
 * \sa blindState() blindStateChanged()
 */
QIviPropertyAttribute<QIviWindowControl::State> QIviWindowControl::blindStateAttribute() const
{
    Q_D(const QIviWindowControl);
    return d->m_blindStateAttribute;
}

QIviProperty *QIviWindowControl::blindStateProperty() const
{
    Q_D(const QIviWindowControl);
    return d->m_blindStateProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIvi.VehicleFunctions::WindowControl::state}

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
   \property QIviWindowControl::state

   Holds a QIviProperty of type \e QIviWindowControl::State where the QIviProperty::value() function returns the current window state.

   \sa AttributeSystem
   \sa state() stateAttribute() open() close()
 */
/*!
 * Returns the current window state.
 *
 * \sa stateChanged() stateAttribute()
 */
QIviWindowControl::State QIviWindowControl::state() const

{
    Q_D(const QIviWindowControl);
    return d->m_state;
}
/*!
 * Returns the attribute defining the boundaries and availability of the state property.
 *
 * \sa state() stateChanged()
 */
QIviPropertyAttribute<QIviWindowControl::State> QIviWindowControl::stateAttribute() const
{
    Q_D(const QIviWindowControl);
    return d->m_stateAttribute;
}

QIviProperty *QIviWindowControl::stateProperty() const
{
    Q_D(const QIviWindowControl);
    return d->m_stateProperty;
}

/*!
 * Sets the current heater mode to \a value.
 *
 * \sa heaterMode() heaterModeChanged() heaterModeAttribute()
 */
void QIviWindowControl::setHeaterMode(QIviWindowControl::HeaterMode value)
{
    Q_D(QIviWindowControl);
    if (QIviWindowControlBackendInterface *backend = d->windowControlBackend())
        backend->setHeaterMode(value, zone());
}

/*!
 * Sets the current blind mode to \a value.
 *
 * \sa blindMode() blindModeChanged() blindModeAttribute()
 */
void QIviWindowControl::setBlindMode(QIviWindowControl::BlindMode value)
{
    Q_D(QIviWindowControl);
    if (QIviWindowControlBackendInterface *backend = d->windowControlBackend())
        backend->setBlindMode(value, zone());
}

/*!
 * \qmlmethod WindowControl::open()
 *
 * Opens the window, if not already in the QIviWindowControl::FullyOpen state.
 *
 * \sa WindowControl::state
 */

/*!
 * Opens the window, if not already in the QIviWindowControl::FullyOpen state.
 *
 * \sa state() close()
 */
void QIviWindowControl::open()
{
    Q_D(QIviWindowControl);
    if (QIviWindowControlBackendInterface *backend = d->windowControlBackend())
        backend->open(zone());
}

/*!
 * \qmlmethod WindowControl::close()
 *
 * Closes the window, if not already in the QIviWindowControl::Closed state.
 *
 * \sa WindowControl::state
 */

/*!
 * Closes the window, if not already in the QIviWindowControl::Closed state.
 *
 * \sa state() open()
 */
void QIviWindowControl::close()
{
    Q_D(QIviWindowControl);
    if (QIviWindowControlBackendInterface *backend = d->windowControlBackend())
        backend->close(zone());
}

/*!
 * \fn void QIviWindowControl::heaterModeChanged(QIviWindowControl::HeaterMode value);
 *
 * This signal is emitted whenever the heater mode changed. The new value is passed as \a value.
 *
 * \sa heaterMode() setHeaterMode() heaterModeAttribute()
 */

/*!
 * \fn void QIviWindowControl::heaterModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::HeaterMode> &attribute);
 *
 * This signal is emitted whenever the attribute for the heaterMode property changes. The new attribute is passed as \a attribute.
 *
 * \sa heaterModeAttribute() heaterMode()
 */

/*!
 * \fn void QIviWindowControl::heaterEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the heater is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isHeaterEnabled() heaterAttribute()
 */

/*!
 * \fn void QIviWindowControl::heaterAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the heater property changes. The new attribute is passed as \a attribute.
 *
 * \sa heaterAttribute() isHeaterEnabled()
 */

/*!
 * \fn void QIviWindowControl::blindModeChanged(QIviWindowControl::BlindMode value);
 *
 * This signal is emitted whenever the blind mode changed. The new value is passed as \a value.
 *
 * \sa blindMode() setBlindMode() blindModeAttribute()
 */

/*!
 * \fn void QIviWindowControl::blindModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::BlindMode> &attribute);
 *
 * This signal is emitted whenever the attribute for the blindMode property changes. The new attribute is passed as \a attribute.
 *
 * \sa blindModeAttribute() blindMode()
 */

/*!
 * \fn void QIviWindowControl::blindStateChanged(QIviWindowControl::State value);
 *
 * This signal is emitted whenever the blind state changed. The new value is passed as \a value.
 *
 * \sa blindState() blindStateAttribute()
 */

/*!
 * \fn void QIviWindowControl::blindStateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute);
 *
 * This signal is emitted whenever the attribute for the blindState property changes. The new attribute is passed as \a attribute.
 *
 * \sa blindStateAttribute() blindState()
 */

/*!
 * \fn void QIviWindowControl::stateChanged(QIviWindowControl::State value);
 *
 * This signal is emitted whenever the state changed. The new value is passed as \a value.
 *
 * \sa state() stateAttribute()
 */

/*!
 * \fn void QIviWindowControl::stateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute);
 *
 * This signal is emitted whenever the attribute for the state property changes. The new attribute is passed as \a attribute.
 *
 * \sa stateAttribute() state()
 */



#include "moc_qiviwindowcontrol.cpp"
