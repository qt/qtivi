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

#include "qiviclimatecontrol.h"
#include "qiviclimatecontrol_p.h"
#include "qiviclimatecontrolbackendinterface.h"
#include <QtIviCore/QIviServiceObject>
#include <QtIviCore/QIviPropertyFactory>

QIviClimateControlPrivate::QIviClimateControlPrivate(const QString &interface, const QString &zone, QIviClimateControl *parent)
    : QIviAbstractZonedFeaturePrivate(interface, zone, parent)
    , m_airflowDirections(0)
    , m_airConditioning(false)
    , m_heater(false)
    , m_targetTemperature(0)
    , m_seatCooler(0)
    , m_seatHeater(0)
    , m_steeringWheelHeater(0)
    , m_fanSpeedLevel(0)
    , m_outsideTemperature(0)
    , m_zoneSynchronization(false)
    , m_defrost(false)
    , m_recirculationMode(QIviClimateControl::RecirculationOff)
    , m_recirculation(false)
    , m_recirculationSensitivityLevel(-1)
    , m_climateMode(QIviClimateControl::ClimateOff)
    , m_automaticClimateFanIntensityLevel(0)
    , q_ptr(parent)
{
}

void QIviClimateControlPrivate::initialize()
{
    QIviAbstractZonedFeaturePrivate::initialize();
    Q_Q(QIviClimateControl);
    m_airFlowDirectionProperty = QIviPropertyFactory<QIviClimateControl::AirflowDirections>::create(q,
                                                                                                     &QIviClimateControl::airflowDirectionsAttribute,
                                                                                                     &QIviClimateControl::airflowDirectionsAttributeChanged,
                                                                                                     &QIviClimateControl::airflowDirections,
                                                                                                     &QIviClimateControl::airflowDirectionsChanged,
                                                                                                     &QIviClimateControl::setAirflowDirections);
    m_airConditioningProperty = QIviPropertyFactory<bool>::create(q,
                                                                   &QIviClimateControl::airConditioningAttribute,
                                                                   &QIviClimateControl::airConditioningAttributeChanged,
                                                                   &QIviClimateControl::isAirConditioningEnabled,
                                                                   &QIviClimateControl::airConditioningEnabledChanged,
                                                                   &QIviClimateControl::setAirConditioningEnabled);
    m_heaterProperty = QIviPropertyFactory<bool>::create(q,
                                                          &QIviClimateControl::heaterAttribute,
                                                          &QIviClimateControl::heaterAttributeChanged,
                                                          &QIviClimateControl::isHeaterEnabled,
                                                          &QIviClimateControl::heaterEnabledChanged,
                                                          &QIviClimateControl::setHeaterEnabled);
    m_targetTemperatureProperty = QIviPropertyFactory<int>::create(q,
                                                                    &QIviClimateControl::targetTemperatureAttribute,
                                                                    &QIviClimateControl::targetTemperatureAttributeChanged,
                                                                    &QIviClimateControl::targetTemperature,
                                                                    &QIviClimateControl::targetTemperatureChanged,
                                                                    &QIviClimateControl::setTargetTemperature);
    m_seatCoolerProperty = QIviPropertyFactory<int>::create(q,
                                                             &QIviClimateControl::seatCoolerAttribute,
                                                             &QIviClimateControl::seatCoolerAttributeChanged,
                                                             &QIviClimateControl::seatCooler,
                                                             &QIviClimateControl::seatCoolerChanged,
                                                             &QIviClimateControl::setSeatCooler);
    m_seatHeaterProperty = QIviPropertyFactory<int>::create(q,
                                                             &QIviClimateControl::seatHeaterAttribute,
                                                             &QIviClimateControl::seatHeaterAttributeChanged,
                                                             &QIviClimateControl::seatHeater,
                                                             &QIviClimateControl::seatHeaterChanged,
                                                             &QIviClimateControl::setSeatHeater);
    m_steeringWheelHeaterProperty = QIviPropertyFactory<int>::create(q,
                                                                      &QIviClimateControl::steeringWheelHeaterAttribute,
                                                                      &QIviClimateControl::steeringWheelHeaterAttributeChanged,
                                                                      &QIviClimateControl::steeringWheelHeater,
                                                                      &QIviClimateControl::steeringWheelHeaterChanged,
                                                                      &QIviClimateControl::setSteeringWheelHeater);
    m_fanSpeedLevelProperty = QIviPropertyFactory<int>::create(q,
                                                                &QIviClimateControl::fanSpeedLevelAttribute,
                                                                &QIviClimateControl::fanSpeedLevelAttributeChanged,
                                                                &QIviClimateControl::fanSpeedLevel,
                                                                &QIviClimateControl::fanSpeedLevelChanged,
                                                                &QIviClimateControl::setFanSpeedLevel);
    m_outsideTemperatureProperty = QIviPropertyFactory<int>::create(q,
                                                                     &QIviClimateControl::outsideTemperatureAttribute,
                                                                     &QIviClimateControl::outsideTemperatureAttributeChanged,
                                                                     &QIviClimateControl::outsideTemperature,
                                                                     &QIviClimateControl::outsideTemperatureChanged);

    m_zoneSynchronizationProperty = QIviPropertyFactory<bool>::create(q,
                                                                       &QIviClimateControl::zoneSynchronizationAttribute,
                                                                       &QIviClimateControl::zoneSynchronizationAttributeChanged,
                                                                       &QIviClimateControl::isZoneSynchronizationEnabled,
                                                                       &QIviClimateControl::zoneSynchronizationEnabledChanged,
                                                                       &QIviClimateControl::setZoneSynchronizationEnabled);

    m_defrostProperty = QIviPropertyFactory<bool>::create(q,
                                                           &QIviClimateControl::defrostAttribute,
                                                           &QIviClimateControl::defrostAttributeChanged,
                                                           &QIviClimateControl::isDefrostEnabled,
                                                           &QIviClimateControl::defrostEnabledChanged,
                                                           &QIviClimateControl::setDefrostEnabled);

    m_recirculationModeProperty = QIviPropertyFactory<QIviClimateControl::RecirculationMode>::create(q,
                                                                                                       &QIviClimateControl::recirculationModeAttribute,
                                                                                                       &QIviClimateControl::recirculationModeAttributeChanged,
                                                                                                       &QIviClimateControl::recirculationMode,
                                                                                                       &QIviClimateControl::recirculationModeChanged,
                                                                                                       &QIviClimateControl::setRecirculationMode);

    m_recirculationProperty = QIviPropertyFactory<bool>::create(q,
                                                                 &QIviClimateControl::recirculationAttribute,
                                                                 &QIviClimateControl::recirculationAttributeChanged,
                                                                 &QIviClimateControl::isRecirculationEnabled,
                                                                 &QIviClimateControl::recirculationEnabledChanged);

    m_recirculationSensitivityLevelProperty = QIviPropertyFactory<int>::create(q,
                                                                                &QIviClimateControl::recirculationSensitivityLevelAttribute,
                                                                                &QIviClimateControl::recirculationSensitivityLevelAttributeChanged,
                                                                                &QIviClimateControl::recirculationSensitivityLevel,
                                                                                &QIviClimateControl::recirculationSensitivityLevelChanged,
                                                                                &QIviClimateControl::setRecirculationSensitivityLevel);

    m_climateModeProperty = QIviPropertyFactory<QIviClimateControl::ClimateMode>::create(q,
                                                                                           &QIviClimateControl::climateModeAttribute,
                                                                                           &QIviClimateControl::climateModeAttributeChanged,
                                                                                           &QIviClimateControl::climateMode,
                                                                                           &QIviClimateControl::climateModeChanged,
                                                                                           &QIviClimateControl::setClimateMode);

    m_automaticClimateFanIntensityLevelProperty = QIviPropertyFactory<int>::create(q,
                                                                                    &QIviClimateControl::automaticClimateFanIntensityLevelAttribute,
                                                                                    &QIviClimateControl::automaticClimateFanIntensityLevelAttributeChanged,
                                                                                    &QIviClimateControl::automaticClimateFanIntensityLevel,
                                                                                    &QIviClimateControl::automaticClimateFanIntensityLevelChanged,
                                                                                    &QIviClimateControl::setAutomaticClimateFanIntensityLevel);
}

