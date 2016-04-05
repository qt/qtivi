/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
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

#ifndef QTIVIPROPERTYFACTORY_H
#define QTIVIPROPERTYFACTORY_H

#include <QtIVICore/qtiviglobal.h>
#include <QtIVICore/QtIVIProperty>

QT_BEGIN_NAMESPACE

template <typename T>
class QtIVIPropertyFactory : public QtIVIProperty
{
public:

    //readwrite property
    template <typename attributeGetterFunc, typename attributeSignalFunc, typename valueGetterFunc, typename valueSignalFunc, typename valueSlotFunc>
    static inline QtIVIPropertyFactory *create(const typename QtPrivate::FunctionPointer<attributeGetterFunc>::Object *sender, attributeGetterFunc attributeGetter, attributeSignalFunc attributeSignal, valueGetterFunc valueGetter, valueSignalFunc valueSignal, valueSlotFunc valueSlot)
    {
        typedef QtPrivate::FunctionPointer<valueSignalFunc> ValueSignalType;
        typedef QtPrivate::FunctionPointer<valueSlotFunc> SlotType;
        typedef QtPrivate::List<T, void> ValueType;

        Q_STATIC_ASSERT_X(int(SlotType::ArgumentCount) == 1,
                          "The value slot needs to have exactly one argument");

        Q_STATIC_ASSERT_X((QtPrivate::CheckCompatibleArguments<typename ValueSignalType::Arguments, typename SlotType::Arguments>::value),
                          "Value signal and value slot arguments are not compatible.");
        Q_STATIC_ASSERT_X((QtPrivate::CheckCompatibleArguments<ValueType, typename SlotType::Arguments>::value),
                          "Property type and value slot arguments are not compatible.");

        QtIVIPropertyFactory *prop = create(sender, attributeGetter, attributeSignal, valueGetter, valueSignal);
        prop->setValueSetterHelper(new QtPrivate::QSlotObject<valueSlotFunc, typename SlotType::Arguments, void>(valueSlot));

        return prop;
    }

    //readonly property (no write slot)
    template <typename attributeGetterFunc, typename attributeSignalFunc, typename valueGetterFunc, typename valueSignalFunc>
    static inline QtIVIPropertyFactory *create(const typename QtPrivate::FunctionPointer<attributeGetterFunc>::Object *sender, attributeGetterFunc attributeGetter, attributeSignalFunc attributeSignal, valueGetterFunc valueGetter, valueSignalFunc valueSignal)
    {
        typedef QtPrivate::FunctionPointer<attributeGetterFunc> AttributeGetterType;
        typedef QtPrivate::FunctionPointer<attributeSignalFunc> AttributeSignalType;
        typedef QtPrivate::FunctionPointer<valueGetterFunc> ValueGetterType;
        typedef QtPrivate::FunctionPointer<valueSignalFunc> ValueSignalType;

        Q_STATIC_ASSERT_X(int(AttributeGetterType::ArgumentCount) == 0,
                          "The attribute getter can't have any arguments' ");

        Q_STATIC_ASSERT_X(int(ValueGetterType::ArgumentCount) == 0,
                          "The value getter can't have any arguments' ");

        Q_STATIC_ASSERT_X(int(ValueSignalType::ArgumentCount) == 1,
                          "The value signal needs to have exactly one argument");

        typedef QtPrivate::List<typename ValueGetterType::ReturnType, void> ValueGetterReturnType;
        typedef QtPrivate::List<QtIVIPropertyAttribute<T>, void> AttributeType;
        typedef QtPrivate::List<typename AttributeGetterType::ReturnType, void> AttributeGetterReturnType;

        Q_STATIC_ASSERT_X((QtPrivate::CheckCompatibleArguments<ValueGetterReturnType, typename ValueSignalType::Arguments>::value),
                          "Value getter return type and value signal arguments are not compatible.");


        Q_STATIC_ASSERT_X((QtPrivate::CheckCompatibleArguments<AttributeType, typename AttributeSignalType::Arguments>::value),
                          "Attribute type and attribute signal arguments are not compatible.");

        Q_STATIC_ASSERT_X((QtPrivate::CheckCompatibleArguments<AttributeGetterReturnType, typename AttributeSignalType::Arguments>::value),
                          "Attribute getter return type and attribute signal arguments are not compatible.");

        //Only Enums declared with Q_ENUM can be converted from an int without knowing the MetaObject where it is defined in
        //We also can't check this at compile time, so we check this at runtime.
        //We use a qFatal here to ensure that we die right when we are detecting this.
        int userType = qMetaTypeId<T>();
        QMetaType metaType(userType);
        if (metaType.flags() & QMetaType::IsEnumeration) {
            const QMetaObject *mo = metaType.metaObject();
            if (!mo)
                //TODO Do we want to use a qFatal() here or just report a qCritical instead ?
                qFatal("The provided enum needs to be declared as Q_ENUM or Q_FLAG");
        }
        QtIVIPropertyFactory *prop = new QtIVIPropertyFactory(userType, sender,
                                                      new QtPrivate::QSlotObject<attributeGetterFunc, typename AttributeGetterType::Arguments, typename AttributeGetterType::ReturnType>(attributeGetter),
                                                      new QtPrivate::QSlotObject<valueGetterFunc, typename ValueGetterType::Arguments, typename ValueGetterType::ReturnType>(valueGetter));

        connect(sender, attributeSignal, prop, [prop](const QtIVIPropertyAttribute<T> &attribute) {prop->updateAttribute(attribute);});
        connect(sender, valueSignal, prop, &QtIVIPropertyFactory::valueChanged);

        return prop;
    }

