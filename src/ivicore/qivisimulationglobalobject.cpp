/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Copyright (C) 2019 The Qt Company Ltd.
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

#include "qivisimulationglobalobject_p.h"
#include <QtDebug>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE

namespace qtivi_helper {
    static const QString unsupportedLiteral = QStringLiteral("unsupported");
    static const QString minLiteral = QStringLiteral("minimum");
    static const QString maxLiteral = QStringLiteral("maximum");
    static const QString rangeLiteral = QStringLiteral("range");
    static const QString domainLiteral = QStringLiteral("domain");
}

using namespace qtivi_helper;

/*!
    \qmltype IviSimulator

    \brief The global object for parsing simulation data inside a QIviSimulationEngine.

    The IviSimulator global object provides access to the simulation data of a QIviSimulationEngine
    and provides helper functions for parsing and checking boundaries.

    \note This object is only available inside a QIviSimulationEngine and cannot be accessed
    outside of it.

    \section1 Data Format
    \target IviSimulatorDataFormat

    The IviSimulator expects its data already in a parsed form. Usually this is done by the
    QIviSimulationEngine::loadSimulationData() function, which expects the file to be in the JSON
    format.

    \section2 Interfaces

    Because a simulation QML file can use multiple interfaces, the simulation data supports
    multiple data sets as well. These are identified by the interface name in a reverse DNS
    notation:

    \badcode
    {
      "QIviClimateControl": {
        ...
      },
      "org.qt-project.QIviWindowControl": {
        ...
      }
    }
    \endcode

    The findData() method can be used to find the data for a specific interface.

    \section2 Properties

    The settings (e.g. boundaries) for every property of the interface are defined inside a data
    set.

    \badcode
    {
      "QIviClimateControl": {
        "airConditioningEnabled": {
          "default": true
        },
        "steeringWheelHeater": {
          "minimum": 0,
          "default": 0
        },
        "fanSpeed": {
          "range": [0, 5]
        }
      }
    }
    \endcode

    For the interface named \c QIviClimateControl, there are settings defined for the properties
    \c airConditioningEnabled, \c steeringWheelHeater and \c fanSpeed.

    The settings object can store multiple constraints which are called \e domains. The following
    domains are currently supported:

    \list
        \value default Holds the default value the property should have when the frontend is
               connected.
        \value minimum Every newly set value needs to be bigger than this value
        \value maximum Every newly set value needs to be smaller than this value
        \value range Every newly set value needs to be between the two values in this list
        \note The values in \c range always override the \c minimum and \c maximum domains
        \value domain Every newly set value needs to be part of this list
        \value unsupported Changing the property is not possible and will show an "unsupported"
               error message
    \endlist

    The value for a specific domain can be loaded using the parseDomainValue() function or using
    defaultValue() when only the \e default domain is of interest.

    \section2 Structures and Enums

    As JSON is not typesafe, structures and enums need to be stored in a special format. An enum
    can be stored like this:

    \badcode
    {
      "QIviClimateControl": {
        "recirculationMode": {
          "default": {
            "type": "enum",
            "value": "QtIviVehicleFunctionsModule::RecirculationOff"
          }
        }
      }
    }
    \endcode

    Structures can be stored in a similar fashion using the name of the structure as \e type:

    \badcode
    {
      "AddressBook": {
        "contactList": {
          "default": [
            {
              "type": "Contact",
              "value": [
                "foo",
                23,
                true
              ]
            },
            {
              "type": "Contact",
              "value": [
                "bar",
                12,
                false
              ]
            }
          ]
        }
      },
      }
    }
    \endcode

    To correctly initialize the structure with these values, the structure needs to provide a
    constructor taking a QVariant as argument. For the given \e contact example this constructor
    can look like this:

    \code
    Contact::Contact(const QVariant &variant)
        : Contact()
    {
        QVariant value = qtivi_convertFromJSON(variant);
        // First try to convert the values to a Map or a List
        // This is needed as it could also store a QStringList or a Hash
        if (value.canConvert(QVariant::Map))
            value.convert(QVariant::Map);
        if (value.canConvert(QVariant::List))
            value.convert(QVariant::List);

        if (value.type() == QVariant::Map) {
            QVariantMap map = value.toMap();
            if (map.contains(QStringLiteral("name")))
                d->m_name = map.value(QStringLiteral("name")).value<QString>();
            if (map.contains(QStringLiteral("age")))
                d->m_age = map.value(QStringLiteral("age")).value<int>();
            if (map.contains(QStringLiteral("isMarried")))
                d->m_isMarried = map.value(QStringLiteral("isMarried")).value<bool>();
        } else if (value.type() == QVariant::List) {
            QVariantList values = value.toList();
            d->m_name = values.value(0).value<QString>();
            d->m_age = values.value(1).value<int>();
            d->m_isMarried = values.value(2).value<bool>();
        }
    }
    \endcode

    \section2 Zones

    For zoned interfaces, the supported zones are usually stored as list in the \e zones property:

    \badcode
    {
      "QIviClimateControl": {
        "zones": [
          "FrontLeft",
          "FrontRight",
          "Rear"
        ]
      }
    }
    \endcode

    For every supported domain it is also possible to provide zone specific values e.g.:

    \badcode
    {
      "QIviClimateControl": {
        "targetTemperature": {
          "maximum": 30.0,
          "default": {
            "FrontLeft": 21.0,
            "FrontRight": 22.5,
            "=": 0.0
          }
        }
      }
    }
    \endcode

    This defines that the maximum value for the \e targetTemperature property is \e 30, the default
    value is zone specific and is \e 21.5 for the \e FrontLeft zone, while it is \e 22.5 for the
    FrontRight zone. The unzoned \e targetTemperature temperature is initialized with \e 0.0.
*/
QIviSimulationGlobalObject::QIviSimulationGlobalObject(QObject *parent)
    : QObject(parent)
{
}

