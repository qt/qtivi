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

#ifndef QTIVIPROPERTYATTRIBUTE_H
#define QTIVIPROPERTYATTRIBUTE_H

#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtCore/QtDebug>
#include <QtIVICore/qtiviglobal.h>

QT_BEGIN_NAMESPACE

//TODO Do we really need to document the base class and move into it's own header ?
class Q_QTIVICORE_EXPORT QtIVIPropertyAttributeBase
{
public:
    enum AttributeType {
        Invalid,
        NoAttributes,
        MinimumMaximum,
        AvailableValues
    };
};

template <class T>
class QtIVIPropertyAttribute : public QtIVIPropertyAttributeBase
{
public:
    //TODO Do we want to keep this and the Invalid type or should NoAttribute be the default type
    QtIVIPropertyAttribute()
        : m_available(false)
        , m_type(QtIVIPropertyAttributeBase::Invalid)
        , m_minimumValue(T())
        , m_maximumValue(T())
    {
        registerConverter();
    }

    QtIVIPropertyAttribute(bool available)
        : m_available(available)
        , m_type(QtIVIPropertyAttributeBase::NoAttributes)
        , m_minimumValue(T())
        , m_maximumValue(T())
    {
        registerConverter();
    }

    QtIVIPropertyAttribute(const T &minValue, const T &maxValue)
        : m_available(true)
        , m_type(QtIVIPropertyAttributeBase::MinimumMaximum)
        , m_minimumValue(minValue)
        , m_maximumValue(maxValue)
    {
        registerConverter();
    }

    QtIVIPropertyAttribute(const QVector<T> &avValues)
        : m_available(true)
        , m_type(QtIVIPropertyAttributeBase::AvailableValues)
        , m_minimumValue(T())
        , m_maximumValue(T())
        , m_availableValues(avValues)
    {
        registerConverter();
    }

    QtIVIPropertyAttributeBase::AttributeType type() const { return m_type; }
    bool isAvailable() const  { return m_available; }
    T minimumValue() const { return m_minimumValue; }
    T maximumValue() const { return m_maximumValue; }
    QVector<T> availableValues() const { return m_availableValues; }

    bool operator==(const QtIVIPropertyAttribute &other) const
    {
        return (m_available == other.m_available) &&
               (m_type == other.m_type) &&
               (m_minimumValue == other.m_minimumValue) &&
               (m_maximumValue == other.m_maximumValue) &&
               (m_availableValues == other.m_availableValues);
    }

    bool operator!=(const QtIVIPropertyAttribute &other) const
    {
        return !operator==(other);
    }

    bool operator<(const QtIVIPropertyAttribute &other) const
    {
        Q_UNUSED(other)
        return false;
    }

    QString toString() const
    {
        QString string;
        QDebug(&string) << *this;
        return string;
    }

    //TODO Pimpl needed ?
private:

    //The converters are needed to get meaningfull output if QCOMPARE failed to compare to variants containing a QtIVIPropertyAttribute
    void registerConverter()
    {
        static bool enabled = QMetaType::registerConverter<QtIVIPropertyAttribute<T>, QString>(&QtIVIPropertyAttribute<T>::toString);
        static bool compEnabled = QMetaType::registerComparators<QtIVIPropertyAttribute<T>>();
        Q_UNUSED(enabled)
        Q_UNUSED(compEnabled)
    }

    bool m_available;
    QtIVIPropertyAttributeBase::AttributeType m_type;
    T m_minimumValue;
    T m_maximumValue;
    QVector<T> m_availableValues;
};


// Not all types used in QtIVIPropertyAttribute have a debug stream operator implemented.
// This functions checks whether it's implemented or not
namespace qt_has_debug_operator_impl {
  typedef char no;
  typedef char yes[2];

  struct any_t {
    template<typename T> any_t( T const& );
  };

  no operator<<( QDebug, any_t const& );

  yes &test( QDebug );
  no test( no );

  template<typename T>
  struct has_debug_operator {
    static QDebug s;
    static T const &t;
    static bool const value = sizeof( test(s << t) ) == sizeof( yes );
  };
}

template<typename T>
struct qt_has_debug_operator :
  qt_has_debug_operator_impl::has_debug_operator<T> {
};

//If no debug operator exists for type T
template <typename T> Q_INLINE_TEMPLATE typename QtPrivate::QEnableIf<!qt_has_debug_operator<T>::value, QDebug>::Type
operator<<(QDebug debug, const QtIVIPropertyAttribute<T> &ptr)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "QtIVIPropertyAttribute(" << ptr.type() << ", " << ptr.isAvailable() << ", rest of the values not representable as string)";
    return debug;
}

//If a debug operator exists for type T
template <typename T> Q_INLINE_TEMPLATE typename QtPrivate::QEnableIf<qt_has_debug_operator<T>::value, QDebug>::Type
operator<<(QDebug debug, const QtIVIPropertyAttribute<T> &ptr)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "QtIVIPropertyAttribute(" << ptr.type() << ", " << ptr.isAvailable() << ", " << ptr.minimumValue() << ", " << ptr.maximumValue() << ", " << ptr.availableValues() << ")";
    return debug;
}

QT_END_NAMESPACE

#endif // QTIVIPROPERTYATTRIBUTE_H
