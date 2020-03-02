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

#include <QtTest>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>

#include <qivipendingreply.h>

#define TEST_FUNCTION_DEF(NAME, TYPE) Q_INVOKABLE QIviPendingReply<TYPE> test_##NAME(TYPE result, bool fail = false); \

#define TEST_FUNCTION(NAME, TYPE) QIviPendingReply<TYPE> TestObject::test_##NAME(TYPE result, bool fail) { \
    QIviPendingReply<TYPE> reply; \
    auto timer = new QTimer(); \
    timer->setSingleShot(true); \
    connect(timer, &QTimer::timeout, this, [reply, fail, result]() mutable { \
        if (fail) \
            reply.setFailed(); \
        else \
            reply.setSuccess(result); \
    }); \
    timer->start(10); \
    return reply; \
} \

class TestGadget
{
    Q_GADGET

public:
    TestGadget(const QString &string = QString(), int integer = -1)
        : m_string(string)
        , m_int(integer)
    {}

    bool operator==(const TestGadget &other) const
    {
        return m_string == other.m_string &&
               m_int == other.m_int;
    }

    QString m_string;
    int m_int;
};

class TestObject : public QObject
{
    Q_OBJECT

public:
    enum TestEnum {
        Value_1,
        Value_2,
        Value_3
    };
    Q_ENUM(TestEnum)

    enum TestFlag {
        TestFlag_1 = 0x1,
        TestFlag_2 = 0x2,
        TestFlag_3 = 0x4,
    };
    Q_DECLARE_FLAGS(TestFlags, TestFlag)
    Q_FLAG(TestFlags)

    TestObject(QObject *parent = nullptr)
      : QObject(parent)
    {}

    Q_INVOKABLE QIviPendingReply<void> test_void(bool fail = false);

    TEST_FUNCTION_DEF(int, int)
    TEST_FUNCTION_DEF(quint16, quint16)
    TEST_FUNCTION_DEF(float, float)
    TEST_FUNCTION_DEF(QString, QString)
    TEST_FUNCTION_DEF(QVariant, QVariant)
    TEST_FUNCTION_DEF(TestEnum, TestObject::TestEnum)
    TEST_FUNCTION_DEF(TestFlags, TestObject::TestFlags)
    TEST_FUNCTION_DEF(TestGadget, TestGadget)

    Q_INVOKABLE TestGadget createGadget(const QString& string, int integer) {
        return TestGadget(string, integer);
    }
};

Q_DECLARE_METATYPE(TestGadget)

QIviPendingReply<void> TestObject::test_void(bool fail) {
    QIviPendingReply<void> reply;
    auto timer = new QTimer();
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, [reply, fail]() mutable {
        if (fail)
            reply.setFailed();
        else
            reply.setSuccess();
    });
    timer->start(10);
    return reply;
}

TEST_FUNCTION(int, int)
TEST_FUNCTION(quint16, quint16)
TEST_FUNCTION(float, float)
TEST_FUNCTION(QString, QString)
TEST_FUNCTION(QVariant, QVariant)
TEST_FUNCTION(TestEnum, TestObject::TestEnum)
TEST_FUNCTION(TestFlags, TestObject::TestFlags)
TEST_FUNCTION(TestGadget, TestGadget)

class tst_QIviPendingReply : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    // This needs to be the first test, as it tests calling a function which returns QIviPendingReply
    // from QML and for this the basic types have to be registered before automatically.
    void testSuccess_qml();
    void testSuccess();
    void testSuccessFromQml();
    void testConversion_qml();
    void testFailed();
    void testFailed_qml();
    void testEmittingTwice();
    void testInvalidReply();
    void testThen_errors();
    void testTypeError();
    void testThenLater();

private:
    template <typename T> void test(const QIviPendingReply<T> &reply, bool failed, T expectedResult = T());
    template <typename T> void test_helper(const QIviPendingReply<T> &reply, bool failed);
    template <typename T> void testQml(TestObject *testObject, const QByteArray qmlFunction, bool failed, T expectedResult = T());
    template <typename T> void qml_helper(TestObject *testObject, const QByteArray qmlFunction, bool failed,
                                          QVariant &result, QVariant &watcherValue, QVariant &replyValue);
};

