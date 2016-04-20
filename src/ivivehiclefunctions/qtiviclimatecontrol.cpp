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

#include "qtiviclimatecontrol.h"
#include "qtiviclimatecontrol_p.h"
#include "qtiviclimatecontrolbackendinterface.h"
#include <QtIVICore/QtIVIServiceObject>
#include <QtIVICore/QtIVIPropertyFactory>

QtIVIClimateControlPrivate::QtIVIClimateControlPrivate(const QString &interface, const QString &zone, QtIVIClimateControl *parent)
    : QtIVIAbstractZonedFeaturePrivate(interface, zone, parent)
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
    , m_recirculationMode(QtIVIClimateControl::RecirculationOff)
    , m_recirculation(false)
    , m_recirculationSensitivityLevel(-1)
    , m_climateMode(QtIVIClimateControl::ClimateOff)
    , m_automaticClimateFanIntensityLevel(0)
    , q_ptr(parent)
{
}

void QtIVIClimateControlPrivate::init()
{
    Q_Q(QtIVIClimateControl);
    m_airFlowDirectionProperty = QtIVIPropertyFactory<QtIVIClimateControl::AirflowDirections>::create(q,
                                                                                                     &QtIVIClimateControl::airflowDirectionsAttribute,
                                                                                                     &QtIVIClimateControl::airflowDirectionsAttributeChanged,
                                                                                                     &QtIVIClimateControl::airflowDirections,
                                                                                                     &QtIVIClimateControl::airflowDirectionsChanged,
                                                                                                     &QtIVIClimateControl::setAirflowDirections);
    m_airConditioningProperty = QtIVIPropertyFactory<bool>::create(q,
                                                                   &QtIVIClimateControl::airConditioningAttribute,
                                                                   &QtIVIClimateControl::airConditioningAttributeChanged,
                                                                   &QtIVIClimateControl::isAirConditioningEnabled,
                                                                   &QtIVIClimateControl::airConditioningEnabledChanged,
                                                                   &QtIVIClimateControl::setAirConditioningEnabled);
    m_heaterProperty = QtIVIPropertyFactory<bool>::create(q,
                                                          &QtIVIClimateControl::heaterAttribute,
                                                          &QtIVIClimateControl::heaterAttributeChanged,
                                                          &QtIVIClimateControl::isHeaterEnabled,
                                                          &QtIVIClimateControl::heaterEnabledChanged,
                                                          &QtIVIClimateControl::setHeaterEnabled);
    m_targetTemperatureProperty = QtIVIPropertyFactory<int>::create(q,
                                                                    &QtIVIClimateControl::targetTemperatureAttribute,
                                                                    &QtIVIClimateControl::targetTemperatureAttributeChanged,
                                                                    &QtIVIClimateControl::targetTemperature,
                                                                    &QtIVIClimateControl::targetTemperatureChanged,
                                                                    &QtIVIClimateControl::setTargetTemperature);
    m_seatCoolerProperty = QtIVIPropertyFactory<int>::create(q,
                                                             &QtIVIClimateControl::seatCoolerAttribute,
                                                             &QtIVIClimateControl::seatCoolerAttributeChanged,
                                                             &QtIVIClimateControl::seatCooler,
                                                             &QtIVIClimateControl::seatCoolerChanged,
                                                             &QtIVIClimateControl::setSeatCooler);
    m_seatHeaterProperty = QtIVIPropertyFactory<int>::create(q,
                                                             &QtIVIClimateControl::seatHeaterAttribute,
                                                             &QtIVIClimateControl::seatHeaterAttributeChanged,
                                                             &QtIVIClimateControl::seatHeater,
                                                             &QtIVIClimateControl::seatHeaterChanged,
                                                             &QtIVIClimateControl::setSeatHeater);
    m_steeringWheelHeaterProperty = QtIVIPropertyFactory<int>::create(q,
                                                                      &QtIVIClimateControl::steeringWheelHeaterAttribute,
                                                                      &QtIVIClimateControl::steeringWheelHeaterAttributeChanged,
                                                                      &QtIVIClimateControl::steeringWheelHeater,
                                                                      &QtIVIClimateControl::steeringWheelHeaterChanged,
                                                                      &QtIVIClimateControl::setSteeringWheelHeater);
    m_fanSpeedLevelProperty = QtIVIPropertyFactory<int>::create(q,
                                                                &QtIVIClimateControl::fanSpeedLevelAttribute,
                                                                &QtIVIClimateControl::fanSpeedLevelAttributeChanged,
                                                                &QtIVIClimateControl::fanSpeedLevel,
                                                                &QtIVIClimateControl::fanSpeedLevelChanged,
                                                                &QtIVIClimateControl::setFanSpeedLevel);
    m_outsideTemperatureProperty = QtIVIPropertyFactory<int>::create(q,
                                                                     &QtIVIClimateControl::outsideTemperatureAttribute,
                                                                     &QtIVIClimateControl::outsideTemperatureAttributeChanged,
                                                                     &QtIVIClimateControl::outsideTemperature,
                                                                     &QtIVIClimateControl::outsideTemperatureChanged);

    m_zoneSynchronizationProperty = QtIVIPropertyFactory<bool>::create(q,
                                                                       &QtIVIClimateControl::zoneSynchronizationAttribute,
                                                                       &QtIVIClimateControl::zoneSynchronizationAttributeChanged,
                                                                       &QtIVIClimateControl::isZoneSynchronizationEnabled,
                                                                       &QtIVIClimateControl::zoneSynchronizationEnabledChanged,
                                                                       &QtIVIClimateControl::setZoneSynchronizationEnabled);

    m_defrostProperty = QtIVIPropertyFactory<bool>::create(q,
                                                           &QtIVIClimateControl::defrostAttribute,
                                                           &QtIVIClimateControl::defrostAttributeChanged,
                                                           &QtIVIClimateControl::isDefrostEnabled,
                                                           &QtIVIClimateControl::defrostEnabledChanged,
                                                           &QtIVIClimateControl::setDefrostEnabled);

    m_recirculationModeProperty = QtIVIPropertyFactory<QtIVIClimateControl::RecirculationMode>::create(q,
                                                                                                       &QtIVIClimateControl::recirculationModeAttribute,
                                                                                                       &QtIVIClimateControl::recirculationModeAttributeChanged,
                                                                                                       &QtIVIClimateControl::recirculationMode,
                                                                                                       &QtIVIClimateControl::recirculationModeChanged,
                                                                                                       &QtIVIClimateControl::setRecirculationMode);

    m_recirculationProperty = QtIVIPropertyFactory<bool>::create(q,
                                                                 &QtIVIClimateControl::recirculationAttribute,
                                                                 &QtIVIClimateControl::recirculationAttributeChanged,
                                                                 &QtIVIClimateControl::isRecirculationEnabled,
                                                                 &QtIVIClimateControl::recirculationEnabledChanged);

    m_recirculationSensitivityLevelProperty = QtIVIPropertyFactory<int>::create(q,
                                                                                &QtIVIClimateControl::recirculationSensitivityLevelAttribute,
                                                                                &QtIVIClimateControl::recirculationSensitivityLevelAttributeChanged,
                                                                                &QtIVIClimateControl::recirculationSensitivityLevel,
                                                                                &QtIVIClimateControl::recirculationSensitivityLevelChanged,
                                                                                &QtIVIClimateControl::setRecirculationSensitivityLevel);

    m_climateModeProperty = QtIVIPropertyFactory<QtIVIClimateControl::ClimateMode>::create(q,
                                                                                           &QtIVIClimateControl::climateModeAttribute,
                                                                                           &QtIVIClimateControl::climateModeAttributeChanged,
                                                                                           &QtIVIClimateControl::climateMode,
                                                                                           &QtIVIClimateControl::climateModeChanged,
                                                                                           &QtIVIClimateControl::setClimateMode);

    m_automaticClimateFanIntensityLevelProperty = QtIVIPropertyFactory<int>::create(q,
                                                                                    &QtIVIClimateControl::automaticClimateFanIntensityLevelAttribute,
                                                                                    &QtIVIClimateControl::automaticClimateFanIntensityLevelAttributeChanged,
                                                                                    &QtIVIClimateControl::automaticClimateFanIntensityLevel,
                                                                                    &QtIVIClimateControl::automaticClimateFanIntensityLevelChanged,
                                                                                    &QtIVIClimateControl::setAutomaticClimateFanIntensityLevel);
}

