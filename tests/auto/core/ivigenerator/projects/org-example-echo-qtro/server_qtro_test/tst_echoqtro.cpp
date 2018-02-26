/****************************************************************************
**
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

EchoQtroTest::EchoQtroTest()
    :QObject()
{
    EchoModule::registerTypes();

    QCoreApplication::setLibraryPaths({QDir::currentPath()});
}

void EchoQtroTest::cleanup()
{
}

void EchoQtroTest::testInitServer()
{
    Server server;
    QVERIFY(server.start());

    //test initial values
    QCOMPARE(server.m_service.lastMessage(), QString());
    QCOMPARE(server.m_service.intValue(), 0);
    QCOMPARE(server.m_service.floatValue1(), qreal(0.0));
    QCOMPARE(server.m_service.floatValue2(), qreal(0.0));
    QCOMPARE(server.m_service.stringValue(), QString());
    QCOMPARE(server.m_service.contactList(), QVariantList());
    QCOMPARE(server.m_service.contact(), Contact());
    QCOMPARE(server.m_service.weekDay(), EchoModule::DaysOfTheWeek());
}

void EchoQtroTest::testInitClient()
{
    QTest::ignoreMessage(QtWarningMsg, "QtRO cannot convert the property weekDay to type EchoModule::DaysOfTheWeek");
    Echo client;
    QVERIFY(client.startAutoDiscovery()==QIviAbstractFeature::ProductionBackendLoaded);

    QCOMPARE(client.lastMessage(), QString());
    QCOMPARE(client.intValue(), 0);
    QCOMPARE(client.floatValue1(), qreal(0.0));
    QCOMPARE(client.floatValue2(), qreal(0.0));
    QCOMPARE(client.stringValue(), QString());
    QCOMPARE(client.contactList(), QVariantList());
    QCOMPARE(client.contact(), Contact());
    QCOMPARE(client.weekDay(), EchoModule::DaysOfTheWeek());
}

void EchoQtroTest::testClient2Server()
{
    Server server;
    QVERIFY(server.start());

    QTest::ignoreMessage(QtWarningMsg, "QtRO cannot convert the property weekDay to type EchoModule::DaysOfTheWeek");
    Echo client;
    QVERIFY(client.startAutoDiscovery()==QIviAbstractFeature::ProductionBackendLoaded);


    //hack that makes sure we wait until the client is connected to the server
    server.m_service.setFloatValue1(1.0);
    QSignalSpy spy(&client, SIGNAL(floatValue1Changed(qreal)));
    spy.wait(1000);
    // end of hack


    //test properties
    QSignalSpy intValueSpy(&server.m_service, SIGNAL(intValueChanged(int)));
    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    client.setIntValue(intValueTestValue);
    intValueSpy.wait(1000);
    QCOMPARE(server.m_service.intValue(), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);

    QSignalSpy floatValue1Spy(&server.m_service, SIGNAL(floatValue1Changed(qreal)));
    QVERIFY(floatValue1Spy.isValid());
    qreal floatValue1TestValue = 1234.5678;
    client.setFloatValue1(floatValue1TestValue);
    floatValue1Spy.wait(1000);
    QCOMPARE(server.m_service.floatValue1(), floatValue1TestValue);
    QCOMPARE(floatValue1Spy[0][0].toReal(), floatValue1TestValue);

    QSignalSpy floatValue2Spy(&server.m_service, SIGNAL(floatValue2Changed(qreal)));
    QVERIFY(floatValue2Spy.isValid());
    qreal floatValue2TestValue = 3.1415;
    client.setFloatValue2(floatValue2TestValue);
    floatValue2Spy.wait(1000);
    QCOMPARE(server.m_service.floatValue2(), floatValue2TestValue);
    QCOMPARE(floatValue2Spy[0][0].toReal(), floatValue2TestValue);

    QSignalSpy stringValueSpy(&server.m_service, SIGNAL(stringValueChanged(QString)));
    QVERIFY(stringValueSpy.isValid());
    QString stringValueTestValue = "hello test";
    client.setStringValue(stringValueTestValue);
    stringValueSpy.wait(1000);
    QCOMPARE(server.m_service.stringValue(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);

    QSignalSpy contactListSpy(&server.m_service, SIGNAL(contactListChanged(QVariantList)));
    QVERIFY(contactListSpy.isValid());
    QVariantList contactListTestValue(
                    { QVariant::fromValue<Contact>(Contact("Mr A.", 20, false)),
                      QVariant::fromValue<Contact>(Contact("Mr B.", 40, true)) });
    client.setContactList(contactListTestValue);
    contactListSpy.wait(1000);
    QCOMPARE(server.m_service.contactList().count(),contactListTestValue.count());
    QCOMPARE(server.m_service.contactList()[0].value<Contact>(), contactListTestValue[0].value<Contact>());
    QCOMPARE(server.m_service.contactList()[1].value<Contact>(), contactListTestValue[1].value<Contact>());
    QVariantList signalArgs = contactListSpy[0][0].toList();
    QCOMPARE(signalArgs[0].value<Contact>(), contactListTestValue[0].value<Contact>());
    QCOMPARE(signalArgs[1].value<Contact>(), contactListTestValue[1].value<Contact>());

    QSignalSpy contactSpy(&server.m_service, SIGNAL(contactChanged(Contact)));
    QVERIFY(contactSpy.isValid());
    Contact contactTestValue("Nemo", 47, true);
    client.setContact(contactTestValue);
    contactSpy.wait(1000);
    QCOMPARE(server.m_service.contact(), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);

    QSignalSpy weekDaySpy(&server.m_service, SIGNAL(weekDayChanged(EchoModule::DaysOfTheWeek)));
    QVERIFY(weekDaySpy.isValid());
    EchoModule::DaysOfTheWeek weekDayTestValue = EchoModule::Wednesday;
    client.setWeekDay(weekDayTestValue);
    weekDaySpy.wait(1000);
    QCOMPARE(server.m_service.weekDay(), weekDayTestValue);
    QCOMPARE(weekDaySpy[0][0].value<EchoModule::DaysOfTheWeek>(), weekDayTestValue);
}

void EchoQtroTest::testServer2Client()
{
    Server server;
    QVERIFY(server.start());

    QTest::ignoreMessage(QtWarningMsg, "QtRO cannot convert the property weekDay to type EchoModule::DaysOfTheWeek");
    Echo client;
    QVERIFY(client.startAutoDiscovery()==QIviAbstractFeature::ProductionBackendLoaded);


    //hack that makes sure we wait until the client is connected to the server
    server.m_service.setFloatValue1(1.0);
    QSignalSpy spy(&client, SIGNAL(floatValue1Changed(qreal)));
    spy.wait(1000);
    // end of hack


    //test properties
    QSignalSpy intValueSpy(&client, SIGNAL(intValueChanged(int)));
    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    server.m_service.setIntValue(intValueTestValue);
    QVERIFY(intValueSpy.wait(1000));
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);

    QSignalSpy floatValue1Spy(&client, SIGNAL(floatValue1Changed(qreal)));
    QVERIFY(floatValue1Spy.isValid());
    qreal floatValue1TestValue = 1234.5678;
    server.m_service.setFloatValue1(floatValue1TestValue);
    floatValue1Spy.wait(1000);
    QCOMPARE(client.floatValue1(), floatValue1TestValue);
    QCOMPARE(floatValue1Spy[0][0].toReal(), floatValue1TestValue);

    QSignalSpy floatValue2Spy(&client, SIGNAL(floatValue2Changed(qreal)));
    QVERIFY(floatValue2Spy.isValid());
    qreal floatValue2TestValue = 234.678;
    server.m_service.setFloatValue2(floatValue2TestValue);
    floatValue2Spy.wait(1000);
    QCOMPARE(client.floatValue2(), floatValue2TestValue);
    QCOMPARE(floatValue2Spy[0][0].toReal(), floatValue2TestValue);

    QSignalSpy stringValueSpy(&client, SIGNAL(stringValueChanged(QString)));
    QVERIFY(stringValueSpy.isValid());
    QString stringValueTestValue = QString("hello test");
    server.m_service.setStringValue(stringValueTestValue);
    stringValueSpy.wait(1000);
    QCOMPARE(client.stringValue(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);

    QSignalSpy contactListSpy(&client, SIGNAL(contactListChanged(QVariantList)));
    QVERIFY(contactListSpy.isValid());
    QVariantList contactListTestValue(
                    { QVariant::fromValue<Contact>(Contact("Mr A.", 20, false)),
                      QVariant::fromValue<Contact>(Contact("Mr B.", 40, true)) });
    server.m_service.setContactList(contactListTestValue);
    contactListSpy.wait(1000);
    QCOMPARE(client.contactList().count(),contactListTestValue.count());
    QCOMPARE(client.contactList()[0].value<Contact>(), contactListTestValue[0].value<Contact>());
    QCOMPARE(client.contactList()[1].value<Contact>(), contactListTestValue[1].value<Contact>());
    QVariantList signalArgs = contactListSpy[0][0].toList();
    QCOMPARE(signalArgs[0].value<Contact>(), contactListTestValue[0].value<Contact>());
    QCOMPARE(signalArgs[1].value<Contact>(), contactListTestValue[1].value<Contact>());

    QSignalSpy contactSpy(&client, SIGNAL(contactChanged(Contact)));
    QVERIFY(contactSpy.isValid());
    Contact contactTestValue("Nemo", 47, true);
    server.m_service.setContact(contactTestValue);
    contactSpy.wait(1000);
    QCOMPARE(client.contact(), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);

    QSignalSpy weekDaySpy(&client, SIGNAL(weekDayChanged(EchoModule::DaysOfTheWeek)));
    QVERIFY(weekDaySpy.isValid());
    EchoModule::DaysOfTheWeek weekDayTestValue = EchoModule::Wednesday;
    server.m_service.setWeekDay(weekDayTestValue);
    weekDaySpy.wait(1000);
    QCOMPARE(client.weekDay(), weekDayTestValue);
    QCOMPARE(weekDaySpy[0][0].value<EchoModule::DaysOfTheWeek>(), weekDayTestValue);
}


