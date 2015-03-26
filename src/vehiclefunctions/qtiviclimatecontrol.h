#ifndef CLIMATECONTROL_H
#define CLIMATECONTROL_H

#include <qtivivehiclefunctionsglobal.h>

#include <QtIVIAbstractFeature>
#include <QObject>
#include <QMap>

class QtIVIClimateControl;
class QtIVIClimateControlBackendInterface;

/* Properties based on http://www.w3.org/2014/automotive/data_spec.html#idl-def-ClimateControl
 * This list contains properties per zone.
 */
class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIClimateZone : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Zone zone READ zone CONSTANT)
    Q_PROPERTY(bool hasTargetTemperature READ hasTargetTemperature NOTIFY hasTargetTemperatureChanged)
    Q_PROPERTY(bool hasSeatCooler READ hasSeatCooler NOTIFY hasSeatCoolerChanged)
    Q_PROPERTY(bool hasSeatHeater READ hasSeatHeater NOTIFY hasSeatHeaterChanged)
    /* current setting of the desired temperature (Unit: celsius) */
    Q_PROPERTY(int targetTemperature READ targetTemperature WRITE setTargetTemperature NOTIFY targetTemperatureChanged)
    /* current status of the seat ventilation ( 0: off, 1: least warm, 10: warmest ) */
    Q_PROPERTY(int seatCooler READ seatCooler WRITE setSeatCooler NOTIFY seatCoolerChanged)
    /* current status of the seat warmer ( 0: off, 1: least warm, 10: warmest ) */
    Q_PROPERTY(int seatHeater READ seatHeater WRITE setSeatHeater NOTIFY seatHeaterChanged)

    Q_ENUMS(Zone)

public:
    /* Based on http://www.w3.org/2014/automotive/vehicle_spec.html#idl-def-Zone */
    enum Zone {
        FrontLeft,
        FrontCenter,
        FrontRight,
        RearLeft,
        RearCenter,
        RearRight
    };

    QtIVIClimateZone(Zone zone, QtIVIClimateControl *parent);

    Zone zone() const;

    bool hasTargetTemperature() const;
    bool hasSeatCooler() const;
    bool hasSeatHeater() const;

    int targetTemperature() const;
    int seatCooler() const;
    int seatHeater() const;

public slots:
    void setTargetTemperature(int);
    void setSeatCooler(int);
    void setSeatHeater(int);

signals:
    void hasTargetTemperatureChanged(bool hasTargetTemperature);
    void hasSeatCoolerChanged(bool hasSeatCooler);
    void hasSeatHeaterChanged(bool hasSeatHeater);

    void targetTemperatureChanged(int targetTemperature);
    void seatCoolerChanged(int seatCooler);
    void seatHeaterChanged(int seatHeater);

private slots:
    void onTargetTemperatureChanged(QtIVIClimateZone::Zone z, int t);
    void onSeatCoolerChanged(QtIVIClimateZone::Zone z, int t);
    void onSeatHeaterChanged(QtIVIClimateZone::Zone z, int t);

private:
    friend class QtIVIClimateControl;

    void setHasTargetTemperature(bool);
    void setHasSeatCooler(bool);
    void setHasSeatHeater(bool);

    Zone m_zone;

    bool m_hasTargetTemperature;
    bool m_hasSeatCooler;
    bool m_hasSeatHeater;

    int m_targetTemperature;
    int m_seatCooler;
    int m_seatHeater;
};


/* Properties based on http://www.w3.org/2014/automotive/data_spec.html#idl-def-ClimateControl
 * This list contains the properties for the whole car.
 */
class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIClimateControl : public QtIVIAbstractFeature
{
    Q_OBJECT

    Q_PROPERTY(AirflowDirection airflowDirection READ airflowDirection WRITE setAirflowDirection NOTIFY airflowDirectionChanged)
    /* current status of the air conditioning system: on (true) or off (false) */
    Q_PROPERTY(bool airConditioning READ isAirConditioningEnabled WRITE setAirConditioningEnabled NOTIFY airConditioningEnabledChanged)
    /* current status of the heating system: on (true) or off (false) */
    Q_PROPERTY(bool heater READ isHeaterEnabled WRITE setHeaterEnabled NOTIFY heaterEnabledChanged)
    /* current setting of air recirculation: on (true) or pulling in outside air (false) */
    Q_PROPERTY(bool airRecirculation READ isAirRecirculationEnabled WRITE setAirRecirculationEnabled NOTIFY airRecirculationEnabledChanged)
    /* current status of steering wheel heater ( 0: off, 1: least warm, 10: warmest ) */
    Q_PROPERTY(bool steeringWheelHeater READ isSteeringWheelHeaterEnabled WRITE setSteeringWheelHeaterEnabled NOTIFY steeringWheelHeaterEnabledChanged)
    /* current status of the fan speed of the air flowing (0: off, 1: weakest, 10: strongest ) */
    Q_PROPERTY(int fanSpeedLevel READ fanSpeedLevel WRITE setFanSpeedLevel NOTIFY fanSpeedLevelChanged)

