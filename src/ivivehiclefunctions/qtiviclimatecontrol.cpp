/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#include "qtiviclimatecontrol.h"

#include "qtiviclimatecontrolbackendinterface.h"

/*!
 * \class QtIVIClimateControlBackendInterface
 *
 * \brief The QtIVIClimateControlBackendInterface defines the interface for backends to the
 * QtIVIClimateControl and QtIVIClimateZone feature classes.
 *
 * \ingroup backends
 * \inmodule QtIVIVehicleFunctions
 *
 * The QtIVIClimateControlBackendInterface is the interface used by \l QtIVIClimateControl and
 * \l QtIVIClimateZone.
 *
 * The interface is discovered by a \l QtIVIClimateControl object, which connects to it and sets up
 * the connections to a set of \l QtIVIClimateZone.
 *
 * For ranged properties (see the documentation of the \l QtIVIClimateControl and \l QtIVIClimateZone),
 * the backend is expected to check the range of the input values.
 *
 * \sa QtIVIClimateControl, QtIVIClimateZone
 */

/*!
 * \fn QtIVIClimateControlBackendInterface::QtIVIClimateControlBackendInterface(QObject *parent=0)
 *
 * Constructs a backend interface.
 *
 * The \a parent is sent to the QObject constructor.
 */

/*!
 * \fn bool QtIVIClimateControlBackendInterface::hasTargetTemperature(QtIVIClimateZone::Zone zone)
 *
 * Returns \c true if the \a zone supports setting a target temperature, otherwise \c false.
 *
 * The value is not expected to change.
 *
 * \sa targetTemperature(), setTargetTemperature(), targetTemperatureChanged()
 */

/*!
 * \fn bool QtIVIClimateControlBackendInterface::hasSeatCooler(QtIVIClimateZone::Zone zone)
 *
 * Returns \c true if the \a zone supports seat cooler, otherwise \c false.
 *
 * The value is not expected to change.
 *
 * \sa seatCooler(), setSeatCooler(), seatCoolerChanged()
 */

/*!
 * \fn bool QtIVIClimateControlBackendInterface::hasSeatHeater(QtIVIClimateZone::Zone zone)
 *
 * Returns \c true if the \a zone supports seat heater, otherwise \c false.
 *
 * The value is not expected to change.
 *
 * \sa seatHeater(), setSeatHeater(), seatHeaterChanged()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::setTargetTemperature(QtIVIClimateZone::Zone zone, int value)
 *
 * Sets the target temperature of \a zone to \a value, wheret he \a value is expressed in
 * centigrades and may be range limited by the backend.
 *
 * This method is expected to emit a \l targetTemperatureChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa targetTemperature(), targetTemperatureChanged(), hasTargetTemperature()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::setSeatCooler(QtIVIClimateZone::Zone zone, int value)
 *
 * Sets the seat ventilation level of \a zone to \a value, where \a value can be \c 0 (off) or
 * between \c 1 (least cool) to \c 10 (coolest).
 *
 * This method is expected to emit a \l seatCoolerChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa seatCooler(), seatCoolerChanged(), hasSeatCooler()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::setSeatHeater(QtIVIClimateZone::Zone zone, int value)
 *
 * Sets the seat heater level of \a zone to \a value, where \a value can be \c 0 (off) or between
 * \c 1 (least warm) to \c 10 (warmest).
 *
 * This method is expected to emit a \l seatHeaterChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa seatHeater(), seatHeaterChanged(), hasSeatHeater()
 */