void QtIVIClimateControlPrivate::clearToDefaults()
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
    m_recirculationMode = QtIVIClimateControl::RecirculationOff;
    m_recirculation = false;
    m_recirculationSensitivityLevel = -1;
    m_climateMode = QtIVIClimateControl::ClimateOff;
    m_automaticClimateFanIntensityLevel = 0;
}

void QtIVIClimateControlPrivate::onAirflowDirectionsChanged(QtIVIClimateControl::AirflowDirections value, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airflowDirections = value;
    emit f->airflowDirectionsChanged(value);
}

void QtIVIClimateControlPrivate::onAirflowDirectionsAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> &airflowDirectionsAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airflowDirectionsAttribute = airflowDirectionsAttribute;
    emit f->airflowDirectionsAttributeChanged(airflowDirectionsAttribute);
}

void QtIVIClimateControlPrivate::onAirConditioningEnabledChanged(bool enabled, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airConditioning = enabled;
    emit f->airConditioningEnabledChanged(enabled);
}

void QtIVIClimateControlPrivate::onAirConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> &airConditioningEnabledAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airConditioningAttribute = airConditioningEnabledAttribute;
    emit f->airConditioningAttributeChanged(airConditioningEnabledAttribute);
}

void QtIVIClimateControlPrivate::onHeaterEnabledChanged(bool enabled, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_heater = enabled;
    emit f->heaterEnabledChanged(enabled);
}

void QtIVIClimateControlPrivate::onHeaterAttributeChanged(const QtIVIPropertyAttribute<bool> &heaterEnabledAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_heaterAttribute = heaterEnabledAttribute;
    emit f->heaterAttributeChanged(heaterEnabledAttribute);
}

void QtIVIClimateControlPrivate::onSteeringWheelHeaterChanged(int value, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_steeringWheelHeater = value;
    emit f->steeringWheelHeaterChanged(value);
}

void QtIVIClimateControlPrivate::onSteeringWheelHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &steeringWheelHeaterAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_steeringWheelHeaterAttribute = steeringWheelHeaterAttribute;
    emit f->steeringWheelHeaterAttributeChanged(steeringWheelHeaterAttribute);
}

void QtIVIClimateControlPrivate::onFanSpeedLevelChanged(int value, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_fanSpeedLevel = value;
    emit f->fanSpeedLevelChanged(value);
}

void QtIVIClimateControlPrivate::onFanSpeedLevelAttributeChanged(const QtIVIPropertyAttribute<int> &fanSpeedAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_fanSpeedLevelAttribute = fanSpeedAttribute;
    emit f->fanSpeedLevelAttributeChanged(fanSpeedAttribute);
}

void QtIVIClimateControlPrivate::onTargetTemperatureChanged(int temperature, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_targetTemperature = temperature;
    emit f->targetTemperatureChanged(temperature);
}

void QtIVIClimateControlPrivate::onTargetTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &temperatureAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_targetTemperatureAttribute = temperatureAttribute;
    emit f->targetTemperatureAttributeChanged(temperatureAttribute);
}

void QtIVIClimateControlPrivate::onSeatCoolerChanged(int value, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatCooler = value;
    emit f->seatCoolerChanged(value);
}

void QtIVIClimateControlPrivate::onSeatCoolerAttributeChanged(const QtIVIPropertyAttribute<int> &seatCoolerAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatCoolerAttribute = seatCoolerAttribute;
    emit f->seatCoolerAttributeChanged(seatCoolerAttribute);
}

void QtIVIClimateControlPrivate::onSeatHeaterChanged(int value, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatHeater = value;
    emit f->seatHeaterChanged(value);
}

void QtIVIClimateControlPrivate::onSeatHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &seatHeaterAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatHeaterAttribute = seatHeaterAttribute;
    emit f->seatHeaterAttributeChanged(seatHeaterAttribute);
}

void QtIVIClimateControlPrivate::onOutsideTemperatureChanged(int outsideTemperature, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_outsideTemperature = outsideTemperature;
    emit f->outsideTemperatureChanged(outsideTemperature);
}

void QtIVIClimateControlPrivate::onOutsideTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_outsideTemperatureAttribute = attribute;
    emit f->outsideTemperatureAttributeChanged(attribute);
}

void QtIVIClimateControlPrivate::onZoneSynchronizationChanged(bool zoneSynchronization, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_zoneSynchronization = zoneSynchronization;
    emit f->zoneSynchronizationEnabledChanged(zoneSynchronization);
}

void QtIVIClimateControlPrivate::onZoneSynchronizationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_zoneSynchronizationAttribute = attribute;
    emit f->zoneSynchronizationAttributeChanged(attribute);
}

void QtIVIClimateControlPrivate::onDefrostChanged(bool defrost, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_defrost = defrost;
    emit f->defrostEnabledChanged(defrost);
}

