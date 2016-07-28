/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#ifndef QIVICLIMATECONTROLBACKENDINTERFACE_H
#define QIVICLIMATECONTROLBACKENDINTERFACE_H

#include <QtCore/QObject>

#include <QtIviCore/qivizonedfeatureinterface.h>
#include <QtIviCore/qivipropertyattribute.h>
#include <QtIviVehicleFunctions/qiviclimatecontrol.h>

QT_BEGIN_NAMESPACE

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QIviClimateControlBackendInterface : public QIviZonedFeatureInterface
{
    Q_OBJECT

public:
    QIviClimateControlBackendInterface(QObject *parent = Q_NULLPTR);

    virtual void setTargetTemperature(int value, const QString &zone) = 0;
    virtual void setSeatCooler(int value, const QString &zone) = 0;
    virtual void setSeatHeater(int value, const QString &zone) = 0;
    virtual void setSteeringWheelHeater(int value, const QString &zone) = 0;
    virtual void setFanSpeedLevel(int value, const QString &zone) = 0;
    virtual void setAirflowDirections(QIviClimateControl::AirflowDirections airflowDirections, const QString &zone) = 0;
    virtual void setAirConditioningEnabled(bool enabled, const QString &zone) = 0;
    virtual void setHeaterEnabled(bool enabled, const QString &zone) = 0;
    virtual void setZoneSynchronizationEnabled(bool zoneSynchronization, const QString &zone) = 0;
    virtual void setDefrostEnabled(bool defrost, const QString &zone) = 0;
    virtual void setRecirculationMode(QIviClimateControl::RecirculationMode recirculationMode, const QString &zone) = 0;
    virtual void setRecirculationSensitivityLevel(int recirculationSensitivityLevel, const QString &zone) = 0;
    virtual void setClimateMode(QIviClimateControl::ClimateMode climateMode, const QString &zone) = 0;
    virtual void setAutomaticClimateFanIntensityLevel(int automaticClimateFanIntensityLevel, const QString &zone) = 0;

Q_SIGNALS:
    void targetTemperatureChanged(int value, const QString &zone = QString());
    void targetTemperatureAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
    void seatCoolerChanged(int value, const QString &zone = QString());
    void seatCoolerAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
    void seatHeaterChanged(int value, const QString &zone = QString());
    void seatHeaterAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
    void steeringWheelHeaterChanged(int value, const QString &zone = QString());
    void steeringWheelHeaterAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
    void fanSpeedLevelChanged(int value, const QString &zone = QString());
    void fanSpeedLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
    void airflowDirectionsChanged(QIviClimateControl::AirflowDirections value, const QString &zone = QString());
    void airflowDirectionsAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::AirflowDirections> &attribute, const QString &zone = QString());
    void airConditioningEnabledChanged(bool value, const QString &zone = QString());
    void airConditioningAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone = QString());
    void heaterEnabledChanged(bool value, const QString &zone = QString());
    void heaterAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone = QString());
    void outsideTemperatureChanged(int outsideTemperature, const QString &zone = QString());
    void outsideTemperatureAttributeChanged(const QIviPropertyAttribute<int> &outsideTemperature, const QString &zone = QString());
    void zoneSynchronizationEnabledChanged(bool zoneSynchronization, const QString &zone = QString());
    void zoneSynchronizationAttributeChanged(const QIviPropertyAttribute<bool> &zoneSynchronization, const QString &zone = QString());
    void defrostEnabledChanged(bool defrost, const QString &zone = QString());
    void defrostAttributeChanged(const QIviPropertyAttribute<bool> &defrost, const QString &zone = QString());
    void recirculationModeChanged(QIviClimateControl::RecirculationMode recirculationMode, const QString &zone = QString());
    void recirculationModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::RecirculationMode> &recirculationMode, const QString &zone = QString());
    void recirculationEnabledChanged(bool recirculation, const QString &zone = QString());
    void recirculationAttributeChanged(const QIviPropertyAttribute<bool> &recirculation, const QString &zone = QString());
    void recirculationSensitivityLevelChanged(int recirculationSensitivityLevel, const QString &zone = QString());
    void recirculationSensitivityLevelAttributeChanged(const QIviPropertyAttribute<int> &recirculationSensitivityLevel, const QString &zone = QString());
    void climateModeChanged(QIviClimateControl::ClimateMode climateMode, const QString &zone = QString());
    void climateModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::ClimateMode> &climateMode, const QString &zone = QString());
    void automaticClimateFanIntensityLevelChanged(int automaticClimateFanIntensityLevel, const QString &zone = QString());
    void automaticClimateFanIntensityLevelAttributeChanged(const QIviPropertyAttribute<int> &automaticClimateFanIntensityLevel, const QString &zone = QString());
};

#define QIviClimateControl_iid "org.qt-project.qtivi.ClimateControl/1.0"

QT_END_NAMESPACE

#endif // QIVICLIMATECONTROLBACKENDINTERFACE_H

