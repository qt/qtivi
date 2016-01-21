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

#ifndef CLIMATECONTROL_H
#define CLIMATECONTROL_H

#include <QtIVICore/QtIVIAbstractZonedFeature>
#include <QtIVICore/QtIVIProperty>
#include <QtIVIVehicleFunctions/qtivivehiclefunctionsglobal.h>

QT_BEGIN_NAMESPACE

class QtIVIClimateControlBackendInterface;

static const QLatin1String QtIVIStringClimateControlInterfaceName("com.qt-project.ClimateControl");

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIClimateControl : public QtIVIAbstractZonedFeature
{
    Q_OBJECT
    Q_PROPERTY(QtIVIProperty* airConditioning MEMBER m_qmlAirConditioning CONSTANT)
    Q_PROPERTY(QtIVIProperty* airflowDirection MEMBER m_qmlAirFlowDirection CONSTANT)
    Q_PROPERTY(QtIVIProperty* heater MEMBER m_qmlHeater CONSTANT)
    Q_PROPERTY(QtIVIProperty* airRecirculation MEMBER m_qmlAirRecirculation CONSTANT)
    Q_PROPERTY(QtIVIProperty* fanSpeedLevel MEMBER m_qmlFanSpeedLevel CONSTANT)
    Q_PROPERTY(QtIVIProperty* steeringWheelHeater MEMBER m_qmlSteeringWheelHeater CONSTANT)
    Q_PROPERTY(QtIVIProperty* targetTemperature MEMBER m_qmlTargetTemperature CONSTANT)
    Q_PROPERTY(QtIVIProperty* seatCooler MEMBER m_qmlSeatCooler CONSTANT)
    Q_PROPERTY(QtIVIProperty* seatHeater MEMBER m_qmlSeatHeater CONSTANT)

public:
    enum AirflowDirection {
        None,
        FloorPanel,
        FloorDuct,
        BiLevel,
        DefrostFloor
    };
    Q_ENUM(AirflowDirection)

    QtIVIClimateControl(const QString &zone=QString(), QObject* parent=0);

    virtual void connectToServiceObject(QtIVIServiceObject *serviceObject) Q_DECL_OVERRIDE;
    virtual void clearServiceObject() Q_DECL_OVERRIDE;

    bool isAirConditioningEnabled() const;
    QtIVIPropertyAttribute<bool> airConditioningAttribute() const;
    QtIVIClimateControl::AirflowDirection airflowDirection() const;
    QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection> airflowDirectionAttribute() const;
    bool isHeaterEnabled() const;
    QtIVIPropertyAttribute<bool> heaterAttribute() const;
    bool isAirRecirculationEnabled() const;
    QtIVIPropertyAttribute<bool> airRecirculationAttribute() const;
    int steeringWheelHeater() const;
    QtIVIPropertyAttribute<int> steeringWheelHeaterAttribute() const;
    int fanSpeedLevel() const;
    QtIVIPropertyAttribute<int> fanSpeedLevelAttribute() const;
    int targetTemperature() const;
    QtIVIPropertyAttribute<int> targetTemperatureAttribute() const;
    int seatCooler() const;
    QtIVIPropertyAttribute<int> seatCoolerAttribute() const;
    int seatHeater() const;
    QtIVIPropertyAttribute<int> seatHeaterAttribute() const;

public Q_SLOTS:
    void setAirConditioningEnabled(bool enabled);
    void setAirflowDirection(QtIVIClimateControl::AirflowDirection value);
    void setHeaterEnabled(bool enabled);
    void setAirRecirculationEnabled(bool enabled);
    void setSteeringWheelHeater(int value);
    void setFanSpeedLevel(int value);
    void setTargetTemperature(int targetTemperature);
    void setSeatCooler(int seatCooler);
    void setSeatHeater(int seatHeater);

Q_SIGNALS:
    void airflowDirectionChanged(QtIVIClimateControl::AirflowDirection value);
    void airflowDirectionAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection> &attribute);
    void airConditioningEnabledChanged(bool enabled);
    void airConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
    void heaterEnabledChanged(bool enabled);
    void heaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
    void airRecirculationEnabledChanged(bool enabled);
    void airRecirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
    void steeringWheelHeaterChanged(int value);
    void steeringWheelHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
    void fanSpeedLevelChanged(int value);
    void fanSpeedLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
    void targetTemperatureChanged(int temperature);
    void targetTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
    void seatCoolerChanged(int value);
    void seatCoolerAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
    void seatHeaterChanged(int value);
    void seatHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);

