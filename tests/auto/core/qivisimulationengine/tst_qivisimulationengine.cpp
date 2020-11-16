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
#include <QIviSimulationEngine>
#include <QIviPendingReply>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QScopedPointer>
#include <QJsonDocument>

#include <private/qivisimulationglobalobject_p.h>

class BaseClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int propertyInBase READ propertyInBase WRITE setPropertyInBase NOTIFY propertyInBaseChanged)
    Q_PROPERTY(bool readOnlyInBase READ readOnlyInBase NOTIFY readOnlyInBaseChanged)
    Q_PROPERTY(long constantInBase READ constantInBase CONSTANT)

public:
    int propertyInBase() const { return m_propertyInBase; }
    bool readOnlyInBase() const { return m_readOnlyInBase; }
    long constantInBase() const { return m_constantInBase; }

public slots:
    void setPropertyInBase(int propertyInBase)
    {
        if (m_propertyInBase == propertyInBase)
            return;

        m_propertyInBase = propertyInBase;
        emit propertyInBaseChanged(m_propertyInBase);
    }

    void setReadOnlyInBase(bool readOnlyInBase)
    {
        if (m_readOnlyInBase == readOnlyInBase)
            return;

        m_readOnlyInBase = readOnlyInBase;
        emit readOnlyInBaseChanged(m_readOnlyInBase);
    }


signals:
    void propertyInBaseChanged(int propertyInBase);
    void readOnlyInBaseChanged(bool readOnlyInBase);
    void signalInBase(bool value = false);

private:
    int m_propertyInBase = -1;
    bool m_readOnlyInBase = false;
    long m_constantInBase = -1;
};

class DerivedClass : public BaseClass
{
    Q_OBJECT
    Q_PROPERTY(int propertyInDerived READ propertyInDerived WRITE setPropertyInDerived NOTIFY propertyInDerivedChanged)
    Q_PROPERTY(bool readOnlyInDerived READ readOnlyInDerived NOTIFY readOnlyInDerivedChanged)
    Q_PROPERTY(long constantInDerived READ constantInDerived CONSTANT)
    Q_PROPERTY(bool complexPropertyInDerived READ complexPropertyInDerived WRITE setComplexPropertyInDerived NOTIFY signalInBase)

public:
    int propertyInDerived() const { return m_propertyInDerived; }
    bool readOnlyInDerived() const { return m_readOnlyInDerived; }
    long constantInDerived() const { return m_constantInDerived; }
    bool complexPropertyInDerived() const { return m_complexPropertyInDerived; }

public slots:
    void setPropertyInDerived(int propertyInDerived)
    {
        if (m_propertyInDerived == propertyInDerived)
            return;

        m_propertyInDerived = propertyInDerived;
        emit propertyInDerivedChanged(m_propertyInDerived);
    }

    void setReadOnlyInDerived(bool readOnlyInDerived)
    {
        if (m_readOnlyInDerived == readOnlyInDerived)
            return;

        m_readOnlyInDerived = readOnlyInDerived;
        emit readOnlyInDerivedChanged(m_readOnlyInDerived);
    }

    void setComplexPropertyInDerived(bool complexPropertyInDerived)
    {
        if (m_complexPropertyInDerived == complexPropertyInDerived)
            return;

        m_complexPropertyInDerived = complexPropertyInDerived;
        emit signalInBase(complexPropertyInDerived);
    }

signals:
    void propertyInDerivedChanged(int propertyInDerived);
    void readOnlyInDerivedChanged(bool readOnlyInBase);
    void signalInDerived(bool value = false);

private:
    int m_propertyInDerived = -1;
    bool m_readOnlyInDerived = false;
    long m_constantInDerived = -1;
    bool m_complexPropertyInDerived = false;
};

class SimpleAPI: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int testProperty READ testProperty WRITE setTestProperty NOTIFY testPropertyChanged)

public:
    int testProperty() const { return m_testProperty; }

