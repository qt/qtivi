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

#include "qtivipropertyfactory.h"

/*!
 * \class QtIVIPropertyFactory
 * \inmodule QtIVICore
 * \brief The QtIVIPropertyFactory is used as a factory class for creating instances of QtIVIProperty
 *
 * QtIVIPropertyFactory will preserve the type information about the template type in QtIVIProperty and
 * does a compile time check whether all provided functions and signals match this type and each other.
 *
 * The following code creates a property of type int:
 * \code
 * QtIVIPropertyFactory<int>::create(this,
 *                                   &QtIVIClimateControl::targetTemperatureAttribute,
 *                                   &QtIVIClimateControl::targetTemperatureAttributeChanged,
 *                                   &QtIVIClimateControl::targetTemperature,
 *                                   &QtIVIClimateControl::targetTemperatureChanged,
 *                                   &QtIVIClimateControl::setTargetTemperature)
 * \endcode
 *
 * This Factory provides two functions, one for creating a read-write property and one for a read-only property.
 */

/*!
 * \fn QtIVIPropertyFactory::create(const QtPrivate::FunctionPointer<attributeGetterFunc>::Object *sender, attributeGetterFunc attributeGetter, attributeSignalFunc attributeSignal, valueGetterFunc valueGetter, valueSignalFunc valueSignal, valueSlotFunc valueSlot)
 *
 *  Returns an new QtIVIProperty instance for a attribute and value of type T.
 *
 *  The \a sender argument needs to be a pointer to the QObject* instance which implements all functions and signals provided next.
 *  The arguments \a attributeGetter and \a attributeSignal need to be functions pointers to your attribute getter function and the attribute signal function.
 *  These arguments need to be of QtIVIPropertyAttribute<T>.
 *  \a valueGetter, \a valueSignal, and \a valueSlot need to be function pointers to the getter, signal, and setter for the value that should be stored in this QtIVIQmlProperty instance.
 *  The value functions need to have arguments and return values of type T.
 *
 * \sa QtIVIPropertyFactory::create(const QtPrivate::FunctionPointer<attributeGetterFunc>::Object *sender, attributeGetterFunc attributeGetter, attributeSignalFunc attributeSignal, valueGetterFunc valueGetter, valueSignalFunc valueSignal)
 */

/*!
 * \fn QtIVIPropertyFactory::create(const QtPrivate::FunctionPointer<attributeGetterFunc>::Object *sender, attributeGetterFunc attributeGetter, attributeSignalFunc attributeSignal, valueGetterFunc valueGetter, valueSignalFunc valueSignal)
 *
 *  Returns a new QtIVIProperty instance for an attribute and value of type T.
 *
 *  The \a sender argument needs to be a pointer to the QObject* instance which implements all functions and signals provided next.
 *  The arguments \a attributeGetter and \a attributeSignal need to be functions pointers to your attribute getter function and the attribute signal function.
 *  These arguments need to be of QtIVIPropertyAttribute<T>.
 *  \a valueGetter, \a valueSignal need to be function pointers to the getter and signal for the value which should be stored in this QtIVIQmlProperty instance.
 *  The value functions need to have arguments and return values of type T.
 *
 *  \note This factory function will create a readonly property as no value setter needs to be provided
 *
 * \sa QtIVIPropertyFactory::create(const QtPrivate::FunctionPointer<attributeGetterFunc>::Object *sender, attributeGetterFunc attributeGetter, attributeSignalFunc attributeSignal, valueGetterFunc valueGetter, valueSignalFunc valueSignal, valueSlotFunc valueSlot)
 */

/*!
 * \fn bool QtIVIPropertyFactory::isAvailable() const
 *
 * \reimp
 */

/*!
 * \fn bool QtIVIPropertyFactory::minimumValue() const
 *
 * \reimp
 */

/*!
 * \fn bool QtIVIPropertyFactory::maximumValue() const
 *
 * \reimp
 */

/*!
 * \fn bool QtIVIPropertyFactory::availableValues() const
 *
 * \reimp
 */

/*!
 * \fn bool QtIVIPropertyFactory::value() const
 *
 * \reimp
 */
