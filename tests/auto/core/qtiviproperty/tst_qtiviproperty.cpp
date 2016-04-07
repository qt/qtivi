/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
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
#include <qtivipropertyattribute.h>
#include <qtivipropertyfactory.h>

#define QTIVIPROPERTY(_type_, _name_) \
Q_PROPERTY(QtIVIProperty* _name_##Property READ _name_##Property CONSTANT)  \
QtIVIProperty *m_##_name_##Property; \
QtIVIPropertyAttribute<_type_> m_##_name_; \
_type_ m_##_name_##Value; \
 \
public: \
QtIVIProperty *_name_##Property() const { return m_##_name_##Property; } \
QtIVIPropertyAttribute<_type_> _name_() const { return m_##_name_; } \
void set##_name_(QtIVIPropertyAttribute<_type_> attribute) { if (attribute == m_##_name_) return; m_##_name_ = attribute; emit _name_##Changed(attribute); } \
 \
_type_ _name_##Value() const { return m_##_name_##Value; } \
void set##_name_##Value(_type_ value) { if (value == m_##_name_##Value) return; m_##_name_##Value = value; emit _name_##ValueChanged(value); } \
 \
Q_SIGNALS: \
void _name_##ValueChanged(_type_ value); \
void _name_##Changed(const QtIVIPropertyAttribute<_type_> _name_); \
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

    enum InvalidEnum {
        InvalidValue_1 = 1,
        InvalidValue_2,
        InvalidValue_3,
    };
    Q_ENUMS(InvalidEnum)

    QTIVIPROPERTY(int, intAttribute)
    QTIVIPROPERTY(QString, readOnlyAttribute)
    QTIVIPROPERTY(TestObject::TestEnum, enumAttribute)

public:
    TestObject(QObject *parent = Q_NULLPTR)
      : QObject(parent)
      , m_intAttributeProperty(QtIVIPropertyFactory<int>::create(this,
                                                                 &TestObject::intAttribute,
                                                                 &TestObject::intAttributeChanged,
                                                                 &TestObject::intAttributeValue,
                                                                 &TestObject::intAttributeValueChanged,
                                                                 &TestObject::setintAttributeValue))
      , m_intAttributeValue(-1)
      , m_readOnlyAttributeProperty(QtIVIPropertyFactory<QString>::create(this,
                                                                          &TestObject::readOnlyAttribute,
                                                                          &TestObject::readOnlyAttributeChanged,
                                                                          &TestObject::readOnlyAttributeValue,
                                                                          &TestObject::readOnlyAttributeValueChanged))
      , m_enumAttributeProperty(QtIVIPropertyFactory<TestObject::TestEnum>::create(this,
                                                                                   &TestObject::enumAttribute,
                                                                                   &TestObject::enumAttributeChanged,
                                                                                   &TestObject::enumAttributeValue,
                                                                                   &TestObject::enumAttributeValueChanged,
                                                                                   &TestObject::setenumAttributeValue))
      , m_enumAttributeValue(TestObject::TestEnum())
    {}


};

Q_DECLARE_METATYPE(QtIVIPropertyAttribute<int>)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<QString>)
Q_DECLARE_METATYPE(TestObject::TestEnum)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<TestObject::TestEnum>)
Q_DECLARE_METATYPE(QList<QQmlError>)

class tst_QtIVIProperty : public QObject
{
    Q_OBJECT

public:
    tst_QtIVIProperty();

private Q_SLOTS:
    void selfTest();
    void attribute_data();
    void attribute();
    void setGetValue();
    void setGetValue_qml();
    void setValueError_qml();
    void readOnly();
    void readOnly_qml();

private:
    void initializeEnumAttribute(TestObject *testObject)
    {
        QSignalSpy propertyValueChangedSpy(testObject->enumAttributeProperty(), &QtIVIProperty::valueChanged);
        QSignalSpy valueChangedSpy(testObject, &TestObject::enumAttributeValueChanged);

        //Initialize
        testObject->setenumAttributeValue(TestObject::TestValue_3);

        QCOMPARE(propertyValueChangedSpy.count(), 1);
        QCOMPARE(propertyValueChangedSpy.at(0).at(0), QVariant::fromValue<TestObject::TestEnum>(TestObject::TestValue_3));
        QCOMPARE(valueChangedSpy.count(), 1);
        QCOMPARE(valueChangedSpy.at(0).at(0), QVariant::fromValue<TestObject::TestEnum>(TestObject::TestValue_3));
    }

    void initializeIntAttribute(TestObject *testObject)
    {
        QSignalSpy propertyValueChangedSpy(testObject->intAttributeProperty(), &QtIVIProperty::valueChanged);
        QSignalSpy valueChangedSpy(testObject, &TestObject::intAttributeValueChanged);

        //Initialize
        testObject->setintAttributeValue(1);

        QCOMPARE(propertyValueChangedSpy.count(), 1);
        QCOMPARE(propertyValueChangedSpy.at(0).at(0), QVariant(1));
        QCOMPARE(valueChangedSpy.count(), 1);
        QCOMPARE(valueChangedSpy.at(0).at(0), QVariant(1));
    }
};