/*!
 * \fn QtIVIClimateZone::Zone QtIVIClimateControlBackendInterface::driverZone() const
 *
 * Returns the \l QtIVIClimateZone::Zone for the driver.
 *
 * The driver zone is not expected to change.
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::setAirflowDirection(QtIVIClimateControl::AirflowDirection airflowDirection)
 *
 * Sets the air flow direction to \a airflowDirection.
 *
 * This method is expected to emit the \l airflowDirectionChanged() signal when receiving a
 * new \a airflowDirection.
 *
 * \sa airflowDirection(), airflowDirectionChanged()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::setAirConditioningEnabled(bool enabled)
 *
 * Enables or disables the air conditioning based on \a enabled.
 *
 * This method is expected to emit the \l airConditioningEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa airConditioningEnabled(), airConditioningEnabledChanged()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::setHeaterEnabled(bool enabled)
 *
 * Enables or disables the heater based on \a enabled.
 *
 * This method is expected to emit the \l heaterEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa heaterEnabled(), heaterEnabledChanged()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::setAirRecirculationEnabled(bool enabled)
 *
 * Enables or disables the air recirculation based on \a enabled.
 *
 * This method is expected to emit the \l airRecirculationEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa airRecirculationEnabled(), airRecirculationEnabledChanged()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::setSteeringWheelHeater(int value)
 *
 * Sets the steering wheel heater level to \a value, where \a value can be \c 0 (off) or between
 * \c 1 (least warm) to \c 10 (warmest).
 *
 * This method is expected to emit a \l steeringWheelHeaterChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa steeringWheelHeater(), steeringWheelHeaterChanged()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::setFanSpeedLevel(int value)
 *
 * Sets the fan speed level to \a value, where \a value can be \c 0 (off) or between
 * \c 1 (weakest) to \c 10 (strongest).
 *
 * This method is expected to emit a \l fanSpeedLevelChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa fanSpeedLevel(), fanSpeedLevelChanged()
 */

/*!
 * \fn int QtIVIClimateControlBackendInterface::targetTemperature(QtIVIClimateZone::Zone zone) const
 *
 * Returns the target temperature in centigrades for \a zone.
 *
 * \sa setTargetTemperature(), targetTemperatureChanged()
 */

/*!
 * \fn int QtIVIClimateControlBackendInterface::seatCooler(QtIVIClimateZone::Zone zone) const
 *
 * Returns the seat ventilation level for \a zone, where the level can be \c 0 (off) or
 * between \c 1 (least cool) to \c 10 (coolest).
 *
 * \sa setSeatCooler(), seatCoolerChanged(), hasTargetTemperature()
 */

/*!
 * \fn int QtIVIClimateControlBackendInterface::seatHeater(QtIVIClimateZone::Zone zone) const
 *
 * Returns the seat heater level for \a zone, where the level can be \c 0 (off) or between
 * \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa setSeatHeater(), seatHeaterChanged(), hasSeatHeater()
 */

/*!
 * \fn QtIVIClimateControl::AirflowDirection QtIVIClimateControlBackendInterface::airflowDirection() const
 *
 * Returns the airflow direction.
 *
 * \sa setAirflowDirection(), airflowDirectionChanged(), hasSeatCooler()
 */

/*!
 * \fn bool QtIVIClimateControlBackendInterface::airConditioningEnabled() const
 *
 * Returns \c true if the air conditioning is enabled, otherwise \c false.
 *
 * \sa setAirConditioningEnabled(), airConditioningEnabledChanged()
 */

/*!
 * \fn bool QtIVIClimateControlBackendInterface::heaterEnabled() const
 *
 * Returns \c true if the air conditioning is enabled, otherwise \c false.
 *
 * \sa setHeaterEnabled(), heaterEnabledChanged()
 */

/*!
 * \fn bool QtIVIClimateControlBackendInterface::airRecirculationEnabled() const
 *
 * Returns \c true if the air conditioning is enabled, otherwise \c false.
 *
 * \sa setAirRecirculationEnabled(), airRecirculationEnabledChanged()
 */

/*!
 * \fn int QtIVIClimateControlBackendInterface::steeringWheelHeater() const
 *
 * Returns the steering wheel heater level, where the level can be \c 0 (off) or between
 * \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa setSteeringWheelHeater(), steeringWheelHeaterChanged()
 */

