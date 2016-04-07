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
#include <QtIVICore/QtIVIPropertyFactory>

QtIVIClimateControlPrivate::QtIVIClimateControlPrivate(QtIVIClimateControl *parent)
    : m_airflowDirection(QtIVIClimateControl::None)
    , m_airFlowDirectionProperty(QtIVIPropertyFactory<QtIVIClimateControl::AirflowDirection>::create(parent,
                                                                                                &QtIVIClimateControl::airflowDirectionAttribute,
                                                                                                &QtIVIClimateControl::airflowDirectionAttributeChanged,
                                                                                                &QtIVIClimateControl::airflowDirection,
                                                                                                &QtIVIClimateControl::airflowDirectionChanged,
                                                                                                &QtIVIClimateControl::setAirflowDirection))
    , m_airConditioning(false)
    , m_airConditioningProperty(QtIVIPropertyFactory<bool>::create(parent,
                                                              &QtIVIClimateControl::airConditioningAttribute,
                                                              &QtIVIClimateControl::airConditioningAttributeChanged,
                                                              &QtIVIClimateControl::isAirConditioningEnabled,
                                                              &QtIVIClimateControl::airConditioningEnabledChanged,
                                                              &QtIVIClimateControl::setAirConditioningEnabled))
    , m_heater(false)
    , m_heaterProperty(QtIVIPropertyFactory<bool>::create(parent,
                                                     &QtIVIClimateControl::heaterAttribute,
                                                     &QtIVIClimateControl::heaterAttributeChanged,
                                                     &QtIVIClimateControl::isHeaterEnabled,
                                                     &QtIVIClimateControl::heaterEnabledChanged,
                                                     &QtIVIClimateControl::setHeaterEnabled))
    , m_airRecirculation(false)
    , m_airRecirculationProperty(QtIVIPropertyFactory<bool>::create(parent,
                                                               &QtIVIClimateControl::airRecirculationAttribute,
                                                               &QtIVIClimateControl::airRecirculationAttributeChanged,
                                                               &QtIVIClimateControl::isAirRecirculationEnabled,
                                                               &QtIVIClimateControl::airRecirculationEnabledChanged,
                                                               &QtIVIClimateControl::setAirRecirculationEnabled))
    , m_targetTemperature(0)
    , m_targetTemperatureProperty(QtIVIPropertyFactory<int>::create(parent,
                                                               &QtIVIClimateControl::targetTemperatureAttribute,
                                                               &QtIVIClimateControl::targetTemperatureAttributeChanged,
                                                               &QtIVIClimateControl::targetTemperature,
                                                               &QtIVIClimateControl::targetTemperatureChanged,
                                                               &QtIVIClimateControl::setTargetTemperature))
    , m_seatCooler(0)
    , m_seatCoolerProperty(QtIVIPropertyFactory<int>::create(parent,
                                                        &QtIVIClimateControl::seatCoolerAttribute,
                                                        &QtIVIClimateControl::seatCoolerAttributeChanged,
                                                        &QtIVIClimateControl::seatCooler,
                                                        &QtIVIClimateControl::seatCoolerChanged,
                                                        &QtIVIClimateControl::setSeatCooler))
    , m_seatHeater(0)
    , m_seatHeaterProperty(QtIVIPropertyFactory<int>::create(parent,
                                                        &QtIVIClimateControl::seatHeaterAttribute,
                                                        &QtIVIClimateControl::seatHeaterAttributeChanged,
                                                        &QtIVIClimateControl::seatHeater,
                                                        &QtIVIClimateControl::seatHeaterChanged,
                                                        &QtIVIClimateControl::setSeatHeater))
    , m_steeringWheelHeater(0)
    , m_steeringWheelHeaterProperty(QtIVIPropertyFactory<int>::create(parent,
                                                                 &QtIVIClimateControl::steeringWheelHeaterAttribute,
                                                                 &QtIVIClimateControl::steeringWheelHeaterAttributeChanged,
                                                                 &QtIVIClimateControl::steeringWheelHeater,
                                                                 &QtIVIClimateControl::steeringWheelHeaterChanged,
                                                                 &QtIVIClimateControl::setSteeringWheelHeater))
    , m_fanSpeedLevel(0)
    , m_fanSpeedLevelProperty(QtIVIPropertyFactory<int>::create(parent,
                                                           &QtIVIClimateControl::fanSpeedLevelAttribute,
                                                           &QtIVIClimateControl::fanSpeedLevelAttributeChanged,
                                                           &QtIVIClimateControl::fanSpeedLevel,
                                                           &QtIVIClimateControl::fanSpeedLevelChanged,
                                                           &QtIVIClimateControl::setFanSpeedLevel))
{
}