void QIviClimateControlPrivate::clearToDefaults()
{
    m_airConditioning = false;
    m_heater = false;
    m_targetTemperature = 0;
    m_seatCooler = 0;
    m_seatHeater = 0;
    m_steeringWheelHeater = 0;
    m_fanSpeedLevel = 0;
    m_outsideTemperature = 0;
    m_zoneSynchronization = false;
    m_defrost = false;
    m_recirculationMode = QIviClimateControl::RecirculationOff;
    m_recirculation = false;
    m_recirculationSensitivityLevel = -1;
    m_climateMode = QIviClimateControl::ClimateOff;
    m_automaticClimateFanIntensityLevel = 0;
}

void QIviClimateControlPrivate::onAirflowDirectionsChanged(QIviClimateControl::AirflowDirections value, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airflowDirections = value;
    emit f->airflowDirectionsChanged(value);
}

void QIviClimateControlPrivate::onAirflowDirectionsAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::AirflowDirections> &airflowDirectionsAttribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airflowDirectionsAttribute = airflowDirectionsAttribute;
    emit f->airflowDirectionsAttributeChanged(airflowDirectionsAttribute);
}

void QIviClimateControlPrivate::onAirConditioningEnabledChanged(bool enabled, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airConditioning = enabled;
    emit f->airConditioningEnabledChanged(enabled);
}

void QIviClimateControlPrivate::onAirConditioningAttributeChanged(const QIviPropertyAttribute<bool> &airConditioningEnabledAttribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airConditioningAttribute = airConditioningEnabledAttribute;
    emit f->airConditioningAttributeChanged(airConditioningEnabledAttribute);
}

void QIviClimateControlPrivate::onHeaterEnabledChanged(bool enabled, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_heater = enabled;
    emit f->heaterEnabledChanged(enabled);
}

void QIviClimateControlPrivate::onHeaterAttributeChanged(const QIviPropertyAttribute<bool> &heaterEnabledAttribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_heaterAttribute = heaterEnabledAttribute;
    emit f->heaterAttributeChanged(heaterEnabledAttribute);
}

void QIviClimateControlPrivate::onSteeringWheelHeaterChanged(int value, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_steeringWheelHeater = value;
    emit f->steeringWheelHeaterChanged(value);
}

void QIviClimateControlPrivate::onSteeringWheelHeaterAttributeChanged(const QIviPropertyAttribute<int> &steeringWheelHeaterAttribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_steeringWheelHeaterAttribute = steeringWheelHeaterAttribute;
    emit f->steeringWheelHeaterAttributeChanged(steeringWheelHeaterAttribute);
}

void QIviClimateControlPrivate::onFanSpeedLevelChanged(int value, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_fanSpeedLevel = value;
    emit f->fanSpeedLevelChanged(value);
}

void QIviClimateControlPrivate::onFanSpeedLevelAttributeChanged(const QIviPropertyAttribute<int> &fanSpeedAttribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_fanSpeedLevelAttribute = fanSpeedAttribute;
    emit f->fanSpeedLevelAttributeChanged(fanSpeedAttribute);
}

void QIviClimateControlPrivate::onTargetTemperatureChanged(int temperature, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_targetTemperature = temperature;
    emit f->targetTemperatureChanged(temperature);
}

void QIviClimateControlPrivate::onTargetTemperatureAttributeChanged(const QIviPropertyAttribute<int> &temperatureAttribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_targetTemperatureAttribute = temperatureAttribute;
    emit f->targetTemperatureAttributeChanged(temperatureAttribute);
}

void QIviClimateControlPrivate::onSeatCoolerChanged(int value, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatCooler = value;
    emit f->seatCoolerChanged(value);
}

void QIviClimateControlPrivate::onSeatCoolerAttributeChanged(const QIviPropertyAttribute<int> &seatCoolerAttribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatCoolerAttribute = seatCoolerAttribute;
    emit f->seatCoolerAttributeChanged(seatCoolerAttribute);
}

void QIviClimateControlPrivate::onSeatHeaterChanged(int value, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatHeater = value;
    emit f->seatHeaterChanged(value);
}

void QIviClimateControlPrivate::onSeatHeaterAttributeChanged(const QIviPropertyAttribute<int> &seatHeaterAttribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatHeaterAttribute = seatHeaterAttribute;
    emit f->seatHeaterAttributeChanged(seatHeaterAttribute);
}

void QIviClimateControlPrivate::onOutsideTemperatureChanged(int outsideTemperature, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_outsideTemperature = outsideTemperature;
    emit f->outsideTemperatureChanged(outsideTemperature);
}

void QIviClimateControlPrivate::onOutsideTemperatureAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_outsideTemperatureAttribute = attribute;
    emit f->outsideTemperatureAttributeChanged(attribute);
}

void QIviClimateControlPrivate::onZoneSynchronizationChanged(bool zoneSynchronization, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_zoneSynchronization = zoneSynchronization;
    emit f->zoneSynchronizationEnabledChanged(zoneSynchronization);
}

void QIviClimateControlPrivate::onZoneSynchronizationAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_zoneSynchronizationAttribute = attribute;
    emit f->zoneSynchronizationAttributeChanged(attribute);
}

void QIviClimateControlPrivate::onDefrostChanged(bool defrost, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_defrost = defrost;
    emit f->defrostEnabledChanged(defrost);
}

void QIviClimateControlPrivate::onDefrostAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_defrostAttribute = attribute;
    emit f->defrostAttributeChanged(attribute);
}

void QIviClimateControlPrivate::onRecirculationModeChanged(QIviClimateControl::RecirculationMode recirculationMode, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculationMode = recirculationMode;
    emit f->recirculationModeChanged(recirculationMode);
}

void QIviClimateControlPrivate::onRecirculationModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::RecirculationMode> &attribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculationModeAttribute = attribute;
    emit f->recirculationModeAttributeChanged(attribute);
}

void QIviClimateControlPrivate::onRecirculationChanged(bool recirculation, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculation = recirculation;
    emit f->recirculationEnabledChanged(recirculation);
}

