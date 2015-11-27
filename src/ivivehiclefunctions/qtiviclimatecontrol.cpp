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

#include "qtiviclimatecontrol.h"
#include "qtiviclimatecontrolbackendinterface.h"

/*!
 * \class QtIVIClimateControlBackendInterface
 * \inmodule QtIVIVehicleFunctions
 * \ingroup backends
 * \inherits QtIVIVehicleBackendInterface
 * \brief The QtIVIClimateControlBackendInterface defines the interface for backends to the
 * QtIVIClimateControl feature class.
 *
 * The QtIVIClimateControlBackendInterface is the interface used by \l QtIVIClimateControl
 *
 * The interface is discovered by a \l QtIVIClimateControl object, which connects to it and sets up
 * the connections to it.
 *
 * \sa QtIVIClimateControl
 */

/*!
 * \fn QtIVIClimateControlBackendInterface::QtIVIClimateControlBackendInterface(QObject *parent=0)
 *
 * Constructs a backend interface.
 *
 * The \a parent is sent to the QObject constructor.
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setTargetTemperature(int value, const QString &zone) = 0
 *
 * Sets the target temperature of \a zone to \a value, wheret he \a value is expressed in
 * centigrades and may be range limited by the backend.
 *
 * This method is expected to emit a \l targetTemperatureChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa targetTemperature(), targetTemperatureChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setSeatCooler(int value, const QString &zone) = 0
 *
 * Sets the seat ventilation level of \a zone to \a value, where \a value can be \c 0 (off) or
 * between \c 1 (least cool) to \c 10 (coolest).
 *
 * This method is expected to emit a \l seatCoolerChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa seatCooler(), seatCoolerChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setSeatHeater(int value, const QString &zone) = 0;
 *
 * Sets the seat heater level of \a zone to \a value, where \a value can be \c 0 (off) or between
 * \c 1 (least warm) to \c 10 (warmest).
 *
 * This method is expected to emit a \l seatHeaterChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa seatHeater(), seatHeaterChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setAirflowDirection(QtIVIClimateControl::AirflowDirection, const QString &zone) = 0;
 *
 * Sets the air flow direction to \a airflowDirection.
 *
 * This method is expected to emit the \l airflowDirectionChanged() signal when receiving a
 * new \a airflowDirection.
 *
 * \sa airflowDirection(), airflowDirectionChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setAirConditioningEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the air conditioning based on \a enabled.
 *
 * This method is expected to emit the \l airConditioningEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa airConditioningEnabled(), airConditioningEnabledChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setHeaterEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the heater based on \a enabled.
 *
 * This method is expected to emit the \l heaterEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa heaterEnabled(), heaterEnabledChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setAirRecirculationEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the air recirculation based on \a enabled.
 *
 * This method is expected to emit the \l airRecirculationEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa airRecirculationEnabled(), airRecirculationEnabledChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setSteeringWheelHeater(int value, const QString &zone) = 0
 *
 * Sets the steering wheel heater level of \a zone to \a value, where \a value can be \c 0 (off)
 * or between \c 1 (least warm) to \c 10 (warmest).
 *
 * This method is expected to emit a \l steeringWheelHeaterChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa steeringWheelHeater(), steeringWheelHeaterChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setFanSpeedLevel(int value, const QString &zone) = 0
 *
 * Sets the fan speed level of \a zone to \a value, where \a value can be \c 0 (off) or between
 * \c 1 (weakest) to \c 10 (strongest).
 *
 * This method is expected to emit a \l fanSpeedLevelChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa fanSpeedLevel(), fanSpeedLevelChanged()
 */

/*!
 * \fn virtual int QtIVIClimateControlBackendInterface::targetTemperature(const QString &zone) = 0
 *
 * Returns the target temperature in centigrades for \a zone.
 *
 * \sa setTargetTemperature(), targetTemperatureChanged()
 */

/*!
 * \fn virtual int QtIVIClimateControlBackendInterface::seatCooler(const QString &zone) const = 0
 *
 * Returns the seat ventilation level for \a zone, where the level can be \c 0 (off) or
 * between \c 1 (least cool) to \c 10 (coolest).
 *
 * \sa setSeatCooler(), seatCoolerChanged()
 */

