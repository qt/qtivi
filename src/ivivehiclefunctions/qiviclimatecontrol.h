/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
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

#include <QtIviCore/QIviAbstractZonedFeature>
#include <QtIviCore/QIviProperty>
#include <QtIviVehicleFunctions/qtivivehiclefunctionsglobal.h>

QT_BEGIN_NAMESPACE

class QIviClimateControlBackendInterface;
class QIviClimateControlPrivate;

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QIviClimateControl : public QIviAbstractZonedFeature
{
    Q_OBJECT
    Q_PROPERTY(QIviProperty *airConditioning READ airConditioningProperty CONSTANT)
    Q_PROPERTY(QIviProperty *airflowDirections READ airflowDirectionsProperty CONSTANT)
    Q_PROPERTY(QIviProperty *heater READ heaterProperty CONSTANT)
    Q_PROPERTY(QIviProperty *fanSpeedLevel READ fanSpeedLevelProperty CONSTANT)
    Q_PROPERTY(QIviProperty *steeringWheelHeater READ steeringWheelHeaterProperty CONSTANT)
    Q_PROPERTY(QIviProperty *targetTemperature READ targetTemperatureProperty CONSTANT)
    Q_PROPERTY(QIviProperty *seatCooler READ seatCoolerProperty CONSTANT)
    Q_PROPERTY(QIviProperty *seatHeater READ seatHeaterProperty CONSTANT)
    Q_PROPERTY(QIviProperty *outsideTemperature READ outsideTemperatureProperty CONSTANT)
    Q_PROPERTY(QIviProperty *zoneSynchronization READ zoneSynchronizationProperty CONSTANT)
    Q_PROPERTY(QIviProperty *defrost READ defrostProperty CONSTANT)
    Q_PROPERTY(QIviProperty *recirculationMode READ recirculationModeProperty CONSTANT)
    Q_PROPERTY(QIviProperty *recirculation READ recirculationProperty CONSTANT)
    Q_PROPERTY(QIviProperty *recirculationSensitivityLevel READ recirculationSensitivityLevelProperty CONSTANT)
    Q_PROPERTY(QIviProperty *climateMode READ climateModeProperty CONSTANT)
    Q_PROPERTY(QIviProperty *automaticClimateFanIntensityLevel READ automaticClimateFanIntensityLevelProperty CONSTANT)

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

    QIviClimateControl(const QString &zone = QString(), QObject *parent = nullptr);
    ~QIviClimateControl();

    bool isAirConditioningEnabled() const;
    QIviPropertyAttribute<bool> airConditioningAttribute() const;
    QIviProperty *airConditioningProperty() const;
    QIviClimateControl::AirflowDirections airflowDirections() const;
    QIviPropertyAttribute<QIviClimateControl::AirflowDirections> airflowDirectionsAttribute() const;
    QIviProperty *airflowDirectionsProperty() const;
    bool isHeaterEnabled() const;
    QIviPropertyAttribute<bool> heaterAttribute() const;
    QIviProperty *heaterProperty() const;
    int steeringWheelHeater() const;
    QIviPropertyAttribute<int> steeringWheelHeaterAttribute() const;
    QIviProperty *steeringWheelHeaterProperty() const;
    int fanSpeedLevel() const;
    QIviPropertyAttribute<int> fanSpeedLevelAttribute() const;
    QIviProperty *fanSpeedLevelProperty() const;
    int targetTemperature() const;
    QIviPropertyAttribute<int> targetTemperatureAttribute() const;
    QIviProperty *targetTemperatureProperty() const;
    int seatCooler() const;
    QIviPropertyAttribute<int> seatCoolerAttribute() const;
    QIviProperty *seatCoolerProperty() const;
    int seatHeater() const;
    QIviPropertyAttribute<int> seatHeaterAttribute() const;
    QIviProperty *seatHeaterProperty() const;
    int outsideTemperature() const;
    QIviPropertyAttribute<int> outsideTemperatureAttribute() const;
    QIviProperty *outsideTemperatureProperty() const;
    bool isZoneSynchronizationEnabled() const;
    QIviPropertyAttribute<bool> zoneSynchronizationAttribute() const;
    QIviProperty *zoneSynchronizationProperty() const;
    bool isDefrostEnabled() const;
    QIviPropertyAttribute<bool> defrostAttribute() const;
    QIviProperty *defrostProperty() const;
    QIviClimateControl::RecirculationMode recirculationMode() const;
    QIviPropertyAttribute<QIviClimateControl::RecirculationMode> recirculationModeAttribute() const;
    QIviProperty *recirculationModeProperty() const;
    bool isRecirculationEnabled() const;
    QIviPropertyAttribute<bool> recirculationAttribute() const;
    QIviProperty *recirculationProperty() const;
    int recirculationSensitivityLevel() const;
    QIviPropertyAttribute<int> recirculationSensitivityLevelAttribute() const;
    QIviProperty *recirculationSensitivityLevelProperty() const;
    QIviClimateControl::ClimateMode climateMode() const;
    QIviPropertyAttribute<QIviClimateControl::ClimateMode> climateModeAttribute() const;
    QIviProperty *climateModeProperty() const;
    int automaticClimateFanIntensityLevel() const;
    QIviPropertyAttribute<int> automaticClimateFanIntensityLevelAttribute() const;
    QIviProperty *automaticClimateFanIntensityLevelProperty() const;

public Q_SLOTS:
    void setAirConditioningEnabled(bool enabled);
    void setAirflowDirections(QIviClimateControl::AirflowDirections value);
    void setHeaterEnabled(bool enabled);
    void setSteeringWheelHeater(int value);
    void setFanSpeedLevel(int value);
    void setTargetTemperature(int targetTemperature);
    void setSeatCooler(int seatCooler);
    void setSeatHeater(int seatHeater);
    void setZoneSynchronizationEnabled(bool enabled);
    void setDefrostEnabled(bool enabled);
    void setRecirculationMode(QIviClimateControl::RecirculationMode recirculationMode);
    void setRecirculationSensitivityLevel(int value);
    void setClimateMode(QIviClimateControl::ClimateMode climateMode);
    void setAutomaticClimateFanIntensityLevel(int value);

Q_SIGNALS:
    void airflowDirectionsChanged(QIviClimateControl::AirflowDirections value);
    void airflowDirectionsAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::AirflowDirections> &attribute);
    void airConditioningEnabledChanged(bool enabled);
    void airConditioningAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
    void heaterEnabledChanged(bool enabled);
    void heaterAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
    void steeringWheelHeaterChanged(int value);
    void steeringWheelHeaterAttributeChanged(const QIviPropertyAttribute<int> &attribute);
    void fanSpeedLevelChanged(int value);
    void fanSpeedLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute);
    void targetTemperatureChanged(int temperature);
    void targetTemperatureAttributeChanged(const QIviPropertyAttribute<int> &attribute);
    void seatCoolerChanged(int value);
    void seatCoolerAttributeChanged(const QIviPropertyAttribute<int> &attribute);
    void seatHeaterChanged(int value);
    void seatHeaterAttributeChanged(const QIviPropertyAttribute<int> &attribute);
    void outsideTemperatureChanged(int value);
    void outsideTemperatureAttributeChanged(const QIviPropertyAttribute<int> &attribute);
    void zoneSynchronizationEnabledChanged(bool enabled);
    void zoneSynchronizationAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
    void defrostEnabledChanged(bool enabled);
    void defrostAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
    void recirculationModeChanged(QIviClimateControl::RecirculationMode value);
    void recirculationModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::RecirculationMode> &attribute);
    void recirculationEnabledChanged(bool enabled);
    void recirculationAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
    void recirculationSensitivityLevelChanged(int value);
    void recirculationSensitivityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute);
    void climateModeChanged(QIviClimateControl::ClimateMode value);
    void climateModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::ClimateMode> &attribute);
    void automaticClimateFanIntensityLevelChanged(int value);
    void automaticClimateFanIntensityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute);