public slots:
    void setTestProperty(int testProperty)
    {
        QIVI_SIMULATION_TRY_CALL(SimpleAPI, "setTestProperty", void, testProperty)

        m_callCounter++;
        if (m_testProperty == testProperty)
            return;

        m_testProperty = testProperty;
        emit testPropertyChanged(testProperty);
    }

    void simpleFunction()
    {
        QIVI_SIMULATION_TRY_CALL(SimpleAPI, "simpleFunction", void)

        m_callCounter++;
        emit simpleFunctionCalled();
    }

    void functionWithArguments(int intArgument, const QString &stringArgument)
    {
        QIVI_SIMULATION_TRY_CALL(SimpleAPI, "functionWithArguments", void, intArgument, stringArgument)

        m_callCounter++;
        emit functionWithArgumentsCalled(intArgument, stringArgument);
    }

    int functionWithReturnValue(int intArgument)
    {
        QIVI_SIMULATION_TRY_CALL(SimpleAPI, "functionWithReturnValue", int, intArgument)

        m_callCounter++;
        emit functionWithReturnValueCalled(intArgument);
        return intArgument;
    }

    QIviPendingReply<void> functionWithVoidPendingReply()
    {
        m_callCounter++;
        return QIviPendingReply<void>::createFailedReply();
    }

    QIviPendingReply<int> functionWithIntPendingReply()
    {
        m_callCounter++;
        return QIviPendingReply<int>::createFailedReply();
    }

signals:
    void testPropertyChanged(int testProperty);
    void simpleFunctionCalled();
    void functionWithArgumentsCalled(int intArgument, const QString &stringArgument);
    void functionWithReturnValueCalled(int intArgument);

public:
    int m_callCounter = 0;
    int m_testProperty = -1;
};

void verifyQml(QQmlEngine *engine, const QByteArray &qml)
{
    QQmlComponent component(engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));
}

QVariant callTestFunction(QObject* object, const QByteArray &function, QVariantList &expectedValues, const QVariant &returnValue, const QVariant &value1, const QVariant &value2)
{
    //call the testfunction
    QVariant retValue;
    QGenericReturnArgument retArgument;
    if (returnValue.isValid()) {
        retValue = QVariant(returnValue.metaType());
        retArgument = QGenericReturnArgument(retValue.typeName(), retValue.data());
    }

    if (!value1.isValid()) {
        QMetaObject::invokeMethod(object, function, retArgument);
    } else if (!value2.isValid()) {
        expectedValues.append(value1);
        QMetaObject::invokeMethod(object, function, retArgument, QGenericArgument(value1.typeName(), value1.data()));
    } else {
        expectedValues.append(value1);
        expectedValues.append(value2);
        QMetaObject::invokeMethod(object, function, retArgument, QGenericArgument(value1.typeName(), value1.data()), QGenericArgument(value2.typeName(), value2.data()));
    }

    return retValue;
}

class tst_QIviSimulationEngine : public QObject
{
    Q_OBJECT

    QVariant parseJson(const QString &json, QString &error) const;

private Q_SLOTS:
    void testUsageInCorrectEngine();

    //QIviSimulationEngine
    void testOverrideEnvVariables();
    void testLoadSimulationData_data();
    void testLoadSimulationData();

    //QML integration
    void testPropertyRead_data();
    void testPropertyRead();
    void testPropertyReadDerived_data();
    void testPropertyReadDerived();
    void testPropertyChange_data();
    void testPropertyChange();
    void testPropertyChangeDerived_data();
    void testPropertyChangeDerived();
    void testPropertyWrite_data();
    void testPropertyWrite();
    void testPropertyWriteDerived_data();
    void testPropertyWriteDerived();
    void testAnimations();

    void testFunctionCalls_data();
    void testFunctionCalls();
    void testFunctionOverride_data();
    void testFunctionOverride();
    void testCallingBaseFunction_data();
    void testCallingBaseFunction();
    void testRecursionPrevention();
    void testMultipleInstances();
};

QVariant tst_QIviSimulationEngine::parseJson(const QString &json, QString& error) const
{
    QJsonParseError pe;
    QVariantMap data = QJsonDocument::fromJson(json.toUtf8(), &pe).toVariant().toMap();
    if (pe.error != QJsonParseError::NoError)
        error = pe.errorString();

    return data;
}

void tst_QIviSimulationEngine::testUsageInCorrectEngine()
{
    QIviSimulationEngine engine;

    BaseClass testObject;
    engine.registerSimulationInstance<BaseClass>(&testObject, "TestAPI", 1, 0, "BaseClass");
    verifyQml(&engine, "import TestAPI 1.0; BaseClass {}");

    QCOMPARE(testObject.propertyInBase(), -1);
    QIviSimulationEngine engine2;
    QQmlComponent component(&engine2);
    component.setData("import QtQuick 2.0; \n\
                       import TestAPI 1.0; \n\
                       BaseClass { \n\
                            Component.onCompleted: { \n\
                                propertyInBase = 100; \n\
                            }\n\
                       } \n\
                       ", QUrl());
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(".* QML BaseClass: QIviSimulationProxy can only be used in the same Engine it is registered in"));
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    QCOMPARE(testObject.propertyInBase(), -1);
}

