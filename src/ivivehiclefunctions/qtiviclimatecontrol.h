/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
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
class QtIVIClimateControlPrivate;

static const QLatin1String QtIVIStringClimateControlInterfaceName("com.qt-project.ClimateControl");

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIClimateControl : public QtIVIAbstractZonedFeature
{
    Q_OBJECT
    Q_PROPERTY(QtIVIProperty* airConditioning READ airConditioningProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* airflowDirection READ airflowDirectionProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* heater READ heaterProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* airRecirculation READ airRecirculationProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* fanSpeedLevel READ fanSpeedLevelProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* steeringWheelHeater READ steeringWheelHeaterProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* targetTemperature READ targetTemperatureProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* seatCooler READ seatCoolerProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* seatHeater READ seatHeaterProperty CONSTANT)

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
    ~QtIVIClimateControl();

    virtual void connectToServiceObject(QtIVIServiceObject *serviceObject) Q_DECL_OVERRIDE;
    virtual void clearServiceObject() Q_DECL_OVERRIDE;

    bool isAirConditioningEnabled() const;
    QtIVIPropertyAttribute<bool> airConditioningAttribute() const;
    QtIVIProperty* airConditioningProperty() const;
    QtIVIClimateControl::AirflowDirection airflowDirection() const;
    QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection> airflowDirectionAttribute() const;
    QtIVIProperty* airflowDirectionProperty() const;
    bool isHeaterEnabled() const;
    QtIVIPropertyAttribute<bool> heaterAttribute() const;
    QtIVIProperty* heaterProperty() const;
    bool isAirRecirculationEnabled() const;
    QtIVIPropertyAttribute<bool> airRecirculationAttribute() const;
    QtIVIProperty* airRecirculationProperty() const;
    int steeringWheelHeater() const;
    QtIVIPropertyAttribute<int> steeringWheelHeaterAttribute() const;
    QtIVIProperty* steeringWheelHeaterProperty() const;
    int fanSpeedLevel() const;
    QtIVIPropertyAttribute<int> fanSpeedLevelAttribute() const;
    QtIVIProperty* fanSpeedLevelProperty() const;
    int targetTemperature() const;
    QtIVIPropertyAttribute<int> targetTemperatureAttribute() const;
    QtIVIProperty* targetTemperatureProperty() const;
    int seatCooler() const;
    QtIVIPropertyAttribute<int> seatCoolerAttribute() const;
    QtIVIProperty* seatCoolerProperty() const;
    int seatHeater() const;
    QtIVIPropertyAttribute<int> seatHeaterAttribute() const;
    QtIVIProperty* seatHeaterProperty() const;

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
    QtIVIClimateControlPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QtIVIClimateControl)
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QtIVIClimateControl::AirflowDirection)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<int>)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<bool>)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection>)

#endif // CLIMATECONTROL_H
