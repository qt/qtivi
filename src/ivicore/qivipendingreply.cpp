/****************************************************************************
**
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

#include "qivipendingreply.h"
#include "qivipendingreply_p.h"
#include "qiviqmlconversion_helper.h"
#include "private/qjsengine_p.h"
#include "private/qjsvalue_p.h"

#include <QtQml>
#include <QDebug>
#include <QJSEngine>

QT_BEGIN_NAMESPACE

#define QTIVI_ADD_STATIC_METATYPE2(MetaTypeName, MetaTypeId, AliasingType, RealName) \
    qIviRegisterPendingReplyType<AliasingType>(RealName);

#define QTIVI_ADD_STATIC_METATYPE(MetaTypeName, MetaTypeId, AliasingType) \
    QTIVI_ADD_STATIC_METATYPE2(MetaTypeName, MetaTypeId, AliasingType, nullptr)

struct QIviPendingReplyRegistrator {
    QIviPendingReplyRegistrator() {
        qRegisterMetaType<QIviPendingReplyBase>("QIviPendingReplyBase");
        QT_FOR_EACH_STATIC_PRIMITIVE_TYPE(QTIVI_ADD_STATIC_METATYPE)
        QT_FOR_EACH_STATIC_PRIMITIVE_POINTER(QTIVI_ADD_STATIC_METATYPE)
        QT_FOR_EACH_STATIC_CORE_POINTER(QTIVI_ADD_STATIC_METATYPE)
        QT_FOR_EACH_STATIC_CORE_TEMPLATE(QTIVI_ADD_STATIC_METATYPE)
        QT_FOR_EACH_STATIC_CORE_CLASS(QTIVI_ADD_STATIC_METATYPE)
        QT_FOR_EACH_STATIC_ALIAS_TYPE(QTIVI_ADD_STATIC_METATYPE2)
    }
};
static QIviPendingReplyRegistrator _registrator;

// TODO make it reentrant

// TODO Add documentation
//* QSharedPointer<WatcherBase> d
//* WatcherBase has signals without argumments
//* WatcherBase has slots which calls the js callback functions
//* Value stored in WatcherBase
//* Do we need to pass the value in the signals ?
//* PendingReply needs a way to set the result
//* automatic type registration for basic types
//* Check the type is still correct when using the QVariant success functions
//* specialization for QVariant
//* call success/failed when reply already ready and functors are set later.
//* not for slots, use the old way instead

QIviPendingReplyWatcherPrivate::QIviPendingReplyWatcherPrivate(int userType, QIviPendingReplyWatcher *parent)
    : QObjectPrivate()
    , q_ptr(parent)
    , m_type(userType)
    , m_resultAvailable(false)
    , m_success(false)
    , m_data()
    , m_callbackEngine(nullptr)
{

}

void QIviPendingReplyWatcherPrivate::setSuccess(const QVariant &value)
{
    Q_Q(QIviPendingReplyWatcher);

    if (m_resultAvailable) {
        qWarning("Result is already set. Ignoring request");
        return;
    }

    m_resultAvailable = true;
    m_data = value;
    m_success = true;
    emit q->valueChanged(value);
    emit q->replySuccess();

    callSuccessCallback();
}

void QIviPendingReplyWatcherPrivate::callSuccessCallback()
{
    if (!m_successFunctor.isUndefined() && m_callbackEngine) {
        QJSValueList list = { m_callbackEngine->toScriptValue(m_data) };
        m_successFunctor.call(list);
    }
}

void QIviPendingReplyWatcherPrivate::callFailedCallback()
{
    if (!m_failedFunctor.isUndefined() && m_callbackEngine)
        m_failedFunctor.call();

    if (m_failedFunctor.isUndefined() && !m_successFunctor.isUndefined()) {
        //Logging category for logging unhandled failed functor with location.
    }
}

QIviPendingReplyWatcher::QIviPendingReplyWatcher(int userType)
    : QObject(*new QIviPendingReplyWatcherPrivate(userType, this))
{
}

//document default value
//note about having no changed signal (gadget) and use of watcher which has one
QVariant QIviPendingReplyWatcher::value() const
{
    Q_D(const QIviPendingReplyWatcher);
    return d->m_data;
}

bool QIviPendingReplyWatcher::isValid() const
{
    Q_D(const QIviPendingReplyWatcher);
    return d->m_type == -1 ? false : true;
}

bool QIviPendingReplyWatcher::isResultAvailable() const
{
    Q_D(const QIviPendingReplyWatcher);
    return d->m_resultAvailable;
}

bool QIviPendingReplyWatcher::isSuccessful() const
{
    Q_D(const QIviPendingReplyWatcher);
    return d->m_success;
}

void QIviPendingReplyWatcher::setSuccess(const QVariant &value)
{
    Q_D(QIviPendingReplyWatcher);

    if (d->m_resultAvailable) {
        qtivi_qmlOrCppWarning(this, QStringLiteral("Result is already set. Ignoring request"));
        return;
    }

    //no type checking needed when we expect a QVariant
    if (d->m_type == qMetaTypeId<QVariant>()) {
        d->setSuccess(value);
        return;
    }

    QVariant var = value;

    //Try to convert the value, if successfully, use the converted value
    QVariant temp(var);
    if (temp.convert(d->m_type))
        var = temp;

    //We need a special conversion for enums from QML as they are saved as int
    QMetaType metaType(d->m_type);
    bool isEnumOrFlag = false;

    const QMetaObject *mo = metaType.metaObject();
    const QString enumName = QString::fromLocal8Bit(QMetaType::typeName(d->m_type)).split(QStringLiteral("::")).last();
    if (mo) {
        QMetaEnum mEnum = mo->enumerator(mo->indexOfEnumerator(enumName.toLocal8Bit().constData()));
        if (mEnum.isValid()) {
            isEnumOrFlag = true;
            if (!mEnum.isFlag() && !mEnum.valueToKey(var.toInt())) {
                qtivi_qmlOrCppWarning(this, QStringLiteral("Enum value out of range"));
                return;
            }
        }
    }

    //Check that the type names match only if it's not a enum, as it will be converted automatically in this case.
    if (!isEnumOrFlag && var.typeName() != QVariant::typeToName(d->m_type)) {
        qtivi_qmlOrCppWarning(this, QString(QStringLiteral("Expected: %1 but got %2")).arg(QLatin1String(QVariant::typeToName(d->m_type)), QLatin1String(QVariant::typeToName(value.userType()))));
        return;
    }

    d->setSuccess(var);
}

void QIviPendingReplyWatcher::setFailed()
{
    Q_D(QIviPendingReplyWatcher);
    if (d->m_resultAvailable) {
        qWarning("Result is already set. Ignoring request");
        return;
    }

    d->m_resultAvailable = true;
    //TODO does it really make sense to emit it again ?
    emit valueChanged(d->m_data);
    emit replyFailed();

    d->callFailedCallback();
}

//Also document that this function needs to be called from a JSEngine(QMLEngine) only
//document that multple callbacks are not supported, use the Javascript promise instead
//check result when using from C++
void QIviPendingReplyWatcher::then(const QJSValue &success, const QJSValue &failed)
{
    if (!success.isUndefined() && !success.isCallable()) {
        qtivi_qmlOrCppWarning(this, QStringLiteral("The success functor is not callable"));
        return;
    }

    if (!failed.isUndefined() && !failed.isCallable()) {
        qtivi_qmlOrCppWarning(this, QStringLiteral("The failed functor is not callable"));
        return;
    }

    Q_D(QIviPendingReplyWatcher);
    d->m_successFunctor = success;
    d->m_failedFunctor = failed;
    d->m_callbackEngine = QJSValuePrivate::engine(&d->m_successFunctor)->jsEngine();
    if (!d->m_callbackEngine)
        d->m_callbackEngine = QJSValuePrivate::engine(&d->m_failedFunctor)->jsEngine();

    if (!d->m_callbackEngine)
        qtivi_qmlOrCppWarning(this, QStringLiteral("Couldn't access the current QJSEngine. The given callbacks will not be called without a valid QJSEngine"));

    if (d->m_resultAvailable) {
        if (d->m_success)
            d->callSuccessCallback();
        else
            d->callFailedCallback();
    }
}

QIviPendingReplyBase::QIviPendingReplyBase(int userType)
    : m_watcher(new QIviPendingReplyWatcher(userType))
{
}

QIviPendingReplyBase::QIviPendingReplyBase(const QIviPendingReplyBase &other)
{
    this->m_watcher = other.m_watcher;
}

QIviPendingReplyBase::~QIviPendingReplyBase()
{
}

//Note: this pointer will only be valid as long a there is a PendingReply object left.
QIviPendingReplyWatcher *QIviPendingReplyBase::watcher() const
{
    return m_watcher.data();
}

QVariant QIviPendingReplyBase::value() const
{
    return m_watcher->value();
}

bool QIviPendingReplyBase::isValid() const
{
    return m_watcher->isValid();
}

bool QIviPendingReplyBase::isResultAvailable() const
{
    return m_watcher->isResultAvailable();
}

bool QIviPendingReplyBase::isSuccessful() const
{
    return m_watcher->isSuccessful();
}

void QIviPendingReplyBase::then(const QJSValue &success, const QJSValue &failed)
{
    m_watcher->then(success, failed);
}

void QIviPendingReplyBase::setSuccess(const QVariant &value)
{
    m_watcher->setSuccess(value);
}

void QIviPendingReplyBase::setFailed()
{
    m_watcher->setFailed();
}

void QIviPendingReplyBase::setSuccessNoCheck(const QVariant &value)
{
    m_watcher->d_func()->setSuccess(value);
}

QT_END_NAMESPACE
