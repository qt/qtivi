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


QtIVIClimateZone::QtIVIClimateZone(const QString &zone, QtIVIClimateControl *parent, QtIVIClimateControlBackendInterface *backend)
    : QObject(parent)
    , m_zone(zone)
    , m_targetTemperature(0)
    , m_seatCooler(0)
    , m_seatHeater(0)
    , m_steeringWheelHeater(0)
    , m_fanSpeedLevel(0)
    , m_features(QtIVIClimateZone::None)
{
    connect(backend, &QtIVIClimateControlBackendInterface::targetTemperatureChanged, this, &QtIVIClimateZone::onTargetTemperatureChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::seatCoolerChanged, this, &QtIVIClimateZone::onSeatCoolerChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::seatHeaterChanged, this, &QtIVIClimateZone::onSeatHeaterChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::steeringWheelHeaterChanged, this, &QtIVIClimateZone::onSteeringWheelHeaterChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::fanSpeedLevelChanged, this, &QtIVIClimateZone::onFanSpeedLevelChanged);

    onTargetTemperatureChanged(zone, backend->targetTemperature(zone));
    onSeatCoolerChanged(zone, backend->seatCooler(zone));
    onSeatHeaterChanged(zone, backend->seatHeater(zone));
    onSteeringWheelHeaterChanged(zone, backend->steeringWheelHeater(zone));
    onFanSpeedLevelChanged(zone, backend->fanSpeedLevel(zone));

    /* Set the has properties last to ensure that the value is available when signalling */
    m_features = backend->zoneFeatures(zone);
}

/*!
 * \property QtIVIClimateZone::zone
 * \brief The current zone.
 *
 * The zone of the object. Can be used to determine where in the vehicle the zone is located.
 */
bool QtIVIClimateZone::hasFeature(QtIVIClimateZone::OptionalFeature f) const
{
    return m_features.testFlag(f);
}

QString QtIVIClimateZone::zone() const
{
    return m_zone;
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
    if (hasFeature(HasTargetTemperature))
        return m_targetTemperature;
    else
        return 0;
}

void QtIVIClimateZone::setTargetTemperature(int t)
{
    if (hasFeature(HasTargetTemperature))
        qobject_cast<QtIVIClimateControl*>(parent())->backend()->setTargetTemperature(m_zone, t);
    else
        qWarning() << "Trying to set ClimateZone::targetTemperature in an unsupported zone or without a backend.";
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
    if (hasFeature(HasSeatCooler))
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

    if (hasFeature(HasSeatCooler))
        qobject_cast<QtIVIClimateControl*>(parent())->backend()->setSeatCooler(m_zone, t);
    else
        qWarning() << "Trying to set ClimateZone::seatCooler in an unsupported zone or without a backend.";
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
    if (hasFeature(HasSeatHeater))
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

    if (hasFeature(HasSeatHeater))
        qobject_cast<QtIVIClimateControl*>(parent())->backend()->setSeatHeater(m_zone, t);
    else
        qWarning() << "Trying to set ClimateZone::seatHeater in an unsupported zone or without a backend.";
}

void QtIVIClimateZone::setSteeringWheelHeater(int t)
{
    if (hasFeature(HasSteeringWheelHeater))
        qobject_cast<QtIVIClimateControl*>(parent())->backend()->setSteeringWheelHeater(m_zone, t);
}

void QtIVIClimateZone::setFanSpeedLevel(int fsl)
{
    if (hasFeature(HasFanSpeed))
        qobject_cast<QtIVIClimateControl*>(parent())->backend()->setFanSpeedLevel(m_zone, fsl);
}


void QtIVIClimateZone::onTargetTemperatureChanged(const QString &z, int t)
{
    if (z == m_zone) {
        m_targetTemperature = t;
        if (hasFeature(HasTargetTemperature))
            emit targetTemperatureChanged(m_targetTemperature);
    }
}

void QtIVIClimateZone::onSeatCoolerChanged(const QString &z, int t)
{
    if (z == m_zone) {
        m_seatCooler = t;
        if (hasFeature(HasSeatCooler))
            emit seatCoolerChanged(m_seatCooler);
    }
}