/*!
 * \fn int QtIVIClimateControlBackendInterface::fanSpeedLevel() const
 *
 * Returns the fan speed level, where the level can be \c 0 (off) or between
 * \c 1 (weakest) to \c 10 (strongest).
 *
 * \sa setFanSpeedLevel(), fanSpeedLevelChanged()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::targetTemperatureChanged(QtIVIClimateZone::Zone zone, int value)
 *
 * The signal is emitted when the target temperature for \a zone is changed to \a value, where
 * value is expressed in centigrades.
 *
 * \sa targetTemperature(), setTargetTemperature()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::seatCoolerChanged(QtIVIClimateZone::Zone zone, int value)
 *
 * The signal is emitted when the seat cooler level is changed for \a zone to \a value, where the
 * \a value can be \c 0 (off) or between \c 1 (least cool) to \c 10 (coolest).
 *
 * \sa seatCooler(), setSeatCooler()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::seatHeaterChanged(QtIVIClimateZone::Zone zone, int value)
 *
 * The signal is emitted when the seat cooler level is changed for \a zone to \a value, where the
 * \a value can be \c 0 (off) or between \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa seatHeater(), setSeatHeater()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::airflowDirectionChanged(QtIVIClimateControl::AirflowDirection airflowDirection)
 *
 * The signal is emitted when the airflow direction is changed to \a airflowDirection.
 *
 * \sa airflowDirection(), setAirflowDirection()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::airConditioningEnabledChanged(bool enabled)
 *
 * The signal is emitted when the air conditioning state is changed to \a enabled.
 *
 * \sa airConditioningEnabled(), setAirConditioningEnabled()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::heaterEnabledChanged(bool enabled)
 *
 * The signal is emitted whent he heater state is changed to \a enabled.
 *
 * \sa heaterEnabled(), setHeaterEnabled()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged(bool enabled)
 *
 * The signal is emitted when the air recirculation state is changed to \a enabled.
 *
 * \sa airRecirculationEnabled(), setAirRecirculationEnabled()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::steeringWheelHeaterChanged(int level)
 *
 * The signals ie emitted when the steering wheel heater level is changed to \a level, where the
 * \a level can be \c 0 (off) or between \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa steeringWheelHeater(), setSteeringWheelHeater()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::fanSpeedLevelChanged(int level)
 *
 * The signals ie emitted when the fan speel level is changed to \a level, where the \a level
 * can be \c 0 (off) or between \c 1 (weakest) to \c 10 (strongest).
 *
 * \sa fanSpeedLevel(), setFanSpeedLevel()
 */


QString QtIVIClimateControlBackendInterface::interfaceName = QLatin1String("com.pelagicore.ClimateControl");



/*!
 * \class QtIVIClimateZone
 *
 * \brief Provides an interface to zone specific properties of climate control.
 *
 * \ingroup features
 * \inmodule QtIVIVehicleFunctions
 *
 * \sa QtIVIClimateControl
 *
 */

/*!
 * \enum QtIVIClimateZone::Zone
 * \value FrontLeft
 *        The front left seat.
 * \value FrontCenter
 *        The front center seat.
 * \value FrontRight
 *        The front right seat.
 * \value RearLeft
 *        The rear left seat.
 * \value RearCenter
 *        The rear center seat.
 * \value RearRight
 *        The rear right seat.
 */


QtIVIClimateZone::QtIVIClimateZone(Zone zone, QtIVIClimateControl *parent)
    : QObject(parent)
    , m_zone(zone)
    , m_hasTargetTemperature(false)
    , m_hasSeatCooler(false)
    , m_hasSeatHeater(false)
    , m_targetTemperature(0)
    , m_seatCooler(0)
    , m_seatHeater(0)
{

}

/*!
 * \property QtIVIClimateZone::zone
 * \brief The current zone.
 *
 * The zone of the object. Can be used to determine where in the vehicle the zone is located.
 *
 * \sa QtIVIClimateZone::Zone
 */
QtIVIClimateZone::Zone QtIVIClimateZone::zone() const
{
    return m_zone;
}

/*!
 * \property QtIVIClimateZone::hasTargetTemperature
 * \brief \c True if the zone has a target temperature.
 *
 * \sa QtIVIClimateZone::targetTemperature
 */
bool QtIVIClimateZone::hasTargetTemperature() const
{
    return m_hasTargetTemperature;
}


