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

#include "qivipropertyfactory.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIviPropertyFactory
    \inmodule QtIviCore
    \brief The QIviPropertyFactory is used as a factory class for creating instances of QIviProperty

    QIviPropertyFactory will preserve the type information about the template type in QIviProperty and
    does a compile time check whether all provided functions and signals match this type and each other.

    The following code creates a property of type int:
    \code
    QIviPropertyFactory<int>::create(this,
                                     &QIviClimateControl::targetTemperatureAttribute,
                                     &QIviClimateControl::targetTemperatureAttributeChanged,
                                     &QIviClimateControl::targetTemperature,
                                     &QIviClimateControl::targetTemperatureChanged,
                                     &QIviClimateControl::setTargetTemperature)
    \endcode

    This Factory provides two functions, one for creating a read-write property and one for a read-only property.
*/

/*!
    \fn QIviPropertyFactory::create(const QtPrivate::FunctionPointer<attributeGetterFunc>::Object *sender, attributeGetterFunc attributeGetter, attributeSignalFunc attributeSignal, valueGetterFunc valueGetter, valueSignalFunc valueSignal, valueSlotFunc valueSlot)

    Returns an new QIviProperty instance for a attribute and value of type T.

    The \a sender argument needs to be a pointer to the QObject* instance which implements all functions and signals provided next.
    The arguments \a attributeGetter and \a attributeSignal need to be functions pointers to your attribute getter function and the attribute signal function.
    These arguments need to be of QIviPropertyAttribute<T>.
    \a valueGetter, \a valueSignal, and \a valueSlot need to be function pointers to the getter, signal, and setter for the value that should be stored in this QIviQmlProperty instance.
    The value functions need to have arguments and return values of type T.

    \sa QIviPropertyFactory::create(const QtPrivate::FunctionPointer<attributeGetterFunc>::Object *sender, attributeGetterFunc attributeGetter, attributeSignalFunc attributeSignal, valueGetterFunc valueGetter, valueSignalFunc valueSignal)
*/

/*!
    \fn QIviPropertyFactory::create(const QtPrivate::FunctionPointer<attributeGetterFunc>::Object *sender, attributeGetterFunc attributeGetter, attributeSignalFunc attributeSignal, valueGetterFunc valueGetter, valueSignalFunc valueSignal)

    Returns a new QIviProperty instance for an attribute and value of type T.

    The \a sender argument needs to be a pointer to the QObject* instance which implements all functions and signals provided next.
    The arguments \a attributeGetter and \a attributeSignal need to be functions pointers to your attribute getter function and the attribute signal function.
    These arguments need to be of QIviPropertyAttribute<T>.
    \a valueGetter, \a valueSignal need to be function pointers to the getter and signal for the value which should be stored in this QIviQmlProperty instance.
    The value functions need to have arguments and return values of type T.

    \note This factory function will create a readonly property as no value setter needs to be provided

    \sa QIviPropertyFactory::create(const QtPrivate::FunctionPointer<attributeGetterFunc>::Object *sender, attributeGetterFunc attributeGetter, attributeSignalFunc attributeSignal, valueGetterFunc valueGetter, valueSignalFunc valueSignal, valueSlotFunc valueSlot)
*/

/*!
    \fn bool QIviPropertyFactory::isAvailable() const

    \reimp
*/

/*!
    \fn bool QIviPropertyFactory::minimumValue() const

    \reimp
*/

/*!
    \fn bool QIviPropertyFactory::maximumValue() const

    \reimp
*/

/*!
    \fn bool QIviPropertyFactory::availableValues() const

    \reimp
*/

/*!
    \fn bool QIviPropertyFactory::value() const

    \reimp
*/

QT_END_NAMESPACE
