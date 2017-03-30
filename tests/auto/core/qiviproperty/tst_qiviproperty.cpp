/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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
#include <qivipropertyattribute.h>
#include <qivipropertyfactory.h>

#define QIVIPROPERTY(_type_, _name_) \
Q_PROPERTY(QIviProperty* _name_##Property READ _name_##Property CONSTANT)  \
QIviProperty *m_##_name_##Property; \
QIviPropertyAttribute<_type_> m_##_name_; \
_type_ m_##_name_##Value; \
 \
public: \
QIviProperty *_name_##Property() const { return m_##_name_##Property; } \
QIviPropertyAttribute<_type_> _name_() const { return m_##_name_; } \
void set##_name_(QIviPropertyAttribute<_type_> attribute) { if (attribute == m_##_name_) return; m_##_name_ = attribute; emit _name_##Changed(attribute); } \
 \
_type_ _name_##Value() const { return m_##_name_##Value; } \
void set##_name_##Value(_type_ value) { if (value == m_##_name_##Value) return; m_##_name_##Value = value; emit _name_##ValueChanged(value); } \
 \
Q_SIGNALS: \
void _name_##ValueChanged(_type_ value); \
void _name_##Changed(const QIviPropertyAttribute<_type_> _name_); \
private:

class TestObject : public QObject
{
    Q_OBJECT

public:
    enum TestEnum {
        TestValue_1 = 1,
        TestValue_2,
        TestValue_3,
    };
    Q_ENUM(TestEnum)

    enum TestFlag {
        TestFlag_1 = 0x1,
        TestFlag_2 = 0x2,
        TestFlag_3 = 0x4,
    };
    Q_DECLARE_FLAGS(TestFlags, TestFlag)
    Q_FLAG(TestFlags)

    enum InvalidEnum {
        InvalidValue_1 = 1,
        InvalidValue_2,
        InvalidValue_3,
    };
    Q_ENUMS(InvalidEnum)

    QIVIPROPERTY(int, intAttribute)
    QIVIPROPERTY(QString, readOnlyAttribute)
    QIVIPROPERTY(TestObject::TestEnum, enumAttribute)
    QIVIPROPERTY(TestObject::TestFlags, flagsAttribute)

public:
    TestObject(QObject *parent = nullptr)
      : QObject(parent)
      , m_intAttributeProperty(QIviPropertyFactory<int>::create(this,
                                                                 &TestObject::intAttribute,
                                                                 &TestObject::intAttributeChanged,
                                                                 &TestObject::intAttributeValue,
                                                                 &TestObject::intAttributeValueChanged,
                                                                 &TestObject::setintAttributeValue))
      , m_intAttributeValue(-1)
      , m_readOnlyAttributeProperty(QIviPropertyFactory<QString>::create(this,
                                                                          &TestObject::readOnlyAttribute,
                                                                          &TestObject::readOnlyAttributeChanged,
                                                                          &TestObject::readOnlyAttributeValue,
                                                                          &TestObject::readOnlyAttributeValueChanged))
      , m_enumAttributeProperty(QIviPropertyFactory<TestObject::TestEnum>::create(this,
                                                                                   &TestObject::enumAttribute,
                                                                                   &TestObject::enumAttributeChanged,
                                                                                   &TestObject::enumAttributeValue,
                                                                                   &TestObject::enumAttributeValueChanged,
                                                                                   &TestObject::setenumAttributeValue))
      , m_enumAttributeValue(TestObject::TestEnum())
      , m_flagsAttributeProperty(QIviPropertyFactory<TestObject::TestFlags>::create(this,
                                                                                   &TestObject::flagsAttribute,
                                                                                   &TestObject::flagsAttributeChanged,
                                                                                   &TestObject::flagsAttributeValue,
                                                                                   &TestObject::flagsAttributeValueChanged,
                                                                                   &TestObject::setflagsAttributeValue))
      , m_flagsAttributeValue(TestObject::TestFlags())
    {}
};

Q_DECLARE_METATYPE(TestObject::TestEnum)
Q_DECLARE_METATYPE(QIviPropertyAttribute<TestObject::TestEnum>)
Q_DECLARE_METATYPE(TestObject::TestFlags)
Q_DECLARE_METATYPE(QIviPropertyAttribute<TestObject::TestFlags>)
Q_DECLARE_OPERATORS_FOR_FLAGS(TestObject::TestFlags)
Q_DECLARE_METATYPE(QList<QQmlError>)

class tst_QIviProperty : public QObject
{
    Q_OBJECT

public:
    tst_QIviProperty();

private Q_SLOTS:
    void selfTest();
    void attribute_data();
    void attribute();
    void setGetValue();
    void setGetValue_flags();
    void setGetValue_qml();
    void setValueError_qml();
    void readOnly();
    void readOnly_qml();

private:
    void initializeEnumAttribute(TestObject *testObject)
    {
        QSignalSpy propertyValueChangedSpy(testObject->enumAttributeProperty(), &QIviProperty::valueChanged);
        QSignalSpy valueChangedSpy(testObject, &TestObject::enumAttributeValueChanged);

        //Initialize
        testObject->setenumAttributeValue(TestObject::TestValue_3);

        QCOMPARE(propertyValueChangedSpy.count(), 1);
        QCOMPARE(propertyValueChangedSpy.at(0).at(0), QVariant::fromValue<TestObject::TestEnum>(TestObject::TestValue_3));
        QCOMPARE(valueChangedSpy.count(), 1);
        QCOMPARE(valueChangedSpy.at(0).at(0), QVariant::fromValue<TestObject::TestEnum>(TestObject::TestValue_3));
    }

    void initializeFlagsAttribute(TestObject *testObject)
    {
        QSignalSpy propertyValueChangedSpy(testObject->flagsAttributeProperty(), &QIviProperty::valueChanged);
        QSignalSpy valueChangedSpy(testObject, &TestObject::flagsAttributeValueChanged);

        //Initialize
        testObject->setflagsAttributeValue(TestObject::TestFlag_3);

        QCOMPARE(propertyValueChangedSpy.count(), 1);
        QCOMPARE(propertyValueChangedSpy.at(0).at(0), QVariant::fromValue<TestObject::TestFlags>(TestObject::TestFlag_3));
        QCOMPARE(valueChangedSpy.count(), 1);
        QCOMPARE(valueChangedSpy.at(0).at(0), QVariant::fromValue<TestObject::TestFlags>(TestObject::TestFlag_3));
    }

    void initializeIntAttribute(TestObject *testObject)
    {
        QSignalSpy propertyValueChangedSpy(testObject->intAttributeProperty(), &QIviProperty::valueChanged);
        QSignalSpy valueChangedSpy(testObject, &TestObject::intAttributeValueChanged);

        //Initialize
        testObject->setintAttributeValue(1);

        QCOMPARE(propertyValueChangedSpy.count(), 1);
        QCOMPARE(propertyValueChangedSpy.at(0).at(0), QVariant(1));
        QCOMPARE(valueChangedSpy.count(), 1);
        QCOMPARE(valueChangedSpy.at(0).at(0), QVariant(1));
    }
};

tst_QIviProperty::tst_QIviProperty()
{
    qRegisterMetaType<QIviPropertyAttribute<int>>();
    qRegisterMetaType<QIviPropertyAttribute<QString>>();
    qRegisterMetaType<TestObject::TestEnum>();
    qRegisterMetaType<QIviPropertyAttribute<TestObject::TestEnum>>();
    qRegisterMetaType<TestObject::TestFlags>();
    qRegisterMetaType<QIviPropertyAttribute<TestObject::TestFlags>>();
    qRegisterMetaType<QList<QQmlError>>();
}

void tst_QIviProperty::selfTest()
{
    TestObject *testObject = new TestObject();

    QCOMPARE(testObject->intAttributeValue(), -1);
    QCOMPARE(testObject->intAttribute().type(), QIviPropertyAttributeBase::Invalid);
    QVERIFY(testObject->intAttributeProperty());

    QCOMPARE(testObject->readOnlyAttributeValue(), QString());
    QCOMPARE(testObject->readOnlyAttribute().type(), QIviPropertyAttributeBase::Invalid);
    QVERIFY(testObject->readOnlyAttributeProperty());

    QCOMPARE(testObject->enumAttributeValue(), TestObject::TestEnum());
    QCOMPARE(testObject->enumAttribute().type(), QIviPropertyAttributeBase::Invalid);
    QVERIFY(testObject->enumAttributeProperty());

    QCOMPARE(testObject->flagsAttributeValue(), TestObject::TestFlags());
    QCOMPARE(testObject->flagsAttribute().type(), QIviPropertyAttributeBase::Invalid);
    QVERIFY(testObject->flagsAttributeProperty());
}

void tst_QIviProperty::attribute_data()
{
    QTest::addColumn<QIviPropertyAttribute<int>>("attribute");
    QTest::addColumn<QVariantList>("exptectedList");
    QTest::newRow("noAttribute") << QIviPropertyAttribute<int>(true) << QVariantList();
    QTest::newRow("minimumMaximum") << QIviPropertyAttribute<int>(0,10) << QVariantList();
    QVector<int> avValues;
    QVariantList avVariant;
    avValues << 0 << 5 << 10;
    avVariant << 0 << 5 << 10;
    QTest::newRow("availableValues") << QIviPropertyAttribute<int>(avValues) << QVariantList(avVariant);
}

void tst_QIviProperty::attribute()
{
    QFETCH(QIviPropertyAttribute<int>, attribute);
    QFETCH(QVariantList, exptectedList);

    TestObject *testObject = new TestObject();

    QSignalSpy maximumChangedSpy(testObject->intAttributeProperty(), &QIviProperty::maximumValueChanged);
    QSignalSpy minimumChangedSpy(testObject->intAttributeProperty(), &QIviProperty::minimumValueChanged);
    QSignalSpy availableValuesChangedSpy(testObject->intAttributeProperty(), &QIviProperty::availableValuesChanged);
    QSignalSpy availableChangedSpy(testObject->intAttributeProperty(), &QIviProperty::availableChanged);
    QSignalSpy noAttChangedSpy(testObject, &TestObject::intAttributeChanged);

    testObject->setintAttribute(attribute);
    QVariant attributeVariant = QVariant::fromValue<QIviPropertyAttribute<int>>(attribute);

    QCOMPARE(maximumChangedSpy.count(), 1);
    QCOMPARE(maximumChangedSpy.at(0).at(0), QVariant::fromValue<int>(attribute.maximumValue()));
    QCOMPARE(minimumChangedSpy.count(), 1);
    QCOMPARE(minimumChangedSpy.at(0).at(0), QVariant::fromValue<int>(attribute.minimumValue()));
    QCOMPARE(availableValuesChangedSpy.count(), 1);
    QCOMPARE(availableValuesChangedSpy.at(0).at(0), QVariant(exptectedList));
    QCOMPARE(availableChangedSpy.count(), 1);
    QCOMPARE(availableChangedSpy.at(0).at(0), QVariant::fromValue<bool>(attribute.isAvailable()));
    QCOMPARE(noAttChangedSpy.count(), 1);
    QCOMPARE(noAttChangedSpy.at(0).at(0), attributeVariant);
    QCOMPARE(testObject->intAttribute(), attribute);
    QCOMPARE(testObject->intAttributeProperty()->isAvailable(), attribute.isAvailable());
    QCOMPARE(testObject->intAttributeProperty()->minimumValue(), QVariant::fromValue<int>(attribute.minimumValue()));
    QCOMPARE(testObject->intAttributeProperty()->maximumValue(), QVariant::fromValue<int>(attribute.maximumValue()));
    QCOMPARE(testObject->intAttributeProperty()->availableValues(), exptectedList);
}

void tst_QIviProperty::setGetValue()
{
    TestObject *testObject = new TestObject();
    QSignalSpy propertyValueChangedSpy(testObject->enumAttributeProperty(), &QIviProperty::valueChanged);
    QSignalSpy valueChangedSpy(testObject, &TestObject::enumAttributeValueChanged);

    //Initialize
    testObject->setenumAttributeValue(TestObject::TestValue_3);

    QCOMPARE(propertyValueChangedSpy.count(), 1);
    QCOMPARE(propertyValueChangedSpy.at(0).at(0), QVariant::fromValue<TestObject::TestEnum>(TestObject::TestValue_3));
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(valueChangedSpy.at(0).at(0), QVariant::fromValue<TestObject::TestEnum>(TestObject::TestValue_3));

    //Read value
    QCOMPARE(testObject->enumAttributeValue(), TestObject::TestValue_3);
    QCOMPARE(testObject->enumAttributeProperty()->value(), QVariant::fromValue<TestObject::TestEnum>(TestObject::TestValue_3));

    propertyValueChangedSpy.clear();
    valueChangedSpy.clear();

    //Set value
    QVariant newValueVariant(TestObject::TestValue_2);
    testObject->enumAttributeProperty()->setValue(newValueVariant);

    QCOMPARE(propertyValueChangedSpy.count(), 1);
    QCOMPARE(propertyValueChangedSpy.at(0).at(0), newValueVariant);
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(valueChangedSpy.at(0).at(0), newValueVariant);

    //Read value
    QCOMPARE(testObject->enumAttributeValue(), TestObject::TestValue_2);
    QCOMPARE(testObject->enumAttributeProperty()->value(), newValueVariant);
}

void tst_QIviProperty::setGetValue_flags()
{
    TestObject *testObject = new TestObject();
    QSignalSpy propertyValueChangedSpy(testObject->flagsAttributeProperty(), &QIviProperty::valueChanged);
    QSignalSpy valueChangedSpy(testObject, &TestObject::flagsAttributeValueChanged);

    //Initialize
    testObject->setflagsAttributeValue(TestObject::TestFlag_3);

    QCOMPARE(propertyValueChangedSpy.count(), 1);
    QCOMPARE(propertyValueChangedSpy.at(0).at(0), QVariant::fromValue<TestObject::TestFlags>(TestObject::TestFlag_3));
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(valueChangedSpy.at(0).at(0), QVariant::fromValue<TestObject::TestFlags>(TestObject::TestFlag_3));

    //Read value
    QCOMPARE(testObject->flagsAttributeValue(), TestObject::TestFlag_3);
    QCOMPARE(testObject->flagsAttributeProperty()->value(), QVariant::fromValue<TestObject::TestFlags>(TestObject::TestFlag_3));

    propertyValueChangedSpy.clear();
    valueChangedSpy.clear();

    //Set value
    QVariant newValueVariant = QVariant::fromValue(TestObject::TestFlags(TestObject::TestFlag_1 | TestObject::TestFlag_2));
    testObject->flagsAttributeProperty()->setValue(newValueVariant);

    QCOMPARE(propertyValueChangedSpy.count(), 1);
    QCOMPARE(propertyValueChangedSpy.at(0).at(0), newValueVariant);
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(valueChangedSpy.at(0).at(0), newValueVariant);

    //Read value
    QCOMPARE(testObject->flagsAttributeValue(), TestObject::TestFlag_1 | TestObject::TestFlag_2);
    QCOMPARE(testObject->flagsAttributeProperty()->value(), newValueVariant);
}

void tst_QIviProperty::setGetValue_qml()
{
    TestObject *testObject = new TestObject();

    initializeEnumAttribute(testObject);
    initializeFlagsAttribute(testObject);
    initializeIntAttribute(testObject);

    QQmlEngine engine;
    qmlRegisterType<TestObject>("TestObject", 1, 0, "TestObject");
    engine.rootContext()->setContextProperty(QStringLiteral("testObject"), testObject);
    QQmlComponent component(&engine, QUrl::fromLocalFile(QFINDTESTDATA("testdata/readWrite.qml")));
    QObject *obj = component.create();

    QVERIFY(obj);
    QCOMPARE(obj->property("intProperty").toInt(), 1);
    QCOMPARE(obj->property("enumProperty").toInt(), 3);
    QCOMPARE(obj->property("flagsProperty").toInt(), 0x4);
    QCOMPARE(obj->property("extendFlag").toInt(), 0x4);
    QVERIFY(!obj->property("isFlag2Set").toBool());
    QVERIFY(obj->property("isFlag3Set").toBool());

    QVERIFY(QMetaObject::invokeMethod(obj, "updateValues"));
    QCOMPARE(obj->property("intProperty").toInt(), 2);
    QCOMPARE(obj->property("enumProperty").toInt(), 2);
    QCOMPARE(obj->property("flagsProperty").toInt(), int(TestObject::TestFlags(TestObject::TestFlag_1 | TestObject::TestFlag_2)));
    QCOMPARE(obj->property("extendFlag").toInt(), int(TestObject::TestFlags(TestObject::TestFlag_1 | TestObject::TestFlag_2 | TestObject::TestFlag_3)));
    QVERIFY(obj->property("isFlag2Set").toBool());
    QVERIFY(!obj->property("isFlag3Set").toBool());
}

void tst_QIviProperty::setValueError_qml()
{
    TestObject *testObject = new TestObject();

    initializeEnumAttribute(testObject);
    initializeIntAttribute(testObject);

    QQmlEngine engine;
    engine.setOutputWarningsToStandardError(false);
    engine.rootContext()->setContextProperty(QStringLiteral("testObject"), testObject);
    QSignalSpy warningsSpy(&engine, &QQmlEngine::warnings);
    QString testData = QFINDTESTDATA("testdata/writeError.qml");
    QQmlComponent component(&engine, QUrl::fromLocalFile(testData));
    QObject *obj = component.create();

    QVERIFY(obj);
    QVERIFY(QMetaObject::invokeMethod(obj, "enumTest"));

    if (!warningsSpy.count())
        QVERIFY(warningsSpy.wait());

    QVERIFY(warningsSpy.count());
    QVERIFY(warningsSpy.at(0).count());
    QList<QQmlError> errorList = warningsSpy.at(0).at(0).value<QList<QQmlError>>();
    QCOMPARE(errorList.count(), 1);
    QCOMPARE(errorList.at(0).toString(), QUrl::fromLocalFile(testData).toString() + ":41: Error: Enum value out of range");
    warningsSpy.clear();

    QVERIFY(QMetaObject::invokeMethod(obj, "intTest"));

    if (!warningsSpy.count())
        QVERIFY(warningsSpy.wait());

    QVERIFY(warningsSpy.count());
    QVERIFY(warningsSpy.at(0).count());
    errorList = warningsSpy.at(0).at(0).value<QList<QQmlError>>();
    QCOMPARE(errorList.count(), 1);
    QCOMPARE(errorList.at(0).toString(), QUrl::fromLocalFile(testData).toString() + ":37: Error: Expected: int but got QString");
}

void tst_QIviProperty::readOnly()
{
    TestObject *testObject = new TestObject();
    QSignalSpy propertyValueChangedSpy(testObject->readOnlyAttributeProperty(), &QIviProperty::valueChanged);
    QSignalSpy valueChangedSpy(testObject, &TestObject::readOnlyAttributeValueChanged);

    //Initialize
    QString value = QString("foo");
    QVariant valueVariant = QVariant::fromValue<QString>(value);
    testObject->setreadOnlyAttributeValue(value);

    QCOMPARE(propertyValueChangedSpy.count(), 1);
    QCOMPARE(propertyValueChangedSpy.at(0).at(0), valueVariant);
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(valueChangedSpy.at(0).at(0), valueVariant);

    //Read value
    QCOMPARE(testObject->readOnlyAttributeValue(), value);
    QCOMPARE(testObject->readOnlyAttributeProperty()->value(), valueVariant);

    propertyValueChangedSpy.clear();
    valueChangedSpy.clear();

    //Try to set the value
    QString newValue = QString("bar");
    QVariant newValueVariant(newValue);
    QTest::ignoreMessage(QtWarningMsg, "TypeError: Cannot assign to read-only property \"value\"");
    testObject->readOnlyAttributeProperty()->setValue(newValueVariant);

    QCOMPARE(propertyValueChangedSpy.count(), 0);
    QCOMPARE(valueChangedSpy.count(), 0);

    //Verify that the value is still the old
    QCOMPARE(testObject->readOnlyAttributeValue(), value);
    QCOMPARE(testObject->readOnlyAttributeProperty()->value(), valueVariant);
}

void tst_QIviProperty::readOnly_qml()
{
    TestObject *testObject = new TestObject();
    QSignalSpy propertyValueChangedSpy(testObject->readOnlyAttributeProperty(), &QIviProperty::valueChanged);
    QSignalSpy valueChangedSpy(testObject, &TestObject::readOnlyAttributeValueChanged);

    //Initialize
    QString value = QString("foo");
    QVariant valueVariant = QVariant::fromValue<QString>(value);
    testObject->setreadOnlyAttributeValue(value);

    QCOMPARE(propertyValueChangedSpy.count(), 1);
    QCOMPARE(propertyValueChangedSpy.at(0).at(0), valueVariant);
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(valueChangedSpy.at(0).at(0), valueVariant);

    //Read value
    QCOMPARE(testObject->readOnlyAttributeValue(), value);
    QCOMPARE(testObject->readOnlyAttributeProperty()->value(), valueVariant);

    propertyValueChangedSpy.clear();
    valueChangedSpy.clear();

    QQmlEngine engine;
    engine.setOutputWarningsToStandardError(false);
    QQmlComponent component(&engine);
    QSignalSpy warningsSpy(&engine, &QQmlEngine::warnings);
    engine.rootContext()->setContextProperty(QStringLiteral("testObject"), testObject);
    component.setData("import QtQuick 2.0; Item { Component.onCompleted: testObject.readOnlyAttributeProperty.value = \"bar\"; }", QUrl());
    QObject *obj = component.create();

    QVERIFY(obj);
    if (!warningsSpy.count())
        QVERIFY(warningsSpy.wait());

    QVERIFY(warningsSpy.count());
    QVERIFY(warningsSpy.at(0).count());
    QList<QQmlError> errorList = warningsSpy.at(0).at(0).value<QList<QQmlError>>();
    QCOMPARE(errorList.count(), 1);
    QCOMPARE(errorList.at(0).toString(), QStringLiteral("<Unknown File>:1: Error: TypeError: Cannot assign to read-only property \"value\""));
}

QTEST_MAIN(tst_QIviProperty)

#include "tst_qiviproperty.moc"

