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

#include "qiviabstractfeature.h"
#include "qiviabstractzonedfeature.h"
#include "qiviserviceobject.h"
#include "qiviabstractfeature_p.h"
#include "qividefaultpropertyoverrider_p.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>

QT_BEGIN_NAMESPACE

QIviDefaultPropertyOverrider::PropertyOverride::PropertyOverride()
    : m_overridable(false)
    , m_overriding(false)
{
}

QIviDefaultPropertyOverrider::PropertyOverride::PropertyOverride(QIviDefaultPropertyOverrider::PropertyOverride &&other)
{
    *this = std::move(other);
}

QIviDefaultPropertyOverrider::PropertyOverride::PropertyOverride(const QMetaProperty &metaProperty, const QVariant &value, bool overridable)
    : m_metaProperty(metaProperty)
    , m_overridable(overridable)
    , m_overriding(false)
    , m_originalValue(value)
{
}

QIviDefaultPropertyOverrider::PropertyOverride &QIviDefaultPropertyOverrider::PropertyOverride::operator=(QIviDefaultPropertyOverrider::PropertyOverride &&other)
{
    m_metaProperty = other.m_metaProperty;
    m_originalValue = other.m_originalValue;
    m_overridenValue = other.m_overridenValue;
    m_overriding = other.m_overriding;
    m_overridable = other.m_overridable;
    return *this;
}

int QIviDefaultPropertyOverrider::PropertyOverride::propertyIndex() const
{
    return m_metaProperty.isValid() ? m_metaProperty.propertyIndex() : -1;
}

bool QIviDefaultPropertyOverrider::PropertyOverride::isValid() const
{
    return m_metaProperty.isValid();
}

bool QIviDefaultPropertyOverrider::PropertyOverride::isAvailable() const
{
    return m_metaProperty.isValid();
}

bool QIviDefaultPropertyOverrider::PropertyOverride::hasNotifySignal() const
{
    return m_metaProperty.hasNotifySignal();
}

bool QIviDefaultPropertyOverrider::PropertyOverride::isWritable() const
{
    return (m_metaProperty.isWritable() && !QMetaType(m_metaProperty.userType()).flags().testFlag(QMetaType::PointerToQObject));
}

bool QIviDefaultPropertyOverrider::PropertyOverride::isOverridable() const
{
    if (!m_overridable)
        return false;
    int propertyOffset = QIviAbstractFeature::staticMetaObject.propertyCount();
    return m_metaProperty.propertyIndex() >= propertyOffset;
}

bool QIviDefaultPropertyOverrider::PropertyOverride::isOverridden() const
{
    return m_overriding;
}

QString QIviDefaultPropertyOverrider::PropertyOverride::name() const
{
    return QString::fromUtf8(m_metaProperty.name());
}

QString QIviDefaultPropertyOverrider::PropertyOverride::typeName() const
{
    const int userType(m_metaProperty.userType());
    return QString::fromLatin1(QMetaType::typeName(userType));
}

QString QIviDefaultPropertyOverrider::PropertyOverride::displayText() const
{
    const QVariant &value = m_overriding ? m_overridenValue : m_originalValue;
    return value.toString();
}

QVariant QIviDefaultPropertyOverrider::PropertyOverride::editValue() const
{
    const QVariant &value = m_overriding ? m_overridenValue : m_originalValue;
    return value;
}

QVariant QIviDefaultPropertyOverrider::PropertyOverride::cppValue() const
{
    return m_overriding ? m_overridenValue : m_originalValue;
}

void QIviDefaultPropertyOverrider::PropertyOverride::setOverriden(bool override)
{
    if (override != m_overriding) {
        m_overriding = override;
        if (m_overriding)
            m_overridenValue = m_originalValue;
    }
}

bool QIviDefaultPropertyOverrider::PropertyOverride::setOverridenValue(const QVariant &value, QIviAbstractFeature *carrier)
{
    Q_ASSERT(isAvailable());

    const bool wasOverride = isOverridden();
    bool isOverride = wasOverride || !isWritable();

    if (isOverride && !wasOverride && isOverridable())
        setOverriden(isOverride);
    if (isOverride)
        m_overridenValue = value;

    if (m_metaProperty.isWritable())
        return m_metaProperty.write(carrier, value);
    else
        return notifyOverridenValue(value, carrier);
}