void QtIVIClimateControlPrivate::clearToDefaults()
{
    m_airConditioning = false;
    m_heater = false;
    m_airRecirculation = false;
    m_targetTemperature = 0;
    m_seatCooler = 0;
    m_seatHeater = 0;
    m_steeringWheelHeater = 0;
    m_fanSpeedLevel = 0;
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
   \value None
          AirflowDirection is not initialized
   \value FloorPanel
          Direct airflow along the floor panel.
   \value FloorDuct
          Direct airflow through the floor duct.
   \value BiLevel
          Direct airflow at both levels.
   \value DefrostFloor
          Direct airflow to defrost.
*/

/*!
   Constructs a climate control object to the given \a zone.

   If \a zone is not provided the General zone will be created.

   The \a parent argument is passed on to the \l QtIVIAbstractZonedFeature base class.
*/
QtIVIClimateControl::QtIVIClimateControl(const QString &zone, QObject* parent)
    : QtIVIAbstractZonedFeature(QtIVIStringClimateControlInterfaceName, zone, parent)
    , d_ptr(new QtIVIClimateControlPrivate(this))
{
    qRegisterMetaType<QtIVIPropertyAttribute<int>>();
    qRegisterMetaType<QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection>>();
    qRegisterMetaType<QtIVIPropertyAttribute<bool>>();
}

QtIVIClimateControl::~QtIVIClimateControl()
{
    delete d_ptr;
}

QtIVIClimateControlBackendInterface *QtIVIClimateControl::climateControlBackend()
{
    return qobject_cast<QtIVIClimateControlBackendInterface*>(backend());
}

/*!
   \reimp
*/
void QtIVIClimateControl::connectToServiceObject(QtIVIServiceObject *serviceObject)
{
    QtIVIAbstractZonedFeature::connectToServiceObject(serviceObject);

    QtIVIClimateControlBackendInterface* backend = climateControlBackend();
    if (!backend)
        return;

    connect(backend, &QtIVIClimateControlBackendInterface::targetTemperatureChanged,
            this, &QtIVIClimateControl::onTargetTemperatureChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::targetTemperatureAttributeChanged,
            this, &QtIVIClimateControl::onTargetTemperatureAttributeChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::seatCoolerChanged,
            this, &QtIVIClimateControl::onSeatCoolerChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::seatCoolerAttributeChanged,
            this, &QtIVIClimateControl::onSeatCoolerAttributeChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::seatHeaterChanged,
            this, &QtIVIClimateControl::onSeatHeaterChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::seatHeaterAttributeChanged,
            this, &QtIVIClimateControl::onSeatHeaterAttributeChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::steeringWheelHeaterChanged,
            this, &QtIVIClimateControl::onSteeringWheelHeaterChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::steeringWheelHeaterAttributeChanged,
            this, &QtIVIClimateControl::onSteeringWheelHeaterAttributeChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::fanSpeedLevelChanged,
            this, &QtIVIClimateControl::onFanSpeedLevelChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::fanSpeedLevelAttributeChanged,
            this, &QtIVIClimateControl::onFanSpeedLevelAttributeChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airflowDirectionChanged,
            this, &QtIVIClimateControl::onAirflowDirectionChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airflowDirectionAttributeChanged,
            this, &QtIVIClimateControl::onAirflowDirectionAttributeChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airConditioningEnabledChanged,
            this, &QtIVIClimateControl::onAirConditioningEnabledChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airConditioningAttributeChanged,
            this, &QtIVIClimateControl::onAirConditioningAttributeChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::heaterEnabledChanged,
            this, &QtIVIClimateControl::onHeaterEnabledChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::heaterAttributeChanged,
            this, &QtIVIClimateControl::onHeaterAttributeChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged,
            this, &QtIVIClimateControl::onAirRecirculationEnabledChanged);
    connect(backend, &QtIVIClimateControlBackendInterface::airRecirculationAttributeChanged,
            this, &QtIVIClimateControl::onAirRecirculationAttributeChanged);

    backend->initializeAttributes();
}

/*!
   \reimp
*/
void QtIVIClimateControl::clearServiceObject()
{
    Q_D(QtIVIClimateControl);
    d->clearToDefaults();
}

void QtIVIClimateControl::onAirflowDirectionChanged(QtIVIClimateControl::AirflowDirection value, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_airflowDirection = value;
    emit f->airflowDirectionChanged(value);
}

void QtIVIClimateControl::onAirflowDirectionAttributeChanged(QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection> airflowDirectionAttribute, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_airflowDirectionAttribute = airflowDirectionAttribute;
    emit f->airflowDirectionAttributeChanged(airflowDirectionAttribute);
}

void QtIVIClimateControl::onAirConditioningEnabledChanged(bool enabled, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_airConditioning = enabled;
    emit f->airConditioningEnabledChanged(enabled);
}

void QtIVIClimateControl::onAirConditioningAttributeChanged(QtIVIPropertyAttribute<bool> airConditioningEnabledAttribute, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_airConditioningAttribute = airConditioningEnabledAttribute;
    emit f->airConditioningAttributeChanged(airConditioningEnabledAttribute);
}

void QtIVIClimateControl::onHeaterEnabledChanged(bool enabled, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_heater = enabled;
    emit f->heaterEnabledChanged(enabled);
}

void QtIVIClimateControl::onHeaterAttributeChanged(QtIVIPropertyAttribute<bool> heaterEnabledAttribute, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_heaterAttribute = heaterEnabledAttribute;
    emit f->heaterAttributeChanged(heaterEnabledAttribute);
}

void QtIVIClimateControl::onAirRecirculationEnabledChanged(bool enabled, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_airRecirculation = enabled;
    emit f->airRecirculationEnabledChanged(enabled);
}

void QtIVIClimateControl::onAirRecirculationAttributeChanged(QtIVIPropertyAttribute<bool> airRecirculationEnabledAttribute, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_airRecirculationAttribute = airRecirculationEnabledAttribute;
    emit f->airRecirculationAttributeChanged(airRecirculationEnabledAttribute);
}

void QtIVIClimateControl::onSteeringWheelHeaterChanged(int value, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_steeringWheelHeater = value;
    emit f->steeringWheelHeaterChanged(value);
}

void QtIVIClimateControl::onSteeringWheelHeaterAttributeChanged(QtIVIPropertyAttribute<int> steeringWheelHeaterAttribute, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_steeringWheelHeaterAttribute = steeringWheelHeaterAttribute;
    emit f->steeringWheelHeaterAttributeChanged(steeringWheelHeaterAttribute);
}

void QtIVIClimateControl::onFanSpeedLevelChanged(int value, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_fanSpeedLevel = value;
    emit f->fanSpeedLevelChanged(value);
}

void QtIVIClimateControl::onFanSpeedLevelAttributeChanged(QtIVIPropertyAttribute<int> fanSpeedAttribute, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_fanSpeedLevelAttribute = fanSpeedAttribute;
    emit f->fanSpeedLevelAttributeChanged(fanSpeedAttribute);
}

void QtIVIClimateControl::onTargetTemperatureChanged(int temperature, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_targetTemperature = temperature;
    emit f->targetTemperatureChanged(temperature);
}

void QtIVIClimateControl::onTargetTemperatureAttributeChanged(QtIVIPropertyAttribute<int> temperatureAttribute, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_targetTemperatureAttribute = temperatureAttribute;
    emit f->targetTemperatureAttributeChanged(temperatureAttribute);
}

void QtIVIClimateControl::onSeatCoolerChanged(int value, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_seatCooler = value;
    emit f->seatCoolerChanged(value);
}

void QtIVIClimateControl::onSeatCoolerAttributeChanged(QtIVIPropertyAttribute<int> seatCoolerAttribute, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_seatCoolerAttribute = seatCoolerAttribute;
    emit f->seatCoolerAttributeChanged(seatCoolerAttribute);
}

void QtIVIClimateControl::onSeatHeaterChanged(int value, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_seatHeater = value;
    emit f->seatHeaterChanged(value);
}

void QtIVIClimateControl::onSeatHeaterAttributeChanged(QtIVIPropertyAttribute<int> seatHeaterAttribute, const QString &zone)
{
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(zoneAt(zone));
    if (!f)
        f = this;
    if (f->zone() != zone)
        return;
    f->d_ptr->m_seatHeaterAttribute = seatHeaterAttribute;
    emit f->seatHeaterAttributeChanged(seatHeaterAttribute);
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
   \qmlqtivipropertyEnum {QtIVIVehicleFunctions::ClimateControl::airflowDirection}

   \e value holds the airflow direction.
   Available values are:
   \value None
          AirflowDirection is not initialized.
   \value FloorPanel
          Direct airflow along the floor panel.
   \value FloorDuct
          Direct airflow through the floor duct.
   \value BiLevel
          Direct airflow at both levels.
   \value DefrostFloor
          Direct airflow to defrost.
 */
/*!
   \property QtIVIClimateControl::airflowDirection

   Holds a QtIVIProperty of type \e QtIVIClimateControl::AirflowDirection where the QtIVIProperty::value() function returns the current air flow direction.

   \sa AttributeSystem
   \sa airflowDirection() setAirflowDirection() airflowDirectionAttribute()
 */
/*!
 * Returns the current air flow direction.
 *
 * \sa setAirflowDirection() airflowDirectionChanged() airflowDirectionAttribute()
 */
QtIVIClimateControl::AirflowDirection QtIVIClimateControl::airflowDirection() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airflowDirection;
}

/*!
 * Returns the attribute defining the boundaries and availability of the air flow property
 *
 * \sa setAirflowDirection() airflowDirection() airflowDirectionChanged()
 */
QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection> QtIVIClimateControl::airflowDirectionAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airflowDirectionAttribute;
}