void QtIVIClimateControlPrivate::onDefrostAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_defrostAttribute = attribute;
    emit f->defrostAttributeChanged(attribute);
}

void QtIVIClimateControlPrivate::onRecirculationModeChanged(QtIVIClimateControl::RecirculationMode recirculationMode, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculationMode = recirculationMode;
    emit f->recirculationModeChanged(recirculationMode);
}

void QtIVIClimateControlPrivate::onRecirculationModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode> &attribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculationModeAttribute = attribute;
    emit f->recirculationModeAttributeChanged(attribute);
}

void QtIVIClimateControlPrivate::onRecirculationChanged(bool recirculation, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculation = recirculation;
    emit f->recirculationEnabledChanged(recirculation);
}

void QtIVIClimateControlPrivate::onRecirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculationAttribute = attribute;
    emit f->recirculationAttributeChanged(attribute);
}

void QtIVIClimateControlPrivate::onRecirculationSensitivityLevelChanged(int recirculationSensitivityLevel, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculationSensitivityLevel = recirculationSensitivityLevel;
    emit f->recirculationSensitivityLevelChanged(recirculationSensitivityLevel);
}

void QtIVIClimateControlPrivate::onRecirculationSensitivityLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_recirculationSensitivityLevelAttribute = attribute;
    emit f->recirculationSensitivityLevelAttributeChanged(attribute);
}

void QtIVIClimateControlPrivate::onClimateModeChanged(QtIVIClimateControl::ClimateMode climateMode, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_climateMode = climateMode;
    emit f->climateModeChanged(climateMode);
}

void QtIVIClimateControlPrivate::onClimateModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode> &attribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_climateModeAttribute = attribute;
    emit f->climateModeAttributeChanged(attribute);
}

void QtIVIClimateControlPrivate::onAutomaticClimateFanIntensityLevelChanged(int automaticClimateFanIntensityLevel, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_automaticClimateFanIntensityLevel = automaticClimateFanIntensityLevel;
    emit f->automaticClimateFanIntensityLevelChanged(automaticClimateFanIntensityLevel);
}

void QtIVIClimateControlPrivate::onAutomaticClimateFanIntensityLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_automaticClimateFanIntensityLevelAttribute = attribute;
    emit f->automaticClimateFanIntensityLevelAttributeChanged(attribute);
}

QtIVIClimateControlBackendInterface *QtIVIClimateControlPrivate::climateControlBackend()
{
    Q_Q(QtIVIClimateControl);
    return qobject_cast<QtIVIClimateControlBackendInterface*>(q->backend());
}

/*!
   \class QtIVIClimateControl
   \inmodule QtIVIVehicleFunctions
   \brief Provides an interface to the climate control.

   The QtIVIClimateControl provides an interface to the climate control of the vehicle.

   The climate control properties are divided into two categories: central or zoned. The central
   properties are exposed directly through the QtIVIClimateControl and the zoned properties are
   exposed through zone objects. The zones are retrieved using the \l zoneAt method.

   The QtIVIClimateControl expects a single backend to be available. It is recommended to use it
   with \l {QtIVIAbstractFeature::}{discoveryMode} set to \l AutoDiscovery.
*/

/*!
    \qmltype ClimateControl
    \instantiates QtIVIClimateControl
    \inqmlmodule QtIVIVehicleFunctions
    \inherits AbstractZonedFeature
    \brief The ClimateControl provides a QML interface to the climate control of the vehicle.
*/

/*!
   \enum QtIVIClimateControl::AirflowDirection
   \value Windshield
          Direct airflow along the windshield.
   \value Dashboard
          Direct airflow through the dashboard.
   \value Floor
          Direct airflow to the floor.
*/

/*!
   \enum QtIVIClimateControl::RecirculationMode
   \value RecirculationOff
          The recirculation is turned off.
   \value RecirculationOn
          The recirculation is turned on.
   \value AutoRecirculation
          The recirculation is turning off and on automatically depending on the air quality.
*/

/*!
   \enum QtIVIClimateControl::ClimateMode
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

   The \a parent argument is passed on to the \l QtIVIAbstractZonedFeature base class.
*/
QtIVIClimateControl::QtIVIClimateControl(const QString &zone, QObject* parent)
    : QtIVIAbstractZonedFeature(*new QtIVIClimateControlPrivate(QtIVIStringClimateControlInterfaceName, zone, this), parent)
{
    Q_D(QtIVIClimateControl);
    d->init();
    qRegisterMetaType<QtIVIPropertyAttribute<int>>();
    qRegisterMetaType<QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections>>();
    qRegisterMetaType<QtIVIPropertyAttribute<bool>>();
    qRegisterMetaType<QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode>>();
    qRegisterMetaType<QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode>>();
}

QtIVIClimateControl::~QtIVIClimateControl()
{
}

