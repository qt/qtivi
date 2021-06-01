/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
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

#ifndef ECHOZONEDSERVICE_H
#define ECHOZONEDSERVICE_H

#include "rep_echozoned_source.h"

class EchoZonedService : public EchoZonedSimpleSource
{
    Q_OBJECT
public:
    explicit EchoZonedService(QObject *parent = nullptr);

public slots:
    QString lastMessage(const QString &zone) override;
    int intValue(const QString &zone) override;
    void setIntValue(int intValue, const QString &zone) override;
    QVariant varValue(const QString &zone) override;
    void setVarValue(const QVariant &varValue, const QString &zone) override;
    QString stringValue(const QString &zone) override;
    void setStringValue(const QString &stringValue, const QString &zone) override;
    QString unsupportedValue(const QString &zone) override;
    void setUnsupportedValue(const QString &unsupportedValue, const QString &zone) override;
    bool zonedValue(const QString &zone) override;
    void setZonedValue(bool zonedValue, const QString &zone) override;
    bool valueWithDefault(const QString &zone) override;
    void setValueWithDefault(bool valueWithDefault, const QString &zone) override;
    bool isEchoEnabled(const QString &zone) override;
    void setEchoEnabled(bool echoEnabled, const QString &zone) override;
    Echomodule::AirflowDirections airflowDirection(const QString &zone) override;
    void setAirflowDirection(Echomodule::AirflowDirections airflowDirection, const QString &zone) override;
    int rangedValue(const QString &zone) override;
    void setRangedValue(int rangedValue, const QString &zone) override;
    int rangedValueWithDefault(const QString &zone) override;
    void setRangedValueWithDefault(int rangedValueWithDefault, const QString &zone) override;
    Contact contact(const QString &zone) override;
    void setContact(const Contact &contact, const QString &zone) override;
    Combo combo(const QString &zone) override;
    void setCombo(const Combo &combo, const QString &zone) override;
    QVariantList intList(const QString &zone) override;
    void setIntList(const QVariantList &intList, const QString &zone) override;
    QVariantList comboList(const QString &zone) override;
    void setComboList(const QVariantList &comboList, const QString &zone) override;
    Echomodule::DaysOfTheWeek weekDay(const QString &zone) override;
    void setWeekDay(Echomodule::DaysOfTheWeek weekDay, const QString &zone) override;
    Echomodule::TestEnum testEnum(const QString &zone) override;
    void setTestEnum(Echomodule::TestEnum testEnum, const QString &zone) override;
    qreal UPPERCASEPROPERTY(const QString &zone) override;
    void setUPPERCASEPROPERTY(qreal UPPERCASEPROPERTY, const QString &zone) override;
    QStringList availableZones() override;
    QVariant echo(const QString &msg, const QString &zone) override;
    QVariant id(const QString &zone) override;
    QVariant varMethod(const QString &zone) override;
    QVariant getCombo(const QString &zone) override;
    QVariant timer(int interval, const QString &zone) override;
    QVariant flagMethod(Echomodule::AirflowDirections direction, const QString &zone) override;
    QVariant enumMethod(Echomodule::TestEnum testEnum, const QString &zone) override;

Q_SIGNALS:
    void echoSlotCalled(const QString &msg, const QString& zone);
    void idSlotCalled(const QString& zone);
    void varMethodSlotCalled(const QString& zone);
    void getComboSlotCalled(const QString& zone);
    void voidSlotCalled(const QString& zone);
    void voidSlot2Called(int param, const QString& zone);
    void flagMethodCalled(Echomodule::AirflowDirections direction, const QString& zone);
    void enumMethodCalled(Echomodule::TestEnum testEnum, const QString& zone);

private:
    struct ZoneObject {
        ZoneObject()
            : m_lastMessage(QString())
            , m_intValue(int(0))
            , m_varValue(QVariant())
            , m_stringValue(QString())
            , m_unsupportedValue(QString())
            , m_zonedValue(bool(false))
            , m_valueWithDefault(bool(false))
            , m_echoEnabled(bool(false))
            , m_airflowDirection(Echomodule::AirflowDirections())
            , m_rangedValue(int(0))
            , m_rangedValueWithDefault(int(0))
            , m_contact(Contact())
            , m_combo(Combo())
            , m_intList(QVariantList())
            , m_comboList(QVariantList())
            , m_weekDay(0)
            , m_testEnum(Echomodule::FirstEnumValue)
            , m_UPPERCASEPROPERTY(qreal(0.0))
        {}

        QString m_zone;
        QString m_lastMessage;
        int m_intValue;
        QVariant m_varValue;
        QString m_stringValue;
        QString m_unsupportedValue;
        bool m_zonedValue;
        bool m_valueWithDefault;
        bool m_echoEnabled;
        Echomodule::AirflowDirections m_airflowDirection;
        int m_rangedValue;
        int m_rangedValueWithDefault;
        Contact m_contact;
        Combo m_combo;
        QVariantList m_intList;
        QVariantList m_comboList;
        Echomodule::DaysOfTheWeek m_weekDay;
        Echomodule::TestEnum m_testEnum;
        qreal m_UPPERCASEPROPERTY;
        QStringList m_propertiesToSync;
    };

    QHash <QString, ZoneObject> m_zoneHash;
    Combo m_testCombo;
    QString m_testId;
};

#endif // ECHOZONEDSERVICE_H