QtIVIProperty *QtIVIClimateControl::airflowDirectionProperty() const
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
   \qmlqtivipropertyBool {QtIVIVehicleFunctions::ClimateControl::airRecirculation}

   \e value is true if the air recirculation is enabled.
*/
/*!
   \property QtIVIClimateControl::airRecirculation

   Holds a QtIVIProperty of type \e bool where the QtIVIProperty::value() function indicates if air recirculation is enabled.

   \sa AttributeSystem
   \sa isAirRecirculationEnabled() setAirRecirculationEnabled() airRecirculationAttribute()
 */
/*!
 * Returns true if air recirculation is enabled.
 *
 * \sa setAirRecirculationEnabled() airRecirculationEnabledChanged() airRecirculationAttribute()
 */
bool QtIVIClimateControl::isAirRecirculationEnabled() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airRecirculation;
}

/*!
 * Returns the attribute defining the boundaries and availability of the air recirculation property.
 *
 * \sa setAirRecirculationEnabled() isAirRecirculationEnabled() airRecirculationEnabledChanged()
 */
QtIVIPropertyAttribute<bool> QtIVIClimateControl::airRecirculationAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airRecirculationAttribute;
}

QtIVIProperty *QtIVIClimateControl::airRecirculationProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airRecirculationProperty;
}

