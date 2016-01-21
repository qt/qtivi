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
    m_airRecirculation(false),
    m_steeringWheelHeater(0),
    m_fanSpeed(2)
{

    ZoneBackend leftZone;
    leftZone.seatCooler = 10;
    leftZone.seatCoolerAttribute = QtIVIPropertyAttribute<int>(0, 10);
    leftZone.seatHeater = 10;
    leftZone.seatHeaterAttribute = QtIVIPropertyAttribute<int>(0, 10);
    leftZone.targetTemperature = 20;
    leftZone.targetTemperatureAttribute = QtIVIPropertyAttribute<int>(0, 10);
    m_zoneMap.insert("FrontLeft",leftZone);

    ZoneBackend rightZone;
    rightZone.seatCooler = 5;
    rightZone.seatCoolerAttribute = QtIVIPropertyAttribute<int>(0, 10);
    rightZone.seatHeater = 0;
    rightZone.seatHeaterAttribute = QtIVIPropertyAttribute<int>(0, 10);
    rightZone.targetTemperature = 20;
    rightZone.targetTemperatureAttribute = QtIVIPropertyAttribute<int>(0, 10);
    m_zoneMap.insert("FrontRight",rightZone);

    ZoneBackend rearZone;
    rearZone.seatCooler = 0;
    rearZone.seatCoolerAttribute = QtIVIPropertyAttribute<int>();
    rearZone.seatHeater = 0;
    rearZone.seatHeaterAttribute = QtIVIPropertyAttribute<int>();
    rearZone.targetTemperature = 10;
    rearZone.targetTemperatureAttribute = QtIVIPropertyAttribute<int>(0, 10);
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
    QVector<QtIVIClimateControl::AirflowDirection> list;
    list << QtIVIClimateControl::BiLevel, QtIVIClimateControl::DefrostFloor;
    emit airflowDirectionAttributeChanged(QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection>(list));
    emit airflowDirectionChanged(m_flowDirection);
    emit airConditioningAttributeChanged(QtIVIPropertyAttribute<bool>(true));
    emit airConditioningEnabledChanged(m_airCondition);
    emit heaterAttributeChanged(QtIVIPropertyAttribute<bool>(true));
    emit heaterEnabledChanged(m_heater);
    emit airRecirculationAttributeChanged(QtIVIPropertyAttribute<bool>(true));
    emit airRecirculationEnabledChanged(m_airRecirculation);
    emit steeringWheelHeaterAttributeChanged(QtIVIPropertyAttribute<int>(false));
    emit steeringWheelHeaterChanged(m_steeringWheelHeater);
    emit fanSpeedLevelAttributeChanged(QtIVIPropertyAttribute<int>(0, 10));
    emit fanSpeedLevelChanged(m_fanSpeed);

    foreach (QString zone, availableZones()) {
        emit targetTemperatureAttributeChanged(m_zoneMap[zone].targetTemperatureAttribute, zone);
        emit targetTemperatureChanged(m_zoneMap[zone].targetTemperature, zone);
        emit seatCoolerAttributeChanged(m_zoneMap[zone].seatCoolerAttribute, zone);
        emit seatCoolerChanged(m_zoneMap[zone].seatCooler, zone);
        emit seatHeaterAttributeChanged(m_zoneMap[zone].seatHeaterAttribute, zone);
        emit seatHeaterChanged(m_zoneMap[zone].seatHeater, zone);
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
    if (!zone.isEmpty() || m_steeringWheelHeater == val)
        return;

    if (val < 0 || val > 10) {
        qWarning() << "SIMULATION SteeringWheelHeater change out of range (0-10)" << val;
        emit steeringWheelHeaterChanged(m_steeringWheelHeater);
        return;
    }

    qWarning() << "SIMULATION SteeringWheelHeater changed to" << val;

    m_steeringWheelHeater = val;
    emit steeringWheelHeaterChanged(val);
}

void ClimateControlBackend::setFanSpeedLevel(int speed, const QString &zone)
{
    if (!zone.isEmpty() || m_fanSpeed == speed)
        return;

    if (speed < 0 || speed > 10) {
        qWarning() << "SIMULATION FanSpeedLevel change out of range (0-10)" << speed;
        emit fanSpeedLevelChanged(m_fanSpeed);
        return;
    }

    qWarning() << "SIMULATION FanSpeedLevel changed to" << speed;

    m_fanSpeed = speed;
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
    if (!zone.isEmpty() || m_airRecirculation == val)
        return;

    qWarning() << "SIMULATION AirRecirculationEnabled changed to" << val;

    m_airRecirculation = val;
    emit airRecirculationEnabledChanged(val);
}
