/****************************************************************************
**
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

#include "qivisimulationglobalobject_p.h"
#include <QtDebug>

QT_BEGIN_NAMESPACE

namespace qtivi_helper {
    static const QString unsupportedLiteral = QStringLiteral("unsupported");
    static const QString minLiteral = QStringLiteral("min");
    static const QString maxLiteral = QStringLiteral("max");
    static const QString domainLiteral = QStringLiteral("domain");
}

using namespace qtivi_helper;

QIviSimulationGlobalObject::QIviSimulationGlobalObject(QObject *parent)
    : QObject(parent)
{
}

QVariant QIviSimulationGlobalObject::simulationData() const
{
    return m_simulationData;
}

void QIviSimulationGlobalObject::setSimulationData(const QVariant &simulationData)
{
    m_simulationData = simulationData;
}

QVariantMap QIviSimulationGlobalObject::findData(const QVariantMap &data, const QString &interface)
{
    QString key = interface;
    forever {
        if (data.contains(key))
            return data.value(key).toMap();

        int index = key.indexOf('.');
        if (index == -1)
            break;
        key = key.right(key.count() - index);
    }

    return QVariantMap();
}

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

QVariant QIviSimulationGlobalObject::defaultValue(const QVariantMap &data, const QString &zone)
{
    return parseDomainValue(data, QStringLiteral("default"), zone);
}

QString QIviSimulationGlobalObject::constraint(const QVariantMap &data, const QString &zone)
{
    const QVariant unsupportedDomain = parseDomainValue(data, unsupportedLiteral, zone);
    const QVariant minDomain = parseDomainValue(data, minLiteral, zone);
    const QVariant maxDomain = parseDomainValue(data, maxLiteral, zone);
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
        return domainDomain.toString();

    return QString();
}

bool QIviSimulationGlobalObject::checkSettings(const QVariantMap &data, const QVariant &value, const QString &zone)
{
    const QVariant unsupportedDomain = parseDomainValue(data, unsupportedLiteral, zone);
    const QVariant minDomain = parseDomainValue(data, minLiteral, zone);
    const QVariant maxDomain = parseDomainValue(data, maxLiteral, zone);
    const QVariant domainDomain = parseDomainValue(data, domainLiteral, zone);

    if (unsupportedDomain.isValid())
        return unsupportedDomain.toBool();
    if (minDomain.isValid() && maxDomain.isValid())
        return !(value < minDomain || value > maxDomain);
    if (minDomain.isValid())
        return value >= minDomain;
    if (maxDomain.isValid())
        return value <= maxDomain;
    if (domainDomain.isValid())
        return domainDomain.toList().contains(value);

    return true;
}

QVariant QIviSimulationGlobalObject::parseDomainValue(const QVariantMap &data, const QString &domain, const QString &zone)
{
    if (!data.contains(domain))
        return QVariant();

    const QVariant domainData = data.value(domain);
    if (domainData.type() == QVariant::Map) {
        const QVariantMap domainMap = domainData.toMap();
        QString z = zone;
        if (zone.isEmpty())
            z = QStringLiteral("=");

        if (domainMap.contains(zone))
            return convertFromJSON(domainMap.value(zone));
    }

    return convertFromJSON(domainData);
}

QGenericArgument QIviSimulationGlobalObject::createArgument(const QVariant &variant)
{
    return QGenericArgument(variant.typeName(), variant.data());
}

QT_END_NAMESPACE
