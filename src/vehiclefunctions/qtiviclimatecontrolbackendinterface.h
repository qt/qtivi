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

    virtual bool hasTargetTemperature(QtIVIClimateZone::Zone) = 0;
    virtual bool hasSeatCooler(QtIVIClimateZone::Zone) = 0;
    virtual bool hasSeatHeater(QtIVIClimateZone::Zone) = 0;
    virtual void setTargetTemperature(QtIVIClimateZone::Zone, int) = 0;
    virtual void setSeatCooler(QtIVIClimateZone::Zone, int) = 0;
    virtual void setSeatHeater(QtIVIClimateZone::Zone, int) = 0;

    virtual QtIVIClimateZone::Zone driverZone() const = 0;

    virtual void setAirflowDirection(QtIVIClimateControl::AirflowDirection) = 0;
    virtual void setAirConditioningEnabled(bool) = 0;
    virtual void setHeaterEnabled(bool) = 0;
    virtual void setAirRecirculationEnabled(bool) = 0;
    virtual void setSteeringWheelHeater(int) = 0;
    virtual void setFanSpeedLevel(int) = 0;

    virtual int targetTemperature(QtIVIClimateZone::Zone) const = 0;
    virtual int seatCooler(QtIVIClimateZone::Zone) const = 0;
    virtual int seatHeater(QtIVIClimateZone::Zone) const = 0;

    virtual QtIVIClimateControl::AirflowDirection airflowDirection() const = 0;
    virtual bool airConditioningEnabled() const = 0;
    virtual bool heaterEnabled() const = 0;
    virtual bool airRecirculationEnabled() const = 0;
    virtual int steeringWheelHeater() const = 0;
    virtual int fanSpeedLevel() const = 0;

signals:
    void targetTemperatureChanged(QtIVIClimateZone::Zone, int);
    void seatCoolerChanged(QtIVIClimateZone::Zone, int);
    void seatHeaterChanged(QtIVIClimateZone::Zone, int);

    void airflowDirectionChanged(QtIVIClimateControl::AirflowDirection);
    void airConditioningEnabledChanged(bool);
    void heaterEnabledChanged(bool);
    void airRecirculationEnabledChanged(bool);
    void steeringWheelHeaterChanged(int);
    void fanSpeedLevelChanged(int);
};

#endif // QTIVICLIMATECONTROLBACKENDINTERFACE_H