void tst_QIviSimulationEngine::testOverrideEnvVariables()
{
    qputenv("QTIVI_SIMULATION_OVERRIDE", "testEngine=test.qml;testEngine=invalidQml=;overrideTest=:/simple.qml");
    qputenv("QTIVI_SIMULATION_DATA_OVERRIDE", "testEngine=test.json;testEngine=invalid=;overrideTest=:/simple.json");
    QIviSimulationEngine engine(QStringLiteral("overrideTest"));

    QTest::ignoreMessage(QtWarningMsg, "Ignoring malformed override: File does not exist: 'test.json'");
    QTest::ignoreMessage(QtWarningMsg, "Ignoring malformed override: 'testEngine=invalid='");
    QTest::ignoreMessage(QtWarningMsg, "Ignoring malformed override: File does not exist: 'test.qml'");
    QTest::ignoreMessage(QtWarningMsg, "Ignoring malformed override: 'testEngine=invalidQml='");

    QTest::ignoreMessage(QtWarningMsg, "Detected matching simulation data override: overrideTest=:/simple.json");
    engine.loadSimulationData(QStringLiteral("invalid.json"));

    auto globalObject = engine.rootContext()->contextProperty(QStringLiteral("IviSimulator")).value<QIviSimulationGlobalObject*>();
    QVariant simulationData = globalObject->simulationData();
    QVERIFY(simulationData.isValid());

    QTest::ignoreMessage(QtWarningMsg, "Detected matching simulation override: overrideTest=qrc:/simple.qml");
    engine.loadSimulation(QStringLiteral("invalid.qml"));
}

void tst_QIviSimulationEngine::testLoadSimulationData_data()
{
    QTest::addColumn<QString>("simulationData");
    QTest::addColumn<QStringList>("expectedErrors");
    QTest::newRow("no such file") << "no-file.json" << QStringList("Cannot open the simulation data file no-file.json:*");
    QTest::newRow("invalid json") << ":/invalid-data.json" << QStringList({ "Error parsing the simulation data in :/invalid-data.json: unterminated array", "Error context:\n.*" });
    QTest::newRow("valid json") << ":/simple.json" << QStringList();
}

void tst_QIviSimulationEngine::testLoadSimulationData()
{
    QFETCH(QString, simulationData);
    QFETCH(QStringList, expectedErrors);

    QIviSimulationEngine engine(QStringLiteral("loadingTest"));
    for (const QString &error : expectedErrors)
        QTest::ignoreMessage(QtCriticalMsg, QRegularExpression(error));
    engine.loadSimulationData(simulationData);

    auto globalObject = engine.rootContext()->contextProperty(QStringLiteral("IviSimulator")).value<QIviSimulationGlobalObject*>();
    QCOMPARE(globalObject->simulationData().isValid(), expectedErrors.isEmpty());
}

void tst_QIviSimulationEngine::testPropertyRead_data()
{
    QTest::addColumn<QByteArray>("property");
    QTest::addColumn<QVariant>("intialValue");
    QTest::newRow("propertyInBase") << QByteArray("propertyInBase") << QVariant::fromValue(-1);
    QTest::newRow("readOnlyInBase") << QByteArray("readOnlyInBase") << QVariant::fromValue(false);
    QTest::newRow("constantInBase") << QByteArray("constantInBase") << QVariant::fromValue(-1);
}

void tst_QIviSimulationEngine::testPropertyRead()
{
    QFETCH(QByteArray, property);
    QFETCH(QVariant, intialValue);

    QIviSimulationEngine engine;

    BaseClass testObject;
    engine.registerSimulationInstance<BaseClass>(&testObject, "TestAPI", 1, 0, "BaseClass");

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     BaseClass { \n\
                         property var initialPropertyValue; \n\
                         property var bindingProperty: PROPERTY; \n\
                         Component.onCompleted: { \n\
                             initialPropertyValue = PROPERTY; \n\
                         }\n\
                     }");

    qml.replace("PROPERTY", property);
    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    //Check that reading the PROPERTY from QML works
    QCOMPARE(obj->property("initialPropertyValue"), intialValue);
    //A bound property should work as well
    QCOMPARE(obj->property("bindingProperty"), intialValue);
}