/*!
 * \fn virtual int QtIVIClimateControlBackendInterface::seatHeater(const QString &zone) const = 0
 *
 * Returns the seat heater level for \a zone, where the level can be \c 0 (off) or between
 * \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa setSeatHeater(), seatHeaterChanged()
 */

/*!
 * \fn virtual QtIVIClimateControl::AirflowDirection QtIVIClimateControlBackendInterface::airflowDirection(const QString &zone) const = 0
 *
 * Returns the airflow direction.
 *
 * \sa setAirflowDirection(), airflowDirectionChanged()
 */

/*!
 * \fn virtual bool QtIVIClimateControlBackendInterface::airConditioningEnabled(const QString &zone) const = 0
 *
 * Returns \c true if the air conditioning is enabled, otherwise \c false.
 *
 * \sa setAirConditioningEnabled(), airConditioningEnabledChanged()
 */

/*!
 * \fn virtual bool QtIVIClimateControlBackendInterface::heaterEnabled(const QString &zone) const = 0
 *
 * Returns \c true if the air conditioning is enabled, otherwise \c false.
 *
 * \sa setHeaterEnabled(), heaterEnabledChanged()
 */

/*!
 * \fn virtual bool QtIVIClimateControlBackendInterface::airRecirculationEnabled(const QString &zone) const = 0
 *
 * Returns \c true if the air conditioning is enabled, otherwise \c false.
 *
 * \sa setAirRecirculationEnabled(), airRecirculationEnabledChanged()
 */

/*!
 * \fn virtual int QtIVIClimateControlBackendInterface::steeringWheelHeater(const QString &zone) const = 0
 *
 * Returns the steering wheel heater level of \a zone, where the level can be \c 0 (off) or between
 * \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa setSteeringWheelHeater(), steeringWheelHeaterChanged()
 */

/*!
 * \fn int virtual QtIVIClimateControlBackendInterface::fanSpeedLevel(const QString &zone) const = 0
 *
 * Returns the fan speed level of \a zone, where the level can be \c 0 (off) or between
 * \c 1 (weakest) to \c 10 (strongest).
 *
 * \sa setFanSpeedLevel(), fanSpeedLevelChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::targetTemperatureChanged(int value, const QString &zone) = 0
 *
 * The signal is emitted when the target temperature for \a zone is changed to \a value, where
 * value is expressed in centigrades.
 *
 * \sa targetTemperature(), setTargetTemperature()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::seatCoolerChanged(int value, const QString &zone) = 0
 *
 * The signal is emitted when the seat cooler level is changed for \a zone to \a value, where the
 * \a value can be \c 0 (off) or between \c 1 (least cool) to \c 10 (coolest).
 *
 * \sa seatCooler(), setSeatCooler()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::seatHeaterChanged(int value, const QString &zone) = 0
 *
 * The signal is emitted when the seat cooler level is changed for \a zone to \a value, where the
 * \a value can be \c 0 (off) or between \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa seatHeater(), setSeatHeater()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::airflowDirectionChanged(QtIVIClimateControl::AirflowDirection airflowDirection, const QString &zone) = 0
 *
 * The signal is emitted when the airflow direction is changed to \a airflowDirection.
 *
 * \sa airflowDirection(), setAirflowDirection()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::airConditioningEnabledChanged(bool enabled, const QString &zone) = 0
 *
 * The signal is emitted when the air conditioning state is changed to \a enabled.
 *
 * \sa airConditioningEnabled(), setAirConditioningEnabled()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::heaterEnabledChanged(bool enabled, const QString &zone) = 0
 *
 * The signal is emitted whent he heater state is changed to \a enabled.
 *
 * \sa heaterEnabled(), setHeaterEnabled()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged(bool enabled, const QString &zone) = 0
 *
 * The signal is emitted when the air recirculation state is changed to \a enabled.
 *
 * \sa airRecirculationEnabled(), setAirRecirculationEnabled()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::steeringWheelHeaterChanged(int level, const QString &zone) = 0
 *
 * The signals ie emitted when the steering wheel heater level of \a zone is changed to \a level, where the
 * \a level can be \c 0 (off) or between \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa steeringWheelHeater(), setSteeringWheelHeater()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::fanSpeedLevelChanged(int level, const QString &zone) = 0
 *
 * The signals ie emitted when the fan speel level of \a zone is changed to \a level, where the \a level
 * can be \c 0 (off) or between \c 1 (weakest) to \c 10 (strongest).
 *
 * \sa fanSpeedLevel(), setFanSpeedLevel()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::targetTemperatureChanged(int value, const QString &zone = QString())
 *
 * This signal is triggered when backend is updated the \a value of the \a zone. \a zone value is optional. If it's
 * empty it updates general feature value without zone.
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::seatCoolerChanged(int value, const QString &zone = QString());
 *
 * This signal is triggered when backend is updated the \a value of the \a zone. \a zone value is optional. If it's
 * empty it updates general feature value without zone.
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::seatHeaterChanged(int value, const QString &zone = QString());
 *
 * This signal is triggered when backend is updated the \a value of the \a zone. \a zone value is optional. If it's
 * empty it updates general feature value without zone.
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::teeringWheelHeaterChanged(int value, const QString &zone = QString());
 *
 * This signal is triggered when backend is updated the \a value of the \a zone. \a zone value is optional. If it's
 * empty it updates general feature value without zone.
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::fanSpeedLevelChanged(int value, const QString &zone = QString());
 *
 * This signal is triggered when backend is updated the \a value of the \a zone. \a zone value is optional. If it's
 * empty it updates general feature value without zone.
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::airflowDirectionChanged(QtIVIClimateControl::AirflowDirection value, const QString &zone = QString());
 *
 * This signal is triggered when backend is updated the \a value of the \a zone. \a zone value is optional. If it's
 * empty it updates general feature value without zone.
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::airConditioningEnabledChanged(bool value, const QString &zone = QString());
 *
 * This signal is triggered when backend is updated the \a value of the \a zone. \a zone value is optional. If it's
 * empty it updates general feature value without zone.
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::heaterEnabledChanged(bool value, const QString &zone = QString());
 *
 * This signal is triggered when backend is updated the \a value of the \a zone. \a zone value is optional. If it's
 * empty it updates general feature value without zone.
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged(bool value, const QString &zone = QString())
 *
 * This signal is triggered when backend is updated the \a value of the \a zone. \a zone value is optional. If it's
 * empty it updates general feature value without zone.
 */

