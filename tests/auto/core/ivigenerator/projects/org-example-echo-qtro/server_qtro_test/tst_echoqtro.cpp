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
#include "echoplugin.h"
#include "server.h"
#include "echo.h"

#define WAIT_AND_COMPARE(spy, value) \
if (spy.count() == !value) \
    spy.wait(); \
QCOMPARE(spy.count(), value); \


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
    QVERIFY(client.startAutoDiscovery()==QIviAbstractFeature::ProductionBackendLoaded);

    QCOMPARE(client.lastMessage(), QString());
    QCOMPARE(client.intValue(), 0);
    QCOMPARE(client.floatValue1(), qreal(0.0));
    QCOMPARE(client.floatValue2(), qreal(0.0));
    QCOMPARE(client.stringValue(), QString());
    QCOMPARE(client.contactList(), QVariantList());
    QCOMPARE(client.contact(), Contact());
    QCOMPARE(client.weekDay(), EchoModule::WeekDay());

    Server server;

    //test initial values
    QCOMPARE(server.m_service.lastMessage(), QString());
    QCOMPARE(server.m_service.intValue(), 0);
    QCOMPARE(server.m_service.floatValue1(), qreal(0.0));
    QCOMPARE(server.m_service.floatValue2(), qreal(0.0));
    QCOMPARE(server.m_service.stringValue(), QString());
    QCOMPARE(server.m_service.contactList(), QVariantList());
    QCOMPARE(server.m_service.contact(), Contact());
    QCOMPARE(server.m_service.weekDay(), EchoModule::WeekDay());

    QString lastMessageTestValue("this is the last message");
    int intValueTestValue(789);
    float floatValue1TestValue(3.14);
    float floatValue2TestValue(2.71);
    QString stringValueTestValue("test string");
    QVariantList contactListTestValue(
                    { QVariant::fromValue<Contact>(Contact("Mr A.", 20, false, "foo")),
                      QVariant::fromValue<Contact>(Contact("Mr B.", 40, true, "bar")) });
    Contact contactTestValue("Nemo", 47, true, 1);
    EchoModule::WeekDay weekDayTestValue = EchoModule::Wednesday;


    server.m_service.setLastMessage(lastMessageTestValue);
    server.m_service.setIntValue(intValueTestValue);
    server.m_service.setFloatValue1(floatValue1TestValue);
    server.m_service.setFloatValue2(floatValue2TestValue);
    server.m_service.setStringValue(stringValueTestValue);
    server.m_service.setContactList(contactListTestValue);
    server.m_service.setContact(contactTestValue);
    server.m_service.setWeekDay(weekDayTestValue);

    QVERIFY(!client.isInitialized());
    QCOMPARE(client.error(), QIviAbstractFeature::NoError);

    //wait until the client has connected and initial values are set
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));

    QVERIFY(server.start());

    QVERIFY(initSpy.isValid());
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test that client gets the same values that were set at the server before connection was established
    QCOMPARE(client.lastMessage(),lastMessageTestValue);
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(client.floatValue1(), floatValue1TestValue);
    QCOMPARE(client.floatValue2(), floatValue2TestValue);
    QCOMPARE(client.stringValue(), stringValueTestValue);
    QVariantList contactList = client.contactList();
    QCOMPARE(contactList[0].value<Contact>(), contactListTestValue[0].value<Contact>());
    QCOMPARE(contactList[1].value<Contact>(), contactListTestValue[1].value<Contact>());
    QCOMPARE(client.contact(), contactTestValue);
    QCOMPARE(client.weekDay(), weekDayTestValue);
}

void EchoQtroTest::testReconnect()
{
    Server server;
    QVERIFY(server.start());

    Echo client;
    QVERIFY(client.startAutoDiscovery()==QIviAbstractFeature::ProductionBackendLoaded);

    //wait until the client has connected and initial values are set
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test disconnection
    QCOMPARE(client.error(), QIviAbstractFeature::NoError);
    QSignalSpy disconnectSpy(&client, SIGNAL(errorChanged(QIviAbstractFeature::Error,QString)));
    QVERIFY(disconnectSpy.isValid());

    server.stop();

    WAIT_AND_COMPARE(disconnectSpy, 1);
    QCOMPARE(client.error(), QIviAbstractFeature::Unknown);

    //test that a remote call fails on a disconnected replica
    QIviPendingReply<QString> idReply = client.id();
    QVERIFY(idReply.isResultAvailable() && !idReply.watcher()->isSuccessful());

    //test reconnection
    QSignalSpy reconnectSpy(&client, SIGNAL(errorChanged(QIviAbstractFeature::Error,QString)));
    QVERIFY(reconnectSpy.isValid());

    server.start();

    WAIT_AND_COMPARE(reconnectSpy, 1);
    QCOMPARE(client.error(), QIviAbstractFeature::NoError);
}