void QtIVIClimateZone::onSeatHeaterChanged(const QString &z, int t)
{
    if (z == m_zone) {
        m_seatHeater = t;
        if (hasFeature(HasSeatHeater))
            emit seatHeaterChanged(m_seatHeater);
    }
}

void QtIVIClimateZone::onSteeringWheelHeaterChanged(const QString &z, int steeringWheelHeater)
{
    if (z == m_zone) {
        m_steeringWheelHeater = steeringWheelHeater;
        if (hasFeature(HasSteeringWheelHeater))
            emit steeringWheelHeaterChanged(m_steeringWheelHeater);
    }
}

void QtIVIClimateZone::onFanSpeedLevelChanged(const QString &z, int fanSpeedLevel)
{
    if (z == m_zone) {
        m_fanSpeedLevel = fanSpeedLevel;
        if (hasFeature(HasFanSpeed))
            emit fanSpeedLevelChanged(m_fanSpeedLevel);
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
    : QtIVIAbstractZoneModelFeature(QtIVIClimateControlBackendInterface::interfaceName, true, parent)
    , m_airflowDirection(QtIVIClimateControl::BiLevel)
    , m_airConditioning(false)
    , m_heater(false)
    , m_airRecirculation(false)
{
}

QStringList QtIVIClimateControl::zones() const
{
    return m_zones.keys();
}

QObject *QtIVIClimateControl::zoneObject(const QString &z) const
{
    return climateZoneObject(z);
}

QtIVIClimateZone *QtIVIClimateControl::climateZoneObject(const QString &z) const
{
    if (m_zones.contains(z))
        return m_zones[z];

    return 0;
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
int QtIVIClimateZone::steeringWheelHeater() const
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
int QtIVIClimateZone::fanSpeedLevel() const
{
    return m_fanSpeedLevel;
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

    QStringList zones = backend->zones();
    foreach(const QString &z, zones)
        m_zones[z] = new QtIVIClimateZone(z, this, backend);

    connect(backend, &QtIVIClimateControlBackendInterface::airflowDirectionChanged, this, &QtIVIClimateControl::onAirflowDirectionChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airConditioningEnabledChanged, this, &QtIVIClimateControl::onAirConditioningEnabledChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::heaterEnabledChanged, this, &QtIVIClimateControl::onHeaterEnabledChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged, this, &QtIVIClimateControl::onAirRecirculationEnabledChanged);

    onAirflowDirectionChanged(backend->airflowDirection());
    onAirConditioningEnabledChanged(backend->airConditioningEnabled());
    onHeaterEnabledChanged(backend->heaterEnabled());
    onAirRecirculationEnabledChanged(backend->airRecirculationEnabled());
}

/*!
 * Reimplemented from QtIVIAbstractFeature::disconnectFromServiceObject()
 */
void QtIVIClimateControl::disconnectFromServiceObject(QtIVIServiceObject *so)
{
    QtIVIClimateControlBackendInterface *backend = qobject_cast<QtIVIClimateControlBackendInterface*>(so->interfaceInstance(QtIVIClimateControlBackendInterface::interfaceName));

    while(m_zones.count() > 0)
        delete m_zones.take(m_zones.firstKey());

    // TODO reset model

    disconnect(backend, &QtIVIClimateControlBackendInterface::airflowDirectionChanged, this, &QtIVIClimateControl::onAirflowDirectionChanged);
    disconnect(backend, &QtIVIClimateControlBackendInterface::airConditioningEnabledChanged, this, &QtIVIClimateControl::onAirConditioningEnabledChanged);
    disconnect(backend, &QtIVIClimateControlBackendInterface::heaterEnabledChanged, this, &QtIVIClimateControl::onHeaterEnabledChanged);
    disconnect(backend, &QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged, this, &QtIVIClimateControl::onAirRecirculationEnabledChanged);
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

    while(m_zones.count() > 0)
        delete m_zones.take(m_zones.firstKey());

    // TODO reset model
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

QtIVIClimateControlBackendInterface *QtIVIClimateControl::backend() const
{
    if (serviceObject())
        return qobject_cast<QtIVIClimateControlBackendInterface*>(serviceObject()->interfaceInstance(QtIVIClimateControlBackendInterface::interfaceName));
    else
        return 0;
}