/*!
 * \class QtIVIClimateControl
 * \inmodule QtIVIVehicleFunctions
 * \brief Provides an interface to the climate control.
 *
 * The QtIVIClimateControl provides an interface to the climate control of the vehicle.
 *
 * The climate control properties are divided into two categories: central or zoned. The central
 * properties are exposed directly through the QtIVIClimateControl and the zoned properties are
 * exposed through zone objects. The zones are retrieved using the \l zoneAt method.
 *
 * The QtIVIClimateControl expects a single backend to be available. It is recommended to use it
 * with \l {QtIVIAbstractFeature::autoDiscovery} {autoDiscovery} enabled.
 *
 */

/*!
    \qmltype ClimateControl
    \instantiates QtIVIClimateControl
    \inqmlmodule QtIVIVehicleFunctions 1.0
    \inherits AbstractZonedFeature
    \brief The ClimateControl provides a QML interface to the climate control of the vehicle
*/

/*!
 * \enum QtIVIClimateControl::AirflowDirection
 * \value FloorPanel
 *        Direct airflow along the floor panel.
 * \value FloorDuct
 *        Direct airflow through the floor duct.
 * \value BiLevel
 *        Direct airflow at both levels.
 * \value DefrostFloor
 *        Direct airflow to defrost.
 */

/*!
    Constructs climate control feature by General zone
*/
QtIVIClimateControl::QtIVIClimateControl(QObject* parent)
    : QtIVIClimateControl(QString(), parent)
{
}

/*!
 * \brief QtIVIClimateControl::QtIVIClimateControl
 * Constructs a climate control object.
 *
 * The \a parent argument is passed on to the \l QtIVIAbstractZonedFeature base class.
 */
QtIVIClimateControl::QtIVIClimateControl(const QString &zone, bool autoDiscovery, QObject* parent)
    : QtIVIAbstractZonedFeature(QtIVIStringClimateControlInterfaceName, zone, autoDiscovery, parent)
    , m_airConditioning(false)
    , m_heater(false)
    , m_airRecirculation(false)
    , m_targetTemperature(0)
    , m_seatCooler(0)
    , m_seatHeater(0)
    , m_steeringWheelHeater(0)
    , m_fanSpeedLevel(0)
{
}