bool QIviDefaultPropertyOverrider::PropertyOverride::notifyOverridenValue(const QVariant &value, QIviAbstractFeature *carrier)
{
    QIviAbstractFeaturePrivate *d = QIviAbstractFeaturePrivate::get(carrier);
    if (d && d->notify(m_metaProperty.name(), value))
        return false;

    QMetaMethod notifySignal = m_metaProperty.notifySignal();
    if (!notifySignal.isValid() || notifySignal.parameterCount() != 1)
        return false;

    switch (value.type()) {
    case QVariant::Int: return notifySignal.invoke(carrier, Q_ARG(int, value.value<int>()));
    case QVariant::String: return notifySignal.invoke(carrier, Q_ARG(QString, value.value<QString>()));
    case QVariant::Double: return notifySignal.invoke(carrier, Q_ARG(double, value.value<double>()));
    case QVariant::Bool: return notifySignal.invoke(carrier, Q_ARG(double, value.value<bool>()));
    default:
        return false;
    }
}

void QIviDefaultPropertyOverrider::PropertyOverride::setOriginalValue(const QVariant &editValue)
{
    m_originalValue = editValue;
}

bool QIviDefaultPropertyOverrider::PropertyOverride::operator==(const QByteArray &property) const
{
    return this->m_metaProperty.name() == property;
}


QIviDefaultPropertyOverrider::QIviDefaultPropertyOverrider(QIviAbstractFeature *carrier, QObject *parent)
    : QObject(parent)
    , m_serviceObject(nullptr)
{
    if (carrier) {
        m_serviceObject = carrier->serviceObject();
        init(carrier);
        m_carriers.push_back(carrier);
    }
}

QIviDefaultPropertyOverrider::~QIviDefaultPropertyOverrider()
{
    for (auto &c : qAsConst(m_carriers))
        setCarrierOverride(false, c);
}

void QIviDefaultPropertyOverrider::init(QIviAbstractFeature *carrier)
{
    if (!carrier || !m_properties.empty())
        return;

    const QMetaObject *mo = carrier->metaObject();
    const int propertyOffset = QIviAbstractFeature::staticMetaObject.propertyCount();
    const int propertyCount = mo->propertyCount() - propertyOffset;

    QIviAbstractFeaturePrivate *carrierPrivate = QIviAbstractFeaturePrivate::get(carrier);
    const bool canOveride = carrierPrivate && carrierPrivate->m_supportsPropertyOverriding;

    const QByteArray normalizedSignal(QMetaObject::normalizedSignature("propertyChanged()"));
    const int propertyChangedSignalIndex(QIviDefaultPropertyOverrider::staticMetaObject.indexOfSignal(normalizedSignal));

    for (int i = 0; i < propertyCount; ++i) {
        const QMetaProperty metaProperty(mo->property(i + propertyOffset));
        qDebug() << i << metaProperty.name() << metaProperty.isValid();
        m_properties.push_back(PropertyOverride(metaProperty, metaProperty.read(carrier), canOveride));
        if (metaProperty.hasNotifySignal()) {
            QMetaObject::connect(carrier, metaProperty.notifySignalIndex(), this, propertyChangedSignalIndex);
        }
    }
}

void QIviDefaultPropertyOverrider::addCarrier(QIviAbstractFeature *carrier)
{
    if (!carrier)
        return;

    init(carrier);
    m_carriers.push_back(carrier);
    setCarrierOverride(true, carrier);
}

void QIviDefaultPropertyOverrider::removeCarrier(QIviAbstractFeature *carrier)
{
    setCarrierOverride(false, carrier);
    m_carriers.erase(std::remove_if(m_carriers.begin(), m_carriers.end(), [carrier](QIviAbstractFeature *c) {
        return carrier == c;
    }), m_carriers.end());
}

