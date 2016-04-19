/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
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
    Q_PROPERTY(QtIVIProperty* airflowDirections READ airflowDirectionsProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* heater READ heaterProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* fanSpeedLevel READ fanSpeedLevelProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* steeringWheelHeater READ steeringWheelHeaterProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* targetTemperature READ targetTemperatureProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* seatCooler READ seatCoolerProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* seatHeater READ seatHeaterProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* outsideTemperature READ outsideTemperatureProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* zoneSynchronization READ zoneSynchronizationProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* defrost READ defrostProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* recirculationMode READ recirculationModeProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* recirculation READ recirculationProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* recirculationSensitivityLevel READ recirculationSensitivityLevelProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* climateMode READ climateModeProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty* automaticClimateFanIntensityLevel READ automaticClimateFanIntensityLevelProperty CONSTANT)

public:
    enum AirflowDirection {
        Windshield = 0x1,
        Dashboard = 0x2,
        Floor = 0x4
    };
    Q_DECLARE_FLAGS(AirflowDirections, AirflowDirection)
    Q_FLAG(AirflowDirections)

    enum RecirculationMode {
        RecirculationOff,
        RecirculationOn,
        AutoRecirculation
    };
    Q_ENUM(RecirculationMode)

    enum ClimateMode {
        ClimateOff,
        ClimateOn,
        AutoClimate
    };
    Q_ENUM(ClimateMode)

    QtIVIClimateControl(const QString &zone=QString(), QObject* parent=0);
    ~QtIVIClimateControl();

    virtual void connectToServiceObject(QtIVIServiceObject *serviceObject) Q_DECL_OVERRIDE;
    virtual void clearServiceObject() Q_DECL_OVERRIDE;

    bool isAirConditioningEnabled() const;
    QtIVIPropertyAttribute<bool> airConditioningAttribute() const;
    QtIVIProperty* airConditioningProperty() const;
    QtIVIClimateControl::AirflowDirections airflowDirections() const;
    QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> airflowDirectionsAttribute() const;
    QtIVIProperty* airflowDirectionsProperty() const;
    bool isHeaterEnabled() const;
    QtIVIPropertyAttribute<bool> heaterAttribute() const;
    QtIVIProperty* heaterProperty() const;
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
    int outsideTemperature() const;
    QtIVIPropertyAttribute<int> outsideTemperatureAttribute() const;
    QtIVIProperty* outsideTemperatureProperty() const;
    bool isZoneSynchronizationEnabled() const;
    QtIVIPropertyAttribute<bool> zoneSynchronizationAttribute() const;
    QtIVIProperty* zoneSynchronizationProperty() const;
    bool isDefrostEnabled() const;
    QtIVIPropertyAttribute<bool> defrostAttribute() const;
    QtIVIProperty* defrostProperty() const;
    QtIVIClimateControl::RecirculationMode recirculationMode() const;
    QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode> recirculationModeAttribute() const;
    QtIVIProperty* recirculationModeProperty() const;
    bool isRecirculationEnabled() const;
    QtIVIPropertyAttribute<bool> recirculationAttribute() const;
    QtIVIProperty* recirculationProperty() const;
    int recirculationSensitivityLevel() const;
    QtIVIPropertyAttribute<int> recirculationSensitivityLevelAttribute() const;
    QtIVIProperty* recirculationSensitivityLevelProperty() const;
    QtIVIClimateControl::ClimateMode climateMode() const;
    QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode> climateModeAttribute() const;
    QtIVIProperty* climateModeProperty() const;
    int automaticClimateFanIntensityLevel() const;
    QtIVIPropertyAttribute<int> automaticClimateFanIntensityLevelAttribute() const;
    QtIVIProperty* automaticClimateFanIntensityLevelProperty() const;

