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

#include "climatecontrolbackend.h"

#include <QDebug>

ClimateControlBackend::ClimateControlBackend(QObject *parent) :
    QIviClimateControlBackendInterface(parent),
    m_flowDirection(QIviClimateControl::Floor | QIviClimateControl::Dashboard),
    m_airCondition(true),
    m_heater(true),
    m_airRecirculation(false),
    m_zoneSynchronization(false),
    m_defrost(false),
    m_steeringWheelHeater(0),
    m_fanSpeed(2),
    m_recirculationMode(QIviClimateControl::RecirculationOff),
    m_recirculationSensitivityLevel(0),
    m_climateMode(QIviClimateControl::ClimateOn),
    m_automaticClimateFanIntensityLevel(0)
{

    ZoneBackend leftZone;
    leftZone.seatCooler = 10;
    leftZone.seatCoolerAttribute = QIviPropertyAttribute<int>(0, 10);
    leftZone.seatHeater = 10;
    leftZone.seatHeaterAttribute = QIviPropertyAttribute<int>(0, 10);
    leftZone.targetTemperature = 20;
    leftZone.targetTemperatureAttribute = QIviPropertyAttribute<int>(16, 28);
    m_zoneMap.insert("FrontLeft", leftZone);

    ZoneBackend rightZone;
    rightZone.seatCooler = 5;
    rightZone.seatCoolerAttribute = QIviPropertyAttribute<int>(0, 10);
    rightZone.seatHeater = 0;
    rightZone.seatHeaterAttribute = QIviPropertyAttribute<int>(0, 10);
    rightZone.targetTemperature = 20;
    rightZone.targetTemperatureAttribute = QIviPropertyAttribute<int>(16, 28);
    m_zoneMap.insert("FrontRight", rightZone);

    ZoneBackend rearZone;
    rearZone.seatCooler = 0;
    rearZone.seatCoolerAttribute = QIviPropertyAttribute<int>();
    rearZone.seatHeater = 0;
    rearZone.seatHeaterAttribute = QIviPropertyAttribute<int>();
    rearZone.targetTemperature = 20;
    rearZone.targetTemperatureAttribute = QIviPropertyAttribute<int>(16, 28);
    m_zoneMap.insert("Rear", rearZone);
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
    QVector<QIviClimateControl::AirflowDirections> airflow;
    airflow << (QIviClimateControl::Floor | QIviClimateControl::Dashboard) << QIviClimateControl::Floor << QIviClimateControl::Dashboard;
    emit airflowDirectionsAttributeChanged(QIviPropertyAttribute<QIviClimateControl::AirflowDirections>(airflow));
    emit airflowDirectionsChanged(m_flowDirection);
    emit airConditioningAttributeChanged(QIviPropertyAttribute<bool>(true));
    emit airConditioningEnabledChanged(m_airCondition);
    emit heaterAttributeChanged(QIviPropertyAttribute<bool>(true));
    emit heaterEnabledChanged(m_heater);
    emit zoneSynchronizationEnabledChanged(m_zoneSynchronization);
    emit zoneSynchronizationAttributeChanged(QIviPropertyAttribute<bool>(true));
    emit defrostEnabledChanged(m_defrost);
    emit defrostAttributeChanged(QIviPropertyAttribute<bool>(true));
    emit steeringWheelHeaterAttributeChanged(QIviPropertyAttribute<int>(0, 10));
    emit steeringWheelHeaterChanged(m_steeringWheelHeater);
    emit fanSpeedLevelAttributeChanged(QIviPropertyAttribute<int>(0, 6));
    emit fanSpeedLevelChanged(m_fanSpeed);

    QVector<QIviClimateControl::RecirculationMode> recirculation;
    recirculation << QIviClimateControl::RecirculationOff << QIviClimateControl::RecirculationOn;
    emit recirculationModeChanged(m_recirculationMode);
    emit recirculationModeAttributeChanged(QIviPropertyAttribute<QIviClimateControl::RecirculationMode>(recirculation));
    emit recirculationSensitivityLevelChanged(m_recirculationSensitivityLevel);
    emit recirculationSensitivityLevelAttributeChanged(QIviPropertyAttribute<int>(false));
    emit recirculationEnabledChanged(m_airRecirculation);
    emit recirculationAttributeChanged(QIviPropertyAttribute<bool>(true));

    QVector<QIviClimateControl::ClimateMode> climate;
    climate << QIviClimateControl::ClimateOff << QIviClimateControl::ClimateOn;
    emit climateModeChanged(m_climateMode);
    emit climateModeAttributeChanged(QIviPropertyAttribute<QIviClimateControl::ClimateMode>(climate));
    emit automaticClimateFanIntensityLevelChanged(m_automaticClimateFanIntensityLevel);
    emit automaticClimateFanIntensityLevelAttributeChanged(QIviPropertyAttribute<int>(false));

    const auto zones = availableZones();
    for (const QString &zone : zones) {
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

void ClimateControlBackend::setAirflowDirections(QIviClimateControl::AirflowDirections direction, const QString &zone)
{
    if (!zone.isEmpty() || m_flowDirection == direction)
        return;

    qWarning() << "SIMULATION AirflowDirections changed to" << direction;

    m_flowDirection = direction;
    emit airflowDirectionsChanged(direction);
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

void ClimateControlBackend::setZoneSynchronizationEnabled(bool zoneSynchronization, const QString &zone)
{
    if (!zone.isEmpty() || m_zoneSynchronization == zoneSynchronization)
        return;

    qWarning() << "SIMULATION ZoneSynchronization changed to" << zoneSynchronization;

    m_zoneSynchronization = zoneSynchronization;
    emit zoneSynchronizationEnabledChanged(zoneSynchronization);
}

void ClimateControlBackend::setDefrostEnabled(bool defrost, const QString &zone)
{
    if (!zone.isEmpty() || m_defrost == defrost)
        return;

    qWarning() << "SIMULATION Defrost changed to" << defrost;

    m_defrost = defrost;
    emit defrostEnabledChanged(defrost);
}

void ClimateControlBackend::setRecirculationMode(QIviClimateControl::RecirculationMode recirculationMode, const QString &zone)
{
    if (!zone.isEmpty() || m_recirculationMode == recirculationMode)
        return;

    qWarning() << "SIMULATION RecirculationMode changed to" << recirculationMode;

    m_recirculationMode = recirculationMode;
    emit recirculationModeChanged(recirculationMode);

    bool recirculation = (m_recirculationMode == QIviClimateControl::RecirculationOn);
    if (recirculation != m_airRecirculation) {
        qWarning() << "SIMULATION recirculation changed to" << recirculation;

        m_airRecirculation = recirculation;
        emit recirculationEnabledChanged(m_airRecirculation);
    }
}

void ClimateControlBackend::setRecirculationSensitivityLevel(int recirculationSensitivityLevel, const QString &zone)
{
    Q_UNUSED(recirculationSensitivityLevel);
    Q_UNUSED(zone);
    qWarning() << "SIMULATION Setting RecirculationSensitivityLevel is not supported!";
}

void ClimateControlBackend::setClimateMode(QIviClimateControl::ClimateMode climateMode, const QString &zone)
{
    if (!zone.isEmpty() || m_climateMode == climateMode)
        return;

    qWarning() << "SIMULATION ZoneSynchronization changed to" << climateMode;

    m_climateMode = climateMode;
    emit climateModeChanged(climateMode);
}

void ClimateControlBackend::setAutomaticClimateFanIntensityLevel(int automaticClimateFanIntensityLevel, const QString &zone)
{
    Q_UNUSED(automaticClimateFanIntensityLevel);
    Q_UNUSED(zone);
    qWarning() << "SIMULATION Setting AutomaticClimateFanIntensityLevel is not supported!";
}
