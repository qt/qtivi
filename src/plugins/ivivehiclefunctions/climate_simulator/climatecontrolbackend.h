/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.pelagicore.com/
**
** This file is part of Qt IVI.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions see http://www.pelagicore.com.
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

    // QtIVIClimateControlBackendInterface interface
public:
    QStringList zones() const;
    QtIVIClimateControl::OptionalFeatures climateFeatures() const;
    QtIVIClimateZone::OptionalFeatures zoneFeatures(const QString &zone) const;

    int targetTemperature(const QString &zone) const;
    int seatCooler(const QString &zone) const;
    int seatHeater(const QString &zone) const;
    int steeringWheelHeater(const QString &zone) const;
    int fanSpeedLevel(const QString &zone) const;

    void setTargetTemperature(const QString &zone, int val);
    void setSeatCooler(const QString &zone, int val);
    void setSeatHeater(const QString &zone, int val);
    void setSteeringWheelHeater(const QString &zone, int val);
    void setFanSpeedLevel(const QString &zone, int);

    QtIVIClimateControl::AirflowDirection airflowDirection() const;
    void setAirflowDirection(QtIVIClimateControl::AirflowDirection direction);
    void setAirConditioningEnabled(bool val);
    void setHeaterEnabled(bool val);
    void setAirRecirculationEnabled(bool val);
    bool airConditioningEnabled() const;
    bool heaterEnabled() const;
    bool airRecirculationEnabled() const;

private:

    QtIVIClimateControl::AirflowDirection m_flowDirection;
    bool m_airCondition;
    bool m_heater;
    bool m_airRecirculation;

    struct ZoneBackend {
        QtIVIClimateZone::OptionalFeatures features;

        int targetTemperature;
        int seatCooler;
        int seatHeater;
        int steeringWheelHeater;
        int fanSpeed;
    };

    QMap<QString,ZoneBackend> m_zoneMap;
};

#endif // CLIMATECONTROLBACKEND_H