/*!
   \reimp
*/
void QtIVIClimateControl::connectToServiceObject(QtIVIServiceObject *serviceObject)
{
    Q_D(QtIVIClimateControl);
    QtIVIAbstractZonedFeature::connectToServiceObject(serviceObject);

    QtIVIClimateControlBackendInterface* backend = d->climateControlBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::targetTemperatureChanged,
            d, &QtIVIClimateControlPrivate::onTargetTemperatureChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::targetTemperatureAttributeChanged,
            d, &QtIVIClimateControlPrivate::onTargetTemperatureAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::seatCoolerChanged,
            d, &QtIVIClimateControlPrivate::onSeatCoolerChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::seatCoolerAttributeChanged,
            d, &QtIVIClimateControlPrivate::onSeatCoolerAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::seatHeaterChanged,
            d, &QtIVIClimateControlPrivate::onSeatHeaterChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::seatHeaterAttributeChanged,
            d, &QtIVIClimateControlPrivate::onSeatHeaterAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::steeringWheelHeaterChanged,
            d, &QtIVIClimateControlPrivate::onSteeringWheelHeaterChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::steeringWheelHeaterAttributeChanged,
            d, &QtIVIClimateControlPrivate::onSteeringWheelHeaterAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::fanSpeedLevelChanged,
            d, &QtIVIClimateControlPrivate::onFanSpeedLevelChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::fanSpeedLevelAttributeChanged,
            d, &QtIVIClimateControlPrivate::onFanSpeedLevelAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::airflowDirectionsChanged,
            d, &QtIVIClimateControlPrivate::onAirflowDirectionsChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::airflowDirectionsAttributeChanged,
            d, &QtIVIClimateControlPrivate::onAirflowDirectionsAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::airConditioningEnabledChanged,
            d, &QtIVIClimateControlPrivate::onAirConditioningEnabledChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::airConditioningAttributeChanged,
            d, &QtIVIClimateControlPrivate::onAirConditioningAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::heaterEnabledChanged,
            d, &QtIVIClimateControlPrivate::onHeaterEnabledChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::heaterAttributeChanged,
            d, &QtIVIClimateControlPrivate::onHeaterAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::outsideTemperatureChanged,
            d, &QtIVIClimateControlPrivate::onOutsideTemperatureChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::outsideTemperatureAttributeChanged,
            d, &QtIVIClimateControlPrivate::onOutsideTemperatureAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::zoneSynchronizationEnabledChanged,
            d, &QtIVIClimateControlPrivate::onZoneSynchronizationChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::zoneSynchronizationAttributeChanged,
            d, &QtIVIClimateControlPrivate::onZoneSynchronizationAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::defrostEnabledChanged,
            d, &QtIVIClimateControlPrivate::onDefrostChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::defrostAttributeChanged,
            d, &QtIVIClimateControlPrivate::onDefrostAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::recirculationModeChanged,
            d, &QtIVIClimateControlPrivate::onRecirculationModeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::recirculationModeAttributeChanged,
            d, &QtIVIClimateControlPrivate::onRecirculationModeAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::recirculationEnabledChanged,
            d, &QtIVIClimateControlPrivate::onRecirculationChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::recirculationAttributeChanged,
            d, &QtIVIClimateControlPrivate::onRecirculationAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::recirculationSensitivityLevelChanged,
            d, &QtIVIClimateControlPrivate::onRecirculationSensitivityLevelChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::recirculationSensitivityLevelAttributeChanged,
            d, &QtIVIClimateControlPrivate::onRecirculationSensitivityLevelAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::climateModeChanged,
            d, &QtIVIClimateControlPrivate::onClimateModeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::climateModeAttributeChanged,
            d, &QtIVIClimateControlPrivate::onClimateModeAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::automaticClimateFanIntensityLevelChanged,
            d, &QtIVIClimateControlPrivate::onAutomaticClimateFanIntensityLevelChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::automaticClimateFanIntensityLevelAttributeChanged,
            d, &QtIVIClimateControlPrivate::onAutomaticClimateFanIntensityLevelAttributeChanged);

    backend->initializeAttributes();
}

void QtIVIClimateControl::disconnectFromServiceObject(QtIVIServiceObject *serviceObject)
{
    QtIVIClimateControlBackendInterface *backend = qobject_cast<QtIVIClimateControlBackendInterface*>(serviceObject->interfaceInstance(QtIVIStringClimateControlInterfaceName));

    if (backend)
        disconnect(backend, 0, this, 0);
}

/*!
   \reimp
*/
void QtIVIClimateControl::clearServiceObject()
{
    Q_D(QtIVIClimateControl);
    d->clearToDefaults();
}

QtIVIClimateControl::QtIVIClimateControl(QtIVIClimateControlPrivate &dd, QObject *parent)
    : QtIVIAbstractZonedFeature(dd, parent)
{
    Q_D(QtIVIClimateControl);
    d->init();
}

/*!
   \reimp
*/
QtIVIAbstractZonedFeature *QtIVIClimateControl::createZoneFeature(const QString &zone)
{
    return new QtIVIClimateControl(zone, this);
}
/*!
   \qmlqtivipropertyBool {QtIVIVehicleFunctions::ClimateControl::airConditioning}

   \e value is true if the air conditioning is enabled.
*/
/*!
   \property QtIVIClimateControl::airConditioning

   Holds a QtIVIProperty of type \e bool where the QtIVIProperty::value() function indicates if air conditioning is enabled.

   \sa AttributeSystem
   \sa isAirConditioningEnabled() setAirConditioningEnabled() airConditioningAttribute()
 */
/*!
 * Returns true if air conditioning is enabled.
 *
 * \sa setAirConditioningEnabled() airConditioningEnabledChanged() airConditioningAttribute()
 */
bool QtIVIClimateControl::isAirConditioningEnabled() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airConditioning;
}

/*!
 * Returns the attribute defining the boundaries and availability of the air conditioning property.
 *
 * \sa setAirConditioningEnabled() isAirConditioningEnabled() airConditioningEnabledChanged()
 */
QtIVIPropertyAttribute<bool> QtIVIClimateControl::airConditioningAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airConditioningAttribute;
}

QtIVIProperty *QtIVIClimateControl::airConditioningProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airConditioningProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIVIVehicleFunctions::ClimateControl::airflowDirections}

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
   \property QtIVIClimateControl::airflowDirections

   Holds a QtIVIProperty of type \e QtIVIClimateControl::AirflowDirections where the QtIVIProperty::value() function returns the current air flow directions.

   \sa AttributeSystem
   \sa airflowDirections() setAirflowDirections() airflowDirectionsAttribute()
 */
/*!
 * Returns the current air flow direction.
 *
 * \sa setAirflowDirections() airflowDirectionsChanged() airflowDirectionsAttribute()
 */
QtIVIClimateControl::AirflowDirections QtIVIClimateControl::airflowDirections() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airflowDirections;
}

/*!
 * Returns the attribute defining the boundaries and availability of the air flow property
 *
 * \sa setAirflowDirections() airflowDirections() airflowDirectionsChanged()
 */
QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> QtIVIClimateControl::airflowDirectionsAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airflowDirectionsAttribute;
}

QtIVIProperty *QtIVIClimateControl::airflowDirectionsProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airFlowDirectionProperty;
}

/*!
   \qmlqtivipropertyBool {QtIVIVehicleFunctions::ClimateControl::heater}

   \e value is true if the heater is enabled.
*/
/*!
   \property QtIVIClimateControl::heater

   Holds a QtIVIProperty of type \e bool where the QtIVIProperty::value() function indicates if the heater is enabled.

   \sa AttributeSystem
   \sa isHeaterEnabled() setHeaterEnabled() heaterAttribute()
 */

/*!
 * Returns true if the heater is enabled.
 *
 * \sa setHeaterEnabled() heaterEnabledChanged() heaterAttribute()
 */
bool QtIVIClimateControl::isHeaterEnabled() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_heater;
}

/*!
 * Returns the attribute defining the boundaries and availability of the heater property.
 *
 * \sa setHeaterEnabled() isHeaterEnabled() heaterEnabledChanged()
 */
QtIVIPropertyAttribute<bool> QtIVIClimateControl::heaterAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_heaterAttribute;
}

