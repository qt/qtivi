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

#include "qtivipropertyattribute.h"

/*!
 * \class QtIVIPropertyAttribute
 * \inmodule QtIVICore
 * \brief The QtIVIPropertyAttribute encapsulates the attributes of a property
 *
 * QtIVIPropertyAttribute sets the boundaries for a property in Qt IVI. In a
 * Qt IVI backend interface, the QtIVIPropertyAttributes are used to control
 * which properties are implemented by a backend and to set the boundaries like
 * the minimum or the maximum value of a property.
 *
 * By default a QtIVIPropertyAttribute is initialized with the "Invalid" type,
 * which indicates that this property is not available. This could be the case
 * when the backend doesn't support this property or the property is not available
 * because the current backend configuration does not support it.
 * For example, a property can be supported by a specific car configuration,
 * but is not part of another.
 */

/*!
 * \enum QtIVIPropertyAttributeBase::AttributeType
 *
 * \value Invalid
 *        The attribute is invalid and is not available.
 * \value NoAttributes
 *        No additional attributes for the property are provided.
 * \value MinimumMaximum
 *        The attribute defines the minimum and the maximum value of the property.
 * \value AvailableValues
 *        The attribute defines a list of values that are available for the property.
 */

/*!
 * \fn QtIVIPropertyAttribute::QtIVIPropertyAttribute()
 * Constructs an invalid QtIVIPropertyAttribute.
 *
 * This constructor can be used to indicate that the property is not available.
 * For example, because the backend doesn't support this property.
 */

/*!
 * \fn QtIVIPropertyAttribute::QtIVIPropertyAttribute(bool available)
 * Constructs an QtIVIPropertyAttribute of type NoAttributes.
 *
 * The argument \a available defines whether the property is supported by the backend.
 */

/*!
 * \fn QtIVIPropertyAttribute::QtIVIPropertyAttribute(const T &minValue, const T &maxValue)
 * Constructs an QtIVIPropertyAttribute of type MinimumMaximum.
 *
 * The arguments \a minValue and \a maxValue define the boundaries the value of the property.
 */

/*!
 * \fn QtIVIPropertyAttribute::QtIVIPropertyAttribute(const QVector<T> &avValues)
 * Constructs an QtIVIPropertyAttribute of type AvailableValues.
 *
 * The argument \a avValues defines the valid values for the property.
 */

/*!
 * \fn AttributeType QtIVIPropertyAttribute::type() const
 *
 * Returns the type of the attribute.
 */

/*!
 * \fn bool QtIVIPropertyAttribute::isAvailable() const
 *
 * Returns true if the property is available for this backend.
 */

/*!
 * \fn T QtIVIPropertyAttribute::minimumValue() const
 *
 * Returns the minimum value of the property.
 * If the attributeType is not MinimumMaximum, a default constructed value is returned.
 */

/*!
 * \fn T QtIVIPropertyAttribute::maximumValue() const
 *
 * Returns the maximum value of the property.
 * If the attributeType is not MinimumMaximum, a default constructed value is returned.
 */

/*!
 * \fn QList<T> QtIVIPropertyAttribute::availableValues() const
 *
 * Returns a list of valid values for this property.
 * If the attributeType is not AvailableValues an empty list is returned.
 */

/*!
 * \fn QString QtIVIPropertyAttribute::toString() const
 *
 * Returns a QString with a string representation of the attribute.
 */

/*!
 * \fn bool QtIVIPropertyAttribute::operator==(const QtIVIPropertyAttribute& other) const
 *
 * Returns \c true if this QtIVIPropertyAttribute is equal to the \a other QtIVIPropertyAttribute; otherwise returns \c false.
 *
 * All elements of QtIVIPropertyAttribute are used for the comparison.
 */

/*!
 * \fn bool QtIVIPropertyAttribute::operator!=(const QtIVIPropertyAttribute& other) const
 *
 * Returns \c true if this QtIVIPropertyAttribute is NOT equal to the \a other QtIVIPropertyAttribute; otherwise returns \c false.
 *
 * All elements of QtIVIPropertyAttribute are used for the comparison.
 */