/*!
 * \property QtIVIClimateZone::targetTemperature
 * \brief The target temperature of the zone.
 *
 * The target temperature of the zone expressed in centigrades.
 *
 * \sa QtIVIClimateZone::hasTargetTemperature
 */
int QtIVIClimateZone::targetTemperature() const
{
    if (m_hasTargetTemperature)
        return m_targetTemperature;
    else
        return 0;
}

void QtIVIClimateZone::setTargetTemperature(int t)
{
    if (m_hasTargetTemperature)
        qobject_cast<QtIVIClimateControl*>(parent())->backend()->setTargetTemperature(m_zone, t);
    else
        qWarning() << "Trying to set ClimateZone::targetTemperature in an unsupported zone or without a backend.";
}

/*!
 * \property QtIVIClimateZone::hasSeatCooler
 * \brief True if the zone has a seat cooler.
 *
 * \sa QtIVIClimateZone::seatCooler
 */

bool QtIVIClimateZone::hasSeatCooler() const
{
    return m_hasSeatCooler;
}

/*!
 * \property QtIVIClimateZone::seatCooler
 * \brief The seat cooler level of the zone.
 *
 * The seat cooler level of the zone, where the level can be \c 0 (off) or between \c 1 (least
 * cool) to \c 10 (coolest).
 *
 * \sa QtIVIClimateZone::hasSeatCooler
 */
int QtIVIClimateZone::seatCooler() const
{
    if (m_hasSeatCooler)
        return m_seatCooler;
    else
        return 0;
}

void QtIVIClimateZone::setSeatCooler(int t)
{
    if (t < 0 || t > 10) {
        qWarning() << "Trying to set ClimateZone::seatCooler to " << t << " which is out of range (0-10).";
        return;
    }

    if (m_hasSeatCooler)
        qobject_cast<QtIVIClimateControl*>(parent())->backend()->setSeatCooler(m_zone, t);
    else
        qWarning() << "Trying to set ClimateZone::seatCooler in an unsupported zone or without a backend.";
}

/*!
 * \property QtIVIClimateZone::hasSeatHeater
 * \brief \c True if the zone has a seat heater.
 *
 * \sa QtIVIClimateZone::seatHeater
 */
bool QtIVIClimateZone::hasSeatHeater() const
{
    return m_hasSeatHeater;
}

/*!
 * \property QtIVIClimateZone::seatHeater
 * \brief The seat heater level of the zone.
 *
 * The seat heater level of the zone, where the level can be \c 0 (off) or between \c 1 (least
 * warm) to \c 10 (warmest).
 *
 * \sa QtIVIClimateZone::hasSeatHeater()
 */
int QtIVIClimateZone::seatHeater() const
{
    if (m_hasSeatHeater)
        return m_seatHeater;
    else
        return 0;
}

void QtIVIClimateZone::setSeatHeater(int t)
{
    if (t < 0 || t > 10) {
        qWarning() << "Trying to set ClimateZone::seatHeater to " << t << " which is out of range (0-10).";
        return;
    }

    if (m_hasSeatHeater)
        qobject_cast<QtIVIClimateControl*>(parent())->backend()->setSeatHeater(m_zone, t);
    else
        qWarning() << "Trying to set ClimateZone::seatHeater in an unsupported zone or without a backend.";
}

void QtIVIClimateZone::onTargetTemperatureChanged(QtIVIClimateZone::Zone z, int t)
{
    if (z == m_zone) {
        m_targetTemperature = t;
        if (m_hasTargetTemperature)
            emit targetTemperatureChanged(m_targetTemperature);
    }
}

void QtIVIClimateZone::onSeatCoolerChanged(QtIVIClimateZone::Zone z, int t)
{
    if (z == m_zone) {
        m_seatCooler = t;
        if (m_hasSeatCooler)
            emit seatCoolerChanged(m_seatCooler);
    }
}

void QtIVIClimateZone::onSeatHeaterChanged(QtIVIClimateZone::Zone z, int t)
{
    if (z == m_zone) {
        m_seatHeater = t;
        if (m_hasSeatHeater)
            emit seatHeaterChanged(m_seatHeater);
    }
}