QtIVIProperty *QtIVIClimateControl::heaterProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_heaterProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::steeringWheelHeater}

    \e value holds the steering wheel heater level, where the level can be between \c minimumValue(off)
    to \c maximumValue (warmest).
 */
/*!
   \property QtIVIClimateControl::steeringWheelHeater

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current steering wheel heater level.

   \sa AttributeSystem
   \sa steeringWheelHeater() setSteeringWheelHeater() steeringWheelHeaterAttribute()
 */
/*!
 * Returns the current steering wheel heater level, where the level can be between \c minimumValue(off) and \c maximumValue (warmest).
 *
 * \sa setSteeringWheelHeater() steeringWheelHeaterChanged() steeringWheelHeaterAttribute()
 */
int QtIVIClimateControl::steeringWheelHeater() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_steeringWheelHeater;
}

/*!
 * Returns the attribute defining the boundaries and availability of the steering wheel heater property.
 *
 * \sa setSteeringWheelHeater() steeringWheelHeater() steeringWheelHeaterChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::steeringWheelHeaterAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_steeringWheelHeaterAttribute;
}

QtIVIProperty *QtIVIClimateControl::steeringWheelHeaterProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_steeringWheelHeaterProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::fanSpeedLevel}

    \e value holds the fan speed level, where the level can be between \c minimumValue(off)
    to \c maximumValue (strongest).
 */
/*!
   \property QtIVIClimateControl::fanSpeedLevel

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current fan speed level.

   \sa AttributeSystem
   \sa fanSpeedLevel() setFanSpeedLevel() fanSpeedLevelAttribute()
 */
/*!
 * Returns the current fan speed level, where the level can be between \c miniumValue(off) and \c maximumValue(strongest).
 *
 * \sa setFanSpeedLevel() fanSpeedLevelChanged() fanSpeedLevelAttribute()
 */
int QtIVIClimateControl::fanSpeedLevel() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_fanSpeedLevel;
}

/*!
 * Returns the attribute defining the boundaries and availability of the fan speed level property
 *
 * \sa setFanSpeedLevel() fanSpeedLevel() fanSpeedLevelChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::fanSpeedLevelAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_fanSpeedLevelAttribute;
}

QtIVIProperty *QtIVIClimateControl::fanSpeedLevelProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_fanSpeedLevelProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::targetTemperature}

   \e value holds the target temperature of the zone expressed in centigrades, where the temperature can be between \c minimumValue(coolest)
   to \c maximumValue (warmest).
 */
/*!
   \property QtIVIClimateControl::targetTemperature

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current target temperature.

   \sa AttributeSystem
   \sa targetTemperature() setTargetTemperature() targetTemperatureAttribute()
 */
/*!
 * Returns the current target temperature expressed in centigrates.
 *
 * \sa setTargetTemperature() targetTemperatureChanged() targetTemperatureAttribute()
 */
int QtIVIClimateControl::targetTemperature() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_targetTemperature;
}

/*!
 * Returns the attribute defining the boundaries and availability of the target temperature property.
 *
 * \sa setTargetTemperature() targetTemperature() targetTemperatureChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::targetTemperatureAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_targetTemperatureAttribute;
}

QtIVIProperty *QtIVIClimateControl::targetTemperatureProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_targetTemperatureProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::seatCooler}

    \e value holds the seat cooler level, where the level can be between \c minimumValue(off)
    to \c maximumValue (coolest).
 */
/*!
   \property QtIVIClimateControl::seatCooler

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current seat cooler level.

   \sa AttributeSystem
   \sa seatCooler() setSeatCooler() seatCoolerAttribute()
 */
/*!
 * Returns the current seat cooler level, where the level can be between \c minimumValue(off) and \c maximumValue(coolest).
 *
 * \sa seatCooler() seatCoolerChanged() seatCoolerAttribute()
 */
int QtIVIClimateControl::seatCooler() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatCooler;
}

/*!
 * Returns the attribute defining the boundaries and availability of the seat cooler property.
 *
 * \sa setSeatCooler() seatCooler() seatCoolerChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::seatCoolerAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatCoolerAttribute;
}

QtIVIProperty *QtIVIClimateControl::seatCoolerProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatCoolerProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::seatHeater}

    \e value holds the seat heater level, where the level can be between \c minimumValue(off)
    to \c maximumValue (warmest).
 */
/*!
   \property QtIVIClimateControl::seatHeater

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current seat heater level.

   \sa AttributeSystem
   \sa seatHeater() setSeatHeater() seatHeaterAttribute()
 */
/*!
 * Returns the current seat heater level, where the level can be between \c minimumValue(off) and \c maximumValue(warmest).
 *
 * \sa seatHeater() seatHeaterChanged() seatHeaterAttribute()
 */
int QtIVIClimateControl::seatHeater() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatHeater;
}

/*!
 * Returns the attribute defining the boundaries and availability of the seat heater property.
 *
 * \sa setSeatHeater() seatHeater() seatHeaterChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::seatHeaterAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatHeaterAttribute;
}

QtIVIProperty *QtIVIClimateControl::seatHeaterProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatHeaterProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::outsideTemperature}

   \e value holds the outside temperature of the zone expressed in centigrades, where the temperature can be between \c minimumValue(coolest)
   to \c maximumValue (warmest).
 */
/*!
   \property QtIVIClimateControl::outsideTemperature

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current outside temperature.

   \sa AttributeSystem
   \sa outsideTemperature() outsideTemperatureAttribute()
 */
/*!
 * Returns the current outside temperature expressed in centigrates.
 *
 * \sa outsideTemperatureChanged() outsideTemperatureAttribute()
 */
int QtIVIClimateControl::outsideTemperature() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_outsideTemperature;
}

/*!
 * Returns the attribute defining the boundaries and availability of the outside temperature property.
 *
 * \sa outsideTemperature() outsideTemperatureChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::outsideTemperatureAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_outsideTemperatureAttribute;
}

QtIVIProperty *QtIVIClimateControl::outsideTemperatureProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_outsideTemperatureProperty;
}

/*!
   \qmlqtivipropertyBool {QtIVIVehicleFunctions::ClimateControl::zoneSynchronization}

   \e value is true if the zone synchronization is enabled.

   Which zones and properties are synchronized is controlled by the backend implementing it.
*/
/*!
   \property QtIVIClimateControl::zoneSynchronization

   Holds a QtIVIProperty of type \e bool where the QtIVIProperty::value() function indicates if zone synchronization is enabled.

   Which zones and properties are synchronized is controlled by the backend implementing it.

   \sa AttributeSystem
   \sa isZoneSynchronizationEnabled() setZoneSynchronizationEnabled() zoneSynchronizationAttribute()
 */