/*!
    \qmlproperty object IviSimulator::simulationData

    Provides the simulation data parsed in QIviSimulationEngine::loadSimulationData()
*/
QVariant QIviSimulationGlobalObject::simulationData() const
{
    return m_simulationData;
}

void QIviSimulationGlobalObject::setSimulationData(const QVariant &simulationData)
{
    m_simulationData = simulationData;
}

/*!
    \qmlmethod IviSimulator::findData(object data, string interface)

    Searches for the key \a interface within \a data and returns the stored values. Returns
    undefined if no data was found for this \a interface.

    If \a interface is a reverse NDS name, it first searches for the full string. If no key was
    found, it starts to search again with a reduced name until it finds a key with this name.

    E.g. for the interface \e org.qt-project.ClimateControl it searches for keys in the following
    order:

    \list 1
        \li org.qt-project.ClimateControl
        \li qt-project.ClimateControl
        \li ClimateControl
    \endlist
*/
QVariantMap QIviSimulationGlobalObject::findData(const QVariantMap &data, const QString &interface)
{
    QString key = interface;
    forever {
        if (data.contains(key))
            return data.value(key).toMap();

        int index = key.indexOf(QLatin1Char('.'));
        if (index == -1)
            break;
        key = key.right(key.count() - index - 1);
    }

    return QVariantMap();
}

/*!
    \qmlmethod IviSimulator::initializeDefault(object data, QObject* object)

    Applies the default values read from \a data to \a object.

    If \a object supports zoneing, the default value is only applied to the correct zone.
*/
void QIviSimulationGlobalObject::initializeDefault(const QVariantMap &data, QObject *object)
{
    for (auto i = data.constBegin(); i != data.constEnd(); ++i) {
        const QVariant defVal = defaultValue(i.value().toMap());
        if (defVal.isValid()) {
            QVariant currentValue = object->property(i.key().toLatin1());
            if (QIviPagingModelInterface *model = currentValue.value<QIviPagingModelInterface*>()) {
                QVariantList list = defVal.toList();
                for (auto i = list.crbegin(); i != list.crend(); ++i)
                    QMetaObject::invokeMethod(model, "insert", Q_ARG(int, 0), createArgument(*i));
            } else {
                object->setProperty(i.key().toLatin1(), defVal);
            }
        }

        QVariant currentValue = object->property("zones");
        QQmlPropertyMap *map = currentValue.value<QQmlPropertyMap*>();
        if (!map)
            continue;
        const QStringList zones = data.value(QStringLiteral("zones")).toStringList();
        for (const QString &zone : zones) {
            const QVariant defVal = defaultValue(i.value().toMap(), zone);
            if (defVal.isValid()) {
                QObject *zoneObject = map->value(zone).value<QObject*>();
                if (zoneObject)
                    zoneObject->setProperty(i.key().toLatin1(), defVal);
            }
        }
    }
}

/*!
    \qmlmethod IviSimulator::defaultValue(object data, string zone)

    Provides the default value stored in \a data for the given \a zone. If \a zone is undefined or
    the data doesn't provide a default value for the given \a zone, it returns the unzoned default
    value if available.

    This is just a convenience function calling parseDomainValue() with the domain \e default.
*/
QVariant QIviSimulationGlobalObject::defaultValue(const QVariantMap &data, const QString &zone)
{
    return parseDomainValue(data, QStringLiteral("default"), zone);
}

