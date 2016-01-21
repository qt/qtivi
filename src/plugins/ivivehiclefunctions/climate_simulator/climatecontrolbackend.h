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

#ifndef CLIMATECONTROLBACKEND_H
#define CLIMATECONTROLBACKEND_H

#include <QObject>
#include <QtIVIVehicleFunctions/QtIVIClimateControlBackendInterface>

class ClimateControlBackend : public QtIVIClimateControlBackendInterface
{
public:
    ClimateControlBackend(QObject* parent = 0);
    ~ClimateControlBackend();

public:
    QStringList availableZones() const;

    void initializeAttributes();

    QtIVIPropertyAttribute<int> targetTemperatureAttribute(const QString &zone) const;
    void setTargetTemperature(int val, const QString &zone);
    void setSeatCooler(int val, const QString &zone);
    void setSeatHeater(int val, const QString &zone);
    void setSteeringWheelHeater(int val, const QString &zone);
    void setFanSpeedLevel(int, const QString &zone);
    void setAirflowDirection(QtIVIClimateControl::AirflowDirection direction, const QString &zone);
    void setAirConditioningEnabled(bool val, const QString &zone);
    void setHeaterEnabled(bool val, const QString &zone);
    void setAirRecirculationEnabled(bool val, const QString &zone);

private:

    QtIVIClimateControl::AirflowDirection m_flowDirection;
    bool m_airCondition;
    bool m_heater;
    bool m_airRecirculation;
    int m_steeringWheelHeater;
    int m_fanSpeed;

    struct ZoneBackend {
        int targetTemperature;
        QtIVIPropertyAttribute<int> targetTemperatureAttribute;
        int seatCooler;
        QtIVIPropertyAttribute<int> seatCoolerAttribute;
        int seatHeater;
        QtIVIPropertyAttribute<int> seatHeaterAttribute;
    };

    QMap<QString,ZoneBackend> m_zoneMap;
};

#endif // CLIMATECONTROLBACKEND_H
