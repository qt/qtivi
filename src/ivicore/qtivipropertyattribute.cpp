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

#include "qtivipropertyattribute.h"

/*!
 * \class QtIVIPropertyAttribute
 * \inmodule QtIVICore
 * \brief The QtIVIPropertyAttribute encapsulates the attributes of a property
 *
 * QtIVIPropertyAttribute sets the boundaries for a property in QtIVI. In a
 * QtIVI backend interface the QtIVIPropertyAttributes are used to control
 * which properties are implemented by a backend and to set the boundaries like
 * the minimum or the maximum value of a property.
 *
 * By default a QtIVIPropertyAttribute is initialized with the type: Invalid
 * and indicates that this property is not available. This could be the case
 * when the backend doesn't support this property or the property is not available
 * because the current backend configuration does not supported it.
 * (e.g. a property can be supported by a specific car configuration, but is not part of another).
 */

/*!
 * \enum QtIVIPropertyAttributeBase::AttributeType
 *
 * \value Invalid
 *        The Attribute is invalid and is not available
 * \value NoAttributes
 *        No additional attributes for the property are provided
 * \value MinimumMaximum
 *        The Attribute defines the minimum and the maximum Value of the property
 * \value AvailableValues
 *        The Attribute defines a list of values which are available for the property
 */

/*!
 * \fn QtIVIPropertyAttribute::QtIVIPropertyAttribute()
 * Constructs an invalid QtIVIPropertyAttribute.
 *
 * This constructor can be used to indicate that the property is not available.
 * For example because the backend doesn't support this property.
 */

/*!
 * \fn QtIVIPropertyAttribute::QtIVIPropertyAttribute(bool available)
 * Constructs an QtIVIPropertyAttribute of type NoAttributes.
 *
 * The argument \a available defines whether the property is supported by the backend
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
 * If the attributeType is not MinimumMaximum a default constructed value is returned.
 */

/*!
 * \fn T QtIVIPropertyAttribute::maximumValue() const
 *
 * Returns the maximum value of the property.
 * If the attributeType is not MinimumMaximum a default constructed value is returned.
 */

/*!
 * \fn QList<T> QtIVIPropertyAttribute::availableValues() const
 *
 * Returns a list of valid values for this property.
 * If the attributeType is not AvailableValues an empty list is returned.
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