QtIVIClimateControlBackendInterface *QtIVIClimateControl::climateControlBackend()
{
    return qobject_cast<QtIVIClimateControlBackendInterface*>(backend());
}

void QtIVIClimateControl::connectToServiceObject(QtIVIServiceObject *serviceObject)
{
    QtIVIAbstractZonedFeature::connectToServiceObject(serviceObject);

    QtIVIClimateControlBackendInterface* backend = climateControlBackend();
    if (!backend)
        return;

    connect(backend, &QtIVIClimateControlBackendInterface::targetTemperatureChanged,
            this, &QtIVIClimateControl::onTargetTemperatureChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::seatCoolerChanged,
            this, &QtIVIClimateControl::onSeatCoolerChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::seatHeaterChanged,
            this, &QtIVIClimateControl::onSeatHeaterChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::steeringWheelHeaterChanged,
            this, &QtIVIClimateControl::onSteeringWheelHeaterChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::fanSpeedLevelChanged,
            this, &QtIVIClimateControl::onFanSpeedLevelChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airflowDirectionChanged,
            this, &QtIVIClimateControl::onAirflowDirectionChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airConditioningEnabledChanged,
            this, &QtIVIClimateControl::onAirConditioningEnabledChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::heaterEnabledChanged,
            this, &QtIVIClimateControl::onHeaterEnabledChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged,
            this, &QtIVIClimateControl::onAirRecirculationEnabledChanged);

    backend->initializeAttributes();
}

void QtIVIClimateControl::clearServiceObject()
{
    m_airConditioning = false;
    m_heater = false;
    m_airRecirculation = false;
    m_targetTemperature = 0;
    m_seatCooler = 0;
    m_seatHeater = 0;
    m_steeringWheelHeater = 0;
    m_fanSpeedLevel = 0;
}

void QtIVIClimateControl::onAirflowDirectionChanged(QtIVIClimateControl::AirflowDirection value, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->m_airflowDirection = value;
    emit f->airflowDirectionChanged(value);
}

void QtIVIClimateControl::onAirConditioningEnabledChanged(bool enabled, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->m_airConditioning = enabled;
    emit f->airConditioningEnabledChanged(enabled);
}

void QtIVIClimateControl::onHeaterEnabledChanged(bool enabled, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->m_heater = enabled;
    emit f->heaterEnabledChanged(enabled);
}

void QtIVIClimateControl::onAirRecirculationEnabledChanged(bool enabled, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->m_airRecirculation = enabled;
    emit f->airRecirculationEnabledChanged(enabled);
}

void QtIVIClimateControl::onSteeringWheelHeaterChanged(int value, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->m_steeringWheelHeater = value;
    emit f->steeringWheelHeaterChanged(value);
}

void QtIVIClimateControl::onFanSpeedLevelChanged(int value, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->m_fanSpeedLevel = value;
    emit f->fanSpeedLevelChanged(value);
}

void QtIVIClimateControl::onTargetTemperatureChanged(int temperature, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->m_targetTemperature = temperature;
    emit f->targetTemperatureChanged(temperature);
}

void QtIVIClimateControl::onSeatCoolerChanged(int value, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->m_seatCooler = value;
    emit f->seatCoolerChanged(value);
}

void QtIVIClimateControl::onSeatHeaterChanged(int value, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->m_seatHeater = value;
    emit f->seatHeaterChanged(value);
}

/*!
    Create new climate control feature instance for the \a zone
*/
QtIVIAbstractZonedFeature *QtIVIClimateControl::createZoneFeature(const QString &zone)
{
    return new QtIVIClimateControl(zone, false, this);
}

/*!
 * \qmlproperty bool ClimateControl::airConditioning
 * \property QtIVIClimateControl::airConditioning
 * \brief True if the air conditioning is enabled.
 */
bool QtIVIClimateControl::isAirConditioningEnabled() const
{
    return m_airConditioning;
}

/*!
 * \qmlproperty QtIVIClimateControl::AirflowDirection ClimateControl::airflowDirection
 * \property QtIVIClimateControl::airflowDirection
 * \brief The airflow direction.
 *
 * \sa QtIVIClimateControl::AirflowDirection
 */
