#ifndef QTIVICLIMATECONTROLBACKENDINTERFACE_H
#define QTIVICLIMATECONTROLBACKENDINTERFACE_H

#include <QObject>

#include "qtiviclimatecontrol.h"

class QtIVIClimateControlBackendInterface : public QObject
{
    Q_OBJECT

public:
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
    virtual void setSteeringWheelHeaterEnabled(bool) = 0;
    virtual void setFanSpeedLevel(int) = 0;

    virtual int targetTemperature(QtIVIClimateZone::Zone) const = 0;
    virtual int seatCooler(QtIVIClimateZone::Zone) const = 0;
    virtual int seatHeater(QtIVIClimateZone::Zone) const = 0;

    virtual QtIVIClimateControl::AirflowDirection airflowDirection() const = 0;
    virtual bool airConditioningEnabled() const = 0;
    virtual bool heaterEnabled() const = 0;
    virtual bool airRecirculationEnabled() const = 0;
    virtual bool steeringWheelHeaterEnabled() const = 0;
    virtual int fanSpeedLevel() const = 0;

signals:
    void targetTemperatureChanged(QtIVIClimateZone::Zone, int);
    void seatCoolerChanged(QtIVIClimateZone::Zone, int);
    void seatHeaterChanged(QtIVIClimateZone::Zone, int);

    void airflowDirectionChanged(QtIVIClimateControl::AirflowDirection);
    void airConditioningEnabledChanged(bool);
    void heaterEnabledChanged(bool);
    void airRecirculationEnabledChanged(bool);
    void steeringWheelHeaterEnabledChanged(bool);
    void fanSpeedLevelChanged(int);
};

#endif // QTIVICLIMATECONTROLBACKENDINTERFACE_H

