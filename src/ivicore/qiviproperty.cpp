/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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

#include "qiviproperty.h"
#include "qiviproperty_p.h"

#include <QMetaEnum>

//Needed for error reporting
#include <private/qv8engine_p.h>
#include <private/qv4engine_p.h>

QT_BEGIN_NAMESPACE

QIviPropertyPrivate::QIviPropertyPrivate(int userType, QtPrivate::QSlotObjectBase *attributeGetter, QtPrivate::QSlotObjectBase *valueGetter)
    : m_type(userType)
    , m_attributeGetter(attributeGetter)
    , m_valueGetter(valueGetter)
    , m_valueSetter(nullptr)
{}

void QIviPropertyPrivate::throwError(QObject *object, const QString &error)
{
    QJSEngine *jsEngine = qjsEngine(object);
    if (Q_UNLIKELY(!jsEngine)) {
        qWarning("%s", qPrintable(error));
        return;
    }
    QV4::ExecutionEngine *v4 = QV8Engine::getV4(jsEngine);
    v4->throwError(error);
}

/*!
 * \class QIviProperty
 * \inmodule QtIviCore
 * \brief The QIviProperty combines a normal property and a QIviPropertyAttribute
 *
 * The QIviProperty is used to retrieve the value of a property and its corresponding
 * QIviPropertyAttribute in an easy way. It's designed for QML (but is not limited to)
 * and acts as a Grouped Property.
 *
 * Although the QIviProperty only contains QVariant properties, it still stores
 * the real type of the value and checks that only values of the correct type can be
 * stored in it.
 * The QIviProperty doesn't store a copy of the values, instead it just acts as a
 * forwarder which calls the respective getters and forwards the notification signals.
 *
 * By default a QIviProperty is also able to write a value change back to its origin,
 * but it also supports read-only properties which don't provide a setter for the value.
 *
 * QIviProperty is an abstract class and can only be created by using the QIviPropertyFactory
 * class.
 */
/*!
 * \property QIviProperty::available
 * True if this property is available.
 * \sa QIviPropertyAttribute::isAvailable()
 */
/*!
 * \property QIviProperty::minimumValue
 * The minimum value of the property.
 * \sa QIviPropertyAttribute::minimumValue()
 */
/*!
 * \property QIviProperty::maximumValue
 * The maximum value of the property.
 * \sa QIviPropertyAttribute::maximumValue()
 */
/*!
 * \property QIviProperty::availableValues
 * All available Values for this property.
 * This can be used when exposing for example an enum and the backend only supports some of the values.
 * \sa QIviPropertyAttribute::availableValues()
 */

/*!
 * Destructor.
 */
QIviProperty::~QIviProperty()
{
}

/*!
 * \property QIviProperty::value
 * Stores the value of the property as a QVariant
 */
void QIviProperty::setValue(const QVariant &value)
{
    Q_D(QIviProperty);
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
    bool isEnumOrFlag = false;

    const QMetaObject *mo = metaType.metaObject();
    QString enumName = QString::fromLocal8Bit(QMetaType::typeName(d->m_type)).split(QStringLiteral("::")).last();
    if (mo) {
        QMetaEnum mEnum = mo->enumerator(mo->indexOfEnumerator(enumName.toLocal8Bit().constData()));
        if (mEnum.isValid()) {
            isEnumOrFlag = true;
            if (!mEnum.isFlag() && !mEnum.valueToKey(var.toInt())) {
                d->throwError(this, QLatin1String("Enum value out of range"));
                return;
            }
        }
    }

    //Check that the type names match only if it's not a enum, as it will be converted automatically in this case.
    if (!isEnumOrFlag && var.typeName() != QVariant::typeToName(d->m_type)) {
        d->throwError(this, QStringLiteral("Expected: %1 but got %2").arg(QLatin1String(QVariant::typeToName(d->m_type)), QLatin1String(QVariant::typeToName(value.userType()))));
        return;
    }

    void *args[] = { nullptr, var.data() };
    d->m_valueSetter->call(parent(), args);
}

/*!
 * \internal
 */
QIviProperty::QIviProperty(int userType, const QObject *receiver,  QtPrivate::QSlotObjectBase *attributeGetter, QtPrivate::QSlotObjectBase *valueGetter)
    : QObject(const_cast<QObject*>(receiver))
    , d_ptr(new QIviPropertyPrivate(userType, attributeGetter, valueGetter))
{
}

/*!
 * \internal
 */
void QIviProperty::setValueSetter(QtPrivate::QSlotObjectBase *valueSetter)
{
    Q_D(QIviProperty);
    d->m_valueSetter = valueSetter;
}

/*!
 * \internal
 */
QtPrivate::QSlotObjectBase *QIviProperty::attributeGetter() const
{
    Q_D(const QIviProperty);
    return d->m_attributeGetter;
}

/*!
 * \internal
 */
QtPrivate::QSlotObjectBase *QIviProperty::valueGetter() const
{
    Q_D(const QIviProperty);
    return d->m_valueGetter;
}

QT_END_NAMESPACE
