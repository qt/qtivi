/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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

#ifndef CLIMATECONTROLBACKEND_H
#define CLIMATECONTROLBACKEND_H

#include <QObject>
#include <QtIviVehicleFunctions/QIviClimateControlBackendInterface>

class ClimateControlBackend : public QIviClimateControlBackendInterface
{
public:
    ClimateControlBackend(QObject *parent = 0);
    ~ClimateControlBackend();

public:
    QStringList availableZones() const;

    void initializeAttributes();

    void setTargetTemperature(int val, const QString &zone);
    void setSeatCooler(int val, const QString &zone);
    void setSeatHeater(int val, const QString &zone);
    void setSteeringWheelHeater(int val, const QString &zone);
    void setFanSpeedLevel(int, const QString &zone);
    void setAirflowDirections(QIviClimateControl::AirflowDirections direction, const QString &zone);
    void setAirConditioningEnabled(bool val, const QString &zone);
    void setHeaterEnabled(bool val, const QString &zone);
    void setZoneSynchronizationEnabled(bool zoneSynchronization, const QString &zone);
    void setDefrostEnabled(bool defrost, const QString &zone);
    void setRecirculationMode(QIviClimateControl::RecirculationMode recirculationMode, const QString &zone);
    void setRecirculationSensitivityLevel(int recirculationSensitivityLevel, const QString &zone);
    void setClimateMode(QIviClimateControl::ClimateMode climateMode, const QString &zone);
    void setAutomaticClimateFanIntensityLevel(int automaticClimateFanIntensityLevel, const QString &zone);

private:

    QIviClimateControl::AirflowDirections m_flowDirection;
    bool m_airCondition;
    bool m_heater;
    bool m_airRecirculation;
    bool m_zoneSynchronization;
    bool m_defrost;
    int m_steeringWheelHeater;
    int m_fanSpeed;
    QIviClimateControl::RecirculationMode m_recirculationMode;
    int m_recirculationSensitivityLevel;
    QIviClimateControl::ClimateMode m_climateMode;
    int m_automaticClimateFanIntensityLevel;

    struct ZoneBackend {
        int targetTemperature;
        QIviPropertyAttribute<int> targetTemperatureAttribute;
        int seatCooler;
        QIviPropertyAttribute<int> seatCoolerAttribute;
        int seatHeater;
        QIviPropertyAttribute<int> seatHeaterAttribute;
    };

    QMap<QString,ZoneBackend> m_zoneMap;
};

#endif // CLIMATECONTROLBACKEND_H