/*!
 * Returns true if zone synchronization is enabled.
 *
 * \sa setZoneSynchronizationEnabled() zoneSynchronizationEnabledChanged() zoneSynchronizationAttribute()
 */
bool QtIVIClimateControl::isZoneSynchronizationEnabled() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_zoneSynchronization;
}

/*!
 * Returns the attribute defining the boundaries and availability of the zone synchronization property.
 *
 * \sa setZoneSynchronizationEnabled() isZoneSynchronizationEnabled() zoneSynchronizationEnabledChanged()
 */
QtIVIPropertyAttribute<bool> QtIVIClimateControl::zoneSynchronizationAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_zoneSynchronizationAttribute;
}

QtIVIProperty *QtIVIClimateControl::zoneSynchronizationProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_zoneSynchronizationProperty;
}

/*!
   \qmlqtivipropertyBool {QtIVIVehicleFunctions::ClimateControl::defrost}

   \e value is true if defrost is enabled. Usually that means that the fans are on the highest level to remove ice from the windshield.
*/
/*!
   \property QtIVIClimateControl::defrost

   Holds a QtIVIProperty of type \e bool where the QtIVIProperty::value() function indicates if defrost is enabled.

   \sa AttributeSystem
   \sa isDefrostEnabled() setDefrostEnabled() defrostAttribute()
 */
/*!
 * Returns true if defrost is enabled.
 *
 * \sa setDefrostEnabled() defrostEnabledChanged() defrostAttribute()
 */
bool QtIVIClimateControl::isDefrostEnabled() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_defrost;
}

/*!
 * Returns the attribute defining the boundaries and availability of the defrost property.
 *
 * \sa setDefrostEnabled() isDefrostEnabled() defrostEnabledChanged()
 */
QtIVIPropertyAttribute<bool> QtIVIClimateControl::defrostAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_defrostAttribute;
}

QtIVIProperty *QtIVIClimateControl::defrostProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_defrostProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIVIVehicleFunctions::ClimateControl::recirculationMode}

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
   \property QtIVIClimateControl::recirculationMode

   Holds a QtIVIProperty of type \e QtIVIClimateControl::RecirculationMode where the QtIVIProperty::value() function returns the current recirculation mode.

   \sa AttributeSystem
   \sa recirculationMode() setRecirculationMode() recirculationModeAttribute()
 */
/*!
 * Returns the current recirculation mode.
 *
 * \sa setRecirculationMode() recirculationModeChanged() recirculationModeAttribute()
 */
QtIVIClimateControl::RecirculationMode QtIVIClimateControl::recirculationMode() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_recirculationMode;
}

/*!
 * Returns the attribute defining the boundaries and availability of the recirculationMode property.
 *
 * \sa setRecirculationMode() recirculationMode() recirculationModeChanged()
 */
QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode> QtIVIClimateControl::recirculationModeAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_recirculationModeAttribute;
}

QtIVIProperty *QtIVIClimateControl::recirculationModeProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_recirculationModeProperty;
}

/*!
   \qmlqtivipropertyBool {QtIVIVehicleFunctions::ClimateControl::recirculation}

   \e value is true if the recirculation is currently running.
*/
/*!
   \property QtIVIClimateControl::recirculation

   Holds a QtIVIProperty of type \e bool where the QtIVIProperty::value() function indicates if recirculation is running.

   \sa AttributeSystem
   \sa isRecirculationEnabled() recirculationAttribute()
 */
/*!
 * Returns true if defrost is enabled.
 *
 * \sa recirculationEnabledChanged() recirculationAttribute()
 */
bool QtIVIClimateControl::isRecirculationEnabled() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_recirculation;
}

/*!
 * Returns the attribute defining the boundaries and availability of the recirculation property.
 *
 * \sa isRecirculationEnabled() recirculationEnabledChanged()
 */
QtIVIPropertyAttribute<bool> QtIVIClimateControl::recirculationAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_recirculationAttribute;
}

QtIVIProperty *QtIVIClimateControl::recirculationProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_recirculationProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::recirculationSensitivityLevel}

    \e value holds the sensitivity level of the recirculation system when the recirculationMode is set to AutoRecirculation, where the level can be between \c minimumValue(least sensitive)
    to \c maximumValue(most sensitive).
 */
/*!
   \property QtIVIClimateControl::recirculationSensitivityLevel

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current sensitivity level of the recicurlcation system when the recirculationMode is set to AutoRecirculation.

   \sa AttributeSystem
   \sa recirculationSensitivityLevel() setRecirculationSensitivityLevel() recirculationSensitivityLevelAttribute()
 */
/*!
 * Returns the current sensitivity level of recicurlcation system when the recirculationMode is set to AutoRecirculation, where the level can be between \c minimumValue(least sensitive) and \c maximumValue(most sensitive).
 *
 * \sa setRecirculationSensitivityLevel() recirculationSensitivityLevelChanged() recirculationSensitivityLevelAttribute()
 */
int QtIVIClimateControl::recirculationSensitivityLevel() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_recirculationSensitivityLevel;
}

/*!
 * Returns the attribute defining the boundaries and availability of the recirculationSensitivityLevel property.
 *
 * \sa setRecirculationSensitivityLevel() recirculationSensitivityLevel() recirculationSensitivityLevelChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::recirculationSensitivityLevelAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_recirculationSensitivityLevelAttribute;
}

QtIVIProperty *QtIVIClimateControl::recirculationSensitivityLevelProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_recirculationSensitivityLevelProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIVIVehicleFunctions::ClimateControl::climateMode}

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
   \property QtIVIClimateControl::climateMode

   Holds a QtIVIProperty of type \e QtIVIClimateControl::ClimateMode where the QtIVIProperty::value() function returns the current climate mode.

   \sa AttributeSystem
   \sa climateMode() setClimateMode() climateModeAttribute()
 */
/*!
 * Returns the current climate mode.
 *
 * \sa setClimateMode() climateModeChanged() climateModeAttribute()
 */
QtIVIClimateControl::ClimateMode QtIVIClimateControl::climateMode() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_climateMode;
}

/*!
 * Returns the attribute defining the boundaries and availability of the climateMode property.
 *
 * \sa isRecirculationEnabled() recirculationEnabledChanged()
 */
QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode> QtIVIClimateControl::climateModeAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_climateModeAttribute;
}