void QIviClimateControlPrivate::onRecirculationAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculationAttribute = attribute;
    emit f->recirculationAttributeChanged(attribute);
}

void QIviClimateControlPrivate::onRecirculationSensitivityLevelChanged(int recirculationSensitivityLevel, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculationSensitivityLevel = recirculationSensitivityLevel;
    emit f->recirculationSensitivityLevelChanged(recirculationSensitivityLevel);
}

void QIviClimateControlPrivate::onRecirculationSensitivityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculationSensitivityLevelAttribute = attribute;
    emit f->recirculationSensitivityLevelAttributeChanged(attribute);
}

void QIviClimateControlPrivate::onClimateModeChanged(QIviClimateControl::ClimateMode climateMode, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_climateMode = climateMode;
    emit f->climateModeChanged(climateMode);
}

void QIviClimateControlPrivate::onClimateModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::ClimateMode> &attribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_climateModeAttribute = attribute;
    emit f->climateModeAttributeChanged(attribute);
}

void QIviClimateControlPrivate::onAutomaticClimateFanIntensityLevelChanged(int automaticClimateFanIntensityLevel, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_automaticClimateFanIntensityLevel = automaticClimateFanIntensityLevel;
    emit f->automaticClimateFanIntensityLevelChanged(automaticClimateFanIntensityLevel);
}

void QIviClimateControlPrivate::onAutomaticClimateFanIntensityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone)
{
    Q_Q(QIviClimateControl);
    QIviClimateControl *f = qobject_cast<QIviClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_automaticClimateFanIntensityLevelAttribute = attribute;
    emit f->automaticClimateFanIntensityLevelAttributeChanged(attribute);
}

QIviClimateControlBackendInterface *QIviClimateControlPrivate::climateControlBackend()
{
    Q_Q(QIviClimateControl);
    return qobject_cast<QIviClimateControlBackendInterface*>(q->backend());
}

/*!
   \class QIviClimateControl
   \inmodule QtIviVehicleFunctions
   \brief Provides an interface to the climate control.

   The QIviClimateControl provides an interface to the climate control of the vehicle.

   The climate control properties are divided into two categories: central or zoned. The central
   properties are exposed directly through the QIviClimateControl and the zoned properties are
   exposed through zone objects. The zones are retrieved using the \l zoneAt method.

   The QIviClimateControl expects a single backend to be available. It is recommended to use it
   with \l {QIviAbstractFeature::}{discoveryMode} set to \l AutoDiscovery.
*/

/*!
    \qmltype ClimateControl
    \instantiates QIviClimateControl
    \inqmlmodule QtIvi.VehicleFunctions
    \inherits AbstractZonedFeature
    \brief The ClimateControl provides a QML interface to the climate control of the vehicle.
*/

/*!
   \enum QIviClimateControl::AirflowDirection
   \value Windshield
          Direct airflow along the windshield.
   \value Dashboard
          Direct airflow through the dashboard.
   \value Floor
          Direct airflow to the floor.
*/

/*!
   \enum QIviClimateControl::RecirculationMode
   \value RecirculationOff
          The recirculation is turned off.
   \value RecirculationOn
          The recirculation is turned on.
   \value AutoRecirculation
          The recirculation is turning off and on automatically depending on the air quality.
*/

/*!
   \enum QIviClimateControl::ClimateMode
   \value ClimateOff
          The climate system is turned off.
   \value ClimateOn
          The climate system is turned on.
   \value AutoClimate
          The climate system is in automatic mode and is adjusting some parts of the system automatically. E.g. lower the fan speed when the targetTemperature is reached.
*/

/*!
   Constructs a climate control object to the given \a zone.

   If \a zone is not provided the General zone will be created.

   The \a parent argument is passed on to the \l QIviAbstractZonedFeature base class.
*/
QIviClimateControl::QIviClimateControl(const QString &zone, QObject *parent)
    : QIviAbstractZonedFeature(*new QIviClimateControlPrivate(QLatin1String(QIviClimateControl_iid), zone, this), parent)
{
    qRegisterMetaType<QIviPropertyAttribute<QIviClimateControl::AirflowDirections>>();
    qRegisterMetaType<QIviPropertyAttribute<QIviClimateControl::RecirculationMode>>();
    qRegisterMetaType<QIviPropertyAttribute<QIviClimateControl::ClimateMode>>();
}

QIviClimateControl::~QIviClimateControl()
{
}

/*!
   \reimp
*/
void QIviClimateControl::connectToServiceObject(QIviServiceObject *serviceObject)
{
    Q_D(QIviClimateControl);
    QIviAbstractZonedFeature::connectToServiceObject(serviceObject);

    QIviClimateControlBackendInterface *backend = d->climateControlBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::targetTemperatureChanged,
            d, &QIviClimateControlPrivate::onTargetTemperatureChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::targetTemperatureAttributeChanged,
            d, &QIviClimateControlPrivate::onTargetTemperatureAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::seatCoolerChanged,
            d, &QIviClimateControlPrivate::onSeatCoolerChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::seatCoolerAttributeChanged,
            d, &QIviClimateControlPrivate::onSeatCoolerAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::seatHeaterChanged,
            d, &QIviClimateControlPrivate::onSeatHeaterChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::seatHeaterAttributeChanged,
            d, &QIviClimateControlPrivate::onSeatHeaterAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::steeringWheelHeaterChanged,
            d, &QIviClimateControlPrivate::onSteeringWheelHeaterChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::steeringWheelHeaterAttributeChanged,
            d, &QIviClimateControlPrivate::onSteeringWheelHeaterAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::fanSpeedLevelChanged,
            d, &QIviClimateControlPrivate::onFanSpeedLevelChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::fanSpeedLevelAttributeChanged,
            d, &QIviClimateControlPrivate::onFanSpeedLevelAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::airflowDirectionsChanged,
            d, &QIviClimateControlPrivate::onAirflowDirectionsChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::airflowDirectionsAttributeChanged,
            d, &QIviClimateControlPrivate::onAirflowDirectionsAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::airConditioningEnabledChanged,
            d, &QIviClimateControlPrivate::onAirConditioningEnabledChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::airConditioningAttributeChanged,
            d, &QIviClimateControlPrivate::onAirConditioningAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::heaterEnabledChanged,
            d, &QIviClimateControlPrivate::onHeaterEnabledChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::heaterAttributeChanged,
            d, &QIviClimateControlPrivate::onHeaterAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::outsideTemperatureChanged,
            d, &QIviClimateControlPrivate::onOutsideTemperatureChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::outsideTemperatureAttributeChanged,
            d, &QIviClimateControlPrivate::onOutsideTemperatureAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::zoneSynchronizationEnabledChanged,
            d, &QIviClimateControlPrivate::onZoneSynchronizationChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::zoneSynchronizationAttributeChanged,
            d, &QIviClimateControlPrivate::onZoneSynchronizationAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::defrostEnabledChanged,
            d, &QIviClimateControlPrivate::onDefrostChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::defrostAttributeChanged,
            d, &QIviClimateControlPrivate::onDefrostAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::recirculationModeChanged,
            d, &QIviClimateControlPrivate::onRecirculationModeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::recirculationModeAttributeChanged,
            d, &QIviClimateControlPrivate::onRecirculationModeAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::recirculationEnabledChanged,
            d, &QIviClimateControlPrivate::onRecirculationChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::recirculationAttributeChanged,
            d, &QIviClimateControlPrivate::onRecirculationAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::recirculationSensitivityLevelChanged,
            d, &QIviClimateControlPrivate::onRecirculationSensitivityLevelChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::recirculationSensitivityLevelAttributeChanged,
            d, &QIviClimateControlPrivate::onRecirculationSensitivityLevelAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::climateModeChanged,
            d, &QIviClimateControlPrivate::onClimateModeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::climateModeAttributeChanged,
            d, &QIviClimateControlPrivate::onClimateModeAttributeChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::automaticClimateFanIntensityLevelChanged,
            d, &QIviClimateControlPrivate::onAutomaticClimateFanIntensityLevelChanged);
    QObjectPrivate::connect(backend, &QIviClimateControlBackendInterface::automaticClimateFanIntensityLevelAttributeChanged,
            d, &QIviClimateControlPrivate::onAutomaticClimateFanIntensityLevelAttributeChanged);

    backend->initializeAttributes();
}

