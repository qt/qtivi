/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
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

#ifndef QIVIPROPERTYTESTER_P_H
#define QIVIPROPERTYTESTER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qtiviglobal_p.h>
#include <QtIviCore/QIviProperty>
#include <QtIviCore/QIviPropertyFactory>
#include <QtIviCore/qiviqmlconversion_helper.h>
#include <QVariant>

QT_BEGIN_NAMESPACE

template <typename Feature, typename Backend, typename T> struct QIviPropertyTestData
{
    typedef void (Feature::*setter_func)(T);
    typedef T (Feature::*getter_func)() const;
    typedef void (Backend::*backend_setter_func)(T, const QString &);
    typedef QIviPropertyAttribute<T> (Feature::*attribute_getter_func)() const;
    typedef void (Backend::*backend_attribute_setter_func)(QIviPropertyAttribute<T>, const QString &);

    QIviPropertyTestData(const QString &pName, const QList<T> &vList, backend_setter_func setValueInBackend_func,
                     const QString &propChangedSignal, const QString &attChangedSignal,
                     setter_func set, getter_func get, const QList<QIviPropertyAttribute<T>> &attList,
                     attribute_getter_func attGet, backend_attribute_setter_func attSet)
        : propertyName(pName)
        , valueList(vList)
        , backend_setter(setValueInBackend_func)
        , propertyChangedSignal(propChangedSignal)
        , attributeChangedSignal(attChangedSignal)
        , setter(set)
        , getter(get)
        , attributeList(attList)
        , attribute_getter(attGet)
        , backend_attribute_setter(attSet)
    {}

    QIviPropertyTestData() {}

    QString propertyName;
    QList<T> valueList;
    backend_setter_func backend_setter;
    QString propertyChangedSignal;
    QString attributeChangedSignal;
    setter_func setter;
    getter_func getter;
    QList<QIviPropertyAttribute<T>> attributeList;
    attribute_getter_func attribute_getter;
    backend_attribute_setter_func backend_attribute_setter;
};

template <typename Feature, typename Backend, typename T>
void testIVIProperty(const QIviPropertyTestData<Feature, Backend, T> &testData, Feature *feature, Backend *backend, const QString &zone = QString())
{
    int valueIndex = 0;
    (backend->*testData.backend_setter)(testData.valueList.at(valueIndex), zone);

    //WTF need to split these two lines ?
    QVariant var = feature->property(testData.propertyName.toLocal8Bit());
    QIviProperty *property = var.value<QIviProperty*>();

    QVERIFY(property);
    QSignalSpy valueSpy(feature, testData.propertyChangedSignal.toLocal8Bit());
    QSignalSpy attributeSpy(feature, testData.attributeChangedSignal.toLocal8Bit());

    //Test Getter
    QCOMPARE((feature->*testData.getter)(), testData.valueList.at(valueIndex));
    QCOMPARE(property->property("value"), qtivi_convertValue(testData.valueList.at(valueIndex)));
    valueIndex++;

    //Test Setter
    if (testData.setter) {
        (feature->*testData.setter)(testData.valueList.at(valueIndex));
        QVERIFY2(valueSpy.count() == 1, "property change signal was not sent, after calling the setter");
        QCOMPARE(valueSpy.takeFirst().at(0).value<T>(), testData.valueList.at(valueIndex));
        QCOMPARE((feature->*testData.getter)(), testData.valueList.at(valueIndex));
        QCOMPARE(property->property("value"), qtivi_convertValue(testData.valueList.at(valueIndex)));
        valueIndex++;
    }

    //Test value change notification
    (backend->*testData.backend_setter)(testData.valueList.at(valueIndex), zone);
    QVERIFY2(valueSpy.count() == 1, "property change signal was not sent, after changing the value in the backend");
    QCOMPARE(valueSpy.takeFirst().at(0).value<T>(), testData.valueList.at(valueIndex));
    QCOMPARE((feature->*testData.getter)(), testData.valueList.at(valueIndex));
    QCOMPARE(property->property("value"), qtivi_convertValue(testData.valueList.at(valueIndex)));
    valueIndex++;

    //Test Setter IVIPROPERTY
    if (testData.setter) {
        property->setProperty("value", QVariant::fromValue(testData.valueList.at(valueIndex)));
        QVERIFY2(valueSpy.count() == 1, "property change signal was not sent, after calling the QIviProperty setter");
        QCOMPARE(valueSpy.takeFirst().at(0).value<T>(), testData.valueList.at(valueIndex));
        QCOMPARE((feature->*testData.getter)(), testData.valueList.at(valueIndex));
        QCOMPARE(property->property("value"), qtivi_convertValue(testData.valueList.at(valueIndex)));
    } else { //incase of a readonly property test that we can't change it from QML
        QTest::ignoreMessage(QtWarningMsg, "TypeError: Cannot assign to read-only property \"value\"");
        property->setProperty("value", QVariant::fromValue(testData.valueList.at(valueIndex)));
        QCOMPARE(property->property("value"), qtivi_convertValue(testData.valueList.at(valueIndex - 1)));
    }

    //Test attribute getter
    QCOMPARE((feature->*testData.attribute_getter)(), testData.attributeList.at(0));
    QCOMPARE(property->property("available").toBool(), testData.attributeList.at(0).isAvailable());
    QCOMPARE(property->property("minimumValue").value<T>(), testData.attributeList.at(0).minimumValue());
    QCOMPARE(property->property("maximumValue").value<T>(), testData.attributeList.at(0).maximumValue());

    //Test attribute change notification
    (backend->*testData.backend_attribute_setter)(testData.attributeList.at(1), zone);
    QVERIFY2(attributeSpy.count() == 1, "property change signal was not sent, after changing the value in the backend");
    QCOMPARE(attributeSpy.takeFirst().at(0).value<QIviPropertyAttribute<T>>(), testData.attributeList.at(1));
    QCOMPARE((feature->*testData.attribute_getter)(), testData.attributeList.at(1));
    QCOMPARE(property->property("available").toBool(), testData.attributeList.at(1).isAvailable());
    QCOMPARE(property->property("minimumValue").value<T>(), testData.attributeList.at(1).minimumValue());
    QCOMPARE(property->property("maximumValue").value<T>(), testData.attributeList.at(1).maximumValue());

    //TODO also check the availableValues here
}