void QtIVIClimateZone::setHasTargetTemperature(bool e)
{
    if(e != m_hasTargetTemperature) {
        m_hasTargetTemperature = e;
        emit hasTargetTemperatureChanged(m_hasTargetTemperature);

        onTargetTemperatureChanged(m_zone, targetTemperature());
    }
}

void QtIVIClimateZone::setHasSeatCooler(bool e)
{
    if(e != m_hasSeatCooler) {
        m_hasSeatCooler = e;
        emit hasSeatCoolerChanged(m_hasSeatCooler);

        onSeatCoolerChanged(m_zone, seatCooler());
    }
}

void QtIVIClimateZone::setHasSeatHeater(bool e)
{
    if(e != m_hasSeatHeater) {
        m_hasSeatHeater = e;
        emit hasSeatHeaterChanged(m_hasSeatHeater);

        onSeatHeaterChanged(m_zone, seatHeater());
    }
}



/*!
 * \class QtIVIClimateControl
 * \brief Provides an interface to the climate control.
 *
 * The QtIVIClimateControl provides an interface to the climate control of the vehicle.
 *
 * The climate control properties are divided into two categories: central or zoned. The central
 * properties are exposed directly through the QtIVIClimateControl while the zoned properties are
 * exposed through \l QtIVIClimateZone objects. The zones are retreived using the \l climateZone
 * method or through the \l driver property.
 *
 * The QtIVIClimateControl expects a single backend to be available. It is recommended to use it
 * with \l {QtIVIAbstractFeature::autoDiscovery} {autoDiscovery} enabled.
 *
 * \sa QtIVIClimateZone
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
 * \brief QtIVIClimateControl::QtIVIClimateControl
 * Constructs a climate control object.
 *
 * The \a parent argument is passed on to the \l QtIVIAbstractFeature base class.
 */
QtIVIClimateControl::QtIVIClimateControl(QObject *parent)
    : QtIVIAbstractFeature(QtIVIClimateControlBackendInterface::interfaceName, true, parent)
    , m_airflowDirection(QtIVIClimateControl::BiLevel)
    , m_airConditioning(false)
    , m_heater(false)
    , m_airRecirculation(false)
    , m_steeringWheelHeater(0)
    , m_fanSpeedLevel(0)
{
    QList<QtIVIClimateZone::Zone> zones;
    zones << QtIVIClimateZone::FrontLeft << QtIVIClimateZone::FrontCenter << QtIVIClimateZone::FrontRight
          << QtIVIClimateZone::RearLeft << QtIVIClimateZone::RearCenter << QtIVIClimateZone::RearRight;
    foreach(QtIVIClimateZone::Zone z, zones)
        m_zones[z] = new QtIVIClimateZone(z, this);
}

/*!
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
 * \property QtIVIClimateControl::airConditioning
 * \brief \c True if the air conditioning is enabled.
 */
bool QtIVIClimateControl::isAirConditioningEnabled() const
{
    return m_airConditioning;
}

/*!
 * \property QtIVIClimateControl::heater
 * \brief \c True if the heater is enabled.
 */
bool QtIVIClimateControl::isHeaterEnabled() const
{
    return m_heater;
}

/*!
 * \property QtIVIClimateControl::airRecirculation
 * \brief \c True if the air recirculation is enabled.
 */
bool QtIVIClimateControl::isAirRecirculationEnabled() const
{
    return m_airRecirculation;
}

/*!
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
 * Returns the \l QtIVIClimateZone object instance for the given zone, \a z.
 *
 * \sa driver
 */
QtIVIClimateZone *QtIVIClimateControl::climateZone(QtIVIClimateZone::Zone z) const
{
    return m_zones[z];
}

/*!
 * \property QtIVIClimateControl::driver
 * \brief The QtIVIClimateZone of the driver.
 *
 * \sa climateZone
 */