/*!
   \reimp
*/
void QIviClimateControl::clearServiceObject()
{
    Q_D(QIviClimateControl);
    d->clearToDefaults();
    QIviAbstractZonedFeature::clearServiceObject();
}

QIviClimateControl::QIviClimateControl(QIviClimateControlPrivate &dd, QObject *parent)
    : QIviAbstractZonedFeature(dd, parent)
{
}

/*!
   \reimp
*/
QIviAbstractZonedFeature *QIviClimateControl::createZoneFeature(const QString &zone)
{
    return new QIviClimateControl(zone, this);
}
/*!
   \qmlqtivipropertyBool {QtIvi.VehicleFunctions::ClimateControl::airConditioning}

   \e value is true if the air conditioning is enabled.
*/
/*!
   \property QIviClimateControl::airConditioning

   Holds a QIviProperty of type \e bool where the QIviProperty::value() function indicates if air conditioning is enabled.

   \sa AttributeSystem
   \sa isAirConditioningEnabled() setAirConditioningEnabled() airConditioningAttribute()
 */
/*!
 * Returns true if air conditioning is enabled.
 *
 * \sa setAirConditioningEnabled() airConditioningEnabledChanged() airConditioningAttribute()
 */
bool QIviClimateControl::isAirConditioningEnabled() const
{
    Q_D(const QIviClimateControl);
    return d->m_airConditioning;
}

/*!
 * Returns the attribute defining the boundaries and availability of the air conditioning property.
 *
 * \sa setAirConditioningEnabled() isAirConditioningEnabled() airConditioningEnabledChanged()
 */
QIviPropertyAttribute<bool> QIviClimateControl::airConditioningAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_airConditioningAttribute;
}

QIviProperty *QIviClimateControl::airConditioningProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_airConditioningProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIvi.VehicleFunctions::ClimateControl::airflowDirections}

   \e value holds the airflow directions.
   Available values are:
   \value Windshield
          Direct airflow along the windshield.
   \value Dashboard
          Direct airflow through the dashboard.
   \value Floor
          Direct airflow to the floor.
 */
/*!
   \property QIviClimateControl::airflowDirections

   Holds a QIviProperty of type \e QIviClimateControl::AirflowDirections where the QIviProperty::value() function returns the current air flow directions.

   \sa AttributeSystem
   \sa airflowDirections() setAirflowDirections() airflowDirectionsAttribute()
 */
/*!
 * Returns the current air flow direction.
 *
 * \sa setAirflowDirections() airflowDirectionsChanged() airflowDirectionsAttribute()
 */
QIviClimateControl::AirflowDirections QIviClimateControl::airflowDirections() const
{
    Q_D(const QIviClimateControl);
    return d->m_airflowDirections;
}

/*!
 * Returns the attribute defining the boundaries and availability of the air flow property
 *
 * \sa setAirflowDirections() airflowDirections() airflowDirectionsChanged()
 */
QIviPropertyAttribute<QIviClimateControl::AirflowDirections> QIviClimateControl::airflowDirectionsAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_airflowDirectionsAttribute;
}

QIviProperty *QIviClimateControl::airflowDirectionsProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_airFlowDirectionProperty;
}

/*!
   \qmlqtivipropertyBool {QtIvi.VehicleFunctions::ClimateControl::heater}

   \e value is true if the heater is enabled.
*/
/*!
   \property QIviClimateControl::heater

   Holds a QIviProperty of type \e bool where the QIviProperty::value() function indicates if the heater is enabled.

   \sa AttributeSystem
   \sa isHeaterEnabled() setHeaterEnabled() heaterAttribute()
 */

/*!
 * Returns true if the heater is enabled.
 *
 * \sa setHeaterEnabled() heaterEnabledChanged() heaterAttribute()
 */
bool QIviClimateControl::isHeaterEnabled() const
{
    Q_D(const QIviClimateControl);
    return d->m_heater;
}

/*!
 * Returns the attribute defining the boundaries and availability of the heater property.
 *
 * \sa setHeaterEnabled() isHeaterEnabled() heaterEnabledChanged()
 */
QIviPropertyAttribute<bool> QIviClimateControl::heaterAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_heaterAttribute;
}

QIviProperty *QIviClimateControl::heaterProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_heaterProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIvi.VehicleFunctions::ClimateControl::steeringWheelHeater}

    \e value holds the steering wheel heater level, where the level can be between \c minimumValue(off)
    to \c maximumValue (warmest).
 */
/*!
   \property QIviClimateControl::steeringWheelHeater

   Holds a QIviProperty of type \e int where the QIviProperty::value() function returns the current steering wheel heater level.

   \sa AttributeSystem
   \sa steeringWheelHeater() setSteeringWheelHeater() steeringWheelHeaterAttribute()
 */
/*!
 * Returns the current steering wheel heater level, where the level can be between \c minimumValue(off) and \c maximumValue (warmest).
 *
 * \sa setSteeringWheelHeater() steeringWheelHeaterChanged() steeringWheelHeaterAttribute()
 */
int QIviClimateControl::steeringWheelHeater() const
{
    Q_D(const QIviClimateControl);
    return d->m_steeringWheelHeater;
}

/*!
 * Returns the attribute defining the boundaries and availability of the steering wheel heater property.
 *
 * \sa setSteeringWheelHeater() steeringWheelHeater() steeringWheelHeaterChanged()
 */
QIviPropertyAttribute<int> QIviClimateControl::steeringWheelHeaterAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_steeringWheelHeaterAttribute;
}

QIviProperty *QIviClimateControl::steeringWheelHeaterProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_steeringWheelHeaterProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIvi.VehicleFunctions::ClimateControl::fanSpeedLevel}

    \e value holds the fan speed level, where the level can be between \c minimumValue(off)
    to \c maximumValue (strongest).
 */