tst_QtIVIProperty::tst_QtIVIProperty()
{
    qRegisterMetaType<QtIVIPropertyAttribute<int>>();
    qRegisterMetaType<QtIVIPropertyAttribute<QString>>();
    qRegisterMetaType<TestObject::TestEnum>();
    qRegisterMetaType<QtIVIPropertyAttribute<TestObject::TestEnum>>();
    qRegisterMetaType<QList<QQmlError>>();
}

void tst_QtIVIProperty::selfTest()
{
    TestObject *testObject = new TestObject();

    QCOMPARE(testObject->intAttributeValue(), -1);
    QCOMPARE(testObject->intAttribute().type(), QtIVIPropertyAttributeBase::Invalid);
    QVERIFY(testObject->intAttributeProperty());

    QCOMPARE(testObject->readOnlyAttributeValue(), QString());
    QCOMPARE(testObject->readOnlyAttribute().type(), QtIVIPropertyAttributeBase::Invalid);
    QVERIFY(testObject->readOnlyAttributeProperty());

    QCOMPARE(testObject->enumAttributeValue(), TestObject::TestEnum());
    QCOMPARE(testObject->enumAttribute().type(), QtIVIPropertyAttributeBase::Invalid);
    QVERIFY(testObject->enumAttributeProperty());
}

void tst_QtIVIProperty::attribute_data()
{
    QTest::addColumn<QtIVIPropertyAttribute<int>>("attribute");
    QTest::addColumn<QVariantList>("exptectedList");
    QTest::newRow("noAttribute") << QtIVIPropertyAttribute<int>(true) << QVariantList();
    QTest::newRow("minimumMaximum") << QtIVIPropertyAttribute<int>(0,10) << QVariantList();
    QVector<int> avValues;
    QVariantList avVariant;
    avValues << 0 << 5 << 10;
    avVariant << 0 << 5 << 10;
    QTest::newRow("availableValues") << QtIVIPropertyAttribute<int>(avValues) << QVariantList(avVariant);
}

void tst_QtIVIProperty::attribute()
{
    QFETCH(QtIVIPropertyAttribute<int>, attribute);
    QFETCH(QVariantList, exptectedList);

    TestObject *testObject = new TestObject();

    QSignalSpy maximumChangedSpy(testObject->intAttributeProperty(), &QtIVIProperty::maximumValueChanged);
    QSignalSpy minimumChangedSpy(testObject->intAttributeProperty(), &QtIVIProperty::minimumValueChanged);
    QSignalSpy availableValuesChangedSpy(testObject->intAttributeProperty(), &QtIVIProperty::availableValuesChanged);
    QSignalSpy availableChangedSpy(testObject->intAttributeProperty(), &QtIVIProperty::availableChanged);
    QSignalSpy noAttChangedSpy(testObject, &TestObject::intAttributeChanged);

    testObject->setintAttribute(attribute);
    QVariant attributeVariant = QVariant::fromValue<QtIVIPropertyAttribute<int>>(attribute);

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

void tst_QtIVIProperty::setGetValue()
{
    TestObject *testObject = new TestObject();
    QSignalSpy propertyValueChangedSpy(testObject->enumAttributeProperty(), &QtIVIProperty::valueChanged);
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

void tst_QtIVIProperty::setGetValue_qml()
{
    TestObject *testObject = new TestObject();

    initializeEnumAttribute(testObject);
    initializeIntAttribute(testObject);

    QQmlEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("testObject"), testObject);
    QQmlComponent component(&engine, QUrl::fromLocalFile(QFINDTESTDATA("testdata/readWrite.qml")));
    QObject *obj = component.create();

    QVERIFY(obj);
    QCOMPARE(obj->property("intProperty").toInt(), 1);
    QCOMPARE(obj->property("enumProperty").toInt(), 3);

    QVERIFY(QMetaObject::invokeMethod(obj, "updateValues"));
    QCOMPARE(obj->property("intProperty").toInt(), 2);
    QCOMPARE(obj->property("enumProperty").toInt(), 2);
}

void tst_QtIVIProperty::setValueError_qml()
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

void tst_QtIVIProperty::readOnly()
{
    TestObject *testObject = new TestObject();
    QSignalSpy propertyValueChangedSpy(testObject->readOnlyAttributeProperty(), &QtIVIProperty::valueChanged);
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

void tst_QtIVIProperty::readOnly_qml()
{
    TestObject *testObject = new TestObject();
    QSignalSpy propertyValueChangedSpy(testObject->readOnlyAttributeProperty(), &QtIVIProperty::valueChanged);
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

QTEST_MAIN(tst_QtIVIProperty)

#include "tst_qtiviproperty.moc"