void tst_QIviSimulationEngine::testPropertyReadDerived_data()
{
    QTest::addColumn<QByteArray>("property");
    QTest::addColumn<QVariant>("intialValue");
    QTest::newRow("propertyInBase") << QByteArray("propertyInBase") << QVariant::fromValue(-1);
    QTest::newRow("readOnlyInBase") << QByteArray("readOnlyInBase") << QVariant::fromValue(false);
    QTest::newRow("constantInBase") << QByteArray("constantInBase") << QVariant::fromValue(-1);
    QTest::newRow("propertyInDerived") << QByteArray("propertyInDerived") << QVariant::fromValue(-1);
    QTest::newRow("readOnlyInDerived") << QByteArray("readOnlyInDerived") << QVariant::fromValue(false);
    QTest::newRow("constantInDerived") << QByteArray("constantInDerived") << QVariant::fromValue(-1);
    QTest::newRow("complexPropertyInDerived") << QByteArray("complexPropertyInDerived") << QVariant::fromValue(false);
}

void tst_QIviSimulationEngine::testPropertyReadDerived()
{
    QFETCH(QByteArray, property);
    QFETCH(QVariant, intialValue);

    QIviSimulationEngine engine;

    DerivedClass testObject;
    engine.registerSimulationInstance<DerivedClass>(&testObject, "TestAPI", 1, 0, "DerivedClass");

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     DerivedClass { \n\
                         property var initialPropertyValue; \n\
                         property var bindingProperty: PROPERTY; \n\
                         Component.onCompleted: { \n\
                             initialPropertyValue = PROPERTY; \n\
                         }\n\
                     }");

    qml.replace("PROPERTY", property);
    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    //Check that reading the PROPERTY from QML works
    QCOMPARE(obj->property("initialPropertyValue"), intialValue);
    //A bound property should work as well
    QCOMPARE(obj->property("bindingProperty"), intialValue);
}

void tst_QIviSimulationEngine::testPropertyChange_data()
{
    QTest::addColumn<QByteArray>("property");
    QTest::addColumn<QByteArray>("slot");
    QTest::addColumn<QByteArray>("setter");
    QTest::addColumn<QVariant>("value");
    QTest::newRow("propertyInBase") << QByteArray("propertyInBase") << QByteArray("onPropertyInBaseChanged") << QByteArray("setPropertyInBase") << QVariant::fromValue(10);
    QTest::newRow("readOnlyInBase") << QByteArray("readOnlyInBase")<< QByteArray("onReadOnlyInBaseChanged")<< QByteArray("setReadOnlyInBase") << QVariant::fromValue(true);
}

void tst_QIviSimulationEngine::testPropertyChange()
{
    QFETCH(QByteArray, property);
    QFETCH(QByteArray, slot);
    QFETCH(QByteArray, setter);
    QFETCH(QVariant, value);

    QIviSimulationEngine engine;

    BaseClass testObject;
    engine.registerSimulationInstance<BaseClass>(&testObject, "TestAPI", 1, 0, "BaseClass");

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     BaseClass { \n\
                         property var updatedPropertyValue; \n\
                         property var bindingProperty: PROPERTY; \n\
                         SLOT: { updatedPropertyValue = PROPERTY } \n\
                     }");

    qml.replace("PROPERTY", property);
    qml.replace("SLOT", slot);
    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    //call the setter
    QMetaObject::invokeMethod(&testObject, setter, QGenericArgument(value.typeName(), value.data()));

    //Check the slot in QML was called and that the new property is updated
    QCOMPARE(obj->property("updatedPropertyValue"), value);
    //A bound property should work as well
    QCOMPARE(obj->property("bindingProperty"), value);
}

void tst_QIviSimulationEngine::testPropertyChangeDerived_data()
{
    QTest::addColumn<QByteArray>("property");
    QTest::addColumn<QByteArray>("slot");
    QTest::addColumn<QByteArray>("setter");
    QTest::addColumn<QVariant>("value");
    QTest::newRow("propertyInBase") << QByteArray("propertyInBase") << QByteArray("onPropertyInBaseChanged") << QByteArray("setPropertyInBase") << QVariant::fromValue(10);
    QTest::newRow("readOnlyInBase") << QByteArray("readOnlyInBase")<< QByteArray("onReadOnlyInBaseChanged")<< QByteArray("setReadOnlyInBase") << QVariant::fromValue(true);
    QTest::newRow("propertyInDerived") << QByteArray("propertyInDerived") << QByteArray("onPropertyInDerivedChanged") << QByteArray("setPropertyInDerived") << QVariant::fromValue(10);
    QTest::newRow("readOnlyInDerived") << QByteArray("readOnlyInDerived") << QByteArray("onReadOnlyInDerivedChanged") << QByteArray("setReadOnlyInDerived") << QVariant::fromValue(true);
    QTest::newRow("complexPropertyInDerived") << QByteArray("complexPropertyInDerived") << QByteArray("onComplexPropertyInDerivedChanged") << QByteArray("setComplexPropertyInDerived") << QVariant::fromValue(true);
}