/*!
   \property QIviClimateControl::fanSpeedLevel

   Holds a QIviProperty of type \e int where the QIviProperty::value() function returns the current fan speed level.

   \sa AttributeSystem
   \sa fanSpeedLevel() setFanSpeedLevel() fanSpeedLevelAttribute()
 */
/*!
 * Returns the current fan speed level, where the level can be between \c miniumValue(off) and \c maximumValue(strongest).
 *
 * \sa setFanSpeedLevel() fanSpeedLevelChanged() fanSpeedLevelAttribute()
 */
int QIviClimateControl::fanSpeedLevel() const
{
    Q_D(const QIviClimateControl);
    return d->m_fanSpeedLevel;
}

/*!
 * Returns the attribute defining the boundaries and availability of the fan speed level property
 *
 * \sa setFanSpeedLevel() fanSpeedLevel() fanSpeedLevelChanged()
 */
QIviPropertyAttribute<int> QIviClimateControl::fanSpeedLevelAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_fanSpeedLevelAttribute;
}

QIviProperty *QIviClimateControl::fanSpeedLevelProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_fanSpeedLevelProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIvi.VehicleFunctions::ClimateControl::targetTemperature}

   \e value holds the target temperature of the zone expressed in centigrades, where the temperature can be between \c minimumValue(coolest)
   to \c maximumValue (warmest).
 */
/*!
   \property QIviClimateControl::targetTemperature

   Holds a QIviProperty of type \e int where the QIviProperty::value() function returns the current target temperature.

   \sa AttributeSystem
   \sa targetTemperature() setTargetTemperature() targetTemperatureAttribute()
 */
/*!
 * Returns the current target temperature expressed in centigrates.
 *
 * \sa setTargetTemperature() targetTemperatureChanged() targetTemperatureAttribute()
 */
int QIviClimateControl::targetTemperature() const
{
    Q_D(const QIviClimateControl);
    return d->m_targetTemperature;
}

/*!
 * Returns the attribute defining the boundaries and availability of the target temperature property.
 *
 * \sa setTargetTemperature() targetTemperature() targetTemperatureChanged()
 */
QIviPropertyAttribute<int> QIviClimateControl::targetTemperatureAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_targetTemperatureAttribute;
}

QIviProperty *QIviClimateControl::targetTemperatureProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_targetTemperatureProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIvi.VehicleFunctions::ClimateControl::seatCooler}

    \e value holds the seat cooler level, where the level can be between \c minimumValue(off)
    to \c maximumValue (coolest).
 */
/*!
   \property QIviClimateControl::seatCooler

   Holds a QIviProperty of type \e int where the QIviProperty::value() function returns the current seat cooler level.

   \sa AttributeSystem
   \sa seatCooler() setSeatCooler() seatCoolerAttribute()
 */
/*!
 * Returns the current seat cooler level, where the level can be between \c minimumValue(off) and \c maximumValue(coolest).
 *
 * \sa seatCooler() seatCoolerChanged() seatCoolerAttribute()
 */
int QIviClimateControl::seatCooler() const
{
    Q_D(const QIviClimateControl);
    return d->m_seatCooler;
}

/*!
 * Returns the attribute defining the boundaries and availability of the seat cooler property.
 *
 * \sa setSeatCooler() seatCooler() seatCoolerChanged()
 */
QIviPropertyAttribute<int> QIviClimateControl::seatCoolerAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_seatCoolerAttribute;
}

QIviProperty *QIviClimateControl::seatCoolerProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_seatCoolerProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIvi.VehicleFunctions::ClimateControl::seatHeater}

    \e value holds the seat heater level, where the level can be between \c minimumValue(off)
    to \c maximumValue (warmest).
 */
/*!
   \property QIviClimateControl::seatHeater

   Holds a QIviProperty of type \e int where the QIviProperty::value() function returns the current seat heater level.

   \sa AttributeSystem
   \sa seatHeater() setSeatHeater() seatHeaterAttribute()
 */
/*!
 * Returns the current seat heater level, where the level can be between \c minimumValue(off) and \c maximumValue(warmest).
 *
 * \sa seatHeater() seatHeaterChanged() seatHeaterAttribute()
 */
int QIviClimateControl::seatHeater() const
{
    Q_D(const QIviClimateControl);
    return d->m_seatHeater;
}

/*!
 * Returns the attribute defining the boundaries and availability of the seat heater property.
 *
 * \sa setSeatHeater() seatHeater() seatHeaterChanged()
 */
QIviPropertyAttribute<int> QIviClimateControl::seatHeaterAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_seatHeaterAttribute;
}

QIviProperty *QIviClimateControl::seatHeaterProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_seatHeaterProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIvi.VehicleFunctions::ClimateControl::outsideTemperature}

   \e value holds the outside temperature of the zone expressed in centigrades, where the temperature can be between \c minimumValue(coolest)
   to \c maximumValue (warmest).
 */
/*!
   \property QIviClimateControl::outsideTemperature

   Holds a QIviProperty of type \e int where the QIviProperty::value() function returns the current outside temperature.

   \sa AttributeSystem
   \sa outsideTemperature() outsideTemperatureAttribute()
 */
/*!
 * Returns the current outside temperature expressed in centigrates.
 *
 * \sa outsideTemperatureChanged() outsideTemperatureAttribute()
 */
int QIviClimateControl::outsideTemperature() const
{
    Q_D(const QIviClimateControl);
    return d->m_outsideTemperature;
}

/*!
 * Returns the attribute defining the boundaries and availability of the outside temperature property.
 *
 * \sa outsideTemperature() outsideTemperatureChanged()
 */
QIviPropertyAttribute<int> QIviClimateControl::outsideTemperatureAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_outsideTemperatureAttribute;
}

QIviProperty *QIviClimateControl::outsideTemperatureProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_outsideTemperatureProperty;
}

/*!
   \qmlqtivipropertyBool {QtIvi.VehicleFunctions::ClimateControl::zoneSynchronization}

   \e value is true if the zone synchronization is enabled.

   Which zones and properties are synchronized is controlled by the backend implementing it.
*/
/*!
   \property QIviClimateControl::zoneSynchronization

   Holds a QIviProperty of type \e bool where the QIviProperty::value() function indicates if zone synchronization is enabled.

   Which zones and properties are synchronized is controlled by the backend implementing it.

   \sa AttributeSystem
   \sa isZoneSynchronizationEnabled() setZoneSynchronizationEnabled() zoneSynchronizationAttribute()
 */
/*!
 * Returns true if zone synchronization is enabled.
 *
 * \sa setZoneSynchronizationEnabled() zoneSynchronizationEnabledChanged() zoneSynchronizationAttribute()
 */
bool QIviClimateControl::isZoneSynchronizationEnabled() const
{
    Q_D(const QIviClimateControl);
    return d->m_zoneSynchronization;
}

/*!
 * Returns the attribute defining the boundaries and availability of the zone synchronization property.
 *
 * \sa setZoneSynchronizationEnabled() isZoneSynchronizationEnabled() zoneSynchronizationEnabledChanged()
 */
QIviPropertyAttribute<bool> QIviClimateControl::zoneSynchronizationAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_zoneSynchronizationAttribute;
}

QIviProperty *QIviClimateControl::zoneSynchronizationProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_zoneSynchronizationProperty;
}

