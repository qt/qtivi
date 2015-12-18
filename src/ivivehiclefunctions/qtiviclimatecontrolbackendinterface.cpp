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

#include "qtiviclimatecontrolbackendinterface.h"

/*!
 * \class QtIVIClimateControlBackendInterface
 * \inmodule QtIVIVehicleFunctions
 * \ingroup backends
 * \inherits QtIVIVehicleBackendInterface
 * \brief The QtIVIClimateControlBackendInterface defines the interface for backends to the
 * QtIVIClimateControl feature class.
 *
 * The QtIVIClimateControlBackendInterface is the interface used by \l QtIVIClimateControl
 *
 * The interface is discovered by a \l QtIVIClimateControl object, which connects to it and sets up
 * the connections to it.
 *
 * \sa QtIVIClimateControl
 */

/*!
 * \fn QtIVIClimateControlBackendInterface::QtIVIClimateControlBackendInterface(QObject *parent=0)
 *
 * Constructs a backend interface.
 *
 * The \a parent is sent to the QObject constructor.
 */
QtIVIClimateControlBackendInterface::QtIVIClimateControlBackendInterface(QObject *parent)
    : QtIVIVehicleBackendInterface(parent)
{
}

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setTargetTemperature(int value, const QString &zone) = 0
 *
 * Sets the target temperature of \a zone to \a value, wheret he \a value is expressed in
 * centigrades and may be range limited by the backend.
 *
 * This method is expected to emit a \l targetTemperatureChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa targetTemperature(), targetTemperatureChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setSeatCooler(int value, const QString &zone) = 0
 *
 * Sets the seat ventilation level of \a zone to \a value, where \a value can be \c 0 (off) or
 * between \c 1 (least cool) to \c 10 (coolest).
 *
 * This method is expected to emit a \l seatCoolerChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa seatCooler(), seatCoolerChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setSeatHeater(int value, const QString &zone) = 0;
 *
 * Sets the seat heater level of \a zone to \a value, where \a value can be \c 0 (off) or between
 * \c 1 (least warm) to \c 10 (warmest).
 *
 * This method is expected to emit a \l seatHeaterChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa seatHeater(), seatHeaterChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setAirflowDirection(QtIVIClimateControl::AirflowDirection, const QString &zone) = 0;
 *
 * Sets the \a zone air flow direction to \a airflowDirection.
 *
 * This method is expected to emit the \l airflowDirectionChanged() signal when receiving a
 * new \a airflowDirection.
 *
 * \sa airflowDirection(), airflowDirectionChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setAirConditioningEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the \a zone air conditioning based on \a enabled.
 *
 * This method is expected to emit the \l airConditioningEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa airConditioningEnabled(), airConditioningEnabledChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setHeaterEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the \a zone heater based on \a enabled.
 *
 * This method is expected to emit the \l heaterEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa heaterEnabled(), heaterEnabledChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setAirRecirculationEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the \a zone air recirculation based on \a enabled.
 *
 * This method is expected to emit the \l airRecirculationEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa airRecirculationEnabled(), airRecirculationEnabledChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setSteeringWheelHeater(int value, const QString &zone) = 0
 *
 * Sets the steering wheel heater level of \a zone to \a value, where \a value can be \c 0 (off)
 * or between \c 1 (least warm) to \c 10 (warmest).
 *
 * This method is expected to emit a \l steeringWheelHeaterChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa steeringWheelHeater(), steeringWheelHeaterChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setFanSpeedLevel(int value, const QString &zone) = 0
 *
 * Sets the fan speed level of \a zone to \a value, where \a value can be \c 0 (off) or between
 * \c 1 (weakest) to \c 10 (strongest).
 *
 * This method is expected to emit a \l fanSpeedLevelChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa fanSpeedLevel(), fanSpeedLevelChanged()
 */

/*!
 * \fn virtual int QtIVIClimateControlBackendInterface::targetTemperature(const QString &zone) const = 0;
 *
 * Returns the target temperature in centigrades for \a zone.
 *
 * \sa setTargetTemperature(), targetTemperatureChanged()
 */

/*!
 * \fn virtual int QtIVIClimateControlBackendInterface::seatCooler(const QString &zone) const = 0
 *
 * Returns the seat ventilation level for \a zone, where the level can be \c 0 (off) or
 * between \c 1 (least cool) to \c 10 (coolest).
 *
 * \sa setSeatCooler(), seatCoolerChanged()
 */