void tst_QIviSimulationEngine::testPropertyChangeDerived()
{
    QFETCH(QByteArray, property);
    QFETCH(QByteArray, slot);
    QFETCH(QByteArray, setter);
    QFETCH(QVariant, value);

    QIviSimulationEngine engine;

    DerivedClass testObject;
    engine.registerSimulationInstance<DerivedClass>(&testObject, "TestAPI", 1, 0, "DerivedClass");

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     DerivedClass { \n\
                         property var updatedPropertyValue; \n\
                         property var bindingProperty: PROPERTY; \n\
                         SLOT: { updatedPropertyValue = PROPERTY } \n\
                     }");

    qml.replace("PROPERTY", property);
    qml.replace("SLOT", slot);
    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    //call the setter
    QMetaObject::invokeMethod(&testObject, setter, QGenericArgument(value.typeName(), value.data()));

    //Check the slot in QML was called and that the new property is updated
    QCOMPARE(obj->property("updatedPropertyValue"), value);
    //A bound property should work as well
    QCOMPARE(obj->property("bindingProperty"), value);
}

void tst_QIviSimulationEngine::testPropertyWrite_data()
{
    QTest::addColumn<QByteArray>("property");
    QTest::addColumn<QByteArray>("slot");
    QTest::addColumn<QVariant>("value");
    QTest::newRow("propertyInBase") << QByteArray("propertyInBase") << QByteArray("onPropertyInBaseChanged") << QVariant::fromValue(10);
}

void tst_QIviSimulationEngine::testPropertyWrite()
{
    QFETCH(QByteArray, property);
    QFETCH(QByteArray, slot);
    QFETCH(QVariant, value);

    QIviSimulationEngine engine;

    BaseClass testObject;
    engine.registerSimulationInstance<BaseClass>(&testObject, "TestAPI", 1, 0, "BaseClass");

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     BaseClass { \n\
                         property var updatedPropertyValue; \n\
                         property var bindingProperty: PROPERTY; \n\
                         SLOT: { updatedPropertyValue = PROPERTY } \n\
                         function changeProperty(newValue) { PROPERTY = newValue } \n\
                     }");

    qml.replace("PROPERTY", property);
    qml.replace("SLOT", slot);
    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    //call the QML setter
    QMetaObject::invokeMethod(obj.data(), "changeProperty", Q_ARG(QVariant, value));

    //Check the slot in QML was called and that the new property is updated
    QCOMPARE(obj->property("updatedPropertyValue"), value);
    //A bound property should work as well
    QCOMPARE(obj->property("bindingProperty"), value);
}

void tst_QIviSimulationEngine::testPropertyWriteDerived_data()
{
    QTest::addColumn<QByteArray>("property");
    QTest::addColumn<QByteArray>("slot");
    QTest::addColumn<QVariant>("value");
    QTest::newRow("propertyInBase") << QByteArray("propertyInBase") << QByteArray("onPropertyInBaseChanged") << QVariant::fromValue(10);
    QTest::newRow("propertyInDerived") << QByteArray("propertyInDerived") << QByteArray("onPropertyInDerivedChanged") << QVariant::fromValue(10);
    QTest::newRow("complexPropertyInDerived") << QByteArray("complexPropertyInDerived") << QByteArray("onComplexPropertyInDerivedChanged")  << QVariant::fromValue(true);
}

void tst_QIviSimulationEngine::testPropertyWriteDerived()
{
    QFETCH(QByteArray, property);
    QFETCH(QByteArray, slot);
    QFETCH(QVariant, value);

    QIviSimulationEngine engine;

    DerivedClass testObject;
    engine.registerSimulationInstance<DerivedClass>(&testObject, "TestAPI", 1, 0, "DerivedClass");

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     DerivedClass { \n\
                         property var updatedPropertyValue; \n\
                         property var bindingProperty: PROPERTY; \n\
                         SLOT: { updatedPropertyValue = PROPERTY } \n\
                         function changeProperty(newValue) { PROPERTY = newValue } \n\
                     }");

    qml.replace("PROPERTY", property);
    qml.replace("SLOT", slot);
    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    //call the QML setter
    QMetaObject::invokeMethod(obj.data(), "changeProperty", Q_ARG(QVariant, value));

    //Check the slot in QML was called and that the new property is updated
    QCOMPARE(obj->property("updatedPropertyValue"), value);
    //A bound property should work as well
    QCOMPARE(obj->property("bindingProperty"), value);
}