public Q_SLOTS:
    void setAirConditioningEnabled(bool enabled);
    void setAirflowDirections(QtIVIClimateControl::AirflowDirections value);
    void setHeaterEnabled(bool enabled);
    void setSteeringWheelHeater(int value);
    void setFanSpeedLevel(int value);
    void setTargetTemperature(int targetTemperature);
    void setSeatCooler(int seatCooler);
    void setSeatHeater(int seatHeater);
    void setZoneSynchronizationEnabled(bool enabled);
    void setDefrostEnabled(bool enabled);
    void setRecirculationMode(QtIVIClimateControl::RecirculationMode recirculationMode);
    void setRecirculationSensitivityLevel(int value);
    void setClimateMode(QtIVIClimateControl::ClimateMode climateMode);
    void setAutomaticClimateFanIntensityLevel(int value);

Q_SIGNALS:
    void airflowDirectionsChanged(QtIVIClimateControl::AirflowDirections value);
    void airflowDirectionsAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> &attribute);
    void airConditioningEnabledChanged(bool enabled);
    void airConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
    void heaterEnabledChanged(bool enabled);
    void heaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
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
    void outsideTemperatureChanged(int value);
    void outsideTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
    void zoneSynchronizationEnabledChanged(bool enabled);
    void zoneSynchronizationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
    void defrostEnabledChanged(bool enabled);
    void defrostAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
    void recirculationModeChanged(QtIVIClimateControl::RecirculationMode value);
    void recirculationModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode> &attribute);
    void recirculationEnabledChanged(bool enabled);
    void recirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
    void recirculationSensitivityLevelChanged(int value);
    void recirculationSensitivityLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
    void climateModeChanged(QtIVIClimateControl::ClimateMode value);
    void climateModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode> &attribute);
    void automaticClimateFanIntensityLevelChanged(int value);
    void automaticClimateFanIntensityLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);

protected:
    QtIVIClimateControl(QtIVIClimateControlPrivate &dd, QObject *parent = Q_NULLPTR);
    virtual QtIVIAbstractZonedFeature* createZoneFeature(const QString &zone) Q_DECL_OVERRIDE;

private:
    Q_DECLARE_PRIVATE(QtIVIClimateControl)
    Q_PRIVATE_SLOT(d_func(), void onAirflowDirectionsChanged(QtIVIClimateControl::AirflowDirections value, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onAirflowDirectionsAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> &airflowDirectionsAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onAirConditioningEnabledChanged(bool enabled, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onAirConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> &airConditioningEnabledAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onHeaterEnabledChanged(bool enabled, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onHeaterAttributeChanged(const QtIVIPropertyAttribute<bool> &heaterEnabledAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSteeringWheelHeaterChanged(int value, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSteeringWheelHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &steeringWheelHeaterAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onFanSpeedLevelChanged(int value, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onFanSpeedLevelAttributeChanged(const QtIVIPropertyAttribute<int> &fanSpeedAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onTargetTemperatureChanged(int temperature, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onTargetTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &temperatureAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSeatCoolerChanged(int value, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSeatCoolerAttributeChanged(const QtIVIPropertyAttribute<int> &seatCoolerAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSeatHeaterChanged(int value, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSeatHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &seatHeaterAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onOutsideTemperatureChanged(int outsideTemperature, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onOutsideTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onZoneSynchronizationChanged(bool isZoneSynchronizationEnabled, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onZoneSynchronizationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onDefrostChanged(bool isDefrostEnabled, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onDefrostAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationModeChanged(QtIVIClimateControl::RecirculationMode recirculationMode, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationChanged(bool isRecirculationEnabled, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationSensitivityLevelChanged(int recirculationSensitivityLevel, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationSensitivityLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onClimateModeChanged(QtIVIClimateControl::ClimateMode climateMode, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onClimateModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onAutomaticClimateFanIntensityLevelChanged(int automaticClimateFanIntensityLevel, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onAutomaticClimateFanIntensityLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone))
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections>)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode>)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode>)
Q_DECLARE_OPERATORS_FOR_FLAGS(QtIVIClimateControl::AirflowDirections)

#endif // CLIMATECONTROL_H