/*!
   \qmlproperty int ClimateZone::steeringWheelHeater
 */
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
 * Returns the current steering wheel heater level, where the level can be between \c 0 (off) and \c 10 (warmest).
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
 * Returns the current fan speed level, where the level can be between \c 0 (off) and \c 10 (strongest).
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

   \e value holds the target temperature of the zone expressed in centigrades, where the temperature can be between \c minimumValue(off)
   to \c maximumValue (strongest).
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
 * Returns the current seat cooler level, where the level can be between \c 0 (off) and \c 10 (coolest).
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
 * Returns the current seat heater level, where the level can be between \c 0 (off) and \c 10 (warmest).
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
 * Sets the air conditioning system enabled, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isAirConditioningEnabled() airConditioningEnabledChanged() airConditioningAttribute()
 */
void QtIVIClimateControl::setAirConditioningEnabled(bool enabled)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setAirConditioningEnabled(enabled, zone());
}

/*!
 * Sets the air flow direction to \a direction.
 *
 * \sa airflowDirection() airflowDirectionChanged() airflowDirectionAttribute()
 */
void QtIVIClimateControl::setAirflowDirection(QtIVIClimateControl::AirflowDirection direction)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setAirflowDirection(direction, zone());
}

/*!
 * Enables the heater, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isHeaterEnabled() heaterEnabledChanged() heaterAttribute()
 */