void tst_QIviPendingReply::initTestCase()
{
    qIviRegisterPendingReplyType<TestObject::TestEnum>();
    qIviRegisterPendingReplyType<TestObject::TestFlags>();
    qIviRegisterPendingReplyType<TestGadget>();
}

template <typename T> void tst_QIviPendingReply::test(const QIviPendingReply<T> &reply, bool failed, T expectedResult)
{
    QCOMPARE(reply.reply(), T());
    test_helper<T>(reply, failed);
    QCOMPARE(reply.reply(), expectedResult);
}

template <typename T> void tst_QIviPendingReply::test_helper(const QIviPendingReply<T> &reply, bool failed)
{
    QVERIFY(reply.isValid());
    QVERIFY(reply.watcher()->isValid());
    QVERIFY(!reply.isResultAvailable());
    QVERIFY(!reply.isSuccessful());
    QSignalSpy successSpy(reply.watcher(), &QIviPendingReplyWatcher::replySuccess);
    QSignalSpy valueChangedSpy(reply.watcher(), &QIviPendingReplyWatcher::valueChanged);
    QSignalSpy failedSpy(reply.watcher(), &QIviPendingReplyWatcher::replyFailed);
    if (failed) {
        failedSpy.wait();
        QCOMPARE(successSpy.count(), 0);
        QCOMPARE(failedSpy.count(), 1);
    } else {
        successSpy.wait();
        QCOMPARE(successSpy.count(), 1);
        QCOMPARE(failedSpy.count(), 0);
    }

    QCOMPARE(valueChangedSpy.count(), 1);
    QVERIFY(reply.isResultAvailable());
    QCOMPARE(reply.isSuccessful(), !failed);

    if (!failed)
        QCOMPARE(reply.value(), valueChangedSpy.at(0).at(0));
}

template <typename T> void tst_QIviPendingReply::testQml(TestObject *testObject, const QByteArray qmlFunction, bool failed, T expectedResult)
{
    QVariant result;
    QVariant watcherValue;
    QVariant replyValue;

    qml_helper<T>(testObject, qmlFunction, failed, result, watcherValue, replyValue);

    if (failed) {
        QCOMPARE(result, QVariant());
        QCOMPARE(watcherValue, QVariant());
        QCOMPARE(replyValue, QVariant());
    } else {
        QCOMPARE(result.value<T>(), expectedResult);
        QCOMPARE(watcherValue.value<T>(), expectedResult);
        QCOMPARE(replyValue.value<T>(), expectedResult);
    }
}

template <typename T> void tst_QIviPendingReply::qml_helper(TestObject *testObject, const QByteArray qmlFunction, bool failed,
                                                            QVariant &result, QVariant &watcherValue, QVariant &replyValue)
{
    qWarning() << "TEST " << qmlFunction;
    QQmlEngine engine;
    qmlRegisterType<TestObject>("testobject", 1, 0, "TestObject");
    engine.rootContext()->setContextProperty("testObject", testObject);

    QByteArray qml ("import QtQuick 2.0; \n\
                     import testobject 1.0; \n\
                     QtObject { \n\
                         property var result; \n\
                         property var reply; \n\
                         property var replyValue; \n\
                         property var replyValid; \n\
                         property var replySuccess; \n\
                         property var replyResultAvailable; \n\
                         property var watcherValue: reply.watcher.value \n\
                         property var watcherValid: reply.watcher.valid \n\
                         property var watcherSuccess: reply.watcher.success \n\
                         property var watcherResultAvailable: reply.watcher.resultAvailable \n\
                         property bool callBackCalled: false; \n\
                         property bool success: false; \n\
                         Component.onCompleted: { \n\
                             reply = testObject.FUNCTION \n\
                             replyValue = reply.value \n\
                             replyValid = reply.valid \n\
                             replySuccess = reply.success \n\
                             replyResultAvailable = reply.resultAvailable \n\
                             \n\
                             reply.then( \n\
                                 function(value) {resultFunction(value, true)}, \n\
                                 function() {resultFunction(undefined, false)} \n\
                             ) \n\
                         }\n\
                         \n\
                         function resultFunction(value, su) { \n\
                             success = su \n\
                             result = value \n\
                             replyValue = reply.value \n\
                             replyValid = reply.valid \n\
                             replySuccess = reply.success \n\
                             replyResultAvailable = reply.resultAvailable \n\
                             callBackCalled = true \n\
                         } \n\
                     }");
    qml.replace("FUNCTION", qmlFunction);
    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    QCOMPARE(obj->property("watcherValue"), QVariant());
    QCOMPARE(obj->property("watcherValid").toBool(), true);
    QCOMPARE(obj->property("watcherSuccess").toBool(), false);
    QCOMPARE(obj->property("watcherResultAvailable").toBool(), false);
    QCOMPARE(obj->property("replyValue"), QVariant());
    QCOMPARE(obj->property("replyValid").toBool(), true);
    QCOMPARE(obj->property("replySuccess").toBool(), false);
    QCOMPARE(obj->property("replyResultAvailable").toBool(), false);

    //Wait until the reply is ready
    QTest::qWait(100);

    QVERIFY(obj->property("callBackCalled").toBool());
    QCOMPARE(obj->property("success").toBool(), !failed);
    QCOMPARE(obj->property("watcherValid").toBool(), true);
    QCOMPARE(obj->property("watcherSuccess").toBool(), !failed);
    QCOMPARE(obj->property("watcherResultAvailable").toBool(), true);
    QCOMPARE(obj->property("replyValid").toBool(), true);
    QCOMPARE(obj->property("replySuccess").toBool(), !failed);
    QCOMPARE(obj->property("replyResultAvailable").toBool(), true);

    result = obj->property("result");
    watcherValue = obj->property("watcherValue");
    replyValue = obj->property("replyValue");
}