QtIVIProperty *QtIVIClimateControl::climateModeProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_climateModeProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::automaticClimateFanIntensityLevel}

    \e value holds the intensity level of the fan when the climateMode is set to AutoClimate, where the level can be between \c minimumValue(least intensity)
    to \c maximumValue(most intensity).
 */
/*!
   \property QtIVIClimateControl::automaticClimateFanIntensityLevel

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current intensity level of the fan when the climateMode is set to AutoClimate.

   \sa AttributeSystem
   \sa automaticClimateFanIntensityLevel() setAutomaticClimateFanIntensityLevel() automaticClimateFanIntensityLevelAttribute()
 */
/*!
 * Returns the current intensity level of the fan when the climateMode is set to AutoClimate, where the level can be between \c minimumValue(least intensity) and \c maximumValue(most intensity).
 *
 * \sa setAutomaticClimateFanIntensityLevel() automaticClimateFanIntensityLevelChanged() automaticClimateFanIntensityLevelAttribute()
 */
int QtIVIClimateControl::automaticClimateFanIntensityLevel() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_automaticClimateFanIntensityLevel;
}

/*!
 * Returns the attribute defining the boundaries and availability of the climateMode property.
 *
 * \sa setAutomaticClimateFanIntensityLevel() automaticClimateFanIntensityLevel() automaticClimateFanIntensityLevelChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::automaticClimateFanIntensityLevelAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_automaticClimateFanIntensityLevelAttribute;
}

QtIVIProperty *QtIVIClimateControl::automaticClimateFanIntensityLevelProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_automaticClimateFanIntensityLevelProperty;
}

/*!
 * Sets the air conditioning system enabled, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isAirConditioningEnabled() airConditioningEnabledChanged() airConditioningAttribute()
 */
void QtIVIClimateControl::setAirConditioningEnabled(bool enabled)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setAirConditioningEnabled(enabled, zone());
}

/*!
 * Sets the air flow direction to \a direction.
 *
 * \sa airflowDirections() airflowDirectionsChanged() airflowDirectionsAttribute()
 */
void QtIVIClimateControl::setAirflowDirections(QtIVIClimateControl::AirflowDirections direction)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setAirflowDirections(direction, zone());
}

/*!
 * Enables the heater, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isHeaterEnabled() heaterEnabledChanged() heaterAttribute()
 */
void QtIVIClimateControl::setHeaterEnabled(bool enabled)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setHeaterEnabled(enabled, zone());
}

/*!
 * Sets the current steering wheel heater level to \a value, where the level can be between \c minimumValue(off) and \c maximumValue(warmest).
 *
 * \sa steeringWheelHeater() steeringWheelHeaterChanged() steeringWheelHeaterAttribute()
 */
void QtIVIClimateControl::setSteeringWheelHeater(int value)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setSteeringWheelHeater(value, zone());
}

/*!
 * Sets the current fan speed level to \a value, where the level can be between \c minimumValue(off) and \c maximumValue(strongest).
 *
 * \sa fanSpeedLevel() fanSpeedLevelChanged() fanSpeedLevelAttribute()
 */
void QtIVIClimateControl::setFanSpeedLevel(int value)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setFanSpeedLevel(value, zone());
}

/*!
 * Sets the current target temperature to \a temperature expressed in centigrades.
 *
 * \sa targetTemperature() targetTemperatureChanged() targetTemperatureAttribute()
 */
void QtIVIClimateControl::setTargetTemperature(int temperature)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setTargetTemperature(temperature, zone());
}

/*!
 * Sets the current seat cooler level to \a value, where the level can be between \c minimumValue(off) and \c maximumValue(coolest).
 *
 * \sa seatCooler() seatCoolerChanged() seatCoolerAttribute()
 */
void QtIVIClimateControl::setSeatCooler(int value)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setSeatCooler(value, zone());
}

/*!
 * Sets the current seat heater level to \a value, where the level can be between \c minimumValue(off) and \c maximumValue(warmest).
 *
 * \sa seatHeater() seatHeaterChanged() seatHeaterAttribute()
 */
void QtIVIClimateControl::setSeatHeater(int value)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setSeatHeater(value, zone());
}

/*!
 * Enables the zone synchronization, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isZoneSynchronizationEnabled() zoneSynchronizationEnabledChanged() zoneSynchronizationAttribute()
 */
void QtIVIClimateControl::setZoneSynchronizationEnabled(bool enabled)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setZoneSynchronizationEnabled(enabled, zone());
}

/*!
 * Enables defrosting of the windshield, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isDefrostEnabled() defrostEnabledChanged() defrostAttribute()
 */
void QtIVIClimateControl::setDefrostEnabled(bool enabled)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setDefrostEnabled(enabled, zone());
}

/*!
 * Sets the recirculation mode to \a recirculationMode.
 *
 * \sa recirculationMode() recirculationModeChanged() recirculationModeAttribute()
 */
void QtIVIClimateControl::setRecirculationMode(QtIVIClimateControl::RecirculationMode recirculationMode)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setRecirculationMode(recirculationMode, zone());
}

/*!
 * Sets the current sensitivity level for the AutomaticRecirculation mode to \a value, where the level can be between \c minimumValue(least sensitive) and \c maximumValue(most sensitive).
 *
 * \sa recirculationSensitivityLevel() recirculationSensitivityLevelChanged() recirculationSensitivityLevelAttribute()
 */
void QtIVIClimateControl::setRecirculationSensitivityLevel(int value)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setRecirculationSensitivityLevel(value, zone());
}

/*!
 * Sets the climate mode to \a climateMode.
 *
 * \sa climateMode() climateModeChanged() climateModeAttribute()
 */
void QtIVIClimateControl::setClimateMode(QtIVIClimateControl::ClimateMode climateMode)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setClimateMode(climateMode, zone());
}

/*!
 * Sets the current fan intensity level for the AutomaticClimate mode to \a value, where the level can be between \c minimumValue(least intensity) and \c maximumValue(most intensity).
 *
 * \sa automaticClimateFanIntensityLevel() automaticClimateFanIntensityLevelChanged() automaticClimateFanIntensityLevelAttribute()
 */
void QtIVIClimateControl::setAutomaticClimateFanIntensityLevel(int value)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setAutomaticClimateFanIntensityLevel(value, zone());
}

/*!
 * \fn void QtIVIClimateControl::airflowDirectionsChanged(QtIVIClimateControl::AirflowDirections value)
 *
 * This signal is emitted whenever the air flow directions change. The new flow directions are passed as \a value.
 *
 * \sa airflowDirections() setAirflowDirections() airflowDirectionsAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::airflowDirectionsAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> &attribute);
 *
 * This signal is emitted whenever the attribute for the airflowDirections property changes. The new attribute is passed as \a attribute.
 *
 * \sa airflowDirectionsAttribute() airflowDirections()
 */
