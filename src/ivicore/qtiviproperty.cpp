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
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "qtiviproperty.h"
#include "qtiviproperty_p.h"

#include <QMetaEnum>

//Needed for error reporting
#include <private/qv8engine_p.h>
#include <private/qv4engine_p.h>

QtIVIPropertyPrivate::QtIVIPropertyPrivate(int userType, QtPrivate::QSlotObjectBase *attributeGetter, QtPrivate::QSlotObjectBase *valueGetter)
    : m_type(userType)
    , m_attributeGetter(attributeGetter)
    , m_valueGetter(valueGetter)
    , m_valueSetter(Q_NULLPTR)
{}

void QtIVIPropertyPrivate::throwError(QObject *object, const QString &error)
{
    QJSEngine* jsEngine = qjsEngine(object);
    if (jsEngine) {
        QV4::ExecutionEngine *v4 = QV8Engine::getV4(jsEngine);
        v4->throwError(error);
    } else {
        qWarning("%s", qPrintable(error));
    }
}

/*!
 * \class QtIVIProperty
 * \inmodule QtIVICore
 * \brief The QtIVIProperty combines a normal property and a QtIVIPropertyAttribute
 *
 * The QtIVIProperty is used to retrieve the value of a property and its corresponding
 * QtIVIPropertyAttribute in an easy way. It's designed for QML (but is not limited to)
 * and acts as a Grouped Property.
 *
 * Although the QtIVIProperty only contains QVariant properties, it still stores
 * the real type of the value and checks that only values of the correct type can be
 * stored in it.
 * The QtIVIProperty doesn't store a copy of the values, instead it just acts as a
 * forwarder which calls the respective getters and forwards the notification signals.
 *
 * By default a QtIVIProperty is also able to write a value change back to its origin,
 * but it also supports read-only properties which don't provide a setter for the value.
 *
 * QtIVIProperty is an abstract class and can only be created by using the QtIVIPropertyFactory
 * class.
 */
/*!
 * \property QtIVIProperty::available
 * True if this property is available.
 * \sa QtIVIPropertyAttribute::isAvailable()
 */
/*!
 * \property QtIVIProperty::minimumValue
 * The minimum value of the property.
 * \sa QtIVIPropertyAttribute::minimumValue()
 */
/*!
 * \property QtIVIProperty::maximumValue
 * The maximum value of the property.
 * \sa QtIVIPropertyAttribute::maximumValue()
 */
/*!
 * \property QtIVIProperty::availableValues
 * All available Values for this property.
 * This can be used when exposing for example an enum and the backend only supports some of the values.
 * \sa QtIVIPropertyAttribute::availableValues()
 */

/*!
 * Destructor.
 */
QtIVIProperty::~QtIVIProperty()
{
}

/*!
 * \property QtIVIProperty::value
 * Stores the value of the property as a QVariant
 */
void QtIVIProperty::setValue(const QVariant &value)
{
    Q_D(QtIVIProperty);
    QVariant var = value;

    //Try to behave like QML would be ;-)
    if (!d->m_valueSetter) {
        d->throwError(this, QLatin1String("TypeError: Cannot assign to read-only property \"value\""));
        return;
    }

    //Try to convert the value, if successfully, use the converted value
    QVariant temp(var);
    if (temp.convert(d->m_type))
        var = temp;

    //We need a special conversion for enums from QML as they are saved as int
    QMetaType metaType(d->m_type);
    if (metaType.flags() & QMetaType::IsEnumeration) {
        const QMetaObject* mo = metaType.metaObject();
        QString enumName = QString::fromLocal8Bit(QMetaType::typeName(d->m_type)).split(QStringLiteral("::")).last();
        if (mo) {
            QMetaEnum mEnum = mo->enumerator(mo->indexOfEnumerator(enumName.toLocal8Bit().constData()));
            if (!mEnum.valueToKey(var.toInt())) {
                d->throwError(this, QLatin1String("Enum value out of range"));
                return;
            }
        }
    } else if (var.typeName() != QVariant::typeToName(d->m_type)) {
        d->throwError(this, QStringLiteral("Expected: %1 bot got %2").arg(QLatin1String(QVariant::typeToName(d->m_type)), QLatin1String(QVariant::typeToName(value.userType()))));
        return;
    }

    void *args[] = { Q_NULLPTR, var.data() };
    d->m_valueSetter->call(parent(), args);
}

/*!
 * \internal
 */
QtIVIProperty::QtIVIProperty(int userType, const QObject *receiver,  QtPrivate::QSlotObjectBase *attributeGetter, QtPrivate::QSlotObjectBase *valueGetter)
    : QObject(const_cast<QObject*>(receiver))
    , d_ptr(new QtIVIPropertyPrivate(userType, attributeGetter, valueGetter))
{
}

/*!
 * \internal
 */
void QtIVIProperty::setValueSetter(QtPrivate::QSlotObjectBase *valueSetter)
{
    Q_D(QtIVIProperty);
    d->m_valueSetter = valueSetter;
}

/*!
 * \internal
 */
QtPrivate::QSlotObjectBase *QtIVIProperty::attributeGetter() const
{
    Q_D(const QtIVIProperty);
    return d->m_attributeGetter;
}

/*!
 * \internal
 */
QtPrivate::QSlotObjectBase *QtIVIProperty::valueGetter() const
{
    Q_D(const QtIVIProperty);
    return d->m_valueGetter;
}