//Animations use a different way to access the properties, so we need to test this explicitly
void tst_QIviSimulationEngine::testAnimations()
{
    QIviSimulationEngine engine;

    DerivedClass testObject;
    engine.registerSimulationInstance<DerivedClass>(&testObject, "TestAPI", 1, 0, "DerivedClass");
    QSignalSpy spy(&testObject, SIGNAL(propertyInBaseChanged(int)));

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     DerivedClass { \n\
                        id: backend \n\
                        property var animation: SequentialAnimation { \n\
                            NumberAnimation { target: backend; property: \"propertyInBase\"; from: 0; to: 130; duration: 500 } \n\
                            ScriptAction { script: backend.animationDone() } \n\
                            running: true \n\
                        } \n\
                        signal animationDone(); \n\
                     }");

    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    //wait until the animation ended
    QSignalSpy animationSpy(obj.data(), SIGNAL(animationDone()));
    animationSpy.wait();

    //Check that the animation has reached it's final value
    QCOMPARE(obj->property("propertyInBase"), 130);

    //we expect at least 2 animation steps (intermediate step and final step)
    QVERIFY2(spy.count() >= 2, qPrintable(QStringLiteral("Emitted signals: ") + QString::number(spy.count())));
}

void tst_QIviSimulationEngine::testFunctionCalls_data()
{
    QTest::addColumn<QByteArray>("function");
    QTest::addColumn<QByteArray>("signal");
    QTest::addColumn<QVariant>("returnValue");
    QTest::addColumn<QVariant>("value1");
    QTest::addColumn<QVariant>("value2");
    QTest::newRow("simpleFunction") << QByteArray("simpleFunction") << QByteArray(SIGNAL(simpleFunctionCalled())) << QVariant() << QVariant() << QVariant();
    QTest::newRow("functionWithArguments") << QByteArray("functionWithArguments") << QByteArray(SIGNAL(functionWithArgumentsCalled(int, QString))) << QVariant() << QVariant(100) << QVariant("Test");
    QTest::newRow("functionWithReturnValue") << QByteArray("functionWithReturnValue") << QByteArray(SIGNAL(functionWithReturnValueCalled(int))) << QVariant(100) << QVariant(100) << QVariant();
}

void tst_QIviSimulationEngine::testFunctionCalls()
{
    QFETCH(QByteArray, function);
    QFETCH(QByteArray, signal);
    QFETCH(QVariant, returnValue);
    QFETCH(QVariant, value1);
    QFETCH(QVariant, value2);

    QIviSimulationEngine engine;

    SimpleAPI testObject;
    engine.registerSimulationInstance<SimpleAPI>(&testObject, "TestAPI", 1, 0, "SimpleAPI");

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     SimpleAPI { \n\
                     }");

    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    QSignalSpy spy(&testObject, signal.data());

    QCOMPARE(testObject.m_callCounter, 0);

    //call the testfunction
    QVariantList expectedValues;
    QVariant retValue = callTestFunction(&testObject, function, expectedValues, returnValue, value1, value2);

    //verify the implementation was called and the signal emitted
    QCOMPARE(testObject.m_callCounter, 1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0), expectedValues);

    //If provided check the returnValue as well
    if (returnValue.isValid())
        QCOMPARE(retValue, returnValue);
}

void tst_QIviSimulationEngine::testFunctionOverride_data()
{
    QTest::addColumn<QByteArray>("function");
    QTest::addColumn<QByteArray>("signal");
    QTest::addColumn<QVariant>("returnValue");
    QTest::addColumn<QVariant>("value1");
    QTest::addColumn<QVariant>("value2");
    QTest::newRow("simpleFunction") << QByteArray("simpleFunction") << QByteArray(SIGNAL(simpleFunctionCalled())) << QVariant() << QVariant() << QVariant();
    QTest::newRow("functionWithArguments") << QByteArray("functionWithArguments") << QByteArray(SIGNAL(functionWithArgumentsCalled(int, QString))) << QVariant() << QVariant(100) << QVariant("Test");
    QTest::newRow("functionWithReturnValue") << QByteArray("functionWithReturnValue") << QByteArray(SIGNAL(functionWithReturnValueCalled(int))) << QVariant(100) << QVariant(100) << QVariant();
}

void tst_QIviSimulationEngine::testFunctionOverride()
{
    QFETCH(QByteArray, function);
    QFETCH(QByteArray, signal);
    QFETCH(QVariant, returnValue);
    QFETCH(QVariant, value1);
    QFETCH(QVariant, value2);

    QIviSimulationEngine engine;

    SimpleAPI testObject;
    engine.registerSimulationInstance<SimpleAPI>(&testObject, "TestAPI", 1, 0, "SimpleAPI");

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     SimpleAPI { \n\
                        function simpleFunction() { \n\
                            simpleFunctionCalled(); \n\
                        } \n\
                        function functionWithArguments(intArgument, stringArgument) { \n\
                            functionWithArgumentsCalled(intArgument, stringArgument); \n\
                        } \n\
                        function functionWithReturnValue(intArgument) { \n\
                            functionWithReturnValueCalled(intArgument) \n\
                            return intArgument; \n\
                        } \n\
                     }");

    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    QSignalSpy spy(&testObject, signal.data());

    QCOMPARE(testObject.m_callCounter, 0);

    //call the testfunction
    QVariantList expectedValues;
    QVariant retValue = callTestFunction(&testObject, function, expectedValues, returnValue, value1, value2);

    //verify the implementation was NOT called but the signal was emitted from QML
    QCOMPARE(testObject.m_callCounter, 0);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0), expectedValues);

    //If provided check the returnValue as well
    if (returnValue.isValid())
        QCOMPARE(retValue, returnValue);
}

