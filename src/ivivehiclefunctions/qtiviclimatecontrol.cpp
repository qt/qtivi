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