private Q_SLOTS:
    void onAirflowDirectionChanged(QtIVIClimateControl::AirflowDirection value, const QString &zone);
    void onAirflowDirectionAttributeChanged(QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection> airflowDirectionAttribute, const QString &zone);
    void onAirConditioningEnabledChanged(bool enabled, const QString &zone);
    void onAirConditioningAttributeChanged(QtIVIPropertyAttribute<bool> airConditioningEnabledAttribute, const QString &zone);
    void onHeaterEnabledChanged(bool enabled, const QString &zone);
    void onHeaterAttributeChanged(QtIVIPropertyAttribute<bool> heaterEnabledAttribute, const QString &zone);
    void onAirRecirculationEnabledChanged(bool enabled, const QString &zone);
    void onAirRecirculationAttributeChanged(QtIVIPropertyAttribute<bool> airRecirculationEnabledAttribute, const QString &zone);
    void onSteeringWheelHeaterChanged(int value, const QString &zone);
    void onSteeringWheelHeaterAttributeChanged(QtIVIPropertyAttribute<int> steeringWheelHeaterAttribute, const QString &zone);
    void onFanSpeedLevelChanged(int value, const QString &zone);
    void onFanSpeedLevelAttributeChanged(QtIVIPropertyAttribute<int> fanSpeedAttribute, const QString &zone);
    void onTargetTemperatureChanged(int temperature, const QString &zone);
    void onTargetTemperatureAttributeChanged(QtIVIPropertyAttribute<int> temperatureAttribute, const QString &zone);
    void onSeatCoolerChanged(int value, const QString &zone);
    void onSeatCoolerAttributeChanged(QtIVIPropertyAttribute<int> seatCoolerAttribute, const QString &zone);
    void onSeatHeaterChanged(int value, const QString &zone);
    void onSeatHeaterAttributeChanged(QtIVIPropertyAttribute<int> seatHeaterAttribute, const QString &zone);

protected:
    virtual QtIVIAbstractZonedFeature* createZoneFeature(const QString &zone) Q_DECL_OVERRIDE;

private:
    QtIVIClimateControlBackendInterface* climateControlBackend();

private:
    AirflowDirection m_airflowDirection;
    QtIVIPropertyAttribute<AirflowDirection> m_airflowDirectionAttribute;
    QtIVIProperty* m_qmlAirFlowDirection;
    bool m_airConditioning;
    QtIVIPropertyAttribute<bool> m_airConditioningAttribute;
    QtIVIProperty* m_qmlAirConditioning;
    bool m_heater;
    QtIVIPropertyAttribute<bool> m_heaterAttribute;
    QtIVIProperty* m_qmlHeater;
    bool m_airRecirculation;
    QtIVIPropertyAttribute<bool> m_airRecirculationAttribute;
    QtIVIProperty* m_qmlAirRecirculation;
    int m_targetTemperature;
    QtIVIPropertyAttribute<int> m_targetTemperatureAttribute;
    QtIVIProperty* m_qmlTargetTemperature;
    int m_seatCooler;
    QtIVIPropertyAttribute<int> m_seatCoolerAttribute;
    QtIVIProperty* m_qmlSeatCooler;
    int m_seatHeater;
    QtIVIPropertyAttribute<int> m_seatHeaterAttribute;
    QtIVIProperty* m_qmlSeatHeater;
    int m_steeringWheelHeater;
    QtIVIPropertyAttribute<int> m_steeringWheelHeaterAttribute;
    QtIVIProperty* m_qmlSteeringWheelHeater;
    int m_fanSpeedLevel;
    QtIVIPropertyAttribute<int> m_fanSpeedLevelAttribute;
    QtIVIProperty* m_qmlFanSpeedLevel;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QtIVIClimateControl::AirflowDirection)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<int>)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<bool>)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection>)

#endif // CLIMATECONTROL_H