void tst_QIviPendingReply::testSuccess_qml()
{
    TestObject testObject;

    QVariant result;
    QVariant watcherValue;
    QVariant replyValue;
    qml_helper<void>(&testObject, "test_void()", false, result, watcherValue, replyValue);
    testQml<int>(&testObject, "test_int(500)", false, 500);
    testQml<quint16>(&testObject, "test_quint16(1234)", false, 1234);
    testQml<float>(&testObject, "test_float(-1234.56)", false, -1234.56);
    testQml<QString>(&testObject, "test_QString('HELLO')", false, "HELLO");
    testQml<TestObject::TestEnum>(&testObject, "test_TestEnum(TestObject.Value_2)", false, TestObject::Value_2);
    testQml<TestObject::TestFlags>(&testObject, "test_TestFlags(TestObject.TestFlag_2 | TestObject.TestFlag_1)", false,
                                           TestObject::TestFlags(TestObject::TestFlag_2 | TestObject::TestFlag_1));
    testQml<TestGadget>(&testObject, "test_TestGadget(testObject.createGadget('FOO', 5))", false, TestGadget("FOO", 5));
}

void tst_QIviPendingReply::testSuccess()
{
    TestObject testObject;

    test_helper<void>(testObject.test_void(), false);
    test<int>(testObject.test_int(500), false, 500);
    test<quint16>(testObject.test_quint16(1234), false, 1234);
    test<float>(testObject.test_float(-1234.56), false, -1234.56);
    test<QString>(testObject.test_QString("HELLO"), false, "HELLO");
    test<QVariant>(testObject.test_QVariant(QVariant("VARIANT")), false, QVariant("VARIANT"));
    test<TestObject::TestEnum>(testObject.test_TestEnum(TestObject::Value_2), false, TestObject::Value_2);
    test<TestObject::TestFlags>(testObject.test_TestFlags(TestObject::TestFlags(TestObject::TestFlag_2 | TestObject::TestFlag_1)), false,
                                TestObject::TestFlags(TestObject::TestFlag_2 | TestObject::TestFlag_1));
    test<TestGadget>(testObject.test_TestGadget(TestGadget("FOO", 5)), false, TestGadget("FOO", 5));
}