void tst_QIviSimulationEngine::testCallingBaseFunction_data()
{
    QTest::addColumn<QByteArray>("function");
    QTest::addColumn<QByteArray>("signal");
    QTest::addColumn<QVariant>("returnValue");
    QTest::addColumn<QVariant>("value1");
    QTest::addColumn<QVariant>("value2");
    QTest::newRow("simpleFunction") << QByteArray("simpleFunction") << QByteArray(SIGNAL(simpleFunctionCalled())) << QVariant() << QVariant() << QVariant();
    QTest::newRow("functionWithArguments") << QByteArray("functionWithArguments") << QByteArray(SIGNAL(functionWithArgumentsCalled(int, QString))) << QVariant() << QVariant(100) << QVariant("Test");
    QTest::newRow("functionWithReturnValue") << QByteArray("functionWithReturnValue") << QByteArray(SIGNAL(functionWithReturnValueCalled(int))) << QVariant(100) << QVariant(100) << QVariant();
}

void tst_QIviSimulationEngine::testCallingBaseFunction()
{
    QFETCH(QByteArray, function);
    QFETCH(QByteArray, signal);
    QFETCH(QVariant, returnValue);
    QFETCH(QVariant, value1);
    QFETCH(QVariant, value2);

    QIviSimulationEngine engine;

    SimpleAPI testObject;
    engine.registerSimulationInstance<SimpleAPI>(&testObject, "TestAPI", 1, 0, "SimpleAPI");

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     SimpleAPI { \n\
                        function simpleFunction() { \n\
                            simpleFunctionCalled(); \n\
                            Base.simpleFunction(); \n\
                        } \n\
                        function functionWithArguments(intArgument, stringArgument) { \n\
                            functionWithArgumentsCalled(intArgument, stringArgument); \n\
                            Base.functionWithArguments(intArgument, stringArgument); \n\
                        } \n\
                        function functionWithReturnValue(intArgument) { \n\
                            functionWithReturnValueCalled(intArgument) \n\
                            return Base.functionWithReturnValue(intArgument); \n\
                        } \n\
                     }");

    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    QSignalSpy spy(&testObject, signal.data());

    QCOMPARE(testObject.m_callCounter, 0);

    //call the testfunction
    QVariantList expectedValues;
    QVariant retValue = callTestFunction(&testObject, function, expectedValues, returnValue, value1, value2);

    //verify the implementation was called once.
    QCOMPARE(testObject.m_callCounter, 1);
    //Verify that the signal was emitted twice: 1. QML, 2. C++ implementation
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(0), expectedValues);
    QCOMPARE(spy.at(1), expectedValues);

    //If provided check the returnValue as well
    if (returnValue.isValid())
        QCOMPARE(retValue, returnValue);
}