/*!
 * \fn void QtIVIClimateControl::airConditioningEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the air conditioning system is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isAirConditioningEnabled() setAirConditioningEnabled() airConditioningAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::airConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the airConditioning property changes. The new attribute is passed as \a attribute.
 *
 * \sa airConditioningAttribute() isAirConditioningEnabled()
 */
/*!
 * \fn void QtIVIClimateControl::heaterEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the heater is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isHeaterEnabled() setHeaterEnabled() heaterAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::heaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the heater property changes. The new attribute is passed as \a attribute.
 *
 * \sa heaterAttribute() isHeaterEnabled()
 */
/*!
 * \fn void QtIVIClimateControl::steeringWheelHeaterChanged(int value);
 *
 * This signal is emitted whenever the steering wheel heater level changed. The new level is passed as \a value.
 *
 * \sa steeringWheelHeater() setSteeringWheelHeater() steeringWheelHeaterAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::steeringWheelHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the steeringWheelHeater property changes. The new attribute is passed as \a attribute.
 *
 * \sa steeringWheelHeaterAttribute() steeringWheelHeater()
 */
/*!
 * \fn void QtIVIClimateControl::fanSpeedLevelChanged(int value);
 *
 * This signal is emitted whenever the fan speed level changed. The new level is passed as \a value.
 *
 * \sa fanSpeedLevel() setFanSpeedLevel() fanSpeedLevelAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::fanSpeedLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the fanSpeedLevel property changes. The new attribute is passed as \a attribute.
 *
 * \sa fanSpeedLevelAttribute() fanSpeedLevel()
 */
/*!
 * \fn void QtIVIClimateControl::targetTemperatureChanged(int temperature);
 *
 * This signal is emitted whenever the target temperature changed. The new temperature is passed as \a temperature.
 *
 * \sa targetTemperature() setTargetTemperature() targetTemperatureAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::targetTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the targetTemperature property changes. The new attribute is passed as \a attribute.
 *
 * \sa targetTemperatureAttribute() targetTemperature()
 */
/*!
 * \fn void QtIVIClimateControl::seatCoolerChanged(int value);
 *
 * This signal is emitted whenever the seat cooler level changed. The new level is passed as \a value.
 *
 * \sa seatCooler() setSeatCooler() seatCoolerAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::seatCoolerAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the seatCooler property changes. The new attribute is passed as \a attribute.
 *
 * \sa seatCoolerAttribute() seatCooler()
 */
/*!
 * \fn void QtIVIClimateControl::seatHeaterChanged(int value);
 *
 * This signal is emitted whenever the seat heater level changed. The new level is passed as \a value.
 *
 * \sa seatHeater() setSeatHeater() seatHeaterAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::seatHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the heater property changes. The new attribute is passed as \a attribute.
 *
 * \sa heaterAttribute() isHeaterEnabled()
 */
/*!
 * \fn void QtIVIClimateControl::outsideTemperatureChanged(int value);
 *
 * This signal is emitted whenever the outside temperature changes. The new temperature is passed as \a value.
 *
 * \sa outsideTemperature() outsideTemperatureAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::outsideTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the outsideTemperature property changes. The new attribute is passed as \a attribute.
 *
 * \sa outsideTemperatureAttribute() outsideTemperature()
 */
/*!
 * \fn void QtIVIClimateControl::zoneSynchronizationEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the zone synchronization is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isZoneSynchronizationEnabled() setZoneSynchronizationEnabled() zoneSynchronizationAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::zoneSynchronizationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the zoneSynchronization property changes. The new attribute is passed as \a attribute.
 *
 * \sa zoneSynchronizationAttribute() isZoneSynchronizationEnabled()
 */
/*!
 * \fn void QtIVIClimateControl::defrostEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the defrost is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isDefrostEnabled() setDefrostEnabled() defrostAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::defrostAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the defrost property changes. The new attribute is passed as \a attribute.
 *
 * \sa defrostAttribute() isDefrostEnabled()
 */
/*!
 * \fn void QtIVIClimateControl::recirculationEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the recirculation is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isRecirculationEnabled() recirculationAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::recirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the heater property changes. The new attribute is passed as \a attribute.
 *
 * \sa recirculationAttribute() isRecirculationEnabled()
 */
/*!
 * \fn void QtIVIClimateControl::recirculationModeChanged(QtIVIClimateControl::RecirculationMode value)
 *
 * This signal is emitted whenever the recirculation mode changes. The new recirculation mode is passed as \a value.
 *
 * \sa recirculationMode() setRecirculationMode() recirculationModeAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::recirculationModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::RecirculationMode> &attribute);
 *
 * This signal is emitted whenever the attribute for the recirculationMode property changes. The new attribute is passed as \a attribute.
 *
 * \sa recirculationModeAttribute() recirculationMode()
 */
/*!
 * \fn void QtIVIClimateControl::recirculationSensitivityLevelChanged(int value);
 *
 * This signal is emitted whenever the recirculation sensitivity level level changes. The new level is passed as \a value.
 *
 * \sa recirculationSensitivityLevel() setRecirculationSensitivityLevel() recirculationSensitivityLevelAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::recirculationSensitivityLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the recirculationSensitivityLevel property changes. The new attribute is passed as \a attribute.
 *
 * \sa recirculationSensitivityLevelAttribute() recirculationSensitivityLevel()
 */
/*!
 * \fn void QtIVIClimateControl::climateModeChanged(QtIVIClimateControl::ClimateMode value)
 *
 * This signal is emitted whenever the climate mode changes. The new climate mode is passed as \a value.
 *
 * \sa climateMode() setClimateMode() climateModeAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::climateModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::ClimateMode> &attribute);
 *
 * This signal is emitted whenever the attribute for the climateMode property changes. The new attribute is passed as \a attribute.
 *
 * \sa climateModeAttribute() climateMode()
 */
/*!
 * \fn void QtIVIClimateControl::automaticClimateFanIntensityLevelChanged(int value);
 *
 * This signal is emitted whenever the fan intensity level changes. The new level is passed as \a value.
 *
 * \sa automaticClimateFanIntensityLevel() setAutomaticClimateFanIntensityLevel() automaticClimateFanIntensityLevelAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::automaticClimateFanIntensityLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the automaticClimateFanIntensityLevel property changes. The new attribute is passed as \a attribute.
 *
 * \sa automaticClimateFanIntensityLevelAttribute() automaticClimateFanIntensityLevel()
 */

 #include "moc_qtiviclimatecontrol.cpp"
