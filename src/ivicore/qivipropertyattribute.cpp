/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

#include "qivipropertyattribute.h"

QT_BEGIN_NAMESPACE

QIviPropertyAttributeBase::QIviPropertyAttributeBase()
{
    registerTypes();
}

void QIviPropertyAttributeBase::registerTypes()
{
    qRegisterMetaType<QIviPropertyAttribute<int>>();
    qRegisterMetaType<QIviPropertyAttribute<bool>>();
    qRegisterMetaType<QIviPropertyAttribute<double>>();
#if defined(QT_COORD_TYPE)
    qRegisterMetaType<QIviPropertyAttribute<qreal>>();
#endif
    qRegisterMetaType<QIviPropertyAttribute<QString>>();
}


/*!
    \class QIviPropertyAttribute
    \obsolete
    \inmodule QtIviCore
    \brief The QIviPropertyAttribute encapsulates the attributes of a property.

    QIviPropertyAttribute sets the boundaries for a property in Qt IVI. In a
    Qt IVI backend interface, the QIviPropertyAttributes are used to control
    which properties are implemented by a backend and to set the boundaries like
    the minimum or the maximum value of a property.

    By default a QIviPropertyAttribute is initialized with the "Invalid" type,
    which indicates that this property is not available. This could be the case
    when the backend doesn't support this property or the property is not available
    because the current backend configuration does not support it.
    For example, a property can be supported by a specific car configuration,
    but is not part of another.
*/

/*!
    \enum QIviPropertyAttributeBase::AttributeType

    \value Invalid
          The attribute is invalid and is not available.
    \value NoAttributes
          No additional attributes for the property are provided.
    \value MinimumMaximum
          The attribute defines the minimum and the maximum value of the property.
    \value AvailableValues
          The attribute defines a list of values that are available for the property.
*/

/*!
    \fn template <class T> QIviPropertyAttribute<T>::QIviPropertyAttribute()
    Constructs an invalid QIviPropertyAttribute.

    This constructor can be used to indicate that the property is not available.
    For example, because the backend doesn't support this property.
*/

/*!
    \fn template <class T> QIviPropertyAttribute<T>::QIviPropertyAttribute(bool available)
    Constructs an QIviPropertyAttribute of type NoAttributes.

    The argument \a available defines whether the property is supported by the backend.
*/

/*!
    \fn template <class T> QIviPropertyAttribute<T>::QIviPropertyAttribute(const T &minValue, const T &maxValue)
    Constructs an QIviPropertyAttribute of type MinimumMaximum.

    The arguments \a minValue and \a maxValue define the boundaries the value of the property.
*/

/*!
    \fn template <class T> QIviPropertyAttribute<T>::QIviPropertyAttribute(const QVector<T> &avValues)
    Constructs an QIviPropertyAttribute of type AvailableValues.

    The argument \a avValues defines the valid values for the property.
*/

/*!
    \fn template <class T> AttributeType QIviPropertyAttribute<T>::type() const

    Returns the type of the attribute.
*/

/*!
    \fn template <class T> bool QIviPropertyAttribute<T>::isAvailable() const

    Returns true if the property is available for this backend.
*/

/*!
    \fn template <class T> T QIviPropertyAttribute<T>::minimumValue() const

    Returns the minimum value of the property.
    If the attributeType is not MinimumMaximum, a default constructed value is returned.
*/

/*!
    \fn template <class T> T QIviPropertyAttribute<T>::maximumValue() const

    Returns the maximum value of the property.
    If the attributeType is not MinimumMaximum, a default constructed value is returned.
*/

/*!
    \fn template <class T> QList<T> QIviPropertyAttribute<T>::availableValues() const

    Returns a list of valid values for this property.
    If the attributeType is not AvailableValues an empty list is returned.
*/

/*!
    \fn template <class T> QString QIviPropertyAttribute<T>::toString() const

    Returns a QString with a string representation of the attribute.
*/

/*!
    \fn template <class T> bool QIviPropertyAttribute<T>::operator==(const QIviPropertyAttribute& other) const

    Returns \c true if this QIviPropertyAttribute is equal to the \a other QIviPropertyAttribute; otherwise returns \c false.

    All elements of QIviPropertyAttribute are used for the comparison.
*/

/*!
    \fn template <class T> bool QIviPropertyAttribute<T>::operator!=(const QIviPropertyAttribute& other) const

    Returns \c true if this QIviPropertyAttribute is NOT equal to the \a other QIviPropertyAttribute; otherwise returns \c false.

    All elements of QIviPropertyAttribute are used for the comparison.
*/

QT_END_NAMESPACE