QtIVIClimateControl::AirflowDirection QtIVIClimateControl::airflowDirection() const
{
    return m_airflowDirection;
}

/*!
 * \qmlproperty bool ClimateControl::heater
 * \property QtIVIClimateControl::heater
 * \brief True if the heater is enabled.
 */
bool QtIVIClimateControl::isHeaterEnabled() const
{
    return m_heater;
}

/*!
 * \qmlproperty bool ClimateControl::airRecirculation
 * \property QtIVIClimateControl::airRecirculation
 * \brief True if the air recirculation is enabled.
 */
bool QtIVIClimateControl::isAirRecirculationEnabled() const
{
    return m_airRecirculation;
}

/*!
 * \qmlproperty int ClimateZone::steeringWheelHeater
 * \property QtIVIClimateControl::steeringWheelHeater
 * \brief The steering wheel heater level.
 *
 * The steering wheel heater level, where the level can be \c 0 (off) or between \c 1 (least warm)
 * to \c 10 (warmest).
 */
int QtIVIClimateControl::steeringWheelHeater() const
{
    return m_steeringWheelHeater;
}

/*!
 * \qmlproperty int ClimateControl::fanSpeedLevel
 * \property QtIVIClimateControl::fanSpeedLevel
 * \brief The fan speed level.
 *
 * The fan speed level, where the level can be \c 0 (off) or between \c 1 (weakest) to \c 10
 * (strongest).
 */
int QtIVIClimateControl::fanSpeedLevel() const
{
    return m_fanSpeedLevel;
}

/*!
 * \qmlproperty int ClimateControl::targetTemperature
 * \property QtIVIClimateControl::targetTemperature
 * \brief The target temperature of the zone.
 *
 * The target temperature of the zone expressed in centigrades.
 */
int QtIVIClimateControl::targetTemperature() const
{
    return m_targetTemperature;
}

/*!
 * \qmlproperty int ClimateControl::seatCooler
 * \property QtIVIClimateControl::seatCooler
 * \brief The seat cooler level of the zone.
 *
 * The seat cooler level of the zone, where the level can be \c 0 (off) or between \c 1 (least
 * cool) to \c 10 (coolest).
 */
int QtIVIClimateControl::seatCooler() const
{
    return m_seatCooler;
}

/*!
 * \qmlproperty int ClimateControl::seatHeater
 * \property QtIVIClimateControl::seatHeater
 * \brief The seat heater level of the zone.
 *
 * The seat heater level of the zone, where the level can be \c 0 (off) or between \c 1 (least
 * warm) to \c 10 (warmest).
 */
int QtIVIClimateControl::seatHeater() const
{
    return m_seatHeater;
}

/*!
    Set air condition \a enabled
*/
void QtIVIClimateControl::setAirConditioningEnabled(bool enabled)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setAirConditioningEnabled(enabled, zone());
}

/*!
    Set air flow \a direction
*/
void QtIVIClimateControl::setAirflowDirection(QtIVIClimateControl::AirflowDirection direction)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setAirflowDirection(direction, zone());
}

/*!
    Set heater \a enabled
*/
void QtIVIClimateControl::setHeaterEnabled(bool enabled)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setHeaterEnabled(enabled, zone());
}

/*!
    Set air recirculation \a enabled
*/
void QtIVIClimateControl::setAirRecirculationEnabled(bool enabled)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setAirRecirculationEnabled(enabled, zone());
}

/*!
    Set steering wheel heater level \a value
*/
void QtIVIClimateControl::setSteeringWheelHeater(int value)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setSteeringWheelHeater(value, zone());
}

/*!
    Set fan speed level \a value
*/
void QtIVIClimateControl::setFanSpeedLevel(int value)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setFanSpeedLevel(value, zone());
}

/*!
    Set target \a temperature
*/
void QtIVIClimateControl::setTargetTemperature(int temperature)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setTargetTemperature(temperature, zone());
}

/*!
    Set seat cooler \a value level
*/
void QtIVIClimateControl::setSeatCooler(int value)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setSeatCooler(value, zone());
}

/*!
    Set seat heater \a value level
*/
void QtIVIClimateControl::setSeatHeater(int value)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setSeatHeater(value, zone());
}

