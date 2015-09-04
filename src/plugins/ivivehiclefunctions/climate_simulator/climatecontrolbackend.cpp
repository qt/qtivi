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
    m_airRecirculation(false)
{
    ZoneBackend leftZone;
    leftZone.features = QtIVIClimateZone::HasFanSpeed |
                        QtIVIClimateZone::HasSeatCooler |
                        QtIVIClimateZone::HasSeatHeater |
                        QtIVIClimateZone::HasSteeringWheelHeater |
                        QtIVIClimateZone::HasTargetTemperature;
    leftZone.seatCooler = 10;
    leftZone.seatHeater = 10;
    leftZone.targetTemperature = 20;
    leftZone.steeringWheelHeater = 0;
    leftZone.fanSpeed = 10;

    m_zoneMap.insert("frontLeft",leftZone);

    ZoneBackend rightZone;
    rightZone.features = QtIVIClimateZone::HasFanSpeed |
                         QtIVIClimateZone::HasSeatCooler |
                         QtIVIClimateZone::HasSeatHeater |
                         QtIVIClimateZone::HasTargetTemperature;
    rightZone.seatCooler = 5;
    rightZone.seatHeater = 0;
    rightZone.targetTemperature = 10;
    rightZone.steeringWheelHeater = 0;
    rightZone.fanSpeed = 10;

    m_zoneMap.insert("frontRight",rightZone);

    ZoneBackend rearZone;
    rearZone.features = QtIVIClimateZone::HasSeatHeater;
    rearZone.seatCooler = 5;
    rearZone.seatHeater = 0;
    rearZone.targetTemperature = 10;
    rearZone.steeringWheelHeater = 0;
    rearZone.fanSpeed = 10;

    m_zoneMap.insert("rear",rearZone);
}

ClimateControlBackend::~ClimateControlBackend()
{
}

QtIVIClimateControl::OptionalFeatures ClimateControlBackend::climateFeatures() const
{
    return QtIVIClimateControl::HasAirConditioning |
           QtIVIClimateControl::HasAirflowDirection |
           QtIVIClimateControl::HasAirRecirculation |
           QtIVIClimateControl::HasHeater;
}

QStringList ClimateControlBackend::zones() const
{
    return m_zoneMap.keys();
}

QtIVIClimateZone::OptionalFeatures ClimateControlBackend::zoneFeatures(const QString &zone) const
{
    if (!m_zoneMap.contains(zone))
        return QtIVIClimateZone::None;

    return m_zoneMap[zone].features;
}

void ClimateControlBackend::setTargetTemperature(const QString &zone, int val)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].targetTemperature == val)
        return;

    qWarning() << "SIMULATION TargetTemperature for Zone" << zone << "changed to" << val;

    m_zoneMap[zone].targetTemperature = val;
    emit targetTemperatureChanged(zone, val);
}

void ClimateControlBackend::setSeatCooler(const QString &zone, int val)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].seatCooler == val)
        return;

    qWarning() << "SIMULATION SeatCooler for Zone" << zone << "changed to" << val;

    m_zoneMap[zone].seatCooler = val;
    emit seatCoolerChanged(zone, val);
}

void ClimateControlBackend::setSeatHeater(const QString &zone, int val)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].seatHeater == val)
        return;

    qWarning() << "SIMULATION SeatHeater for Zone" << zone << "changed to" << val;

    m_zoneMap[zone].seatHeater = val;
    emit seatHeaterChanged(zone, val);
}

void ClimateControlBackend::setSteeringWheelHeater(const QString &zone, int val)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (val < 0 || val > 10) {
        qWarning() << "SIMULATION SteeringWheelHeater for Zone" << zone << "change out of range (0-10)" << val;
        emit steeringWheelHeaterChanged(zone, m_zoneMap[zone].steeringWheelHeater);
        return;
    }

    if (m_zoneMap[zone].steeringWheelHeater == val)
        return;

    qWarning() << "SIMULATION SteeringWheelHeater for" << zone << "changed to" << val;

    m_zoneMap[zone].steeringWheelHeater = val;
    emit steeringWheelHeaterChanged(zone, val);
}

void ClimateControlBackend::setFanSpeedLevel(const QString &zone, int speed)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (speed < 0 || speed > 10) {
        qWarning() << "SIMULATION FanSpeedLevel change out of range (0-10)" << speed;
        emit fanSpeedLevelChanged(zone, m_zoneMap[zone].fanSpeed);
        return;
    }

    if (m_zoneMap[zone].fanSpeed == speed)
        return;

    qWarning() << "SIMULATION FanSpeedLevel for Zone" << zone << "changed to" << speed;

    m_zoneMap[zone].fanSpeed = speed;
    emit fanSpeedLevelChanged(zone, speed);
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

int ClimateControlBackend::targetTemperature(const QString &zone) const
{
    if (!m_zoneMap.contains(zone))
        return -1;

    return m_zoneMap[zone].targetTemperature;
}

int ClimateControlBackend::seatCooler(const QString &zone) const
{
    if (!m_zoneMap.contains(zone))
        return -1;

    return m_zoneMap[zone].seatCooler;
}

int ClimateControlBackend::seatHeater(const QString &zone) const
{
    if (!m_zoneMap.contains(zone))
        return -1;

    return m_zoneMap[zone].seatHeater;
}

int ClimateControlBackend::steeringWheelHeater(const QString &zone) const
{
    if (!m_zoneMap.contains(zone))
        return -1;

    return m_zoneMap[zone].steeringWheelHeater;
}

int ClimateControlBackend::fanSpeedLevel(const QString &zone) const
{
    if (!m_zoneMap.contains(zone))
        return -1;

    return m_zoneMap[zone].fanSpeed;
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