QtIVIClimateZone *QtIVIClimateControl::driver() const
{
    if (backend())
        return m_zones[backend()->driverZone()];
    else {
        qWarning() << "Returning driver zone without a backend, using default driver zone.";
        return m_zones[QtIVIClimateZone::FrontLeft];
    }
}

/*!
 * \property QtIVIClimateControl::frontLeftZone
 * The front left zone QtIVIClimateZone object.
 *
 * \sa frontCenterZone, frontRightZone,
 *     rearLeftZone, rearCenterZone, rearRightZone,
 *     driver, climateZone()
 */
QtIVIClimateZone *QtIVIClimateControl::frontLeftZone() const
{
    return m_zones[QtIVIClimateZone::FrontLeft];
}

/*!
 * \property QtIVIClimateControl::frontCenterZone
 * The front center zone QtIVIClimateZone object.
 *
 * \sa frontLeftZone, frontRightZone,
 *     rearLeftZone, rearCenterZone, rearRightZone,
 *     driver, climateZone()
 */
QtIVIClimateZone *QtIVIClimateControl::frontCenterZone() const
{
    return m_zones[QtIVIClimateZone::FrontCenter];
}

/*!
 * \property QtIVIClimateControl::frontRightZone
 * The front right zone QtIVIClimateZone object.
 *
 * \sa frontLeftZone, frontCenterZone,
 *     rearLeftZone, rearCenterZone, rearRightZone,
 *     driver, climateZone()
 */
QtIVIClimateZone *QtIVIClimateControl::frontRightZone() const
{
    return m_zones[QtIVIClimateZone::FrontRight];
}

/*!
 * \property QtIVIClimateControl::rearLeftZone
 * The rear left zone QtIVIClimateZone object.
 *
 * \sa frontLeftZone(), frontCenterZone(), frontRightZone(),
 *     rearCenterZone(), rearRightZone(),
 *     driver, climateZone()
 */
QtIVIClimateZone *QtIVIClimateControl::rearLeftZone() const
{
    return m_zones[QtIVIClimateZone::RearLeft];
}

/*!
 * \property QtIVIClimateControl::rearCenterZone
 * The rear center zone QtIVIClimateZone object.
 *
 * \sa frontLeftZone(), frontCenterZone(), frontRightZone(),
 *     rearLeftZone(), rearRightZone(),
 *     driver, climateZone()
 */
QtIVIClimateZone *QtIVIClimateControl::rearCenterZone() const
{
    return m_zones[QtIVIClimateZone::RearCenter];
}

/*!
 * \property QtIVIClimateControl::rearRightZone
 * The rear right zone QtIVIClimateZone object.
 *
 * \sa frontLeftZone(), frontCenterZone(), frontRightZone(),
 *     rearLeftZone(), rearCenterZone(),
 *     driver, climateZone()
 */
QtIVIClimateZone *QtIVIClimateControl::rearRightZone() const
{
    return m_zones[QtIVIClimateZone::RearRight];
}

void QtIVIClimateControl::setAirflowDirection(QtIVIClimateControl::AirflowDirection ad)
{
    if(backend())
        backend()->setAirflowDirection(ad);
}

void QtIVIClimateControl::setAirConditioningEnabled(bool e)
{
    if(backend())
        backend()->setAirConditioningEnabled(e);
}

void QtIVIClimateControl::setHeaterEnabled(bool e)
{
    if(backend())
        backend()->setHeaterEnabled(e);
}

void QtIVIClimateControl::setAirRecirculationEnabled(bool e)
{
    if(backend())
        backend()->setAirRecirculationEnabled(e);
}

void QtIVIClimateControl::setSteeringWheelHeater(int t)
{
    if(backend())
        backend()->setSteeringWheelHeater(t);
}

void QtIVIClimateControl::setFanSpeedLevel(int fsl)
{
    if(backend())
        backend()->setFanSpeedLevel(fsl);
}

/*!
 * Reimplemented from QtIVIAbstractFeature::acceptServiceObject()
 */
bool QtIVIClimateControl::acceptServiceObject(QtIVIServiceObject *so)
{
    return (qobject_cast<QtIVIClimateControlBackendInterface*>(so->interfaceInstance(QtIVIClimateControlBackendInterface::interfaceName)) != NULL);
}