protected:
    QIviClimateControl(QIviClimateControlPrivate &dd, QObject *parent = nullptr);
    virtual QIviAbstractZonedFeature *createZoneFeature(const QString &zone) override;

    virtual void connectToServiceObject(QIviServiceObject *serviceObject) override;
    virtual void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIviClimateControl)
    Q_PRIVATE_SLOT(d_func(), void onAirflowDirectionsChanged(QIviClimateControl::AirflowDirections value, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onAirflowDirectionsAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::AirflowDirections> &airflowDirectionsAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onAirConditioningEnabledChanged(bool enabled, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onAirConditioningAttributeChanged(const QIviPropertyAttribute<bool> &airConditioningEnabledAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onHeaterEnabledChanged(bool enabled, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onHeaterAttributeChanged(const QIviPropertyAttribute<bool> &heaterEnabledAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSteeringWheelHeaterChanged(int value, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSteeringWheelHeaterAttributeChanged(const QIviPropertyAttribute<int> &steeringWheelHeaterAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onFanSpeedLevelChanged(int value, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onFanSpeedLevelAttributeChanged(const QIviPropertyAttribute<int> &fanSpeedAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onTargetTemperatureChanged(int temperature, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onTargetTemperatureAttributeChanged(const QIviPropertyAttribute<int> &temperatureAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSeatCoolerChanged(int value, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSeatCoolerAttributeChanged(const QIviPropertyAttribute<int> &seatCoolerAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSeatHeaterChanged(int value, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onSeatHeaterAttributeChanged(const QIviPropertyAttribute<int> &seatHeaterAttribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onOutsideTemperatureChanged(int outsideTemperature, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onOutsideTemperatureAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onZoneSynchronizationChanged(bool isZoneSynchronizationEnabled, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onZoneSynchronizationAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onDefrostChanged(bool isDefrostEnabled, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onDefrostAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationModeChanged(QIviClimateControl::RecirculationMode recirculationMode, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::RecirculationMode> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationChanged(bool isRecirculationEnabled, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationSensitivityLevelChanged(int recirculationSensitivityLevel, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onRecirculationSensitivityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onClimateModeChanged(QIviClimateControl::ClimateMode climateMode, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onClimateModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::ClimateMode> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onAutomaticClimateFanIntensityLevelChanged(int automaticClimateFanIntensityLevel, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onAutomaticClimateFanIntensityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone))
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIviPropertyAttribute<QIviClimateControl::AirflowDirections>)
Q_DECLARE_METATYPE(QIviPropertyAttribute<QIviClimateControl::RecirculationMode>)
Q_DECLARE_METATYPE(QIviPropertyAttribute<QIviClimateControl::ClimateMode>)
Q_DECLARE_OPERATORS_FOR_FLAGS(QIviClimateControl::AirflowDirections)

#endif // CLIMATECONTROL_H