void QIviDefaultPropertyOverrider::setCarrierOverride(bool override, QIviAbstractFeature *carrier)
{
    if (!carrier)
        return;
    QIviAbstractFeaturePrivate *const pPriv = QIviAbstractFeaturePrivate::get(carrier);
    if (!pPriv)
        return;
    if (override && pPriv->m_propertyOverride == nullptr) {
        pPriv->m_propertyOverride = this;
    } else if (!override && pPriv->m_propertyOverride == this) {
        pPriv->m_propertyOverride = nullptr;
    }
}

int QIviDefaultPropertyOverrider::numCarriers() const
{
    return (int) m_carriers.size();
}

QVariant QIviDefaultPropertyOverrider::property(int propertyIndex) const
{
    if (m_carriers.empty())
        return {};

    const PropertyOverride &property = propertyForIndex(propertyIndex);
    return property.cppValue();
}

void QIviDefaultPropertyOverrider::setProperty(int propertyIndex, const QVariant &value)
{
    if (m_carriers.empty())
        return;

    PropertyOverride &property = propertyForIndex(propertyIndex);
    property.setOriginalValue(value);
}

bool QIviDefaultPropertyOverrider::isOverridden(int propertyIndex) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = propertyForIndex(propertyIndex);
    return property.isOverridden();
}

bool QIviDefaultPropertyOverrider::isWritableAt(int index) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = m_properties.at(index);
    return property.isWritable();
}

bool QIviDefaultPropertyOverrider::isOverridableAt(int index) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = m_properties.at(index);
    return property.isOverridable();
}

bool QIviDefaultPropertyOverrider::isAvailableAt(int index) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = m_properties.at(index);
    return property.isAvailable();
}

bool QIviDefaultPropertyOverrider::isOverriddenAt(int index) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = m_properties.at(index);
    return property.isOverridden();
}

bool QIviDefaultPropertyOverrider::hasNotifySignalAt(int index) const
{
    if (m_carriers.empty())
        return false;

    const PropertyOverride &property = m_properties.at(index);
    return property.hasNotifySignal();
}

QString QIviDefaultPropertyOverrider::nameAt(int index) const
{
    if (m_carriers.empty())
        return {};

    const PropertyOverride &property = m_properties.at(index);
    return property.name();
}

QString QIviDefaultPropertyOverrider::typeNameAt(int index) const
{
    if (m_carriers.empty())
        return {};

    const PropertyOverride &property = m_properties.at(index);
    return property.typeName();
}

QString QIviDefaultPropertyOverrider::displayTextAt(int index) const
{
    if (m_carriers.empty())
        return {};

    const PropertyOverride &property = m_properties.at(index);
    return property.displayText();
}

QVariant QIviDefaultPropertyOverrider::iviConstraintsAt(int index) const
{
    if (m_carriers.empty())
        return {};

    auto carrier = m_carriers.front();
    const PropertyOverride &property = m_properties.at(index);

    QByteArray constraintsJSON;
    for (int i=0; i<carrier->metaObject()->classInfoCount(); i++) {
        QMetaClassInfo ci = carrier->metaObject()->classInfo(i);
        if (QLatin1String(ci.name()) == QLatin1String("IviPropertyDomains")) {
            constraintsJSON = QByteArray(ci.value());
            break;
        }
    }
    if (constraintsJSON.isEmpty())
        return {};

    QJsonDocument loadDoc = QJsonDocument::fromJson(constraintsJSON);
    if (loadDoc.isNull() || loadDoc.isEmpty() || !loadDoc.isObject())
        return {};

    if (!loadDoc.object().contains(property.name()))
        return {};

    QJsonValue constraints = loadDoc.object().value(property.name());
    QLatin1String range("range");
    if (constraints.toObject().contains(range)) {
        QJsonArray vals = constraints.toObject().value(range).toArray();
        return QVariantList() << range << vals.at(0).toDouble() << vals.at(1).toDouble();
    }
    QLatin1String minimum("minimum");
    if (constraints.toObject().contains(minimum)) {
        double val = constraints.toObject().value(minimum).toDouble();
        return QVariantList() << minimum << val;
    }
    QLatin1String maximum("maximum");
    if (constraints.toObject().contains(maximum)) {
        double val = constraints.toObject().value(maximum).toDouble();
        return QVariantList() << maximum << val;
    }
    QLatin1String domain("domain");
    if (constraints.toObject().contains(domain)) {
        const QVariantList vals = constraints.toObject().value(domain).toArray().toVariantList();
        QVariantList res;
        res << domain << vals;
        return res;
    }
    return {};
}