    Q_PROPERTY(QtIVIClimateZone* frontLeftZone READ frontLeftZone CONSTANT)
    Q_PROPERTY(QtIVIClimateZone* frontCenterZone READ frontCenterZone CONSTANT)
    Q_PROPERTY(QtIVIClimateZone* frontRightZone READ frontRightZone CONSTANT)
    Q_PROPERTY(QtIVIClimateZone* rearLeftZone READ rearLeftZone CONSTANT)
    Q_PROPERTY(QtIVIClimateZone* rearCenterZone READ rearCenterZone CONSTANT)
    Q_PROPERTY(QtIVIClimateZone* rearRightZone READ rearRightZone CONSTANT)

    Q_PROPERTY(QtIVIClimateZone* driver READ driver CONSTANT)

    Q_ENUMS(AirflowDirection)

public:
    QtIVIClimateControl(QObject *parent=0);

    /* Based on http://www.w3.org/2014/automotive/data_spec.html#idl-def-AirflowDirection */
    enum AirflowDirection {
        FloorPanel,
        FloorDuct,
        BiLevel,
        DefrostFloor
    };

    AirflowDirection airflowDirection() const;

    bool isAirConditioningEnabled() const;
    bool isHeaterEnabled() const;
    bool isAirRecirculationEnabled() const;
    bool isSteeringWheelHeaterEnabled() const;
    int fanSpeedLevel() const;

    QtIVIClimateZone *climateZone(QtIVIClimateZone::Zone) const;
    QtIVIClimateZone *driver() const;

    QtIVIClimateZone *frontLeftZone() const;
    QtIVIClimateZone *frontCenterZone() const;
    QtIVIClimateZone *frontRightZone() const;
    QtIVIClimateZone *rearLeftZone() const;
    QtIVIClimateZone *rearCenterZone() const;
    QtIVIClimateZone *rearRightZone() const;

public slots:
    void setAirflowDirection(QtIVIClimateControl::AirflowDirection);
    void setAirConditioningEnabled(bool);
    void setHeaterEnabled(bool);
    void setAirRecirculationEnabled(bool);
    void setSteeringWheelHeaterEnabled(bool);
    void setFanSpeedLevel(int);

signals:
    void airflowDirectionChanged(QtIVIClimateControl::AirflowDirection airflowDirection);
    void airConditioningEnabledChanged(bool airConditioning);
    void heaterEnabledChanged(bool heaterEnabled);
    void airRecirculationEnabledChanged(bool airRecirculation);
    void steeringWheelHeaterEnabledChanged(bool steeringWheelHeater);
    void fanSpeedLevelChanged(int fanSpeedLevel);

protected:
    virtual bool acceptServiceObject(QtIVIServiceObject *so);
    virtual void connectToServiceObject(QtIVIServiceObject *so);
    virtual void disconnectFromServiceObject(QtIVIServiceObject *so);
    virtual void clearServiceObject();

private slots:
    void onAirflowDirectionChanged(QtIVIClimateControl::AirflowDirection airflowDirection);
    void onAirConditioningEnabledChanged(bool airConditioning);
    void onHeaterEnabledChanged(bool heater);
    void onAirRecirculationEnabledChanged(bool airRecirculation);
    void onSteeringWheelHeaterEnabledChanged(bool steeringWheelHeater);
    void onFanSpeedLevelChanged(int fanSpeedLevel);

private:
    friend class QtIVIClimateZone;

    QtIVIClimateControlBackendInterface *backend() const;

    AirflowDirection m_airflowDirection;
    bool m_airConditioning;
    bool m_heater;
    bool m_airRecirculation;
    bool m_steeringWheelHeater;
    int m_fanSpeedLevel;

    QMap<QtIVIClimateZone::Zone, QtIVIClimateZone*> m_zones;
};

#endif // CLIMATECONTROL_H