/*!
 * \fn virtual int QtIVIClimateControlBackendInterface::seatHeater(const QString &zone) const = 0
 *
 * Returns the seat heater level for \a zone, where the level can be \c 0 (off) or between
 * \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa setSeatHeater(), seatHeaterChanged()
 */

/*!
 * \fn virtual QtIVIClimateControl::AirflowDirection QtIVIClimateControlBackendInterface::airflowDirection(const QString &zone) const = 0
 *
 * Returns the \a zone airflow direction.
 *
 * \sa setAirflowDirection(), airflowDirectionChanged()
 */

/*!
 * \fn virtual bool QtIVIClimateControlBackendInterface::airConditioningEnabled(const QString &zone) const = 0
 *
 * Returns \c true if the air conditioning is enabled in the \a zone, otherwise \c false.
 *
 * \sa setAirConditioningEnabled(), airConditioningEnabledChanged()
 */

/*!
 * \fn virtual bool QtIVIClimateControlBackendInterface::heaterEnabled(const QString &zone) const = 0
 *
 * Returns \c true if the air conditioning is enabled in the \a zone, otherwise \c false.
 *
 * \sa setHeaterEnabled(), heaterEnabledChanged()
 */

/*!
 * \fn virtual bool QtIVIClimateControlBackendInterface::airRecirculationEnabled(const QString &zone) const = 0
 *
 * Returns \c true if the air conditioning is enabled in the \a zone, otherwise \c false.
 *
 * \sa setAirRecirculationEnabled(), airRecirculationEnabledChanged()
 */

/*!
 * \fn virtual int QtIVIClimateControlBackendInterface::steeringWheelHeater(const QString &zone) const = 0
 *
 * Returns the steering wheel heater level of \a zone, where the level can be \c 0 (off) or between
 * \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa setSteeringWheelHeater(), steeringWheelHeaterChanged()
 */

/*!
 * \fn virtual int QtIVIClimateControlBackendInterface::fanSpeedLevel(const QString &zone) const = 0
 *
 * Returns the fan speed level of \a zone, where the level can be \c 0 (off) or between
 * \c 1 (weakest) to \c 10 (strongest).
 *
 * \sa setFanSpeedLevel(), fanSpeedLevelChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::targetTemperatureChanged(int value, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the target temperature for \a zone is changed to \a value, where
 * value is expressed in centigrades.
 *
 * \sa targetTemperature(), setTargetTemperature()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::seatCoolerChanged(int value, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the seat cooler level is changed for \a zone to \a value, where the
 * \a value can be \c 0 (off) or between \c 1 (least cool) to \c 10 (coolest).
 *
 * \sa seatCooler(), setSeatCooler()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::seatHeaterChanged(int value, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the seat cooler level is changed for \a zone to \a value, where the
 * \a value can be \c 0 (off) or between \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa seatHeater(), setSeatHeater()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::airflowDirectionChanged(QtIVIClimateControl::AirflowDirection airflowDirection, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone airflow direction is changed to \a airflowDirection.
 *
 * \sa airflowDirection(), setAirflowDirection()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::airConditioningEnabledChanged(bool enabled, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone air conditioning state is changed to \a enabled.
 *
 * \sa airConditioningEnabled(), setAirConditioningEnabled()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::heaterEnabledChanged(bool enabled, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone heater state is changed to \a enabled.
 *
 * \sa heaterEnabled(), setHeaterEnabled()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged(bool enabled, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone air recirculation state is changed to \a enabled.
 *
 * \sa airRecirculationEnabled(), setAirRecirculationEnabled()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::steeringWheelHeaterChanged(int level, const QString &zone = QString()) = 0
 *
 * The signals ie emitted when the steering wheel heater level of \a zone is changed to \a level, where the
 * \a level can be \c 0 (off) or between \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa steeringWheelHeater(), setSteeringWheelHeater()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::fanSpeedLevelChanged(int level, const QString &zone = QString()) = 0
 *
 * The signals ie emitted when the fan speel level of \a zone is changed to \a level, where the \a level
 * can be \c 0 (off) or between \c 1 (weakest) to \c 10 (strongest).
 *
 * \sa fanSpeedLevel(), setFanSpeedLevel()
 */