void tst_QIviSimulationEngine::testRecursionPrevention()
{
    QIviSimulationEngine engine;

    SimpleAPI testObject;
    engine.registerSimulationInstance<SimpleAPI>(&testObject, "TestAPI", 1, 0, "SimpleAPI");

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     SimpleAPI { \n\
                        function setTestProperty(value) { \n\
                            testProperty = value \n\
                        } \n\
                     }");

    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    QSignalSpy spy(&testObject, SIGNAL(testPropertyChanged(int)));

    QCOMPARE(testObject.m_callCounter, 0);

    // call the setProperty function
    // This will set the testProperty, which will cause the setProperty function to be called again
    // from C++ as it's the WRITE method of the property
    QVariantList expectedValues;
    callTestFunction(&testObject, "setTestProperty", expectedValues, QVariant(), QVariant(5), QVariant());

    //verify the implementation was called once.
    QCOMPARE(testObject.m_callCounter, 1);
    //Verify that the signal was emitted once (only from the C++ method)
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0), expectedValues);
}

void tst_QIviSimulationEngine::testMultipleInstances()
{
    QIviSimulationEngine engine;

    SimpleAPI testObject;
    engine.registerSimulationInstance<SimpleAPI>(&testObject, "TestAPI", 1, 0, "SimpleAPI");

    QByteArray qml ("import QtQuick 2.0; \n\
                     import TestAPI 1.0; \n\
                     Item { \n\
                         signal firstInstanceCalled(); \n\
                         signal secondInstanceCalled(); \n\
                         SimpleAPI { \n\
                            function simpleFunction() { \n\
                                simpleFunctionCalled(); \n\
                                firstInstanceCalled(); \n\
                            } \n\
                            function functionWithReturnValue(intArgument) { \n\
                                functionWithReturnValueCalled(intArgument) \n\
                                firstInstanceCalled(); \n\
                                return intArgument; \n\
                            } \n\
                         } \n\
                         SimpleAPI { \n\
                            function functionWithArguments(intArgument, stringArgument) { \n\
                                functionWithArgumentsCalled(intArgument, stringArgument); \n\
                                secondInstanceCalled(); \n\
                            } \n\
                            function functionWithReturnValue(intArgument) { \n\
                                functionWithReturnValueCalled(intArgument) \n\
                                secondInstanceCalled(); \n\
                                return intArgument; \n\
                            } \n\
                         } \n\
                     }");

    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    QSignalSpy firstInstanceSpy(obj.data(), SIGNAL(firstInstanceCalled()));
    QSignalSpy secondInstanceSpy(obj.data(), SIGNAL(secondInstanceCalled()));

    QSignalSpy simpleSpy(&testObject, SIGNAL(simpleFunctionCalled()));
    QSignalSpy argumentsSpy(&testObject, SIGNAL(functionWithArgumentsCalled(int, QString)));
    QSignalSpy returnValueSpy(&testObject, SIGNAL(functionWithReturnValueCalled(int)));

    // call the simpleFunction function (defined in the first instance)
    QVariantList expectedValues;
    callTestFunction(&testObject, "simpleFunction", expectedValues, QVariant(), QVariant(), QVariant());

    //Verify that the first QML instance in QML was called
    QCOMPARE(firstInstanceSpy.count(), 1);
    QCOMPARE(secondInstanceSpy.count(), 0);
    firstInstanceSpy.clear();

    //Verify that the signal was emitted from QML
    QCOMPARE(simpleSpy.count(), 1);
    QCOMPARE(simpleSpy.at(0), expectedValues);

    // call the functionWithArguments function (defined in the second instance)
    expectedValues.clear();
    callTestFunction(&testObject, "functionWithArguments", expectedValues, QVariant(), QVariant(100), QVariant("Test"));

    //Verify that the second QML instance in QML was called
    QCOMPARE(firstInstanceSpy.count(), 0);
    QCOMPARE(secondInstanceSpy.count(), 1);
    secondInstanceSpy.clear();

    //Verify that the signal was emitted from QML
    QCOMPARE(argumentsSpy.count(), 1);
    QCOMPARE(argumentsSpy.at(0), expectedValues);

    // call the functionWithReturnValue function (defined in both instances)
    expectedValues.clear();
    callTestFunction(&testObject, "functionWithReturnValue", expectedValues, QVariant(), QVariant(100), QVariant());

    //Verify that the second QML instance in QML was called as it is the first once which is created
    QCOMPARE(firstInstanceSpy.count(), 1);
    QCOMPARE(secondInstanceSpy.count(), 0);

    //Verify that the signal was emitted from QML
    QCOMPARE(returnValueSpy.count(), 1);
    QCOMPARE(returnValueSpy.at(0), expectedValues);
}

QTEST_MAIN(tst_QIviSimulationEngine)

#include "tst_qivisimulationengine.moc"