void tst_QIviPendingReply::testSuccessFromQml()
{
    // Instead of using the PendingReply in QML and react on the result using then()
    // we test here to set the result using the setSuccess function

    QIviPendingReply<void> voidReply;
    QIviPendingReply<int> intReply;

    QVERIFY(!voidReply.isResultAvailable());
    QVERIFY(!intReply.isResultAvailable());

    QQmlEngine engine;
    engine.rootContext()->setContextProperty("voidReply", QVariant::fromValue(QIviPendingReplyBase(voidReply)));
    engine.rootContext()->setContextProperty("intReply", QVariant::fromValue(QIviPendingReplyBase(intReply)));

    QByteArray qml ("import QtQuick 2.0; \n\
                     QtObject { \n\
                         Component.onCompleted: { \n\
                             voidReply.setSuccess(true) \n\
                             intReply.setSuccess(5) \n\
                         }\n\
                     }");
    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    QVERIFY(voidReply.isResultAvailable());
    QVERIFY(voidReply.isSuccessful());

    QVERIFY(intReply.isResultAvailable());
    QVERIFY(intReply.isSuccessful());
    QCOMPARE(intReply.value(), 5);
}

void tst_QIviPendingReply::testConversion_qml()
{
    TestObject testObject;

    //Use convertable types an check that the conversion works
    //int->float
    testQml<float>(&testObject, "test_float(-1234)", false, -1234);
    //int->enum
    testQml<TestObject::TestEnum>(&testObject, "test_TestEnum(1)", false, TestObject::Value_2);
    //int->flag
    testQml<TestObject::TestFlags>(&testObject, "test_TestFlags(0x3)", false,
                                           TestObject::TestFlags(TestObject::TestFlag_2 | TestObject::TestFlag_1));
}

void tst_QIviPendingReply::testFailed()
{
    TestObject testObject;

    test_helper<void>(testObject.test_void(true), true);
    test<int>(testObject.test_int(500, true), true);
    test<quint16>(testObject.test_quint16(1234, true), true);
    test<float>(testObject.test_float(-1234.56, true), true);
    test<QString>(testObject.test_QString("HELLO", true), true);
    test<TestObject::TestEnum>(testObject.test_TestEnum(TestObject::Value_2, true), true);
    test<TestObject::TestFlags>(testObject.test_TestFlags(TestObject::TestFlags(TestObject::TestFlag_2 | TestObject::TestFlag_1), true), true);
    test<TestGadget>(testObject.test_TestGadget(TestGadget("FOO", 5), true), true);
}

void tst_QIviPendingReply::testFailed_qml()
{
    TestObject testObject;
    QVariant result;
    QVariant watcherValue;
    QVariant replyValue;
    qml_helper<void>(&testObject, "test_void(true)", true, result, watcherValue, replyValue);
    testQml<int>(&testObject, "test_int(500, true)", true);
    testQml<quint16>(&testObject, "test_quint16(1234, true)", true);
    testQml<float>(&testObject, "test_float(-1234.56, true)", true);
    testQml<QString>(&testObject, "test_QString('HELLO', true)", true);
    testQml<TestObject::TestEnum>(&testObject, "test_TestEnum(TestObject.Value_2, true)", true);
    testQml<TestObject::TestFlags>(&testObject, "test_TestFlags(TestObject.TestFlag_2 | TestObject.TestFlag_1, true)", true);
    testQml<TestGadget>(&testObject, "test_TestGadget(testObject.createGadget('FOO', 5), true)", true);
}

void tst_QIviPendingReply::testEmittingTwice()
{
    QIviPendingReply<QString> reply;
    QSignalSpy successSpy(reply.watcher(), &QIviPendingReplyWatcher::replySuccess);
    reply.setSuccess(QStringLiteral("TEST"));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();

    //Verify the signal is not emitted again
    QTest::ignoreMessage(QtWarningMsg, "Result is already set. Ignoring request");
    reply.setSuccess(QStringLiteral("TEST2"));
    QCOMPARE(successSpy.count(), 0);
}

void tst_QIviPendingReply::testInvalidReply()
{
    QIviPendingReplyBase reply;
    QVERIFY(!reply.isValid());
}

void tst_QIviPendingReply::testThen_errors()
{
    QIviPendingReply<QString> reply;
    QJSEngine engine;
    QJSValue value = engine.newObject();
    value.setProperty("foo", "bar");

    QTest::ignoreMessage(QtWarningMsg, "The success functor is not callable");
    reply.then(value);

    QTest::ignoreMessage(QtWarningMsg, "The failed functor is not callable");
    reply.then(QJSValue(), value);

//    Enable me once know how we could test this condition.
//    QJSValue functor = engine.evaluate("function(value){}");
//    QTest::ignoreMessage(QtWarningMsg, "Couldn't access the current QJSEngine. The given callbacks will not be called without a valid QJSEngine");
//    reply.then(functor);
}