void QtIVIClimateControl::setHeaterEnabled(bool enabled)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setHeaterEnabled(enabled, zone());
}

/*!
 * Sets the air recirculation system enabled, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isAirRecirculationEnabled() steeringWheelHeaterChanged() steeringWheelHeaterAttribute()
 */
void QtIVIClimateControl::setAirRecirculationEnabled(bool enabled)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setAirRecirculationEnabled(enabled, zone());
}

/*!
 * Sets the current steering wheel heater level to \a value, where the level can be between \c 0 (off) and \c 10 (warmest).
 *
 * \sa steeringWheelHeater() steeringWheelHeaterChanged() steeringWheelHeaterAttribute()
 */
void QtIVIClimateControl::setSteeringWheelHeater(int value)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setSteeringWheelHeater(value, zone());
}

/*!
 * Sets the current fan speed level to \a value, where the level can be between \c 0 (off) and \c 10 (strongest).
 *
 * \sa fanSpeedLevel() fanSpeedLevelChanged() fanSpeedLevelAttribute()
 */
void QtIVIClimateControl::setFanSpeedLevel(int value)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setFanSpeedLevel(value, zone());
}

/*!
 * Sets the current target temperature to \a temperature expressed in centigrades.
 *
 * \sa targetTemperature() targetTemperatureChanged() targetTemperatureAttribute()
 */
void QtIVIClimateControl::setTargetTemperature(int temperature)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setTargetTemperature(temperature, zone());
}

/*!
 * Sets the current seat cooler level to \a value, where the level can be between \c 0 (off) and \c 10 (coolest).
 *
 * \sa seatCooler() seatCoolerChanged() seatCoolerAttribute()
 */
void QtIVIClimateControl::setSeatCooler(int value)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setSeatCooler(value, zone());
}

/*!
 * Sets the current seat heater level to \a value, where the level can be between \c 0 (off) and \c 10 (warmest).
 *
 * \sa seatHeater() seatHeaterChanged() seatHeaterAttribute()
 */
void QtIVIClimateControl::setSeatHeater(int value)
{
    if (QtIVIClimateControlBackendInterface* backend = climateControlBackend())
        backend->setSeatHeater(value, zone());
}

/*!
 * \fn void QtIVIClimateControl::airflowDirectionChanged(QtIVIClimateControl::AirflowDirection value)
 *
 * This signal is emitted whenever the air flow direction changes. The new flow direction is passed as \a value.
 *
 * \sa airflowDirection() setAirflowDirection() airflowDirectionAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::airflowDirectionAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection> &attribute);
 *
 * This signal is emitted whenever the attribute for the airflowDirection property changes. The new attribute is passed as \a attribute.
 *
 * \sa airflowDirectionAttribute() airflowDirection()
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
 * \fn void QtIVIClimateControl::airRecirculationEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the air recirculation system is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isAirRecirculationEnabled() setAirRecirculationEnabled() airRecirculationAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::airRecirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the airRecirculation property changes. The new attribute is passed as \a attribute.
 *
 * \sa airRecirculationAttribute() isAirRecirculationEnabled()
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