/*!
 * Reimplemented from QtIVIAbstractFeature::connectToServiceObject()
 */
void QtIVIClimateControl::connectToServiceObject(QtIVIServiceObject *so)
{
    QtIVIClimateControlBackendInterface *backend = qobject_cast<QtIVIClimateControlBackendInterface*>(so->interfaceInstance(QtIVIClimateControlBackendInterface::interfaceName));

    QList<QtIVIClimateZone::Zone> zones;
    zones << QtIVIClimateZone::FrontLeft << QtIVIClimateZone::FrontCenter << QtIVIClimateZone::FrontRight
          << QtIVIClimateZone::RearLeft << QtIVIClimateZone::RearCenter << QtIVIClimateZone::RearRight;
    foreach(QtIVIClimateZone::Zone z, zones) {
        connect(backend, &QtIVIClimateControlBackendInterface::targetTemperatureChanged, m_zones[z], &QtIVIClimateZone::onTargetTemperatureChanged);
        connect(backend, &QtIVIClimateControlBackendInterface::seatCoolerChanged, m_zones[z], &QtIVIClimateZone::onSeatCoolerChanged);
        connect(backend, &QtIVIClimateControlBackendInterface::seatHeaterChanged, m_zones[z], &QtIVIClimateZone::onSeatHeaterChanged);
        m_zones[z]->onTargetTemperatureChanged(z, backend->targetTemperature(z));
        m_zones[z]->onSeatCoolerChanged(z, backend->seatCooler(z));
        m_zones[z]->onSeatHeaterChanged(z, backend->seatHeater(z));
        /* Set the has properties last to ensure that the value is available when signalling */
        m_zones[z]->setHasTargetTemperature(backend->hasTargetTemperature(z));
        m_zones[z]->setHasSeatCooler(backend->hasSeatCooler(z));
        m_zones[z]->setHasSeatHeater(backend->hasSeatHeater(z));
    }

    connect(backend, &QtIVIClimateControlBackendInterface::airflowDirectionChanged, this, &QtIVIClimateControl::onAirflowDirectionChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airConditioningEnabledChanged, this, &QtIVIClimateControl::onAirConditioningEnabledChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::heaterEnabledChanged, this, &QtIVIClimateControl::onHeaterEnabledChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged, this, &QtIVIClimateControl::onAirRecirculationEnabledChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::steeringWheelHeaterChanged, this, &QtIVIClimateControl::onSteeringWheelHeaterChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::fanSpeedLevelChanged, this, &QtIVIClimateControl::onFanSpeedLevelChanged);

    onAirflowDirectionChanged(backend->airflowDirection());
    onAirConditioningEnabledChanged(backend->airConditioningEnabled());
    onHeaterEnabledChanged(backend->heaterEnabled());
    onAirRecirculationEnabledChanged(backend->airRecirculationEnabled());
    onSteeringWheelHeaterChanged(backend->steeringWheelHeater());
    onFanSpeedLevelChanged(backend->fanSpeedLevel());
}

/*!
 * Reimplemented from QtIVIAbstractFeature::disconnectFromServiceObject()
 */