void EchoQtroTest::testClient2Server()
{
    Server server;
    QVERIFY(server.start());

    Echo client;
    QVERIFY(client.startAutoDiscovery()==QIviAbstractFeature::ProductionBackendLoaded);


    //wait until the client has connected and initial values are set
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test properties
    QSignalSpy intValueSpy(&server.m_service, SIGNAL(intValueChanged(int)));
    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    client.setIntValue(intValueTestValue);
    WAIT_AND_COMPARE(intValueSpy, 1);
    QCOMPARE(server.m_service.intValue(), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);

    QSignalSpy floatValue1Spy(&server.m_service, SIGNAL(floatValue1Changed(qreal)));
    QVERIFY(floatValue1Spy.isValid());
    qreal floatValue1TestValue = 1234.5678;
    client.setFloatValue1(floatValue1TestValue);
    WAIT_AND_COMPARE(floatValue1Spy, 1);
    QCOMPARE(server.m_service.floatValue1(), floatValue1TestValue);
    QCOMPARE(floatValue1Spy[0][0].toReal(), floatValue1TestValue);

    QSignalSpy floatValue2Spy(&server.m_service, SIGNAL(floatValue2Changed(qreal)));
    QVERIFY(floatValue2Spy.isValid());
    qreal floatValue2TestValue = 3.1415;
    client.setFloatValue2(floatValue2TestValue);
    WAIT_AND_COMPARE(floatValue2Spy, 1);
    QCOMPARE(server.m_service.floatValue2(), floatValue2TestValue);
    QCOMPARE(floatValue2Spy[0][0].toReal(), floatValue2TestValue);

    QSignalSpy stringValueSpy(&server.m_service, SIGNAL(stringValueChanged(QString)));
    QVERIFY(stringValueSpy.isValid());
    QString stringValueTestValue = "hello test";
    client.setStringValue(stringValueTestValue);
    WAIT_AND_COMPARE(stringValueSpy, 1);
    QCOMPARE(server.m_service.stringValue(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);

    QSignalSpy contactListSpy(&server.m_service, SIGNAL(contactListChanged(QVariantList)));
    QVERIFY(contactListSpy.isValid());
    QVariantList contactListTestValue(
                    { QVariant::fromValue<Contact>(Contact("Mr A.", 20, false, "foo")),
                      QVariant::fromValue<Contact>(Contact("Mr B.", 40, true, "bar")) });
    client.setContactList(contactListTestValue);
    WAIT_AND_COMPARE(contactListSpy, 1);
    QCOMPARE(server.m_service.contactList().count(),contactListTestValue.count());
    QCOMPARE(server.m_service.contactList()[0].value<Contact>(), contactListTestValue[0].value<Contact>());
    QCOMPARE(server.m_service.contactList()[1].value<Contact>(), contactListTestValue[1].value<Contact>());
    QVariantList signalArgs = contactListSpy[0][0].toList();
    QCOMPARE(signalArgs[0].value<Contact>(), contactListTestValue[0].value<Contact>());
    QCOMPARE(signalArgs[1].value<Contact>(), contactListTestValue[1].value<Contact>());

    QSignalSpy contactSpy(&server.m_service, SIGNAL(contactChanged(Contact)));
    QVERIFY(contactSpy.isValid());
    Contact contactTestValue("Nemo", 47, true, 1);
    client.setContact(contactTestValue);
    WAIT_AND_COMPARE(contactSpy, 1);
    QCOMPARE(server.m_service.contact(), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);

    QSignalSpy weekDaySpy(&server.m_service, SIGNAL(weekDayChanged(EchoModule::WeekDay)));
    QVERIFY(weekDaySpy.isValid());
    EchoModule::WeekDay weekDayTestValue = EchoModule::Thursday;
    client.setWeekDay(weekDayTestValue);
    WAIT_AND_COMPARE(weekDaySpy, 1);
    QCOMPARE(server.m_service.weekDay(), weekDayTestValue);
    QCOMPARE(weekDaySpy[0][0].value<EchoModule::WeekDay>(), weekDayTestValue);
}

void EchoQtroTest::testServer2Client()
{
    Server server;
    QVERIFY(server.start());

    Echo client;
    QVERIFY(client.startAutoDiscovery()==QIviAbstractFeature::ProductionBackendLoaded);


    //wait until the client has connected and initial values are set
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test properties
    QSignalSpy intValueSpy(&client, SIGNAL(intValueChanged(int)));
    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    server.m_service.setIntValue(intValueTestValue);
    WAIT_AND_COMPARE(intValueSpy, 1);
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);

    QSignalSpy floatValue1Spy(&client, SIGNAL(floatValue1Changed(qreal)));
    QVERIFY(floatValue1Spy.isValid());
    qreal floatValue1TestValue = 1234.5678;
    server.m_service.setFloatValue1(floatValue1TestValue);
    WAIT_AND_COMPARE(floatValue1Spy, 1);
    QCOMPARE(client.floatValue1(), floatValue1TestValue);
    QCOMPARE(floatValue1Spy[0][0].toReal(), floatValue1TestValue);

    QSignalSpy floatValue2Spy(&client, SIGNAL(floatValue2Changed(qreal)));
    QVERIFY(floatValue2Spy.isValid());
    qreal floatValue2TestValue = 234.678;
    server.m_service.setFloatValue2(floatValue2TestValue);
    WAIT_AND_COMPARE(floatValue2Spy, 1);
    QCOMPARE(client.floatValue2(), floatValue2TestValue);
    QCOMPARE(floatValue2Spy[0][0].toReal(), floatValue2TestValue);

    QSignalSpy stringValueSpy(&client, SIGNAL(stringValueChanged(QString)));
    QVERIFY(stringValueSpy.isValid());
    QString stringValueTestValue = QString("hello test");
    server.m_service.setStringValue(stringValueTestValue);
    WAIT_AND_COMPARE(stringValueSpy, 1);
    QCOMPARE(client.stringValue(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);

    QSignalSpy contactListSpy(&client, SIGNAL(contactListChanged(QVariantList)));
    QVERIFY(contactListSpy.isValid());
    QVariantList contactListTestValue(
                    { QVariant::fromValue<Contact>(Contact("Mr A.", 20, false, "foo")),
                      QVariant::fromValue<Contact>(Contact("Mr B.", 40, true, "bar")) });
    server.m_service.setContactList(contactListTestValue);
    WAIT_AND_COMPARE(contactListSpy, 1);
    QCOMPARE(client.contactList().count(),contactListTestValue.count());
    QCOMPARE(client.contactList()[0].value<Contact>(), contactListTestValue[0].value<Contact>());
    QCOMPARE(client.contactList()[1].value<Contact>(), contactListTestValue[1].value<Contact>());
    QVariantList signalArgs = contactListSpy[0][0].toList();
    QCOMPARE(signalArgs[0].value<Contact>(), contactListTestValue[0].value<Contact>());
    QCOMPARE(signalArgs[1].value<Contact>(), contactListTestValue[1].value<Contact>());

    QSignalSpy contactSpy(&client, SIGNAL(contactChanged(Contact)));
    QVERIFY(contactSpy.isValid());
    Contact contactTestValue("Nemo", 47, true, 1);
    server.m_service.setContact(contactTestValue);
    WAIT_AND_COMPARE(contactSpy, 1);
    QCOMPARE(client.contact(), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);

    QSignalSpy weekDaySpy(&client, SIGNAL(weekDayChanged(EchoModule::WeekDay)));
    QVERIFY(weekDaySpy.isValid());
    EchoModule::WeekDay weekDayTestValue = EchoModule::Friday;
    server.m_service.setWeekDay(weekDayTestValue);
    WAIT_AND_COMPARE(weekDaySpy, 1);
    QCOMPARE(client.weekDay(), weekDayTestValue);
    QCOMPARE(weekDaySpy[0][0].value<EchoModule::WeekDay>(), weekDayTestValue);
}

void EchoQtroTest::testSlots()
{
    Server server;
    server.start();

    Echo client;
    client.startAutoDiscovery();


    //wait until the client has connected and initial values are set
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test slots by calling them on the client
    QSignalSpy echoSpy(&server.m_service, SIGNAL(echoSlotCalled(const QString&)));
    QVERIFY(echoSpy.isValid());
    QString echoTestValue("this will be echoed");
    QIviPendingReply<QString> echoReply = client.echo(echoTestValue);
    QSignalSpy echoReplySpy(echoReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(echoReplySpy, 1);
    QCOMPARE(echoReply.reply(), echoTestValue);
    QCOMPARE(echoSpy.count(),1);
    QCOMPARE(echoSpy[0][0].toString(), echoTestValue);

    QSignalSpy idSpy(&server.m_service, SIGNAL(idSlotCalled()));
    QVERIFY(idSpy.isValid());
    QIviPendingReply<QString> idReply = client.id();
    QSignalSpy idReplySpy(idReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(idReplySpy, 1);
    QCOMPARE(idReply.reply(), server.m_service.m_testId);
    QCOMPARE(idSpy.count(),1);

    QSignalSpy getComboSpy(&server.m_service, SIGNAL(getComboSlotCalled()));
    QVERIFY(getComboSpy.isValid());
    QIviPendingReply<Combo> comboReply = client.getCombo();
    QSignalSpy comboReplySpy(comboReply.watcher(), SIGNAL(replySuccess()));
    WAIT_AND_COMPARE(comboReplySpy, 1);
    QCOMPARE(comboReply.reply(), server.m_service.m_testCombo);
    QCOMPARE(getComboSpy.count(),1);

    QSignalSpy voidSlotSpy(&server.m_service, SIGNAL(voidSlotCalled()));
    QVERIFY(voidSlotSpy.isValid());
    client.voidSlot();
    WAIT_AND_COMPARE(voidSlotSpy, 1);

    QSignalSpy voidSlot2Spy(&server.m_service, SIGNAL(voidSlot2Called(int)));
    int voidSlot2TestValue = 776;
    QVERIFY(voidSlot2Spy.isValid());
    client.voidSlot2(voidSlot2TestValue);
    voidSlot2Spy.wait(1000);
    QCOMPARE(voidSlot2Spy.count(),1);
    QCOMPARE(voidSlot2Spy[0][0].toInt(), voidSlot2TestValue);
}

void EchoQtroTest::testMultipleSlotCalls()
{
    Server server;
    server.start();

    Echo client;
    client.startAutoDiscovery();


    //wait until the client has connected and initial values are set
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    initSpy.wait(1000);
    QCOMPARE(initSpy.count(), 1);
    QVERIFY(client.isInitialized());

    //test the pending replies by calling the same slot with 3 different values
    QSignalSpy echoSpy(&server.m_service, SIGNAL(echoSlotCalled(const QString&)));
    QVERIFY(echoSpy.isValid());
    QString echoTestValue("first");
    QString echoTestValue2("second");
    QString echoTestValue3("third");
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
    QCOMPARE(echoSpy.count(),3);
    QCOMPARE(echoSpy[0][0].toString(), echoTestValue);
    QCOMPARE(echoSpy[1][0].toString(), echoTestValue2);
    QCOMPARE(echoSpy[2][0].toString(), echoTestValue3);
}

void EchoQtroTest::testSignals()
{
    Server server;
    server.start();

    Echo client;
    client.startAutoDiscovery();


    //wait until the client has connected and initial values are set
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test custom signals (other than property notifiers) from server to client
    QSignalSpy anotherChangedSpy(&client, SIGNAL(anotherChanged(AnotherStruct)));
    QVERIFY(anotherChangedSpy.isValid());
    AnotherStruct anotherTestValue(7);
    server.m_service.anotherChanged(anotherTestValue);
    WAIT_AND_COMPARE(anotherChangedSpy, 1);
    QCOMPARE(anotherChangedSpy[0][0].value<AnotherStruct>(), anotherTestValue);

    QSignalSpy foobarSpy(&client, SIGNAL(foobar(QString)));
    QVERIFY(foobarSpy.isValid());
    QString foobarTestValue("foo and bar");
    server.m_service.foobar(foobarTestValue);
    WAIT_AND_COMPARE(foobarSpy, 1);
    QCOMPARE(foobarSpy[0][0].toString(), foobarTestValue);

    QSignalSpy somethingSpy(&client, SIGNAL(somethingHappened()));
    QVERIFY(somethingSpy.isValid());;
    server.m_service.somethingHappened();
    WAIT_AND_COMPARE(somethingSpy, 1);
}