void tst_QIviPendingReply::testTypeError()
{
    //Use convertable types an check that the conversion works
    //wrong data type
    QIviPendingReply<float> floatReply;
    QTest::ignoreMessage(QtWarningMsg, "Expected: float but got QString");
    static_cast<QIviPendingReplyBase*>(&floatReply)->setSuccess(QVariant("TEST_STRING"));
    QVERIFY(!floatReply.isResultAvailable());
    //Enum out of scope
    QIviPendingReply<TestObject::TestEnum> enumReply;
    QTest::ignoreMessage(QtWarningMsg, "Enum value out of range");
    static_cast<QIviPendingReplyBase*>(&enumReply)->setSuccess(QVariant(10));
    QVERIFY(!enumReply.isResultAvailable());
}

void tst_QIviPendingReply::testThenLater()
{
    bool failed = false;
    TestObject testObject;
    QQmlEngine engine;
    engine.rootContext()->setContextProperty("testObject", &testObject);

    QByteArray qml ("import QtQuick 2.0; \n\
                     import testobject 1.0; \n\
                     QtObject { \n\
                         property var result; \n\
                         property var reply; \n\
                         property var replyValue; \n\
                         property var replyValid; \n\
                         property var replySuccess; \n\
                         property var replyResultAvailable; \n\
                         property bool callBackCalled: false; \n\
                         property bool success: false; \n\
                         Component.onCompleted: { \n\
                             reply = testObject.test_QString('TEST_STRING') \n\
                             replyValue = reply.value \n\
                             replyValid = reply.valid \n\
                             replySuccess = reply.success \n\
                             replyResultAvailable = reply.resultAvailable \n\
                         }\n\
                         \n\
                         function callThen() { \n\
                             reply.then( \n\
                                 function(value) {resultFunction(value, true)}, \n\
                                 function() {resultFunction(undefined, false)} \n\
                             ) \n\
                         } \n \
                         \n \
                         function resultFunction(value, su) { \n\
                             success = su \n\
                             result = value \n\
                             replyValue = reply.value \n\
                             replyValid = reply.valid \n\
                             replySuccess = reply.success \n\
                             replyResultAvailable = reply.resultAvailable \n\
                             callBackCalled = true \n\
                         } \n\
                     }");
    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    QCOMPARE(obj->property("replyValue"), QVariant());
    QCOMPARE(obj->property("replyValid").toBool(), true);
    QCOMPARE(obj->property("replySuccess").toBool(), false);
    QCOMPARE(obj->property("replyResultAvailable").toBool(), false);

    //Wait until the reply is ready
    QTest::qWait(100);

    //Check that the properties are not updated yet, as no callback is registered
    QCOMPARE(obj->property("replyValue"), QVariant());
    QCOMPARE(obj->property("replyValid").toBool(), true);
    QCOMPARE(obj->property("replySuccess").toBool(), false);
    QCOMPARE(obj->property("replyResultAvailable").toBool(), false);

    //Connect the callbacks
    QMetaObject::invokeMethod(obj.data(), "callThen");

    //Check that after the callbacks are connected they are also executed
    QVERIFY(obj->property("callBackCalled").toBool());
    QCOMPARE(obj->property("success").toBool(), !failed);
    QCOMPARE(obj->property("replyValid").toBool(), true);
    QCOMPARE(obj->property("replySuccess").toBool(), !failed);
    QCOMPARE(obj->property("replyResultAvailable").toBool(), true);

    if (failed) {
        QCOMPARE(obj->property("result"), QVariant());
        QCOMPARE(obj->property("replyValue"), QVariant());
    } else {
        QCOMPARE(obj->property("result").value<QString>(), QString("TEST_STRING"));
        QCOMPARE(obj->property("replyValue").value<QString>(), QString("TEST_STRING"));
    }
}

QTEST_MAIN(tst_QIviPendingReply)

#include "tst_qivipendingreply.moc"
