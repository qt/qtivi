/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "echozonedservice.h"

#include <QtIviRemoteObjectsHelper>
#include <QTimer>

#define SET_VALUE(m_VALUE, VALUE, CHANGED_SIGNAL) \
    if (m_zoneHash.value(zone).m_VALUE == VALUE) \
        return; \
 \
    m_zoneHash[zone].m_VALUE = VALUE; \
    emit CHANGED_SIGNAL(VALUE, zone); \


EchoZonedService::EchoZonedService(QObject *parent)
    : EchoZonedSimpleSource(parent)
    , m_testCombo(Contact(QStringLiteral("Antti"), 34, true, QVariant()), EchoModule::Friday)
    , m_testId(QStringLiteral("id123"))
{
    m_zoneHash.insert(QString(), ZoneObject());
    m_zoneHash.insert(QStringLiteral("FrontLeft"), ZoneObject());
}

QString EchoZonedService::lastMessage(const QString &zone)
{
    return m_zoneHash.value(zone).m_lastMessage;
}

int EchoZonedService::intValue(const QString &zone)
{
    return m_zoneHash.value(zone).m_intValue;
}

void EchoZonedService::setIntValue(int intValue, const QString &zone)
{
    SET_VALUE(m_intValue, intValue, intValueChanged);
}

QVariant EchoZonedService::varValue(const QString &zone)
{
    return m_zoneHash.value(zone).m_varValue;
}

void EchoZonedService::setVarValue(const QVariant &varValue, const QString &zone)
{
    SET_VALUE(m_varValue, varValue, varValueChanged);
}

QString EchoZonedService::stringValue(const QString &zone)
{
    return m_zoneHash.value(zone).m_stringValue;
}

void EchoZonedService::setStringValue(const QString &stringValue, const QString &zone)
{
    SET_VALUE(m_stringValue, stringValue, stringValueChanged);
}

QString EchoZonedService::unsupportedValue(const QString &zone)
{
    return m_zoneHash.value(zone).m_unsupportedValue;
}

void EchoZonedService::setUnsupportedValue(const QString &unsupportedValue, const QString &zone)
{
    SET_VALUE(m_unsupportedValue, unsupportedValue, unsupportedValueChanged);
}

bool EchoZonedService::zonedValue(const QString &zone)
{
    return m_zoneHash.value(zone).m_zonedValue;
}

void EchoZonedService::setZonedValue(bool zonedValue, const QString &zone)
{
    SET_VALUE(m_zonedValue, zonedValue, zonedValueChanged);
}

bool EchoZonedService::valueWithDefault(const QString &zone)
{
    return m_zoneHash.value(zone).m_valueWithDefault;
}

void EchoZonedService::setValueWithDefault(bool valueWithDefault, const QString &zone)
{
    SET_VALUE(m_valueWithDefault, valueWithDefault, valueWithDefaultChanged);
}

bool EchoZonedService::isEchoEnabled(const QString &zone)
{
    return m_zoneHash.value(zone).m_echoEnabled;
}

void EchoZonedService::setEchoEnabled(bool echoEnabled, const QString &zone)
{
    SET_VALUE(m_echoEnabled, echoEnabled, echoEnabledChanged);
}

EchoModule::AirflowDirections EchoZonedService::airflowDirection(const QString &zone)
{
    return m_zoneHash.value(zone).m_airflowDirection;
}

void EchoZonedService::setAirflowDirection(EchoModule::AirflowDirections airflowDirection, const QString &zone)
{
    SET_VALUE(m_airflowDirection, airflowDirection, airflowDirectionChanged);
}

int EchoZonedService::rangedValue(const QString &zone)
{
    return m_zoneHash.value(zone).m_rangedValue;
}

void EchoZonedService::setRangedValue(int rangedValue, const QString &zone)
{
    SET_VALUE(m_rangedValue, rangedValue, rangedValueChanged);
}

int EchoZonedService::rangedValueWithDefault(const QString &zone)
{
    return m_zoneHash.value(zone).m_rangedValueWithDefault;
}

void EchoZonedService::setRangedValueWithDefault(int rangedValueWithDefault, const QString &zone)
{
    SET_VALUE(m_rangedValueWithDefault, rangedValueWithDefault, rangedValueWithDefaultChanged);
}

Contact EchoZonedService::contact(const QString &zone)
{
    return m_zoneHash.value(zone).m_contact;
}

void EchoZonedService::setContact(const Contact &contact, const QString &zone)
{
    SET_VALUE(m_contact, contact, contactChanged);
}

Combo EchoZonedService::combo(const QString &zone)
{
    return m_zoneHash.value(zone).m_combo;
}

void EchoZonedService::setCombo(const Combo &combo, const QString &zone)
{
    SET_VALUE(m_combo, combo, comboChanged);
}

QVariantList EchoZonedService::intList(const QString &zone)
{
    return m_zoneHash.value(zone).m_intList;
}

void EchoZonedService::setIntList(const QVariantList &intList, const QString &zone)
{
    SET_VALUE(m_intList, intList, intListChanged);
}

QVariantList EchoZonedService::comboList(const QString &zone)
{
    return m_zoneHash.value(zone).m_comboList;
}

void EchoZonedService::setComboList(const QVariantList &comboList, const QString &zone)
{
    SET_VALUE(m_comboList, comboList, comboListChanged);
}

qreal EchoZonedService::UPPERCASEPROPERTY(const QString &zone)
{
    return m_zoneHash.value(zone).m_UPPERCASEPROPERTY;
}

void EchoZonedService::setUPPERCASEPROPERTY(qreal UPPERCASEPROPERTY, const QString &zone)
{
    SET_VALUE(m_UPPERCASEPROPERTY, UPPERCASEPROPERTY, UPPERCASEPROPERTYChanged);
}

QStringList EchoZonedService::availableZones()
{
    auto keys = m_zoneHash.keys();
    keys.removeAll(QString());
    return keys;
}

QVariant EchoZonedService::echo(const QString &msg, const QString &zone)
{
    emit echoSlotCalled(msg, zone);
    return msg;
}

QVariant EchoZonedService::id(const QString &zone)
{
    emit idSlotCalled(zone);
    return m_testId;
}

QVariant EchoZonedService::varMethod(const QString &zone)
{
    emit varMethodSlotCalled(zone);
    return QVariant("FOOOO");
}

QVariant EchoZonedService::getCombo(const QString &zone)
{
    emit getComboSlotCalled(zone);
    return QVariant::fromValue(m_testCombo);
}

QVariant EchoZonedService::timer(int interval, const QString &zone)
{
    static quint64 counter = 0;
    QIviRemoteObjectPendingResult pendingResult(counter++, false);
    QTimer::singleShot(interval, this, [this, pendingResult, zone](){
        emit pendingResultAvailable(pendingResult.id(), true, zone);
    });
    return QVariant::fromValue(pendingResult);
}