/*!
    \qmlmethod IviSimulator::constraint(object data, string zone)

    Searches for all boundary settings in \a data for the given \a zone and returns the constraint
    (which is enforced for newly set values) in a human readable form.

    This is useful for error messages in connection with checkSettings().

    \sa checkSettings()
*/
QString QIviSimulationGlobalObject::constraint(const QVariantMap &data, const QString &zone)
{
    const QVariant unsupportedDomain = parseDomainValue(data, unsupportedLiteral, zone);
    QVariant minDomain = parseDomainValue(data, minLiteral, zone);
    QVariant maxDomain = parseDomainValue(data, maxLiteral, zone);
    const QVariant rangeDomain = parseDomainValue(data, rangeLiteral, zone);
    if (rangeDomain.isValid()) {
        QVariantList range = rangeDomain.toList();
        if (range.count() == 2) {
            minDomain = range.at(0);
            maxDomain = range.at(1);
        } else {
            qWarning("Domain 'range' needs to be list of exactly two values");
        }
    }
    const QVariant domainDomain = parseDomainValue(data, domainLiteral, zone);

    if (unsupportedDomain.isValid())
        return unsupportedLiteral;
    if (minDomain.isValid() && maxDomain.isValid())
        return QStringLiteral("[") + minDomain.toString() + QStringLiteral("-") + maxDomain.toString() + QStringLiteral("]") ;
    if (minDomain.isValid())
        return QStringLiteral(">= ") + minDomain.toString();
    if (maxDomain.isValid())
        return QStringLiteral("<= ") + maxDomain.toString();
    if (domainDomain.isValid())
        return QString::fromUtf8(QJsonDocument::fromVariant(domainDomain).toJson(QJsonDocument::Compact));

    return QString();
}

/*!
    \qmlmethod IviSimulator::checkSettings(object data, var value, string zone)

    Searches for all boundary settings in \a data for the given \a zone and returns whether the
    provided \a value meets this constraint.

    To show meaningful error messages when the value is not within the boundaries, the constraint()
    function can be used.

    \sa constraint()
*/
bool QIviSimulationGlobalObject::checkSettings(const QVariantMap &data, const QVariant &value, const QString &zone)
{
    const QVariant unsupportedDomain = parseDomainValue(data, unsupportedLiteral, zone);
    QVariant minDomain = parseDomainValue(data, minLiteral, zone);
    QVariant maxDomain = parseDomainValue(data, maxLiteral, zone);
    const QVariant rangeDomain = parseDomainValue(data, rangeLiteral, zone);
    if (rangeDomain.isValid()) {
        QVariantList range = rangeDomain.toList();
        if (range.count() == 2) {
            minDomain = range.at(0);
            maxDomain = range.at(1);
        } else {
            qWarning("Domain 'range' needs to be list of exactly two values");
        }
    }
    const QVariant domainDomain = parseDomainValue(data, domainLiteral, zone);
    bool valueToDouble = value.canConvert(QMetaType::fromType<double>());
    bool minDomainToDouble = minDomain.canConvert(QMetaType::fromType<double>());
    bool maxDomainToDouble = maxDomain.canConvert(QMetaType::fromType<double>());

    if (unsupportedDomain.isValid()) {
        return !unsupportedDomain.toBool();
    } else if (minDomain.isValid() && maxDomain.isValid()) {
        if (!valueToDouble || !minDomainToDouble || !maxDomainToDouble) {
            qWarning() << "Can't compare values: " << value << minDomain << maxDomain;
            return false;
        }
        return !(value.toDouble() < minDomain.toDouble() || value.toDouble() > maxDomain.toDouble());
    } else if (minDomain.isValid()) {
        if (!valueToDouble || !minDomainToDouble) {
            qWarning() << "Can't compare values: " << value << minDomain;
            return false;
        }
        return value.toDouble() >= minDomain.toDouble();
    } else if (maxDomain.isValid()) {
        if (!valueToDouble || !maxDomainToDouble) {
            qWarning() << "Can't compare values: " << value << maxDomain;
            return false;
        }
        return value.toDouble() <= maxDomain.toDouble();
    } if (domainDomain.isValid()) {
        return domainDomain.toList().contains(value);
    }

    return true;
}

/*!
    \qmlmethod IviSimulator::parseDomainValue(object data, string domain, string zone)

    Search for the \a domain in \a data for the given \a zone. If \a zone is undefined or
    the data doesn't provide this domain for the given \a zone, it returns the unzoned domain
    value if available.
*/
QVariant QIviSimulationGlobalObject::parseDomainValue(const QVariantMap &data, const QString &domain, const QString &zone)
{
    if (!data.contains(domain))
        return QVariant();

    const QVariant domainData = data.value(domain);
    if (domainData.metaType().id() == QMetaType::QVariantMap) {
        const QVariantMap domainMap = domainData.toMap();
        QString z = zone;
        if (zone.isEmpty())
            z = QStringLiteral("=");

        if (domainMap.contains(z))
            return qtivi_convertFromJSON(domainMap.value(z));
    }

    return qtivi_convertFromJSON(domainData);
}

QGenericArgument QIviSimulationGlobalObject::createArgument(const QVariant &variant)
{
    return QGenericArgument(variant.typeName(), variant.data());
}

QT_END_NAMESPACE
