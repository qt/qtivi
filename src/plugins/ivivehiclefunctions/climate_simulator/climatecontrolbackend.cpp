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
    leftZone.seatCooler = 10;
    leftZone.seatHeater = 10;
    leftZone.targetTemperature = 20;
    leftZone.steeringWheelHeater = 0;
    leftZone.fanSpeed = 10;
    m_zoneMap.insert("FrontLeft",leftZone);

    ZoneBackend rightZone;
    rightZone.seatCooler = 5;
    rightZone.seatHeater = 0;
    rightZone.targetTemperature = 10;
    rightZone.steeringWheelHeater = 0;
    rightZone.fanSpeed = 10;

    m_zoneMap.insert("FrontRight",rightZone);

    ZoneBackend rearZone;
    rearZone.seatCooler = 5;
    rearZone.seatHeater = 0;
    rearZone.targetTemperature = 10;
    rearZone.steeringWheelHeater = 0;
    rearZone.fanSpeed = 10;

    m_zoneMap.insert("Rear",rearZone);
}

ClimateControlBackend::~ClimateControlBackend()
{
}

QStringList ClimateControlBackend::availableZones() const
{
    return m_zoneMap.keys();
}

void ClimateControlBackend::initializeAttributes()
{
    emit airflowDirectionChanged(m_flowDirection);
    emit airConditioningEnabledChanged(m_airCondition);
    emit heaterEnabledChanged(m_heater);
    emit airRecirculationEnabledChanged(m_airRecirculation);

    foreach (QString zone, availableZones()) {
        emit targetTemperatureChanged(m_zoneMap[zone].targetTemperature, zone);
        emit seatCoolerChanged(m_zoneMap[zone].seatCooler, zone);
        emit seatHeaterChanged(m_zoneMap[zone].seatHeater, zone);
        emit steeringWheelHeaterChanged(m_zoneMap[zone].steeringWheelHeater, zone);
        emit fanSpeedLevelChanged(m_zoneMap[zone].fanSpeed, zone);
    }
}

void ClimateControlBackend::setTargetTemperature(int val, const QString &zone)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].targetTemperature == val)
        return;

    qWarning() << "SIMULATION TargetTemperature for Zone" << zone << "changed to" << val;

    m_zoneMap[zone].targetTemperature = val;
    emit targetTemperatureChanged(val, zone);
}

void ClimateControlBackend::setSeatCooler(int val, const QString &zone)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].seatCooler == val)
        return;

    qWarning() << "SIMULATION SeatCooler for Zone" << zone << "changed to" << val;

    m_zoneMap[zone].seatCooler = val;
    emit seatCoolerChanged(val, zone);
}

void ClimateControlBackend::setSeatHeater(int val, const QString &zone)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].seatHeater == val)
        return;

    qWarning() << "SIMULATION SeatHeater for Zone" << zone << "changed to" << val;

    m_zoneMap[zone].seatHeater = val;
    emit seatHeaterChanged(val, zone);
}

void ClimateControlBackend::setSteeringWheelHeater(int val, const QString &zone)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (val < 0 || val > 10) {
        qWarning() << "SIMULATION SteeringWheelHeater for Zone" << zone << "change out of range (0-10)" << val;
        emit steeringWheelHeaterChanged(m_zoneMap[zone].steeringWheelHeater, zone);
        return;
    }

    if (m_zoneMap[zone].steeringWheelHeater == val)
        return;

    qWarning() << "SIMULATION SteeringWheelHeater for" << zone << "changed to" << val;

    m_zoneMap[zone].steeringWheelHeater = val;
    emit steeringWheelHeaterChanged(val, zone);
}

void ClimateControlBackend::setFanSpeedLevel(int speed, const QString &zone)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (speed < 0 || speed > 10) {
        qWarning() << "SIMULATION FanSpeedLevel change out of range (0-10)" << speed;
        emit fanSpeedLevelChanged(m_zoneMap[zone].fanSpeed, zone);
        return;
    }

    if (m_zoneMap[zone].fanSpeed == speed)
        return;

    qWarning() << "SIMULATION FanSpeedLevel for Zone" << zone << "changed to" << speed;

    m_zoneMap[zone].fanSpeed = speed;
    emit fanSpeedLevelChanged(speed, zone);
}

void ClimateControlBackend::setAirflowDirection(QtIVIClimateControl::AirflowDirection direction, const QString &zone)
{
    if (!zone.isEmpty() || m_flowDirection == direction)
        return;

    qWarning() << "SIMULATION AirflowDirection changed to" << direction;

    m_flowDirection = direction;
    emit airflowDirectionChanged(direction);
}

void ClimateControlBackend::setAirConditioningEnabled(bool val, const QString &zone)
{
    if (!zone.isEmpty()|| m_airCondition == val)
        return;

    qWarning() << "SIMULATION AirConditionEnabled changed to" << val;

    m_airCondition = val;
    emit airConditioningEnabledChanged(val);
}

void ClimateControlBackend::setHeaterEnabled(bool val, const QString &zone)
{
    if (!zone.isEmpty() || m_heater == val)
        return;

    qWarning() << "SIMULATION HeaterEnabled changed to" << val;

    m_heater = val;
    emit heaterEnabledChanged(val);
}

void ClimateControlBackend::setAirRecirculationEnabled(bool val, const QString &zone)
{
    if (!zone.isEmpty() || m_heater == val)
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

QtIVIClimateControl::AirflowDirection ClimateControlBackend::airflowDirection(const QString &zone) const
{
    if (!zone.isEmpty())
        return QtIVIClimateControl::None;

    return m_flowDirection;
}

bool ClimateControlBackend::airConditioningEnabled(const QString &zone) const
{
    if (!zone.isEmpty())
        return -1;

    return  m_airCondition;
}

bool ClimateControlBackend::heaterEnabled(const QString &zone) const
{
    if (!zone.isEmpty())
        return -1;

    return  m_heater;
}

bool ClimateControlBackend::airRecirculationEnabled(const QString &zone) const
{
    if (!zone.isEmpty())
        return -1;

    return  m_airRecirculation;
}