/*!
   \qmlqtivipropertyBool {QtIvi.VehicleFunctions::ClimateControl::defrost}

   \e value is true if defrost is enabled. Usually that means that the fans are on the highest level to remove ice from the windshield.
*/
/*!
   \property QIviClimateControl::defrost

   Holds a QIviProperty of type \e bool where the QIviProperty::value() function indicates if defrost is enabled.

   \sa AttributeSystem
   \sa isDefrostEnabled() setDefrostEnabled() defrostAttribute()
 */
/*!
 * Returns true if defrost is enabled.
 *
 * \sa setDefrostEnabled() defrostEnabledChanged() defrostAttribute()
 */
bool QIviClimateControl::isDefrostEnabled() const
{
    Q_D(const QIviClimateControl);
    return d->m_defrost;
}

/*!
 * Returns the attribute defining the boundaries and availability of the defrost property.
 *
 * \sa setDefrostEnabled() isDefrostEnabled() defrostEnabledChanged()
 */
QIviPropertyAttribute<bool> QIviClimateControl::defrostAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_defrostAttribute;
}

QIviProperty *QIviClimateControl::defrostProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_defrostProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIvi.VehicleFunctions::ClimateControl::recirculationMode}

   \e value holds the recirculation mode.
   Available values are:
   \value RecirculationOff
          The recirculation is turned off.
   \value RecirculationOn
          The recirculation is turned on.
   \value AutoRecirculation
          The recirculation is turning off and on automatically depending on the air quality.
 */
/*!
   \property QIviClimateControl::recirculationMode

   Holds a QIviProperty of type \e QIviClimateControl::RecirculationMode where the QIviProperty::value() function returns the current recirculation mode.

   \sa AttributeSystem
   \sa recirculationMode() setRecirculationMode() recirculationModeAttribute()
 */
/*!
 * Returns the current recirculation mode.
 *
 * \sa setRecirculationMode() recirculationModeChanged() recirculationModeAttribute()
 */
QIviClimateControl::RecirculationMode QIviClimateControl::recirculationMode() const
{
    Q_D(const QIviClimateControl);
    return d->m_recirculationMode;
}

/*!
 * Returns the attribute defining the boundaries and availability of the recirculationMode property.
 *
 * \sa setRecirculationMode() recirculationMode() recirculationModeChanged()
 */
QIviPropertyAttribute<QIviClimateControl::RecirculationMode> QIviClimateControl::recirculationModeAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_recirculationModeAttribute;
}

QIviProperty *QIviClimateControl::recirculationModeProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_recirculationModeProperty;
}

/*!
   \qmlqtivipropertyBool {QtIvi.VehicleFunctions::ClimateControl::recirculation}

   \e value is true if the recirculation is currently running.
*/
/*!
   \property QIviClimateControl::recirculation

   Holds a QIviProperty of type \e bool where the QIviProperty::value() function indicates if recirculation is running.

   \sa AttributeSystem
   \sa isRecirculationEnabled() recirculationAttribute()
 */
/*!
 * Returns true if defrost is enabled.
 *
 * \sa recirculationEnabledChanged() recirculationAttribute()
 */
bool QIviClimateControl::isRecirculationEnabled() const
{
    Q_D(const QIviClimateControl);
    return d->m_recirculation;
}

/*!
 * Returns the attribute defining the boundaries and availability of the recirculation property.
 *
 * \sa isRecirculationEnabled() recirculationEnabledChanged()
 */
QIviPropertyAttribute<bool> QIviClimateControl::recirculationAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_recirculationAttribute;
}

QIviProperty *QIviClimateControl::recirculationProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_recirculationProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIvi.VehicleFunctions::ClimateControl::recirculationSensitivityLevel}

    \e value holds the sensitivity level of the recirculation system when the recirculationMode is set to AutoRecirculation, where the level can be between \c minimumValue(least sensitive)
    to \c maximumValue(most sensitive).
 */
/*!
   \property QIviClimateControl::recirculationSensitivityLevel

   Holds a QIviProperty of type \e int where the QIviProperty::value() function returns the current sensitivity level of the recicurlcation system when the recirculationMode is set to AutoRecirculation.

   \sa AttributeSystem
   \sa recirculationSensitivityLevel() setRecirculationSensitivityLevel() recirculationSensitivityLevelAttribute()
 */
/*!
 * Returns the current sensitivity level of recicurlcation system when the recirculationMode is set to AutoRecirculation, where the level can be between \c minimumValue(least sensitive) and \c maximumValue(most sensitive).
 *
 * \sa setRecirculationSensitivityLevel() recirculationSensitivityLevelChanged() recirculationSensitivityLevelAttribute()
 */
int QIviClimateControl::recirculationSensitivityLevel() const
{
    Q_D(const QIviClimateControl);
    return d->m_recirculationSensitivityLevel;
}

/*!
 * Returns the attribute defining the boundaries and availability of the recirculationSensitivityLevel property.
 *
 * \sa setRecirculationSensitivityLevel() recirculationSensitivityLevel() recirculationSensitivityLevelChanged()
 */
QIviPropertyAttribute<int> QIviClimateControl::recirculationSensitivityLevelAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_recirculationSensitivityLevelAttribute;
}

QIviProperty *QIviClimateControl::recirculationSensitivityLevelProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_recirculationSensitivityLevelProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIvi.VehicleFunctions::ClimateControl::climateMode}

   \e value holds the climate mode.
   Available values are:
   \value ClimateOff
          The climate system is turned off.
   \value ClimateOn
          The climate system is turned on.
   \value AutoClimate
          The climate system is in automatic mode and is adjusting some parts of the system automatically. e.g. lower the fan speed when the targetTemperature is reached.
 */
/*!
   \property QIviClimateControl::climateMode

   Holds a QIviProperty of type \e QIviClimateControl::ClimateMode where the QIviProperty::value() function returns the current climate mode.

   \sa AttributeSystem
   \sa climateMode() setClimateMode() climateModeAttribute()
 */
/*!
 * Returns the current climate mode.
 *
 * \sa setClimateMode() climateModeChanged() climateModeAttribute()
 */
QIviClimateControl::ClimateMode QIviClimateControl::climateMode() const
{
    Q_D(const QIviClimateControl);
    return d->m_climateMode;
}

/*!
 * Returns the attribute defining the boundaries and availability of the climateMode property.
 *
 * \sa isRecirculationEnabled() recirculationEnabledChanged()
 */
QIviPropertyAttribute<QIviClimateControl::ClimateMode> QIviClimateControl::climateModeAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_climateModeAttribute;
}

QIviProperty *QIviClimateControl::climateModeProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_climateModeProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIvi.VehicleFunctions::ClimateControl::automaticClimateFanIntensityLevel}

    \e value holds the intensity level of the fan when the climateMode is set to AutoClimate, where the level can be between \c minimumValue(least intensity)
    to \c maximumValue(most intensity).
 */