QVariant QIviDefaultPropertyOverrider::editValueAt(int index) const
{
    if (m_carriers.empty())
        return {};

    const PropertyOverride &property = m_properties.at(index);
    return property.editValue();
}

QString QIviDefaultPropertyOverrider::label() const
{
    if (m_carriers.empty())
        return {};
    QString name = m_serviceObject ? m_serviceObject->objectName() : QString();
    if (name.isEmpty())
        name = typeName();
    if (const QIviAbstractZonedFeature *zoned = qobject_cast<const QIviAbstractZonedFeature *>(m_carriers.front()))
        // not translated; the zone API is fixed to English, too
        name += QString::fromLatin1(" [Zone: %1]").arg(zoned->zone());
    return name;
}

QString QIviDefaultPropertyOverrider::description() const
{
    if (!m_serviceObject)
        return {};
    return QString(QLatin1String("Backend Type: %1\nInstances in zone: %2"))
            .arg(QString::fromLatin1(m_serviceObject->metaObject()->className()))
            .arg(m_carriers.size());
}

QString QIviDefaultPropertyOverrider::typeName() const
{
    if (m_carriers.empty())
        return {};
    return QString::fromLatin1(m_carriers.front()->metaObject()->className());
}

bool QIviDefaultPropertyOverrider::setOverride(int index, bool isOverride)
{
    PropertyOverride &property = m_properties.at(index);
    if (property.isOverridable() && isOverride != property.isOverridden()) {
        if (!isOverride) {
            QByteArray flag = QString(QLatin1String("%1DirtyOverride")).arg(property.name()).toLatin1();
            for (const auto &carrier : m_carriers) {
                carrier->setProperty(flag.data(), true);
                property.setOverridenValue(property.m_originalValue, carrier);
            }
        }
        property.setOverriden(isOverride);
        return true;
    }
    return false;
}

bool QIviDefaultPropertyOverrider::setOverridenValue(int index, const QVariant &value)
{
    PropertyOverride &property = m_properties.at(index);
    bool res = false;
    for (const auto &carrier : m_carriers) {
        if (property.setOverridenValue(value, carrier))
            res = true;
    }
    return true;
}

int QIviDefaultPropertyOverrider::propertyCount() const
{
    return m_properties.size();
}

const QIviDefaultPropertyOverrider::PropertyOverride &QIviDefaultPropertyOverrider::propertyForIndex(int index) const
{
    static QIviDefaultPropertyOverrider::PropertyOverride dummy;
    for (const auto &p: m_properties) {
        if (p.propertyIndex() == index)
            return p;
    }
    return dummy;
}

QIviDefaultPropertyOverrider::PropertyOverride &QIviDefaultPropertyOverrider::propertyForIndex(int index)
{
    static QIviDefaultPropertyOverrider::PropertyOverride dummy;
    for (auto &p: m_properties) {
        if (p.propertyIndex() == index)
            return p;
    }
    return dummy;
}

int QIviDefaultPropertyOverrider::indexOfProperty(const QByteArray &property) const
{
    for (uint i = 0; i < m_properties.size(); i++) {
        if (m_properties.at(i) == property) {
            return i;
        }
    }
    return -1;
}

bool QIviDefaultPropertyOverrider::handles(const QObject *carrier) const
{
    return std::find_if(m_carriers.begin(), m_carriers.end(), [carrier](const QIviAbstractFeature *p) {
        return p == carrier;
    }) != m_carriers.end();
}

QString QIviDefaultPropertyOverrider::serviceId() const
{
    return m_serviceObject ? m_serviceObject->id() : QString();
}

QT_END_NAMESPACE

#include "moc_qividefaultpropertyoverrider_p.cpp"
