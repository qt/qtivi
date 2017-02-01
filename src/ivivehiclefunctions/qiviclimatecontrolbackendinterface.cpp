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

#include "qiviclimatecontrolbackendinterface.h"

/*!
 * \class QIviClimateControlBackendInterface
 * \inmodule QtIviVehicleFunctions
 * \ingroup backends
 * \inherits QIviZonedFeatureInterface
 * \brief The QIviClimateControlBackendInterface defines the interface for backends to the
 * QIviClimateControl feature class.
 *
 * The QIviClimateControlBackendInterface is the interface used by \l QIviClimateControl
 *
 * The interface is discovered by a \l QIviClimateControl object, which connects to it and sets up
 * the connections to it.
 *
 * \sa QIviClimateControl
 */

/*!
 * \fn QIviClimateControlBackendInterface::QIviClimateControlBackendInterface(QObject *parent=0)
 *
 * Constructs a backend interface.
 *
 * The \a parent is sent to the QObject constructor.
 */
QIviClimateControlBackendInterface::QIviClimateControlBackendInterface(QObject *parent)
    : QIviZonedFeatureInterface(parent)
{
}

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setTargetTemperature(int value, const QString &zone) = 0
 *
 * Sets the target temperature of \a zone to \a value, where the \a value is expressed in
 * centigrades and may be range limited by the backend.
 *
 * This method is expected to emit a \l targetTemperatureChanged() signal when the internal state changes
 * due to this function call. The signal is even expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa targetTemperatureChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setSeatCooler(int value, const QString &zone) = 0
 *
 * Sets the seat ventilation level of \a zone to \a value. The range can be defined using the attribute system.
 *
 * This method is expected to emit a \l seatCoolerChanged() signal when the internal state changes
 * due to this function call. The signal is even expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa seatCoolerChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setSeatHeater(int value, const QString &zone) = 0;
 *
 * Sets the seat heater level of \a zone to \a value. The range can be defined using the attribute system.
 *
 * This method is expected to emit a \l seatHeaterChanged() signal when the internal state changes
 * due to this function call. The signal is even expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa seatHeaterChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setAirflowDirections(QIviClimateControl::AirflowDirections, const QString &zone) = 0;
 *
 * Sets the \a zone air flow directions to \a airflowDirections.
 *
 * This method is expected to emit the \l airflowDirectionsChanged() signal when the internal state changes
 * due to this function call.
 *
 * \sa airflowDirectionsChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setAirConditioningEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the \a zone air conditioning based on \a enabled.
 *
 * This method is expected to emit the \l airConditioningEnabledChanged() signal when the internal state changes
 * due to this function call.
 *
 * \sa airConditioningEnabledChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setHeaterEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the \a zone heater based on \a enabled.
 *
 * This method is expected to emit the \l heaterEnabledChanged() signal when the internal state changes
 * due to this function call.
 *
 * \sa heaterEnabledChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setSteeringWheelHeater(int value, const QString &zone) = 0
 *
 * Sets the steering wheel heater level of \a zone to \a value. The range can be defined using the attribute system.
 *
 * This method is expected to emit a \l steeringWheelHeaterChanged() signal when the internal state changes
 * due to this function call. The signal is even expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa steeringWheelHeaterChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setFanSpeedLevel(int value, const QString &zone) = 0
 *
 * Sets the fan speed level of \a zone to \a value. The range can be defined using the attribute system.
 *
 * This method is expected to emit a \l fanSpeedLevelChanged() signal when the internal state changes
 * due to this function call. The signal is even expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa fanSpeedLevelChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setZoneSynchronizationEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the \a zone synchronization based on \a enabled.
 *
 * This method is expected to emit the \l zoneSynchronizationEnabledChanged() signal when the internal state changes
 * due to this function call.
 *
 * \sa zoneSynchronizationEnabledChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setDefrostEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the \a zone defrost mode based on \a enabled.
 *
 * This method is expected to emit the \l defrostEnabledChanged() signal when the internal state changes
 * due to this function call.
 *
 * \sa defrostEnabledChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setRecirculationMode(QIviClimateControl::RecirculationMode, const QString &zone) = 0;
 *
 * Sets the \a zone recirculation mode to \a recirculationMode.
 *
 * This method is expected to emit the \l recirculationModeChanged() signal when the internal state changes
 * due to this function call.
 *
 * \sa recirculationModeChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setRecirculationSensitivityLevel(int value, const QString &zone) = 0
 *
 * Sets the recirculation sensitivity level of \a zone to \a value. The range can be defined using the attribute system.
 *
 * This method is expected to emit a \l recirculationSensitivityLevelChanged() signal when the internal state changes
 * due to this function call. The signal is even expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa recirculationSensitivityLevelChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setClimateMode(QIviClimateControl::ClimateMode, const QString &zone) = 0;
 *
 * Sets the \a zone climate mode to \a climateMode.
 *
 * This method is expected to emit the \l climateModeChanged() signal when the internal state changes
 * due to this function call.
 *
 * \sa climateModeChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::setAutomaticClimateFanIntensityLevel(int value, const QString &zone) = 0
 *
 * Sets the fan intensity level for the AutomaticClimate mode of \a zone to \a value. The range can be defined using the attribute system.
 *
 * This method is expected to emit a \l automaticClimateFanIntensityLevelChanged() signal when the internal state changes
 * due to this function call. The signal is even expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa automaticClimateFanIntensityLevelChanged()
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::targetTemperatureChanged(int value, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the target temperature for \a zone is changed to \a value, where
 * value is expressed in centigrades.
 *
 * \sa setTargetTemperature()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::targetTemperatureAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the target temperature attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::seatCoolerChanged(int value, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the seat cooler level is changed for \a zone to \a value. The range can be defined using the attribute system.
 *
 * \sa setSeatCooler()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::seatCoolerAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the seat cooler level attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::seatHeaterChanged(int value, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the seat heater level is changed for \a zone to \a value. The range can be defined using the attribute system.
 *
 * \sa setSeatHeater()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::seatHeaterAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the seat heater attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::airflowDirectionsChanged(QIviClimateControl::AirflowDirections airflowDirections, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone airflow directions changed to \a airflowDirections.
 *
 * \sa setAirflowDirections()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::airflowDirectionsAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::AirflowDirections> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the zone airflow direction attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::airConditioningEnabledChanged(bool enabled, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone air conditioning state is changed to \a enabled.
 *
 * \sa setAirConditioningEnabled()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::airConditioningAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the air conditioning state attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::heaterEnabledChanged(bool enabled, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone heater state is changed to \a enabled.
 *
 * \sa setHeaterEnabled()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::heaterAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the heater state attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::steeringWheelHeaterChanged(int level, const QString &zone = QString()) = 0
 *
 * The signals is emitted when the steering wheel heater level of \a zone is changed to \a level. The range can be defined using the attribute system.
 *
 * \sa setSteeringWheelHeater()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::steeringWheelHeaterAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the steering wheel heater level attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::fanSpeedLevelChanged(int level, const QString &zone = QString()) = 0
 *
 * The signals is emitted when the fan speel level of \a zone is changed to \a level. The range can be defined using the attribute system.
 *
 * \sa setFanSpeedLevel()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::fanSpeedLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the recirculation sensitivity level for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::outsideTemperatureChanged(int value, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the outside temperature for \a zone is changed to \a value, where
 * value is expressed in centigrades.
 */

/*!
 * \fn void QIviClimateControlBackendInterface::outsideTemperatureAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the outside temperature attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::zoneSynchronizationEnabledChanged(bool enabled, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone synchronization state is changed to \a enabled.
 *
 * \sa setZoneSynchronizationEnabled()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::zoneSynchronizationAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the zone synchronization state attribute for \a zone is changed to \a attribute.
 */


/*!
 * \fn virtual void QIviClimateControlBackendInterface::defrostEnabledChanged(bool enabled, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone defrost state is changed to \a enabled.
 *
 * \sa setDefrostEnabled()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::defrostAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the defrost state attribute for \a zone is changed to \a attribute.
 */


/*!
 * \fn virtual void QIviClimateControlBackendInterface::recirculationModeChanged(QIviClimateControl::RecirculationMode recirculationMode, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone recirculation mode changed to \a recirculationMode.
 *
 * \sa setRecirculationMode()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::recirculationModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::RecirculationMode> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the recirculation mode attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::recirculationEnabledChanged(bool enabled, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone recirculation state is changed to \a enabled.
 */

/*!
 * \fn void QIviClimateControlBackendInterface::recirculationAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the recirculation state attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::recirculationSensitivityLevelChanged(int level, const QString &zone = QString()) = 0
 *
 * The signals is emitted when the recirculation sensitivity level for \a zone is changed to \a level. The range can be defined using the attribute system.
 *
 * \sa setRecirculationSensitivityLevel()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::recirculationSensitivityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the fan intensity level for the AutomaticClimate mode for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::climateModeChanged(QIviClimateControl::ClimateMode climateMode, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone climate mode changed to \a climateMode.
 *
 * \sa setAirflowDirections()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::climateModeAttributeChanged(const QIviPropertyAttribute<QIviClimateControl::ClimateMode> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the climate mode attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QIviClimateControlBackendInterface::automaticClimateFanIntensityLevelChanged(int level, const QString &zone = QString()) = 0
 *
 * The signals is emitted when the fan intensity level for the AutomaticClimate mode for \a zone is changed to \a level. The range can be defined using the attribute system.
 *
 * \sa setAutomaticClimateFanIntensityLevel()
 */

/*!
 * \fn void QIviClimateControlBackendInterface::automaticClimateFanIntensityLevelAttributeChanged(const QIviPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the fan intensity level for the AutomaticClimate mode for \a zone is changed to \a attribute.
 */
