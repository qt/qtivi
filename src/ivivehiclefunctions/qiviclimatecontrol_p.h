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

#ifndef QIVICLIMATECONTROL_P_H
#define QIVICLIMATECONTROL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtIviCore/QIviProperty>
#include "private/qiviabstractzonedfeature_p.h"

#include "qiviclimatecontrol.h"

QT_BEGIN_NAMESPACE

class QIviClimateControlPrivate : public QIviAbstractZonedFeaturePrivate
{
public:
    QIviClimateControlPrivate(const QString &interface, const QString &zone, QIviClimateControl *parent);

    void init();
    void clearToDefaults();

    void onAirflowDirectionsChanged(QIviClimateControl::AirflowDirections value, const QString &zone);
    void onAirflowDirectionsAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::AirflowDirections> &airflowDirectionsAttribute, const QString &zone);
    void onAirConditioningEnabledChanged(bool enabled, const QString &zone);
    void onAirConditioningAttributeChanged(const QIviPropertyAttribute<bool> &airConditioningEnabledAttribute, const QString &zone);
    void onHeaterEnabledChanged(bool enabled, const QString &zone);
    void onHeaterAttributeChanged(const QIviPropertyAttribute<bool> &heaterEnabledAttribute, const QString &zone);
    void onSteeringWheelHeaterChanged(int value, const QString &zone);
    void onSteeringWheelHeaterAttributeChanged(const QIviPropertyAttribute<int> &steeringWheelHeaterAttribute, const QString &zone);
    void onFanSpeedLevelChanged(int value, const QString &zone);
    void onFanSpeedLevelAttributeChanged(const QIviPropertyAttribute<int> &fanSpeedAttribute, const QString &zone);
    void onTargetTemperatureChanged(int temperature, const QString &zone);
    void onTargetTemperatureAttributeChanged(const QIviPropertyAttribute<int> &temperatureAttribute, const QString &zone);
    void onSeatCoolerChanged(int value, const QString &zone);
    void onSeatCoolerAttributeChanged(const QIviPropertyAttribute<int> &seatCoolerAttribute, const QString &zone);
    void onSeatHeaterChanged(int value, const QString &zone);
    void onSeatHeaterAttributeChanged(const QIviPropertyAttribute<int> &seatHeaterAttribute, const QString &zone);
    void onOutsideTemperatureChanged(int outsideTemperature, const QString &zone);
    void onOutsideTemperatureAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone);
    void onZoneSynchronizationChanged(bool zoneSynchronization, const QString &zone);
    void onZoneSynchronizationAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone);
    void onDefrostChanged(bool defrost, const QString &zone);
    void onDefrostAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone);
    void onRecirculationModeChanged(QIviClimateControl::RecirculationMode recirculationMode, const QString &zone);
    void onRecirculationModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::RecirculationMode> &attribute, const QString &zone);
    void onRecirculationChanged(bool recirculation, const QString &zone);
    void onRecirculationAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone);
    void onRecirculationSensitivityLevelChanged(int recirculationSensitivityLevel, const QString &zone);
    void onRecirculationSensitivityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone);
    void onClimateModeChanged(QIviClimateControl::ClimateMode climateMode, const QString &zone);
    void onClimateModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::ClimateMode> &attribute, const QString &zone);
    void onAutomaticClimateFanIntensityLevelChanged(int automaticClimateFanIntensityLevel, const QString &zone);
    void onAutomaticClimateFanIntensityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone);

    QIviClimateControlBackendInterface* climateControlBackend();

    QIviClimateControl::AirflowDirections m_airflowDirections;
    QIviPropertyAttribute<QIviClimateControl::AirflowDirections> m_airflowDirectionsAttribute;
    QIviProperty *m_airFlowDirectionProperty;
    bool m_airConditioning;
    QIviPropertyAttribute<bool> m_airConditioningAttribute;
    QIviProperty *m_airConditioningProperty;
    bool m_heater;
    QIviPropertyAttribute<bool> m_heaterAttribute;
    QIviProperty *m_heaterProperty;
    int m_targetTemperature;
    QIviPropertyAttribute<int> m_targetTemperatureAttribute;
    QIviProperty *m_targetTemperatureProperty;
    int m_seatCooler;
    QIviPropertyAttribute<int> m_seatCoolerAttribute;
    QIviProperty *m_seatCoolerProperty;
    int m_seatHeater;
    QIviPropertyAttribute<int> m_seatHeaterAttribute;
    QIviProperty *m_seatHeaterProperty;
    int m_steeringWheelHeater;
    QIviPropertyAttribute<int> m_steeringWheelHeaterAttribute;
    QIviProperty *m_steeringWheelHeaterProperty;
    int m_fanSpeedLevel;
    QIviPropertyAttribute<int> m_fanSpeedLevelAttribute;
    QIviProperty *m_fanSpeedLevelProperty;
    int m_outsideTemperature;
    QIviPropertyAttribute<int> m_outsideTemperatureAttribute;
    QIviProperty *m_outsideTemperatureProperty;
    bool m_zoneSynchronization;
    QIviPropertyAttribute<bool> m_zoneSynchronizationAttribute;
    QIviProperty *m_zoneSynchronizationProperty;
    bool m_defrost;
    QIviPropertyAttribute<bool> m_defrostAttribute;
    QIviProperty *m_defrostProperty;
    QIviClimateControl::RecirculationMode m_recirculationMode;
    QIviPropertyAttribute<QIviClimateControl::RecirculationMode> m_recirculationModeAttribute;
    QIviProperty *m_recirculationModeProperty;
    bool m_recirculation;
    QIviPropertyAttribute<bool> m_recirculationAttribute;
    QIviProperty *m_recirculationProperty;
    int m_recirculationSensitivityLevel;
    QIviPropertyAttribute<int> m_recirculationSensitivityLevelAttribute;
    QIviProperty *m_recirculationSensitivityLevelProperty;
    QIviClimateControl::ClimateMode m_climateMode;
    QIviPropertyAttribute<QIviClimateControl::ClimateMode> m_climateModeAttribute;
    QIviProperty *m_climateModeProperty;
    int m_automaticClimateFanIntensityLevel;
    QIviPropertyAttribute<int> m_automaticClimateFanIntensityLevelAttribute;
    QIviProperty *m_automaticClimateFanIntensityLevelProperty;

    QIviClimateControl * const q_ptr;
    Q_DECLARE_PUBLIC(QIviClimateControl)
};

QT_END_NAMESPACE

#endif // QIVICLIMATECONTROL_P_H
