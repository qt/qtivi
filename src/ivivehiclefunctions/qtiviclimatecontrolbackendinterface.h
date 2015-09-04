/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#ifndef QTIVICLIMATECONTROLBACKENDINTERFACE_H
#define QTIVICLIMATECONTROLBACKENDINTERFACE_H

#include <QObject>

#include "qtiviclimatecontrol.h"

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIClimateControlBackendInterface : public QObject
{
    Q_OBJECT

public:
    QtIVIClimateControlBackendInterface(QObject *parent=0) : QObject(parent) {}

    static QString interfaceName;

    virtual QStringList zones() const = 0;

    virtual QtIVIClimateControl::OptionalFeatures climateFeatures() const = 0;
    virtual QtIVIClimateZone::OptionalFeatures zoneFeatures(const QString &zone) const = 0;

    virtual void setTargetTemperature(const QString &zone, int) = 0;
    virtual void setSeatCooler(const QString &zone, int) = 0;
    virtual void setSeatHeater(const QString &zone, int) = 0;
    virtual void setSteeringWheelHeater(const QString &zone, int) = 0;
    virtual void setFanSpeedLevel(const QString &zone, int) = 0;

    virtual void setAirflowDirection(QtIVIClimateControl::AirflowDirection) = 0;
    virtual void setAirConditioningEnabled(bool) = 0;
    virtual void setHeaterEnabled(bool) = 0;
    virtual void setAirRecirculationEnabled(bool) = 0;

    virtual int targetTemperature(const QString &zone) const = 0;
    virtual int seatCooler(const QString &zone) const = 0;
    virtual int seatHeater(const QString &zone) const = 0;
    virtual int steeringWheelHeater(const QString &zone) const = 0;
    virtual int fanSpeedLevel(const QString &zone) const = 0;

    virtual QtIVIClimateControl::AirflowDirection airflowDirection() const = 0;
    virtual bool airConditioningEnabled() const = 0;
    virtual bool heaterEnabled() const = 0;
    virtual bool airRecirculationEnabled() const = 0;

signals:
    void targetTemperatureChanged(const QString &zone, int);
    void seatCoolerChanged(const QString &zone, int);
    void seatHeaterChanged(const QString &zone, int);
    void steeringWheelHeaterChanged(const QString &zone, int);
    void fanSpeedLevelChanged(const QString &zone, int);

    void airflowDirectionChanged(QtIVIClimateControl::AirflowDirection);
    void airConditioningEnabledChanged(bool);
    void heaterEnabledChanged(bool);
    void airRecirculationEnabledChanged(bool);
};

#endif // QTIVICLIMATECONTROLBACKENDINTERFACE_H

