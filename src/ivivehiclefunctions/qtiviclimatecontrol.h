/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#ifndef CLIMATECONTROL_H
#define CLIMATECONTROL_H

#include <QtIVIVehicleFunctions/qtivivehiclefunctionsglobal.h>
#include <QtIVICore/QtIVIAbstractZoneModelFeature>
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

    Q_PROPERTY(QString zone READ zone CONSTANT)

    /* current setting of the desired temperature (Unit: celsius) */
    Q_PROPERTY(int targetTemperature READ targetTemperature WRITE setTargetTemperature NOTIFY targetTemperatureChanged)
    /* current status of the seat ventilation ( 0: off, 1: least warm, 10: warmest ) */
    Q_PROPERTY(int seatCooler READ seatCooler WRITE setSeatCooler NOTIFY seatCoolerChanged)
    /* current status of the seat warmer ( 0: off, 1: least warm, 10: warmest ) */
    Q_PROPERTY(int seatHeater READ seatHeater WRITE setSeatHeater NOTIFY seatHeaterChanged)
    /* current status of steering wheel heater ( 0: off, 1: least warm, 10: warmest ) */
    Q_PROPERTY(int steeringWheelHeater READ steeringWheelHeater WRITE setSteeringWheelHeater NOTIFY steeringWheelHeaterChanged)
    /* current status of the fan speed of the air flowing (0: off, 1: weakest, 10: strongest ) */
    Q_PROPERTY(int fanSpeedLevel READ fanSpeedLevel WRITE setFanSpeedLevel NOTIFY fanSpeedLevelChanged)

    Q_ENUMS(OptionalFeature)

public:
    enum OptionalFeature {
        None = 0x0,
        HasTargetTemperature = 0x1,
        HasSeatCooler = 0x2,
        HasSeatHeater = 0x4,
        HasSteeringWheelHeater = 0x8,
        HasFanSpeed = 0x10
    };
    Q_DECLARE_FLAGS(OptionalFeatures, OptionalFeature)

    bool hasFeature(OptionalFeature f) const;

    QString zone() const;

    int targetTemperature() const;
    int seatCooler() const;
    int seatHeater() const;
    int steeringWheelHeater() const;
    int fanSpeedLevel() const;

public slots:
    void setTargetTemperature(int);
    void setSeatCooler(int);
    void setSeatHeater(int);
    void setSteeringWheelHeater(int);
    void setFanSpeedLevel(int);

signals:
    void targetTemperatureChanged(int targetTemperature);
    void seatCoolerChanged(int seatCooler);
    void seatHeaterChanged(int seatHeater);
    void steeringWheelHeaterChanged(int steeringWheelHeater);
    void fanSpeedLevelChanged(int fanSpeedLevel);

private slots:
    void onTargetTemperatureChanged(const QString &z, int t);
    void onSeatCoolerChanged(const QString &z, int t);
    void onSeatHeaterChanged(const QString &z, int t);
    void onSteeringWheelHeaterChanged(const QString &z, int steeringWheelHeater);
    void onFanSpeedLevelChanged(const QString &z, int fanSpeedLevel);

private:
    friend class QtIVIClimateControl;

    QtIVIClimateZone(const QString &zone, QtIVIClimateControl *parent, QtIVIClimateControlBackendInterface *backend);

    QString m_zone;

    int m_targetTemperature;
    int m_seatCooler;
    int m_seatHeater;
    int m_steeringWheelHeater;
    int m_fanSpeedLevel;

    QFlags<OptionalFeature> m_features;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QtIVIClimateZone::OptionalFeatures)


/* Properties based on http://www.w3.org/2014/automotive/data_spec.html#idl-def-ClimateControl
 * This list contains the properties for the whole car.
 */
class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIClimateControl : public QtIVIAbstractZoneModelFeature
{
    Q_OBJECT

    /* airflow direction */
    Q_PROPERTY(AirflowDirection airflowDirection READ airflowDirection WRITE setAirflowDirection NOTIFY airflowDirectionChanged)
    /* current status of the air conditioning system: on (true) or off (false) */
    Q_PROPERTY(bool airConditioning READ isAirConditioningEnabled WRITE setAirConditioningEnabled NOTIFY airConditioningEnabledChanged)
    /* current status of the heating system: on (true) or off (false) */
    Q_PROPERTY(bool heater READ isHeaterEnabled WRITE setHeaterEnabled NOTIFY heaterEnabledChanged)
    /* current setting of air recirculation: on (true) or pulling in outside air (false) */
    Q_PROPERTY(bool airRecirculation READ isAirRecirculationEnabled WRITE setAirRecirculationEnabled NOTIFY airRecirculationEnabledChanged)

    Q_ENUMS(AirflowDirection)
    Q_ENUMS(OptionalFeature)

public:
    QtIVIClimateControl(QObject *parent=0);

    virtual QStringList zones() const;
    virtual QObject *zoneObject(const QString &z) const;
    QtIVIClimateZone *climateZoneObject(const QString &z) const;

    enum OptionalFeature {
        None = 0x0,
        HasAirflowDirection = 0x1,
        HasAirConditioning = 0x2,
        HasHeater = 0x4,
        HasAirRecirculation = 0x8
    };
    Q_DECLARE_FLAGS(OptionalFeatures, OptionalFeature)

    bool hasFeature(OptionalFeature f) const;

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

public slots:
    void setAirflowDirection(QtIVIClimateControl::AirflowDirection);
    void setAirConditioningEnabled(bool);
    void setHeaterEnabled(bool);
    void setAirRecirculationEnabled(bool);

signals:
    void airflowDirectionChanged(QtIVIClimateControl::AirflowDirection airflowDirection);
    void airConditioningEnabledChanged(bool airConditioning);
    void heaterEnabledChanged(bool heaterEnabled);
    void airRecirculationEnabledChanged(bool airRecirculation);

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

private:
    friend class QtIVIClimateZone;

    AirflowDirection m_airflowDirection;
    bool m_airConditioning;
    bool m_heater;
    bool m_airRecirculation;

    QtIVIClimateControlBackendInterface *backend() const;

    QMap<QString, QtIVIClimateZone*> m_zones;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QtIVIClimateControl::OptionalFeatures)

#endif // CLIMATECONTROL_H
