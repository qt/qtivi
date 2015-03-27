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
    bool hasTargetTemperature(QtIVIClimateZone::Zone zone);
    bool hasSeatCooler(QtIVIClimateZone::Zone zone);
    bool hasSeatHeater(QtIVIClimateZone::Zone zone);
    void setTargetTemperature(QtIVIClimateZone::Zone zone, int val);
    void setSeatCooler(QtIVIClimateZone::Zone zone, int val);
    void setSeatHeater(QtIVIClimateZone::Zone zone, int val);
    QtIVIClimateZone::Zone driverZone() const;
    void setAirflowDirection(QtIVIClimateControl::AirflowDirection direction);
    void setAirConditioningEnabled(bool val);
    void setHeaterEnabled(bool val);
    void setAirRecirculationEnabled(bool val);
    void setSteeringWheelHeater(int val);
    void setFanSpeedLevel(int);
    int targetTemperature(QtIVIClimateZone::Zone zone) const;
    int seatCooler(QtIVIClimateZone::Zone zone) const;
    int seatHeater(QtIVIClimateZone::Zone zone) const;
    QtIVIClimateControl::AirflowDirection airflowDirection() const;
    bool airConditioningEnabled() const;
    bool heaterEnabled() const;
    bool airRecirculationEnabled() const;
    int steeringWheelHeater() const;
    int fanSpeedLevel() const;

private:

    QtIVIClimateControl::AirflowDirection m_flowDirection;
    bool m_airCondition;
    bool m_heater;
    bool m_airRecirculation;
    int m_steeringWheelHeater;
    int m_fanSpeed;

    struct ZoneBackend{
        bool hasTargetTemperature;
        int targetTemperature;
        bool hasSeatCooler;
        int seattCooler;
        bool hasSeatHeater;
        int seatHeater;
    };

    QMap<QtIVIClimateZone::Zone,ZoneBackend> m_zoneMap;
};

#endif // CLIMATECONTROLBACKEND_H
