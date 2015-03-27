/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#include "climatecontrolbackend.h"

#include <QDebug>

ClimateControlBackend::ClimateControlBackend(QObject *parent) :
    QtIVIClimateControlBackendInterface(parent),
    m_flowDirection(QtIVIClimateControl::BiLevel),
    m_airCondition(true),
    m_heater(true),
    m_airRecirculation(false),
    m_steeringWheelHeater(0),
    m_fanSpeed(10)
{
    ZoneBackend leftZone;
    leftZone.hasSeatCooler = true;
    leftZone.seattCooler = 10;
    leftZone.hasSeatHeater = true;
    leftZone.seatHeater = 10;
    leftZone.targetTemperature = 20;

    m_zoneMap.insert(QtIVIClimateZone::FrontLeft,leftZone);

    ZoneBackend rightZone;
    rightZone.hasSeatCooler = false;
    rightZone.seattCooler = 5;
    rightZone.hasSeatHeater = false;
    rightZone.seatHeater = 0;
    rightZone.targetTemperature = 10;

    m_zoneMap.insert(QtIVIClimateZone::FrontRight,rightZone);
}

ClimateControlBackend::~ClimateControlBackend()
{
}

bool ClimateControlBackend::hasTargetTemperature(QtIVIClimateZone::Zone zone)
{
    if (!m_zoneMap.contains(zone))
        return false;

    return m_zoneMap[zone].hasTargetTemperature;
}

bool ClimateControlBackend::hasSeatCooler(QtIVIClimateZone::Zone zone)
{
    if (!m_zoneMap.contains(zone))
        return false;

    return m_zoneMap[zone].hasSeatCooler;
}

bool ClimateControlBackend::hasSeatHeater(QtIVIClimateZone::Zone zone)
{
    if (!m_zoneMap.contains(zone))
        return false;

    return m_zoneMap[zone].hasSeatHeater;
}

void ClimateControlBackend::setTargetTemperature(QtIVIClimateZone::Zone zone, int val)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].targetTemperature == val)
        return;

    qWarning() << "SIMULATION TargetTemperature for Zone" << zone << "changed to" << val;

    m_zoneMap[zone].targetTemperature = val;
    emit targetTemperatureChanged(zone, val);
}

void ClimateControlBackend::setSeatCooler(QtIVIClimateZone::Zone zone, int val)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].seattCooler == val)
        return;

    qWarning() << "SIMULATION SeatCooler for Zone" << zone << "changed to" << val;

    m_zoneMap[zone].seattCooler = val;
    emit seatCoolerChanged(zone, val);
}

void ClimateControlBackend::setSeatHeater(QtIVIClimateZone::Zone zone, int val)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].seatHeater == val)
        return;

    qWarning() << "SIMULATION SeatHeater for Zone" << zone << "changed to" << val;

    m_zoneMap[zone].seatHeater = val;
    emit seatHeaterChanged(zone, val);
}

QtIVIClimateZone::Zone ClimateControlBackend::driverZone() const
{
    return QtIVIClimateZone::FrontLeft;
}

void ClimateControlBackend::setAirflowDirection(QtIVIClimateControl::AirflowDirection direction)
{
    if (m_flowDirection == direction)
        return;

    qWarning() << "SIMULATION AirflowDirection changed to" << direction;

    m_flowDirection = direction;
    emit airflowDirectionChanged(direction);
}

void ClimateControlBackend::setAirConditioningEnabled(bool val)
{
    if (m_airCondition == val)
        return;

    qWarning() << "SIMULATION AirConditionEnabled changed to" << val;

    m_airCondition = val;
    emit airConditioningEnabledChanged(val);
}

void ClimateControlBackend::setHeaterEnabled(bool val)
{
    if (m_heater == val)
        return;

    qWarning() << "SIMULATION HeaterEnabled changed to" << val;

    m_heater = val;
    emit heaterEnabledChanged(val);
}

void ClimateControlBackend::setAirRecirculationEnabled(bool val)
{
    if (m_heater == val)
        return;

    qWarning() << "SIMULATION AirRecirculationEnabled changed to" << val;

    m_heater = val;
    emit airRecirculationEnabledChanged(val);
}

void ClimateControlBackend::setSteeringWheelHeater(int val)
{
    if (m_steeringWheelHeater == val)
        return;

    qWarning() << "SIMULATION SteeringWheelHeater changed to" << val;

    m_steeringWheelHeater = val;
    emit steeringWheelHeaterChanged(val);
}

void ClimateControlBackend::setFanSpeedLevel(int speed)
{
    if (m_fanSpeed == speed)
        return;

    qWarning() << "SIMULATION FanSpeedLevel changed to" << speed;

    m_fanSpeed = speed;
    emit fanSpeedLevelChanged(speed);
}

int ClimateControlBackend::targetTemperature(QtIVIClimateZone::Zone zone) const
{
    if (!m_zoneMap.contains(zone))
        return -1;

    return m_zoneMap[zone].targetTemperature;
}

int ClimateControlBackend::seatCooler(QtIVIClimateZone::Zone zone) const
{
    if (!m_zoneMap.contains(zone))
        return -1;

    return m_zoneMap[zone].seattCooler;
}

int ClimateControlBackend::seatHeater(QtIVIClimateZone::Zone zone) const
{
    if (!m_zoneMap.contains(zone))
        return -1;

    return m_zoneMap[zone].seatHeater;
}

QtIVIClimateControl::AirflowDirection ClimateControlBackend::airflowDirection() const
{
    return m_flowDirection;
}

bool ClimateControlBackend::airConditioningEnabled() const
{
    return m_airCondition;
}

bool ClimateControlBackend::heaterEnabled() const
{
    return m_heater;
}

bool ClimateControlBackend::airRecirculationEnabled() const
{
    return m_airRecirculation;
}

int ClimateControlBackend::steeringWheelHeater() const
{
    return m_steeringWheelHeater;
}

int ClimateControlBackend::fanSpeedLevel() const
{
    return m_fanSpeed;
}
