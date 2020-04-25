/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

#include "tst_echoqtro.h"
#include "server.h"
#include "echo.h"
#include "echozoned.h"

#define WAIT_AND_COMPARE(spy, value) \
if (spy.count() == !value) \
    spy.wait(); \
QCOMPARE(spy.count(), value); \


static QString frontLeftZone = QStringLiteral("FrontLeft");

EchoQtroTest::EchoQtroTest()
    :QObject()
{
    EchoModule::registerTypes();
}

void EchoQtroTest::cleanup()
{
}

void EchoQtroTest::testInit()
{
    Echo client;
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    QCOMPARE(client.lastMessage(), QString());
    QCOMPARE(client.intValue(), 0);
    QCOMPARE(client.floatValue1(), qreal(0.0));
    QCOMPARE(client.floatValue2(), qreal(0.0));
    QCOMPARE(client.stringValue(), QString());
    QCOMPARE(client.comboList(), QVariantList());
    QCOMPARE(client.contact(), Contact());
    QCOMPARE(client.weekDay(), EchoModule::WeekDay());
    QCOMPARE(client.testEnum(), EchoModule::FirstEnumValue);

    Server server;

    //test initial values
    QCOMPARE(server.m_echoService.lastMessage(), QString());
    QCOMPARE(server.m_echoService.intValue(), 0);
    QCOMPARE(server.m_echoService.floatValue1(), qreal(0.0));
    QCOMPARE(server.m_echoService.floatValue2(), qreal(0.0));
    QCOMPARE(server.m_echoService.stringValue(), QString());
    QCOMPARE(server.m_echoService.comboList(), QVariantList());
    QCOMPARE(server.m_echoService.contact(), Contact());
    QCOMPARE(server.m_echoService.weekDay(), EchoModule::WeekDay());
    QCOMPARE(server.m_echoService.testEnum(), EchoModule::FirstEnumValue);

    QLatin1String lastMessageTestValue("this is the last message");
    int intValueTestValue(789);
    qreal floatValue1TestValue(3.14);
    qreal floatValue2TestValue(2.71);
    QLatin1String stringValueTestValue("test string");
    QVariantList comboListTestValue(
                    { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), EchoModule::Monday)),
                      QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), EchoModule::Wednesday)) });
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    EchoModule::WeekDay weekDayTestValue = EchoModule::Wednesday;
    EchoModule::TestEnum testEnumTestValue = EchoModule::SecondEnumValue;

    server.m_echoService.setLastMessage(lastMessageTestValue);
    server.m_echoService.setIntValue(intValueTestValue);
    server.m_echoService.setFloatValue1(floatValue1TestValue);
    server.m_echoService.setFloatValue2(floatValue2TestValue);
    server.m_echoService.setStringValue(stringValueTestValue);
    server.m_echoService.setComboList(comboListTestValue);
    server.m_echoService.setContact(contactTestValue);
    server.m_echoService.setWeekDay(weekDayTestValue);
    server.m_echoService.setTestEnum(testEnumTestValue);

    QVERIFY(!client.isInitialized());
    QCOMPARE(client.error(), QIviAbstractFeature::NoError);

    //wait until the client has connected and initial values are set
    QSignalSpy lastMessageChangedSpy(&client, SIGNAL(lastMessageChanged(QString)));
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));

    QVERIFY(server.start());

    QVERIFY(initSpy.isValid());
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //make sure the change signal is only emitted once with the correct value
    QCOMPARE(lastMessageChangedSpy.count(), 1);
    QCOMPARE(lastMessageChangedSpy.at(0).at(0).toString(), lastMessageTestValue);

    //test that client gets the same values that were set at the server before connection was established
    QCOMPARE(client.lastMessage(), lastMessageTestValue);
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(client.floatValue1(), floatValue1TestValue);
    QCOMPARE(client.floatValue2(), floatValue2TestValue);
    QCOMPARE(client.stringValue(), stringValueTestValue);
    QVariantList comboList = client.comboList();
    QCOMPARE(comboList[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(comboList[1].value<Combo>(), comboListTestValue[1].value<Combo>());
    QCOMPARE(client.contact(), contactTestValue);
    QCOMPARE(client.weekDay(), weekDayTestValue);
    QCOMPARE(client.testEnum(), testEnumTestValue);


    lastMessageChangedSpy.clear();
    //test that a second instance is also initialized with the correct values
    Echo client2;

    QSignalSpy lastMessageChangedSpy2(&client2, SIGNAL(lastMessageChanged(QString)));
    QSignalSpy initSpy2(&client2, SIGNAL(isInitializedChanged(bool)));

    QVERIFY(client2.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    QVERIFY(initSpy2.isValid());
    WAIT_AND_COMPARE(initSpy2, 1);
    QVERIFY(client2.isInitialized());

    //make sure the change signal is only emitted once with the correct value
    QCOMPARE(lastMessageChangedSpy2.count(), 1);
    QCOMPARE(lastMessageChangedSpy2.at(0).at(0).toString(), lastMessageTestValue);

    //make sure the first instance doesn't emit the change signal again
    QCOMPARE(lastMessageChangedSpy.count(), 0);
}

void EchoQtroTest::testZonedInit()
{
    EchoZoned client;
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //Just compare a few of them
    QCOMPARE(client.intValue(), 0);
    QCOMPARE(client.varValue(), QVariant());
    QCOMPARE(client.stringValue(), QString());
    QCOMPARE(client.airflowDirection(), EchoModule::AirflowDirections());
    QCOMPARE(client.contact(), Contact());
    QCOMPARE(client.comboList(), QVariantList());
    QCOMPARE(client.UPPERCASEPROPERTY(), qreal(0.0));
    QCOMPARE(client.testEnum(), EchoModule::FirstEnumValue);
    QCOMPARE(client.availableZones(), QStringList());

    Server server;

    //test initial values
    QCOMPARE(server.m_echoZonedService.intValue(QString()), 0);
    QCOMPARE(server.m_echoZonedService.varValue(QString()), QVariant());
    QCOMPARE(server.m_echoZonedService.stringValue(QString()), QString());
    QCOMPARE(server.m_echoZonedService.airflowDirection(QString()), EchoModule::AirflowDirections());
    QCOMPARE(server.m_echoZonedService.contact(QString()), Contact());
    QCOMPARE(server.m_echoZonedService.comboList(QString()), QVariantList());
    QCOMPARE(server.m_echoZonedService.UPPERCASEPROPERTY(QString()), qreal(0.0));
    QCOMPARE(server.m_echoZonedService.testEnum(QString()), EchoModule::FirstEnumValue);

    int intValueTestValue(789);
    QVariant varValueTestValue(789);
    qreal floatValueTestValue(3.14);
    QLatin1String stringValueTestValue("test string");
    QVariantList comboListTestValue(
                    { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), EchoModule::Monday)),
                      QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), EchoModule::Wednesday)) });
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    EchoModule::AirflowDirections airflowTestValue = EchoModule::Windshield;
    EchoModule::TestEnum testEnumTestValue = EchoModule::SecondEnumValue;


    server.m_echoZonedService.setIntValue(intValueTestValue, QString());
    server.m_echoZonedService.setVarValue(varValueTestValue, QString());
    server.m_echoZonedService.setStringValue(stringValueTestValue, QString());

    server.m_echoZonedService.setAirflowDirection(airflowTestValue, frontLeftZone);
    server.m_echoZonedService.setContact(contactTestValue, frontLeftZone);
    server.m_echoZonedService.setComboList(comboListTestValue, frontLeftZone);
    server.m_echoZonedService.setUPPERCASEPROPERTY(floatValueTestValue, frontLeftZone);
    server.m_echoZonedService.setTestEnum(testEnumTestValue, frontLeftZone);


    QVERIFY(!client.isInitialized());
    QCOMPARE(client.error(), QIviAbstractFeature::NoError);

    //wait until the client has connected and initial values are set
    QSignalSpy stringValueChangedSpy(&client, SIGNAL(stringValueChanged(QString)));
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QSignalSpy availableZonesSpy(&client, SIGNAL(availableZonesChanged(QStringList)));

    QVERIFY(server.start());

    QVERIFY(initSpy.isValid());
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());
    QVERIFY(availableZonesSpy.count());
    QCOMPARE(client.availableZones(), QStringList({"FrontLeft"}));

    //make sure the change signal is only emitted once with the correct value
    QCOMPARE(stringValueChangedSpy.count(), 1);
    QCOMPARE(stringValueChangedSpy.at(0).at(0).toString(), stringValueTestValue);

    //test that client gets the same values that were set at the server before connection was established
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(client.varValue(), varValueTestValue);
    QCOMPARE(client.stringValue(), stringValueTestValue);

    EchoZoned *zone = qobject_cast<EchoZoned*>(client.zoneAt(frontLeftZone));
    QVERIFY(zone);
    QCOMPARE(zone->airflowDirection(), airflowTestValue);
    QCOMPARE(zone->contact(), contactTestValue);
    QVariantList comboList = zone->comboList();
    QCOMPARE(comboList[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(comboList[1].value<Combo>(), comboListTestValue[1].value<Combo>());
    QCOMPARE(zone->UPPERCASEPROPERTY(), floatValueTestValue);
    QCOMPARE(zone->testEnum(), testEnumTestValue);

    stringValueChangedSpy.clear();
    //test that a second instance is also initialized with the correct values
    EchoZoned client2;

    QSignalSpy stringValueChangedSpy2(&client2, SIGNAL(stringValueChanged(QString)));
    QSignalSpy initSpy2(&client2, SIGNAL(isInitializedChanged(bool)));

    QVERIFY(client2.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    QVERIFY(initSpy2.isValid());
    WAIT_AND_COMPARE(initSpy2, 1);
    QVERIFY(client2.isInitialized());

    //make sure the change signal is only emitted once with the correct value
    QCOMPARE(stringValueChangedSpy2.count(), 1);
    QCOMPARE(stringValueChangedSpy2.at(0).at(0).toString(), stringValueTestValue);

    //make sure the first instance doesn't emit the change signal again
    QCOMPARE(stringValueChangedSpy.count(), 0);
}

void EchoQtroTest::testReconnect()
{
    Server server;
    QVERIFY(server.start());

    Echo client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    EchoZoned zonedClient;
    QSignalSpy zonedInitSpy(&zonedClient, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(zonedInitSpy.isValid());
    QVERIFY(zonedClient.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(zonedInitSpy, 1);
    QVERIFY(zonedClient.isInitialized());

    //test disconnection
    QCOMPARE(client.error(), QIviAbstractFeature::NoError);
    QCOMPARE(zonedClient.error(), QIviAbstractFeature::NoError);
    QSignalSpy disconnectSpy(&client, SIGNAL(errorChanged(QIviAbstractFeature::Error, QString)));
    QSignalSpy zonedDisconnectSpy(&zonedClient, SIGNAL(errorChanged(QIviAbstractFeature::Error, QString)));
    QVERIFY(disconnectSpy.isValid());
    QVERIFY(zonedDisconnectSpy.isValid());

    server.stop();

    WAIT_AND_COMPARE(disconnectSpy, 1);
    QCOMPARE(client.error(), QIviAbstractFeature::Unknown);
    WAIT_AND_COMPARE(zonedDisconnectSpy, 1);
    QCOMPARE(zonedClient.error(), QIviAbstractFeature::Unknown);

    //test that a remote call fails on a disconnected replica
    QIviPendingReply<QString> idReply = client.id();
    QVERIFY(idReply.isResultAvailable() && !idReply.watcher()->isSuccessful());

    QIviPendingReply<QString> zonedIdReply = zonedClient.id();
    QVERIFY(zonedIdReply.isResultAvailable() && !zonedIdReply.watcher()->isSuccessful());

    //test reconnection
    QSignalSpy reconnectSpy(&client, SIGNAL(errorChanged(QIviAbstractFeature::Error, QString)));
    QVERIFY(reconnectSpy.isValid());
    QSignalSpy zonedReconnectSpy(&zonedClient, SIGNAL(errorChanged(QIviAbstractFeature::Error, QString)));
    QVERIFY(zonedReconnectSpy.isValid());

    server.start();

    WAIT_AND_COMPARE(reconnectSpy, 1);
    QCOMPARE(client.error(), QIviAbstractFeature::NoError);
    WAIT_AND_COMPARE(zonedReconnectSpy, 1);
    QCOMPARE(zonedClient.error(), QIviAbstractFeature::NoError);
}

void EchoQtroTest::testClient2Server()
{
    Server server;
    QVERIFY(server.start());

    Echo client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test properties
    QSignalSpy intValueSpy(&server.m_echoService, SIGNAL(intValueChanged(int)));
    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    client.setIntValue(intValueTestValue);
    WAIT_AND_COMPARE(intValueSpy, 1);
    QCOMPARE(server.m_echoService.intValue(), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);

    QSignalSpy floatValue1Spy(&server.m_echoService, SIGNAL(floatValue1Changed(qreal)));
    QVERIFY(floatValue1Spy.isValid());
    qreal floatValue1TestValue = 1234.5678;
    client.setFloatValue1(floatValue1TestValue);
    WAIT_AND_COMPARE(floatValue1Spy, 1);
    QCOMPARE(server.m_echoService.floatValue1(), floatValue1TestValue);
    QCOMPARE(floatValue1Spy[0][0].toReal(), floatValue1TestValue);

    QSignalSpy floatValue2Spy(&server.m_echoService, SIGNAL(floatValue2Changed(qreal)));
    QVERIFY(floatValue2Spy.isValid());
    qreal floatValue2TestValue = 3.1415;
    client.setFloatValue2(floatValue2TestValue);
    WAIT_AND_COMPARE(floatValue2Spy, 1);
    QCOMPARE(server.m_echoService.floatValue2(), floatValue2TestValue);
    QCOMPARE(floatValue2Spy[0][0].toReal(), floatValue2TestValue);

    QSignalSpy stringValueSpy(&server.m_echoService, SIGNAL(stringValueChanged(QString)));
    QVERIFY(stringValueSpy.isValid());
    QString stringValueTestValue = QStringLiteral("hello test");
    client.setStringValue(stringValueTestValue);
    WAIT_AND_COMPARE(stringValueSpy, 1);
    QCOMPARE(server.m_echoService.stringValue(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);

    QSignalSpy comboListSpy(&server.m_echoService, SIGNAL(comboListChanged(QVariantList)));
    QVERIFY(comboListSpy.isValid());
    QVariantList comboListTestValue(
                    { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), EchoModule::Monday)),
                      QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), EchoModule::Wednesday)) });
    client.setComboList(comboListTestValue);
    WAIT_AND_COMPARE(comboListSpy, 1);
    QCOMPARE(server.m_echoService.comboList().count(),comboListTestValue.count());
    QCOMPARE(server.m_echoService.comboList().at(0).value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(server.m_echoService.comboList().at(1).value<Combo>(), comboListTestValue[1].value<Combo>());
    QVariantList signalArgs = comboListSpy[0][0].toList();
    QCOMPARE(signalArgs[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(signalArgs[1].value<Combo>(), comboListTestValue[1].value<Combo>());

    QSignalSpy contactSpy(&server.m_echoService, SIGNAL(contactChanged(Contact)));
    QVERIFY(contactSpy.isValid());
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    client.setContact(contactTestValue);
    WAIT_AND_COMPARE(contactSpy, 1);
    QCOMPARE(server.m_echoService.contact(), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);

    QSignalSpy weekDaySpy(&server.m_echoService, SIGNAL(weekDayChanged(EchoModule::DaysOfTheWeek)));
    QVERIFY(weekDaySpy.isValid());
    EchoModule::DaysOfTheWeek weekDayTestValue = EchoModule::Thursday;
    client.setWeekDay(weekDayTestValue);
    WAIT_AND_COMPARE(weekDaySpy, 1);
    QCOMPARE(server.m_echoService.weekDay(), weekDayTestValue);
    QCOMPARE(weekDaySpy[0][0].value<EchoModule::DaysOfTheWeek>(), weekDayTestValue);

    QSignalSpy testEnumSpy(&server.m_echoService, SIGNAL(testEnumChanged(EchoModule::TestEnum)));
    QVERIFY(testEnumSpy.isValid());
    EchoModule::TestEnum testEnumTestValue = EchoModule::SecondEnumValue;
    client.setTestEnum(testEnumTestValue);
    WAIT_AND_COMPARE(testEnumSpy, 1);
    QCOMPARE(server.m_echoService.testEnum(), testEnumTestValue);
    QCOMPARE(testEnumSpy[0][0].value<EchoModule::TestEnum>(), testEnumTestValue);
}

void EchoQtroTest::testZonedClient2Server()
{
    Server server;
    QVERIFY(server.start());

    EchoZoned client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test properties
    QSignalSpy intValueSpy(&server.m_echoZonedService, SIGNAL(intValueChanged(int, QString)));
    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    client.setIntValue(intValueTestValue);
    WAIT_AND_COMPARE(intValueSpy, 1);
    QCOMPARE(server.m_echoZonedService.intValue(QString()), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);
    QCOMPARE(intValueSpy[0][1].toString(), QString());

    QSignalSpy UPPERCASEPROPERTYSpy(&server.m_echoZonedService, SIGNAL(UPPERCASEPROPERTYChanged(qreal, QString)));
    QVERIFY(UPPERCASEPROPERTYSpy.isValid());
    qreal floatValueTestValue = 1234.5678;
    client.setUPPERCASEPROPERTY(floatValueTestValue);
    WAIT_AND_COMPARE(UPPERCASEPROPERTYSpy, 1);
    QCOMPARE(server.m_echoZonedService.UPPERCASEPROPERTY(QString()), floatValueTestValue);
    QCOMPARE(UPPERCASEPROPERTYSpy[0][0].toReal(), floatValueTestValue);
    QCOMPARE(UPPERCASEPROPERTYSpy[0][1].toString(), QString());

    EchoZoned *zone = qobject_cast<EchoZoned*>(client.zoneAt(frontLeftZone));
    QVERIFY(zone);

    QSignalSpy stringValueSpy(&server.m_echoZonedService, SIGNAL(stringValueChanged(QString, QString)));
    QVERIFY(stringValueSpy.isValid());
    QString stringValueTestValue = QStringLiteral("hello test");
    zone->setStringValue(stringValueTestValue);
    WAIT_AND_COMPARE(stringValueSpy, 1);
    QCOMPARE(server.m_echoZonedService.stringValue(frontLeftZone), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][1].toString(), frontLeftZone);

    QSignalSpy comboListSpy(&server.m_echoZonedService, SIGNAL(comboListChanged(QVariantList, QString)));
    QVERIFY(comboListSpy.isValid());
    QVariantList comboListTestValue(
                    { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), EchoModule::Monday)),
                      QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), EchoModule::Wednesday)) });
    zone->setComboList(comboListTestValue);
    WAIT_AND_COMPARE(comboListSpy, 1);
    QVariantList comboList = server.m_echoZonedService.comboList(frontLeftZone);
    QCOMPARE(comboList.count(),comboListTestValue.count());
    QCOMPARE(comboList.at(0).value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(comboList.at(1).value<Combo>(), comboListTestValue[1].value<Combo>());
    QVariantList signalArgs = comboListSpy[0][0].toList();
    QCOMPARE(signalArgs[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(signalArgs[1].value<Combo>(), comboListTestValue[1].value<Combo>());
    QCOMPARE(comboListSpy[0][1].toString(), frontLeftZone);

    QSignalSpy contactSpy(&server.m_echoZonedService, SIGNAL(contactChanged(Contact, QString)));
    QVERIFY(contactSpy.isValid());
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    zone->setContact(contactTestValue);
    WAIT_AND_COMPARE(contactSpy, 1);
    QCOMPARE(server.m_echoZonedService.contact(frontLeftZone), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);
    QCOMPARE(contactSpy[0][1].toString(), frontLeftZone);

    QSignalSpy airflowSpy(&server.m_echoZonedService, SIGNAL(airflowDirectionChanged(EchoModule::AirflowDirections, QString)));
    QVERIFY(airflowSpy.isValid());
    EchoModule::AirflowDirections airflowTestValue = EchoModule::Windshield;
    zone->setAirflowDirection(airflowTestValue);
    WAIT_AND_COMPARE(airflowSpy, 1);
    QCOMPARE(server.m_echoZonedService.airflowDirection(frontLeftZone), airflowTestValue);
    QCOMPARE(airflowSpy[0][0].value<EchoModule::AirflowDirections>(), airflowTestValue);
    QCOMPARE(airflowSpy[0][1].toString(), frontLeftZone);

    QSignalSpy testEnumSpy(&server.m_echoZonedService, SIGNAL(testEnumChanged(EchoModule::TestEnum, QString)));
    QVERIFY(testEnumSpy.isValid());
    EchoModule::TestEnum testEnumTestValue = EchoModule::SecondEnumValue;
    zone->setTestEnum(testEnumTestValue);
    WAIT_AND_COMPARE(testEnumSpy, 1);
    QCOMPARE(server.m_echoZonedService.testEnum(frontLeftZone), testEnumTestValue);
    QCOMPARE(testEnumSpy[0][0].value<EchoModule::TestEnum>(), testEnumTestValue);
    QCOMPARE(testEnumSpy[0][1].toString(), frontLeftZone);
}

void EchoQtroTest::testServer2Client()
{
    Server server;
    QVERIFY(server.start());

    Echo client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);


    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test properties
    QSignalSpy intValueSpy(&client, SIGNAL(intValueChanged(int)));
    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    server.m_echoService.setIntValue(intValueTestValue);
    WAIT_AND_COMPARE(intValueSpy, 1);
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);

    QSignalSpy floatValue1Spy(&client, SIGNAL(floatValue1Changed(qreal)));
    QVERIFY(floatValue1Spy.isValid());
    qreal floatValue1TestValue = 1234.5678;
    server.m_echoService.setFloatValue1(floatValue1TestValue);
    WAIT_AND_COMPARE(floatValue1Spy, 1);
    QCOMPARE(client.floatValue1(), floatValue1TestValue);
    QCOMPARE(floatValue1Spy[0][0].toReal(), floatValue1TestValue);

    QSignalSpy floatValue2Spy(&client, SIGNAL(floatValue2Changed(qreal)));
    QVERIFY(floatValue2Spy.isValid());
    qreal floatValue2TestValue = 234.678;
    server.m_echoService.setFloatValue2(floatValue2TestValue);
    WAIT_AND_COMPARE(floatValue2Spy, 1);
    QCOMPARE(client.floatValue2(), floatValue2TestValue);
    QCOMPARE(floatValue2Spy[0][0].toReal(), floatValue2TestValue);

    QSignalSpy stringValueSpy(&client, SIGNAL(stringValueChanged(QString)));
    QVERIFY(stringValueSpy.isValid());
    QLatin1String stringValueTestValue("hello test");
    server.m_echoService.setStringValue(stringValueTestValue);
    WAIT_AND_COMPARE(stringValueSpy, 1);
    QCOMPARE(client.stringValue(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);

    QSignalSpy comboListSpy(&client, SIGNAL(comboListChanged(QVariantList)));
    QVERIFY(comboListSpy.isValid());
    QVariantList comboListTestValue(
                    { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), EchoModule::Monday)),
                      QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), EchoModule::Wednesday)) });
    server.m_echoService.setComboList(comboListTestValue);
    WAIT_AND_COMPARE(comboListSpy, 1);
    QCOMPARE(client.comboList().count(),comboListTestValue.count());
    QCOMPARE(client.comboList().at(0).value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(client.comboList().at(1).value<Combo>(), comboListTestValue[1].value<Combo>());
    QVariantList signalArgs = comboListSpy[0][0].toList();
    QCOMPARE(signalArgs[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(signalArgs[1].value<Combo>(), comboListTestValue[1].value<Combo>());

    QSignalSpy contactSpy(&client, SIGNAL(contactChanged(Contact)));
    QVERIFY(contactSpy.isValid());
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    server.m_echoService.setContact(contactTestValue);
    WAIT_AND_COMPARE(contactSpy, 1);
    QCOMPARE(client.contact(), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);

    QSignalSpy weekDaySpy(&client, SIGNAL(weekDayChanged(EchoModule::DaysOfTheWeek)));
    QVERIFY(weekDaySpy.isValid());
    EchoModule::WeekDay weekDayTestValue = EchoModule::Friday;
    server.m_echoService.setWeekDay(weekDayTestValue);
    WAIT_AND_COMPARE(weekDaySpy, 1);

    QCOMPARE(client.weekDay(), weekDayTestValue);
    QCOMPARE(weekDaySpy[0][0].value<EchoModule::DaysOfTheWeek>(), weekDayTestValue);

    QSignalSpy testEnumSpy(&client, SIGNAL(testEnumChanged(EchoModule::TestEnum)));
    QVERIFY(testEnumSpy.isValid());
    EchoModule::TestEnum testEnumTestValue = EchoModule::SecondEnumValue;
    server.m_echoService.setTestEnum(testEnumTestValue);
    WAIT_AND_COMPARE(testEnumSpy, 1);
    QCOMPARE(client.testEnum(), testEnumTestValue);
    QCOMPARE(testEnumSpy[0][0].value<EchoModule::TestEnum>(), testEnumTestValue);
}

void EchoQtroTest::testZonedServer2Client()
{
    Server server;
    QVERIFY(server.start());

    EchoZoned client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test properties
    QSignalSpy intValueSpy(&client, SIGNAL(intValueChanged(int)));
    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    server.m_echoZonedService.setIntValue(intValueTestValue, QString());
    WAIT_AND_COMPARE(intValueSpy, 1);
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);

    QSignalSpy UPPERCASEPROPERTYSpy(&client, SIGNAL(UPPERCASEPROPERTYChanged(qreal)));
    QVERIFY(UPPERCASEPROPERTYSpy.isValid());
    qreal floatValueTestValue = 1234.5678;
    server.m_echoZonedService.setUPPERCASEPROPERTY(floatValueTestValue, QString());
    WAIT_AND_COMPARE(UPPERCASEPROPERTYSpy, 1);
    QCOMPARE(client.UPPERCASEPROPERTY(), floatValueTestValue);
    QCOMPARE(UPPERCASEPROPERTYSpy[0][0].toReal(), floatValueTestValue);

    EchoZoned *zone = qobject_cast<EchoZoned*>(client.zoneAt(frontLeftZone));
    QVERIFY(zone);

    QSignalSpy stringValueSpy(zone, SIGNAL(stringValueChanged(QString)));
    QVERIFY(stringValueSpy.isValid());
    QString stringValueTestValue = QStringLiteral("hello test");
    server.m_echoZonedService.setStringValue(stringValueTestValue, frontLeftZone);
    WAIT_AND_COMPARE(stringValueSpy, 1);
    QCOMPARE(zone->stringValue(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);

    QSignalSpy comboListSpy(zone, SIGNAL(comboListChanged(QVariantList)));
    QVERIFY(comboListSpy.isValid());
    QVariantList comboListTestValue(
                    { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), EchoModule::Thursday)),
                      QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), EchoModule::Tuesday)) });
    server.m_echoZonedService.setComboList(comboListTestValue, frontLeftZone);
    WAIT_AND_COMPARE(comboListSpy, 1);
    QVariantList comboList = zone->comboList();
    QCOMPARE(comboList.count(), comboListTestValue.count());
    QCOMPARE(comboList.at(0).value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(comboList.at(1).value<Combo>(), comboListTestValue[1].value<Combo>());
    QVariantList signalArgs = comboListSpy[0][0].toList();
    QCOMPARE(signalArgs[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(signalArgs[1].value<Combo>(), comboListTestValue[1].value<Combo>());

    QSignalSpy contactSpy(zone, SIGNAL(contactChanged(Contact)));
    QVERIFY(contactSpy.isValid());
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    server.m_echoZonedService.setContact(contactTestValue, frontLeftZone);
    WAIT_AND_COMPARE(contactSpy, 1);
    QCOMPARE(zone->contact(), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);

    QSignalSpy airflowSpy(zone, SIGNAL(airflowDirectionChanged(EchoModule::AirflowDirections)));
    QVERIFY(airflowSpy.isValid());
    EchoModule::AirflowDirections airflowTestValue = EchoModule::Windshield;
    server.m_echoZonedService.setAirflowDirection(airflowTestValue, frontLeftZone);
    WAIT_AND_COMPARE(airflowSpy, 1);
    QCOMPARE(zone->airflowDirection(), airflowTestValue);
    QCOMPARE(airflowSpy[0][0].value<EchoModule::AirflowDirections>(), airflowTestValue);

    QSignalSpy testEnumSpy(zone, SIGNAL(testEnumChanged(EchoModule::TestEnum)));
    QVERIFY(testEnumSpy.isValid());
    EchoModule::TestEnum testEnumTestValue = EchoModule::SecondEnumValue;
    server.m_echoZonedService.setTestEnum(testEnumTestValue, frontLeftZone);
    WAIT_AND_COMPARE(testEnumSpy, 1);
    QCOMPARE(zone->testEnum(), testEnumTestValue);
    QCOMPARE(testEnumSpy[0][0].value<EchoModule::TestEnum>(), testEnumTestValue);
}

void EchoQtroTest::testSlots()
{
    Server server;
    server.start();

    Echo client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);


    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test slots by calling them on the client
    QSignalSpy echoSpy(&server.m_echoService, SIGNAL(echoSlotCalled(const QString&)));
    QVERIFY(echoSpy.isValid());
    QLatin1String echoTestValue("this will be echoed");
    QIviPendingReply<QString> echoReply = client.echo(echoTestValue);
    QSignalSpy echoReplySpy(echoReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(echoReplySpy, 1);
    QCOMPARE(echoReply.reply(), echoTestValue);
    QCOMPARE(echoSpy.count(), 1);
    QCOMPARE(echoSpy[0][0].toString(), echoTestValue);

    QSignalSpy idSpy(&server.m_echoService, SIGNAL(idSlotCalled()));
    QVERIFY(idSpy.isValid());
    QIviPendingReply<QString> idReply = client.id();
    QSignalSpy idReplySpy(idReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(idReplySpy, 1);
    QCOMPARE(idReply.reply(), server.m_echoService.m_testId);
    QCOMPARE(idSpy.count(), 1);

    QSignalSpy getComboSpy(&server.m_echoService, SIGNAL(getComboSlotCalled()));
    QVERIFY(getComboSpy.isValid());
    QIviPendingReply<Combo> comboReply = client.getCombo();
    QSignalSpy comboReplySpy(comboReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(comboReplySpy, 1);
    QCOMPARE(comboReply.reply(), server.m_echoService.m_testCombo);
    QCOMPARE(getComboSpy.count(), 1);

    QSignalSpy voidSlotSpy(&server.m_echoService, SIGNAL(voidSlotCalled()));
    QVERIFY(voidSlotSpy.isValid());
    client.voidSlot();
    WAIT_AND_COMPARE(voidSlotSpy, 1);

    QSignalSpy voidSlot2Spy(&server.m_echoService, SIGNAL(voidSlot2Called(int)));
    int voidSlot2TestValue = 776;
    QVERIFY(voidSlot2Spy.isValid());
    client.voidSlot2(voidSlot2TestValue);
    voidSlot2Spy.wait(1000);
    QCOMPARE(voidSlot2Spy.count(), 1);
    QCOMPARE(voidSlot2Spy[0][0].toInt(), voidSlot2TestValue);

    QSignalSpy flagMethodSpy(&server.m_echoService, SIGNAL(flagMethodCalled(EchoModule::AirflowDirections)));
    QVERIFY(flagMethodSpy.isValid());
    EchoModule::AirflowDirections flagTestValue = EchoModule::Dashboard;
    QIviPendingReply<EchoModule::AirflowDirections> flagMethodReply = client.flagMethod(flagTestValue);
    QSignalSpy flagMethodReplySpy(flagMethodReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(flagMethodReplySpy, 1);
    QCOMPARE(flagMethodReply.reply(), flagTestValue);
    QCOMPARE(flagMethodSpy.count(), 1);
    QCOMPARE(flagMethodSpy[0][0].value<EchoModule::AirflowDirections>(), flagTestValue);

    QSignalSpy enumMethodSpy(&server.m_echoService, SIGNAL(enumMethodCalled(EchoModule::TestEnum)));
    QVERIFY(enumMethodSpy.isValid());
    EchoModule::TestEnum enumTestValue = EchoModule::SecondEnumValue;
    QIviPendingReply<EchoModule::TestEnum> enumMethodReply = client.enumMethod(enumTestValue);
    QSignalSpy enumMethodReplySpy(enumMethodReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(enumMethodReplySpy, 1);
    QCOMPARE(enumMethodReply.reply(), enumTestValue);
    QCOMPARE(enumMethodSpy.count(), 1);
    QCOMPARE(enumMethodSpy[0][0].value<EchoModule::TestEnum>(), enumTestValue);
}

void EchoQtroTest::testZonedSlots()
{
    Server server;
    server.start();

    EchoZoned client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);


    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test slots by calling them on the client
    QSignalSpy echoSpy(&server.m_echoZonedService, SIGNAL(echoSlotCalled(QString, QString)));
    QVERIFY(echoSpy.isValid());
    QLatin1String echoTestValue("this will be echoed");
    QIviPendingReply<QString> echoReply = client.echo(echoTestValue);
    QSignalSpy echoReplySpy(echoReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(echoReplySpy, 1);
    QCOMPARE(echoReply.reply(), echoTestValue);
    QCOMPARE(echoSpy.count(), 1);
    QCOMPARE(echoSpy[0][0].toString(), echoTestValue);
    QCOMPARE(echoSpy[0][1].toString(), QString());

    QSignalSpy idSpy(&server.m_echoZonedService, SIGNAL(idSlotCalled(QString)));
    QVERIFY(idSpy.isValid());
    QIviPendingReply<QString> idReply = client.id();
    QSignalSpy idReplySpy(idReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(idReplySpy, 1);
    QCOMPARE(idReply.reply(), server.m_echoService.m_testId);
    QCOMPARE(idSpy.count(), 1);
    QCOMPARE(idSpy[0][0].toString(), QString());

    EchoZoned *zone = qobject_cast<EchoZoned*>(client.zoneAt(frontLeftZone));
    QVERIFY(zone);

    QSignalSpy getComboSpy(&server.m_echoZonedService, SIGNAL(getComboSlotCalled(QString)));
    QVERIFY(getComboSpy.isValid());
    QIviPendingReply<Combo> comboReply = zone->getCombo();
    QSignalSpy comboReplySpy(comboReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(comboReplySpy, 1);
    QCOMPARE(comboReply.reply(), server.m_echoService.m_testCombo);
    QCOMPARE(getComboSpy.count(), 1);

    QSignalSpy flagMethodSpy(&server.m_echoZonedService, SIGNAL(flagMethodCalled(EchoModule::AirflowDirections, QString)));
    QVERIFY(flagMethodSpy.isValid());
    EchoModule::AirflowDirections flagTestValue = EchoModule::Dashboard;
    QIviPendingReply<EchoModule::AirflowDirections> flagMethodReply = zone->flagMethod(flagTestValue);
    QSignalSpy flagMethodReplySpy(flagMethodReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(flagMethodReplySpy, 1);
    QCOMPARE(flagMethodReply.reply(), flagTestValue);
    QCOMPARE(flagMethodSpy.count(), 1);
    QCOMPARE(flagMethodSpy[0][0].value<EchoModule::AirflowDirections>(), flagTestValue);
    QCOMPARE(flagMethodSpy[0][1].toString(), frontLeftZone);

    QSignalSpy enumMethodSpy(&server.m_echoZonedService, SIGNAL(enumMethodCalled(EchoModule::TestEnum, QString)));
    QVERIFY(enumMethodSpy.isValid());
    EchoModule::TestEnum enumTestValue = EchoModule::SecondEnumValue;
    QIviPendingReply<EchoModule::TestEnum> enumMethodReply = zone->enumMethod(enumTestValue);
    QSignalSpy enumMethodReplySpy(enumMethodReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(enumMethodReplySpy, 1);
    QCOMPARE(enumMethodReply.reply(), enumTestValue);
    QCOMPARE(enumMethodSpy.count(), 1);
    QCOMPARE(enumMethodSpy[0][0].value<EchoModule::TestEnum>(), enumTestValue);
    QCOMPARE(enumMethodSpy[0][1].toString(), frontLeftZone);
}

void EchoQtroTest::testMultipleSlotCalls()
{
    Server server;
    server.start();

    Echo client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);


    //wait until the client has connected and initial values are set
    initSpy.wait(1000);
    QCOMPARE(initSpy.count(), 1);
    QVERIFY(client.isInitialized());

    //test the pending replies by calling the same slot with 3 different values
    QSignalSpy echoSpy(&server.m_echoService, SIGNAL(echoSlotCalled(const QString&)));
    QVERIFY(echoSpy.isValid());
    QLatin1String echoTestValue("first");
    QLatin1String echoTestValue2("second");
    QLatin1String echoTestValue3("third");
    QIviPendingReply<QString> echoReply = client.echo(echoTestValue);
    QIviPendingReply<QString> echoReply2 = client.echo(echoTestValue2);
    QIviPendingReply<QString> echoReply3 = client.echo(echoTestValue3);
    QSignalSpy echoReplySpy(echoReply.watcher(), SIGNAL(replySuccess()));
    QSignalSpy echoReplySpy2(echoReply2.watcher(), SIGNAL(replySuccess()));
    QSignalSpy echoReplySpy3(echoReply3.watcher(), SIGNAL(replySuccess()));
    echoReplySpy3.wait();
    QCOMPARE(echoReplySpy.count(), 1);
    QCOMPARE(echoReplySpy2.count(), 1);
    QCOMPARE(echoReplySpy3.count(), 1);
    QCOMPARE(echoReply.reply(), echoTestValue);
    QCOMPARE(echoReply2.reply(), echoTestValue2);
    QCOMPARE(echoReply3.reply(), echoTestValue3);
    QCOMPARE(echoSpy.count(), 3);
    QCOMPARE(echoSpy[0][0].toString(), echoTestValue);
    QCOMPARE(echoSpy[1][0].toString(), echoTestValue2);
    QCOMPARE(echoSpy[2][0].toString(), echoTestValue3);

    EchoZoned zonedClient;
    QSignalSpy zonedInitSpy(&zonedClient, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(zonedInitSpy.isValid());
    QVERIFY(zonedClient.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    zonedInitSpy.wait(1000);
    QCOMPARE(zonedInitSpy.count(), 1);
    QVERIFY(zonedClient.isInitialized());

    EchoZoned *zone = qobject_cast<EchoZoned*>(zonedClient.zoneAt(frontLeftZone));
    QVERIFY(zone);

    QSignalSpy echoZonedSpy(&server.m_echoZonedService, SIGNAL(echoSlotCalled(QString, QString)));
    QVERIFY(echoSpy.isValid());
    QIviPendingReply<QString> echoZonedReply = zone->echo(echoTestValue);
    QIviPendingReply<QString> echoZonedReply2 = zone->echo(echoTestValue2);
    QIviPendingReply<QString> echoZonedReply3 = zone->echo(echoTestValue3);
    QSignalSpy echoZonedReplySpy(echoZonedReply.watcher(), SIGNAL(replySuccess()));
    QSignalSpy echoZonedReplySpy2(echoZonedReply2.watcher(), SIGNAL(replySuccess()));
    QSignalSpy echoZonedReplySpy3(echoZonedReply3.watcher(), SIGNAL(replySuccess()));
    echoZonedReplySpy3.wait();
    QCOMPARE(echoZonedReplySpy.count(), 1);
    QCOMPARE(echoZonedReplySpy2.count(), 1);
    QCOMPARE(echoZonedReplySpy3.count(), 1);
    QCOMPARE(echoZonedReply.reply(), echoTestValue);
    QCOMPARE(echoZonedReply2.reply(), echoTestValue2);
    QCOMPARE(echoZonedReply3.reply(), echoTestValue3);
    QCOMPARE(echoZonedSpy.count(),3);
    QCOMPARE(echoZonedSpy[0][0].toString(), echoTestValue);
    QCOMPARE(echoZonedSpy[0][1].toString(), frontLeftZone);
    QCOMPARE(echoZonedSpy[1][0].toString(), echoTestValue2);
    QCOMPARE(echoZonedSpy[0][1].toString(), frontLeftZone);
    QCOMPARE(echoZonedSpy[2][0].toString(), echoTestValue3);
    QCOMPARE(echoZonedSpy[0][1].toString(), frontLeftZone);
}

void EchoQtroTest::testAsyncSlotResults()
{
    Server server;
    server.start();

    Echo client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    initSpy.wait(1000);
    QCOMPARE(initSpy.count(), 1);
    QVERIFY(client.isInitialized());

    // test the timer() function which uses a pendingReply on the server side to return the
    // function when the timer is finished.
    QIviPendingReply<void> reply = client.timer(1000);
    QIviPendingReply<void> reply2 = client.timer(500);
    QSignalSpy echoReplySpy(reply.watcher(), SIGNAL(replySuccess()));
    QSignalSpy echoReplySpy2(reply2.watcher(), SIGNAL(replySuccess()));

    //Wait for the second reply to return first. Verify the other reply is not yet ready.
    echoReplySpy2.wait();
    QCOMPARE(echoReplySpy2.count(), 1);
    QCOMPARE(echoReplySpy.count(), 0);

    //Wait for the first reply and verify both replies were sent.
    echoReplySpy.wait();
    QCOMPARE(echoReplySpy2.count(), 1);
    QCOMPARE(echoReplySpy.count(), 1);

    EchoZoned zonedClient;
    QSignalSpy zonedInitSpy(&zonedClient, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(zonedInitSpy.isValid());
    QVERIFY(zonedClient.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    zonedInitSpy.wait(1000);
    QCOMPARE(zonedInitSpy.count(), 1);
    QVERIFY(zonedClient.isInitialized());

    EchoZoned *zone = qobject_cast<EchoZoned*>(zonedClient.zoneAt(frontLeftZone));
    QVERIFY(zone);

    // test the timer() function which uses a pendingReply on the server side to return the
    // function when the timer is finished.
    QIviPendingReply<QString> zonedReply = zonedClient.timer(1000);
    QIviPendingReply<QString> zonedReply2 = zone->timer(500);
    QSignalSpy zonedEchoReplySpy(zonedReply.watcher(), SIGNAL(replySuccess()));
    QSignalSpy zonedEchoReplySpy2(zonedReply2.watcher(), SIGNAL(replySuccess()));

    //Wait for the second reply to return first. Verify the other reply is not yet ready.
    zonedEchoReplySpy2.wait();
    QCOMPARE(zonedEchoReplySpy2.count(), 1);
    QCOMPARE(zonedEchoReplySpy.count(), 0);
    QCOMPARE(zonedReply2.value(), frontLeftZone);

    //Wait for the first reply and verify both replies were sent.
    zonedEchoReplySpy.wait();
    QCOMPARE(zonedEchoReplySpy2.count(), 1);
    QCOMPARE(zonedEchoReplySpy.count(), 1);
    QCOMPARE(zonedReply.value(), QString());
}

void EchoQtroTest::testSignals()
{
    Server server;
    server.start();

    Echo client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test custom signals (other than property notifiers) from server to client
    QSignalSpy anotherChangedSpy(&client, SIGNAL(anotherChanged(AnotherStruct)));
    QVERIFY(anotherChangedSpy.isValid());
    AnotherStruct anotherTestValue(7);
    server.m_echoService.anotherChanged(anotherTestValue);
    WAIT_AND_COMPARE(anotherChangedSpy, 1);
    QCOMPARE(anotherChangedSpy[0][0].value<AnotherStruct>(), anotherTestValue);

    QSignalSpy foobarSpy(&client, SIGNAL(foobar(QString)));
    QVERIFY(foobarSpy.isValid());
    QLatin1String foobarTestValue("foo and bar");
    server.m_echoService.foobar(foobarTestValue);
    WAIT_AND_COMPARE(foobarSpy, 1);
    QCOMPARE(foobarSpy[0][0].toString(), foobarTestValue);

    QSignalSpy somethingSpy(&client, SIGNAL(somethingHappened()));
    QVERIFY(somethingSpy.isValid());;
    server.m_echoService.somethingHappened();
    WAIT_AND_COMPARE(somethingSpy, 1);


    EchoZoned zonedClient;
    QSignalSpy zonedInitSpy(&zonedClient, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(zonedInitSpy.isValid());
    QVERIFY(zonedClient.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    zonedInitSpy.wait(1000);
    QCOMPARE(zonedInitSpy.count(), 1);
    QVERIFY(zonedClient.isInitialized());

    //test custom signals (other than property notifiers) from server to client
    QSignalSpy zonedAnotherChangedSpy(&zonedClient, SIGNAL(anotherChanged(AnotherStruct)));
    QVERIFY(zonedAnotherChangedSpy.isValid());
    server.m_echoZonedService.anotherChanged(anotherTestValue, QString());
    WAIT_AND_COMPARE(zonedAnotherChangedSpy, 1);
    QCOMPARE(zonedAnotherChangedSpy[0][0].value<AnotherStruct>(), anotherTestValue);

    EchoZoned *zone = qobject_cast<EchoZoned*>(zonedClient.zoneAt(frontLeftZone));
    QVERIFY(zone);

    QSignalSpy zonedFoobarSpy(zone, SIGNAL(foobar(QString)));
    QVERIFY(zonedFoobarSpy.isValid());
    server.m_echoZonedService.foobar(foobarTestValue, frontLeftZone);
    WAIT_AND_COMPARE(zonedFoobarSpy, 1);
    QCOMPARE(zonedFoobarSpy[0][0].toString(), foobarTestValue);

    QSignalSpy zonedSomethingSpy(zone, SIGNAL(somethingHappened()));
    QVERIFY(zonedSomethingSpy.isValid());;
    server.m_echoZonedService.somethingHappened(frontLeftZone);
    WAIT_AND_COMPARE(zonedSomethingSpy, 1);
}

void EchoQtroTest::testModel()
{
    Server server;
    server.start();

    Echo client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() == QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //Give QtRO time to actually call our server side
    QTest::qWait(200);

    QIviPagingModel* model = client.contactList();
    QVERIFY(model->isInitialized());
    QCOMPARE(model->rowCount(), 0);

    //Test inserting a row
    Contact testContact(QStringLiteral("Mr A."), 20, false, "foo");
    QSignalSpy countSpy(model, SIGNAL(countChanged()));
    server.m_contactsModelService.insert(0, testContact);

    WAIT_AND_COMPARE(countSpy, 1);
    QCOMPARE(model->rowCount(), 1);
    QCOMPARE(model->at<Contact>(0), testContact);
    countSpy.clear();

    //test updating a row
    QSignalSpy changedSpy(model, SIGNAL(dataChanged( QModelIndex, QModelIndex, QVector<int>)));
    Contact updatedContact(QStringLiteral("Mr B."), 30, true, QVariant());
    server.m_contactsModelService.update(0, updatedContact);

    WAIT_AND_COMPARE(changedSpy, 1);
    QCOMPARE(model->rowCount(), 1);
    QCOMPARE(model->at<Contact>(0), updatedContact);

    //Test removing a row
    server.m_contactsModelService.remove(0);
    WAIT_AND_COMPARE(countSpy, 1);
    QCOMPARE(model->rowCount(), 0);
}