/*!
   \property QIviClimateControl::automaticClimateFanIntensityLevel

   Holds a QIviProperty of type \e int where the QIviProperty::value() function returns the current intensity level of the fan when the climateMode is set to AutoClimate.

   \sa AttributeSystem
   \sa automaticClimateFanIntensityLevel() setAutomaticClimateFanIntensityLevel() automaticClimateFanIntensityLevelAttribute()
 */
/*!
 * Returns the current intensity level of the fan when the climateMode is set to AutoClimate, where the level can be between \c minimumValue(least intensity) and \c maximumValue(most intensity).
 *
 * \sa setAutomaticClimateFanIntensityLevel() automaticClimateFanIntensityLevelChanged() automaticClimateFanIntensityLevelAttribute()
 */
int QIviClimateControl::automaticClimateFanIntensityLevel() const
{
    Q_D(const QIviClimateControl);
    return d->m_automaticClimateFanIntensityLevel;
}

/*!
 * Returns the attribute defining the boundaries and availability of the climateMode property.
 *
 * \sa setAutomaticClimateFanIntensityLevel() automaticClimateFanIntensityLevel() automaticClimateFanIntensityLevelChanged()
 */
QIviPropertyAttribute<int> QIviClimateControl::automaticClimateFanIntensityLevelAttribute() const
{
    Q_D(const QIviClimateControl);
    return d->m_automaticClimateFanIntensityLevelAttribute;
}

QIviProperty *QIviClimateControl::automaticClimateFanIntensityLevelProperty() const
{
    Q_D(const QIviClimateControl);
    return d->m_automaticClimateFanIntensityLevelProperty;
}

/*!
 * Sets the air conditioning system enabled, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isAirConditioningEnabled() airConditioningEnabledChanged() airConditioningAttribute()
 */
void QIviClimateControl::setAirConditioningEnabled(bool enabled)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setAirConditioningEnabled(enabled, zone());
}

/*!
 * Sets the air flow direction to \a direction.
 *
 * \sa airflowDirections() airflowDirectionsChanged() airflowDirectionsAttribute()
 */
void QIviClimateControl::setAirflowDirections(QIviClimateControl::AirflowDirections direction)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setAirflowDirections(direction, zone());
}

/*!
 * Enables the heater, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isHeaterEnabled() heaterEnabledChanged() heaterAttribute()
 */
void QIviClimateControl::setHeaterEnabled(bool enabled)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setHeaterEnabled(enabled, zone());
}

/*!
 * Sets the current steering wheel heater level to \a value, where the level can be between \c minimumValue(off) and \c maximumValue(warmest).
 *
 * \sa steeringWheelHeater() steeringWheelHeaterChanged() steeringWheelHeaterAttribute()
 */
void QIviClimateControl::setSteeringWheelHeater(int value)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setSteeringWheelHeater(value, zone());
}

/*!
 * Sets the current fan speed level to \a value, where the level can be between \c minimumValue(off) and \c maximumValue(strongest).
 *
 * \sa fanSpeedLevel() fanSpeedLevelChanged() fanSpeedLevelAttribute()
 */
void QIviClimateControl::setFanSpeedLevel(int value)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setFanSpeedLevel(value, zone());
}

/*!
 * Sets the current target temperature to \a temperature expressed in centigrades.
 *
 * \sa targetTemperature() targetTemperatureChanged() targetTemperatureAttribute()
 */
void QIviClimateControl::setTargetTemperature(int temperature)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setTargetTemperature(temperature, zone());
}

/*!
 * Sets the current seat cooler level to \a value, where the level can be between \c minimumValue(off) and \c maximumValue(coolest).
 *
 * \sa seatCooler() seatCoolerChanged() seatCoolerAttribute()
 */
void QIviClimateControl::setSeatCooler(int value)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setSeatCooler(value, zone());
}

/*!
 * Sets the current seat heater level to \a value, where the level can be between \c minimumValue(off) and \c maximumValue(warmest).
 *
 * \sa seatHeater() seatHeaterChanged() seatHeaterAttribute()
 */
void QIviClimateControl::setSeatHeater(int value)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setSeatHeater(value, zone());
}

/*!
 * Enables the zone synchronization, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isZoneSynchronizationEnabled() zoneSynchronizationEnabledChanged() zoneSynchronizationAttribute()
 */
void QIviClimateControl::setZoneSynchronizationEnabled(bool enabled)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setZoneSynchronizationEnabled(enabled, zone());
}

/*!
 * Enables defrosting of the windshield, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isDefrostEnabled() defrostEnabledChanged() defrostAttribute()
 */
void QIviClimateControl::setDefrostEnabled(bool enabled)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setDefrostEnabled(enabled, zone());
}

/*!
 * Sets the recirculation mode to \a recirculationMode.
 *
 * \sa recirculationMode() recirculationModeChanged() recirculationModeAttribute()
 */
void QIviClimateControl::setRecirculationMode(QIviClimateControl::RecirculationMode recirculationMode)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setRecirculationMode(recirculationMode, zone());
}

/*!
 * Sets the current sensitivity level for the AutomaticRecirculation mode to \a value, where the level can be between \c minimumValue(least sensitive) and \c maximumValue(most sensitive).
 *
 * \sa recirculationSensitivityLevel() recirculationSensitivityLevelChanged() recirculationSensitivityLevelAttribute()
 */
void QIviClimateControl::setRecirculationSensitivityLevel(int value)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setRecirculationSensitivityLevel(value, zone());
}

/*!
 * Sets the climate mode to \a climateMode.
 *
 * \sa climateMode() climateModeChanged() climateModeAttribute()
 */
void QIviClimateControl::setClimateMode(QIviClimateControl::ClimateMode climateMode)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setClimateMode(climateMode, zone());
}

/*!
 * Sets the current fan intensity level for the AutomaticClimate mode to \a value, where the level can be between \c minimumValue(least intensity) and \c maximumValue(most intensity).
 *
 * \sa automaticClimateFanIntensityLevel() automaticClimateFanIntensityLevelChanged() automaticClimateFanIntensityLevelAttribute()
 */
void QIviClimateControl::setAutomaticClimateFanIntensityLevel(int value)
{
    Q_D(QIviClimateControl);
    if (QIviClimateControlBackendInterface *backend = d->climateControlBackend())
        backend->setAutomaticClimateFanIntensityLevel(value, zone());
}

/*!
 * \fn void QIviClimateControl::airflowDirectionsChanged(QIviClimateControl::AirflowDirections value)
 *
 * This signal is emitted whenever the air flow directions change. The new flow directions are passed as \a value.
 *
 * \sa airflowDirections() setAirflowDirections() airflowDirectionsAttribute()
 */
/*!
 * \fn void QIviClimateControl::airflowDirectionsAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::AirflowDirections> &attribute);
 *
 * This signal is emitted whenever the attribute for the airflowDirections property changes. The new attribute is passed as \a attribute.
 *
 * \sa airflowDirectionsAttribute() airflowDirections()
 */
/*!
 * \fn void QIviClimateControl::airConditioningEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the air conditioning system is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isAirConditioningEnabled() setAirConditioningEnabled() airConditioningAttribute()
 */
/*!
 * \fn void QIviClimateControl::airConditioningAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the airConditioning property changes. The new attribute is passed as \a attribute.
 *
 * \sa airConditioningAttribute() isAirConditioningEnabled()
 */
