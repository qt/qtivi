/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

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