#define PROPERTY_TEST_DATA_IMPL(_feature_, _backend_, _type_, _prop_, _capitalProp_, _valueList_, _attributeList_, _getter_, _setter_, _signal_, _backSetter_) \
QIviPropertyTestData<_feature_, _backend_, _type_>(#_prop_, \
                                                _valueList_, \
                                                _backSetter_, \
                                                SIGNAL(_signal_), \
                                                SIGNAL(_prop_##AttributeChanged(QIviPropertyAttribute<_type_>)), \
                                                _setter_, \
                                                _getter_, \
                                                _attributeList_, \
                                                &_feature_::_prop_##Attribute, \
                                                &_backend_::set##_capitalProp_##Attribute) \

#define PROPERTY_TEST_DATA(_feature_, _backend_, _type_, _prop_, _capitalProp_, _valueList_, _attributeList_) \
        PROPERTY_TEST_DATA_IMPL(_feature_, _backend_, _type_, _prop_, _capitalProp_, _valueList_, _attributeList_, \
                                &_feature_::_prop_, &_feature_::set##_capitalProp_, _prop_##Changed(_type_), &_backend_::set##_capitalProp_)

#define PROPERTY_TEST_DATA_READONLY(_feature_, _backend_, _type_, _prop_, _capitalProp_, _valueList_, _attributeList_) \
        PROPERTY_TEST_DATA_IMPL(_feature_, _backend_, _type_, _prop_, _capitalProp_, _valueList_, _attributeList_, \
                                &_feature_::_prop_, 0, _prop_##Changed(_type_), &_backend_::set##_capitalProp_)

#define PROPERTY_TEST_DATA_BOOL(_feature_, _backend_, _prop_, _capitalProp_, _valueList_, _attributeList_) \
        PROPERTY_TEST_DATA_IMPL(_feature_, _backend_, bool, _prop_, _capitalProp_, _valueList_, _attributeList_, \
                                &_feature_::is##_capitalProp_##Enabled, &_feature_::set##_capitalProp_##Enabled, _prop_##EnabledChanged(bool), &_backend_::set##_capitalProp_##Enabled)

#define PROPERTY_TEST_DATA_BOOL_READONLY(_feature_, _backend_, _prop_, _capitalProp_, _valueList_, _attributeList_) \
        PROPERTY_TEST_DATA_IMPL(_feature_, _backend_, bool, _prop_, _capitalProp_, _valueList_, _attributeList_, \
                                &_feature_::is##_capitalProp_##Enabled, 0, _prop_##EnabledChanged(bool), &_backend_::set##_capitalProp_##Enabled)

QT_END_NAMESPACE

#endif // QIVIPROPERTYTESTER_P_H