/*!
 * \fn void QIviClimateControl::heaterEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the heater is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isHeaterEnabled() setHeaterEnabled() heaterAttribute()
 */
/*!
 * \fn void QIviClimateControl::heaterAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the heater property changes. The new attribute is passed as \a attribute.
 *
 * \sa heaterAttribute() isHeaterEnabled()
 */
/*!
 * \fn void QIviClimateControl::steeringWheelHeaterChanged(int value);
 *
 * This signal is emitted whenever the steering wheel heater level changed. The new level is passed as \a value.
 *
 * \sa steeringWheelHeater() setSteeringWheelHeater() steeringWheelHeaterAttribute()
 */
/*!
 * \fn void QIviClimateControl::steeringWheelHeaterAttributeChanged(const QIviPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the steeringWheelHeater property changes. The new attribute is passed as \a attribute.
 *
 * \sa steeringWheelHeaterAttribute() steeringWheelHeater()
 */
/*!
 * \fn void QIviClimateControl::fanSpeedLevelChanged(int value);
 *
 * This signal is emitted whenever the fan speed level changed. The new level is passed as \a value.
 *
 * \sa fanSpeedLevel() setFanSpeedLevel() fanSpeedLevelAttribute()
 */
/*!
 * \fn void QIviClimateControl::fanSpeedLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the fanSpeedLevel property changes. The new attribute is passed as \a attribute.
 *
 * \sa fanSpeedLevelAttribute() fanSpeedLevel()
 */
/*!
 * \fn void QIviClimateControl::targetTemperatureChanged(int temperature);
 *
 * This signal is emitted whenever the target temperature changed. The new temperature is passed as \a temperature.
 *
 * \sa targetTemperature() setTargetTemperature() targetTemperatureAttribute()
 */
/*!
 * \fn void QIviClimateControl::targetTemperatureAttributeChanged(const QIviPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the targetTemperature property changes. The new attribute is passed as \a attribute.
 *
 * \sa targetTemperatureAttribute() targetTemperature()
 */
/*!
 * \fn void QIviClimateControl::seatCoolerChanged(int value);
 *
 * This signal is emitted whenever the seat cooler level changed. The new level is passed as \a value.
 *
 * \sa seatCooler() setSeatCooler() seatCoolerAttribute()
 */
/*!
 * \fn void QIviClimateControl::seatCoolerAttributeChanged(const QIviPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the seatCooler property changes. The new attribute is passed as \a attribute.
 *
 * \sa seatCoolerAttribute() seatCooler()
 */
/*!
 * \fn void QIviClimateControl::seatHeaterChanged(int value);
 *
 * This signal is emitted whenever the seat heater level changed. The new level is passed as \a value.
 *
 * \sa seatHeater() setSeatHeater() seatHeaterAttribute()
 */
/*!
 * \fn void QIviClimateControl::seatHeaterAttributeChanged(const QIviPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the heater property changes. The new attribute is passed as \a attribute.
 *
 * \sa heaterAttribute() isHeaterEnabled()
 */
/*!
 * \fn void QIviClimateControl::outsideTemperatureChanged(int value);
 *
 * This signal is emitted whenever the outside temperature changes. The new temperature is passed as \a value.
 *
 * \sa outsideTemperature() outsideTemperatureAttribute()
 */
/*!
 * \fn void QIviClimateControl::outsideTemperatureAttributeChanged(const QIviPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the outsideTemperature property changes. The new attribute is passed as \a attribute.
 *
 * \sa outsideTemperatureAttribute() outsideTemperature()
 */
/*!
 * \fn void QIviClimateControl::zoneSynchronizationEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the zone synchronization is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isZoneSynchronizationEnabled() setZoneSynchronizationEnabled() zoneSynchronizationAttribute()
 */
/*!
 * \fn void QIviClimateControl::zoneSynchronizationAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the zoneSynchronization property changes. The new attribute is passed as \a attribute.
 *
 * \sa zoneSynchronizationAttribute() isZoneSynchronizationEnabled()
 */
/*!
 * \fn void QIviClimateControl::defrostEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the defrost is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isDefrostEnabled() setDefrostEnabled() defrostAttribute()
 */
/*!
 * \fn void QIviClimateControl::defrostAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the defrost property changes. The new attribute is passed as \a attribute.
 *
 * \sa defrostAttribute() isDefrostEnabled()
 */
/*!
 * \fn void QIviClimateControl::recirculationEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the recirculation is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isRecirculationEnabled() recirculationAttribute()
 */
/*!
 * \fn void QIviClimateControl::recirculationAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the heater property changes. The new attribute is passed as \a attribute.
 *
 * \sa recirculationAttribute() isRecirculationEnabled()
 */
/*!
 * \fn void QIviClimateControl::recirculationModeChanged(QIviClimateControl::RecirculationMode value)
 *
 * This signal is emitted whenever the recirculation mode changes. The new recirculation mode is passed as \a value.
 *
 * \sa recirculationMode() setRecirculationMode() recirculationModeAttribute()
 */
/*!
 * \fn void QIviClimateControl::recirculationModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::RecirculationMode> &attribute);
 *
 * This signal is emitted whenever the attribute for the recirculationMode property changes. The new attribute is passed as \a attribute.
 *
 * \sa recirculationModeAttribute() recirculationMode()
 */
/*!
 * \fn void QIviClimateControl::recirculationSensitivityLevelChanged(int value);
 *
 * This signal is emitted whenever the recirculation sensitivity level level changes. The new level is passed as \a value.
 *
 * \sa recirculationSensitivityLevel() setRecirculationSensitivityLevel() recirculationSensitivityLevelAttribute()
 */
/*!
 * \fn void QIviClimateControl::recirculationSensitivityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the recirculationSensitivityLevel property changes. The new attribute is passed as \a attribute.
 *
 * \sa recirculationSensitivityLevelAttribute() recirculationSensitivityLevel()
 */
/*!
 * \fn void QIviClimateControl::climateModeChanged(QIviClimateControl::ClimateMode value)
 *
 * This signal is emitted whenever the climate mode changes. The new climate mode is passed as \a value.
 *
 * \sa climateMode() setClimateMode() climateModeAttribute()
 */
/*!
 * \fn void QIviClimateControl::climateModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::ClimateMode> &attribute);
 *
 * This signal is emitted whenever the attribute for the climateMode property changes. The new attribute is passed as \a attribute.
 *
 * \sa climateModeAttribute() climateMode()
 */
/*!
 * \fn void QIviClimateControl::automaticClimateFanIntensityLevelChanged(int value);
 *
 * This signal is emitted whenever the fan intensity level changes. The new level is passed as \a value.
 *
 * \sa automaticClimateFanIntensityLevel() setAutomaticClimateFanIntensityLevel() automaticClimateFanIntensityLevelAttribute()
 */
/*!
 * \fn void QIviClimateControl::automaticClimateFanIntensityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the automaticClimateFanIntensityLevel property changes. The new attribute is passed as \a attribute.
 *
 * \sa automaticClimateFanIntensityLevelAttribute() automaticClimateFanIntensityLevel()
 */

 #include "moc_qiviclimatecontrol.cpp"