void QtIVIClimateControl::disconnectFromServiceObject(QtIVIServiceObject *so)
{
    QtIVIClimateControlBackendInterface *backend = qobject_cast<QtIVIClimateControlBackendInterface*>(so->interfaceInstance(QtIVIClimateControlBackendInterface::interfaceName));

    QList<QtIVIClimateZone::Zone> zones;
    zones << QtIVIClimateZone::FrontLeft << QtIVIClimateZone::FrontCenter << QtIVIClimateZone::FrontRight
          << QtIVIClimateZone::RearLeft << QtIVIClimateZone::RearCenter << QtIVIClimateZone::RearRight;
    foreach(QtIVIClimateZone::Zone z, zones) {
        disconnect(backend, &QtIVIClimateControlBackendInterface::targetTemperatureChanged, m_zones[z], &QtIVIClimateZone::onTargetTemperatureChanged);
        disconnect(backend, &QtIVIClimateControlBackendInterface::seatCoolerChanged, m_zones[z], &QtIVIClimateZone::onSeatCoolerChanged);
        disconnect(backend, &QtIVIClimateControlBackendInterface::seatHeaterChanged, m_zones[z], &QtIVIClimateZone::onSeatHeaterChanged);
    }

    disconnect(backend, &QtIVIClimateControlBackendInterface::airflowDirectionChanged, this, &QtIVIClimateControl::onAirflowDirectionChanged);
    disconnect(backend, &QtIVIClimateControlBackendInterface::airConditioningEnabledChanged, this, &QtIVIClimateControl::onAirConditioningEnabledChanged);
    disconnect(backend, &QtIVIClimateControlBackendInterface::heaterEnabledChanged, this, &QtIVIClimateControl::onHeaterEnabledChanged);
    disconnect(backend, &QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged, this, &QtIVIClimateControl::onAirRecirculationEnabledChanged);
    disconnect(backend, &QtIVIClimateControlBackendInterface::steeringWheelHeaterChanged, this, &QtIVIClimateControl::onSteeringWheelHeaterChanged);
    disconnect(backend, &QtIVIClimateControlBackendInterface::fanSpeedLevelChanged, this, &QtIVIClimateControl::onFanSpeedLevelChanged);
}

/*!
 * Reimplemented from QtIVIAbstractFeature::clearServiceObject()
 */
void QtIVIClimateControl::clearServiceObject()
{
    /* Safe defaults */
    onAirflowDirectionChanged(QtIVIClimateControl::BiLevel);
    onAirConditioningEnabledChanged(false);
    onHeaterEnabledChanged(false);
    onAirRecirculationEnabledChanged(false);
    onSteeringWheelHeaterChanged(0);
    onFanSpeedLevelChanged(0);

    QList<QtIVIClimateZone::Zone> zones;
    zones << QtIVIClimateZone::FrontLeft << QtIVIClimateZone::FrontCenter << QtIVIClimateZone::FrontRight
          << QtIVIClimateZone::RearLeft << QtIVIClimateZone::RearCenter << QtIVIClimateZone::RearRight;
    foreach(QtIVIClimateZone::Zone z, zones) {
        m_zones[z]->setHasTargetTemperature(false);
        m_zones[z]->setHasSeatCooler(false);
        m_zones[z]->setHasSeatHeater(false);
    }
}

void QtIVIClimateControl::onAirflowDirectionChanged(QtIVIClimateControl::AirflowDirection airflowDirection)
{
    m_airflowDirection = airflowDirection;
    emit airflowDirectionChanged(m_airflowDirection);
}

void QtIVIClimateControl::onAirConditioningEnabledChanged(bool airConditioning)
{
    m_airConditioning = airConditioning;
    emit airConditioningEnabledChanged(m_airConditioning);
}

void QtIVIClimateControl::onHeaterEnabledChanged(bool heater)
{
    m_heater = heater;
    emit heaterEnabledChanged(m_heater);
}

void QtIVIClimateControl::onAirRecirculationEnabledChanged(bool airRecirculation)
{
    m_airRecirculation = airRecirculation;
    emit airRecirculationEnabledChanged(m_airRecirculation);
}

void QtIVIClimateControl::onSteeringWheelHeaterChanged(int steeringWheelHeater)
{
    m_steeringWheelHeater = steeringWheelHeater;
    emit steeringWheelHeaterChanged(m_steeringWheelHeater);
}

void QtIVIClimateControl::onFanSpeedLevelChanged(int fanSpeedLevel)
{
    m_fanSpeedLevel = fanSpeedLevel;
    emit fanSpeedLevelChanged(m_fanSpeedLevel);
}

QtIVIClimateControlBackendInterface *QtIVIClimateControl::backend() const
{
    if (serviceObject())
        return qobject_cast<QtIVIClimateControlBackendInterface*>(serviceObject()->interfaceInstance(QtIVIClimateControlBackendInterface::interfaceName));
    else
        return 0;
}