    bool isAvailable() const Q_DECL_OVERRIDE
    {
        return callAttributeGetter().isAvailable();
    }

    QVariant minimumValue() const Q_DECL_OVERRIDE
    {
        return QVariant::fromValue<T>(callAttributeGetter().minimumValue());
    }
    QVariant maximumValue() const Q_DECL_OVERRIDE
    {
        return QVariant::fromValue<T>(callAttributeGetter().maximumValue());
    }
    QVariantList availableValues() const Q_DECL_OVERRIDE
    {
        return convertAvailableValues(callAttributeGetter().availableValues());
    }

    QVariant value() const Q_DECL_OVERRIDE
    {
        T val;
        void *args[] = { reinterpret_cast<void*>(&val), QVariant().data() };
        valueGetter()->call(parent(), args);
        return val;
    }

private:
    QtIVIPropertyFactory(int userType, const QObject *receiver, QtPrivate::QSlotObjectBase *attGetter, QtPrivate::QSlotObjectBase *valGetter)
        : QtIVIProperty(userType, receiver, attGetter, valGetter)
    {}

    //We need to know the exact type here to allocate it before calling the getter.
    //The call function will just use the operator=() function to assign it to our local instance,
    //but will not create a new one.
    QtIVIPropertyAttribute<T> callAttributeGetter() const {
        QtIVIPropertyAttribute<T> attribute;
        void *args[] = { reinterpret_cast<void*>(&attribute), QVariant().data() };
        attributeGetter()->call(parent(), args);
        return attribute;
    }

    void updateAttribute(const QtIVIPropertyAttribute<T> &attribute)
    {
        Q_EMIT availableChanged(attribute.isAvailable());
        Q_EMIT minimumValueChanged(QVariant::fromValue<T>(attribute.minimumValue()));
        Q_EMIT maximumValueChanged(QVariant::fromValue<T>(attribute.maximumValue()));
        Q_EMIT availableValuesChanged(convertAvailableValues(attribute.availableValues()));
    }

    QVariantList convertAvailableValues(const QVector<T> &aValues) const
    {
        QVariantList list;
        Q_FOREACH (const T &val, aValues) {
            //As QML doesn't support Enums in Lists we need to convert it to int
            //TODO Do we really want to do this ?
            int userType = qMetaTypeId<T>();
            QMetaType metaType(userType);
            QVariant var = QVariant::fromValue<T>(val);
            if (metaType.flags() & QMetaType::IsEnumeration)
                var = var.toInt();
            list.append(var);
        }
        return list;
    }


    //Just needed as we can't call the protected function directly in the create() functions
    void setValueSetterHelper(QtPrivate::QSlotObjectBase *valueSetter)
    {
        setValueSetter(valueSetter);
    }
};

QT_END_NAMESPACE

#endif // QTIVIPROPERTYFACTORY_H
