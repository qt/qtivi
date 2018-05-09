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

#include "qdltregistration.h"
#include "qdltregistration_p.h"

#include <QDebug>
#include <QLoggingCategory>

#define GENIVIEXTRAS_DEBUG 0
#if GENIVIEXTRAS_DEBUG
#  include <iostream>
#endif

QT_BEGIN_NAMESPACE

void qtGeniviLogLevelChangedHandler(char context_id[], uint8_t log_level, uint8_t trace_status)
{
    globalDltRegistration()->d_ptr->dltLogLevelChanged(context_id, log_level, trace_status);
}

Q_GLOBAL_STATIC(QDltRegistration, dltRegistration)

QDltRegistration *globalDltRegistration()
{
    return dltRegistration();
}

QT_END_NAMESPACE

QDltRegistrationPrivate::QDltRegistrationPrivate(QDltRegistration *parent)
    : q_ptr(parent)
    , m_dltAppRegistered(false)
    , m_registerOnFirstUse(false)
{
}

void QDltRegistrationPrivate::registerCategory(const QLoggingCategory *category, DltContext *dltContext, const char *dltCtxName, const char *dltCtxDescription)
{
    CategoryInfo info;
    info.m_category = const_cast<QLoggingCategory*>(category);
    info.m_ctxName = dltCtxName;
    info.m_ctxDescription = dltCtxDescription;
    info.m_context = dltContext;

    if (!m_registerOnFirstUse) {
        registerCategory(info);
    } else {
        info.m_registered = false;
    }

    m_categoryInfoHash.insert(QString::fromLatin1(category->categoryName()), info);
}

void QDltRegistrationPrivate::registerCategory(CategoryInfo &info)
{
    Q_ASSERT_X(m_dltAppRegistered, "registerCategory", "A DLT Application needs to be registered before registering a Logging Category");

#if GENIVIEXTRAS_DEBUG
    std::cout << "REGISTERING CONTEXT " << info.m_ctxName.constData() << std::endl;
#endif
    DLT_REGISTER_CONTEXT_LL_TS(*info.m_context, info.m_ctxName, info.m_ctxDescription, category2dltLevel(info.m_category), DLT_TRACE_STATUS_DEFAULT);

#if QT_CONFIG(dlt_2_12)
    //TODO move to lamda once c++11 is ok to be used
    DLT_REGISTER_LOG_LEVEL_CHANGED_CALLBACK(*info.m_context, &qtGeniviLogLevelChangedHandler);
#endif
    info.m_registered = true;
}

void QDltRegistrationPrivate::registerApplication()
{
    Q_ASSERT_X(!m_dltAppID.isEmpty(), "registerApplication", "dltAppID needs to be a valid char * on the first call.");
    DLT_REGISTER_APP(m_dltAppID.toLocal8Bit(), m_dltAppDescription.toLocal8Bit());
    m_dltAppRegistered = true;
}

void QDltRegistrationPrivate::setDefaultCategory(const QString &category)
{
    Q_ASSERT_X(m_categoryInfoHash.contains(category), "setDefaultContext", "The category needs to be registered as a dlt logging category before it can be set as a default context");
    m_defaultCategory = category;
}

DltContext *QDltRegistrationPrivate::context(const char *categoryName)
{
    QString category = QString::fromLatin1(categoryName);
    if (!m_categoryInfoHash.contains(category) && !m_defaultCategory.isEmpty())
        category = m_defaultCategory;

    CategoryInfo &info = m_categoryInfoHash[category];
    if (info.m_context && !info.m_registered) {
        if (!m_dltAppRegistered)
            registerApplication();
        registerCategory(info);
    }

    return info.m_context;
}

void QDltRegistrationPrivate::dltLogLevelChanged(char context_id[], uint8_t log_level, uint8_t trace_status)
{
    Q_Q(QDltRegistration);
    Q_UNUSED(trace_status)

    for (auto it = m_categoryInfoHash.begin(); it != m_categoryInfoHash.end(); ++it) {
        if (it.value().m_ctxName != context_id)
            continue;

        QList<QtMsgType> msgTypes;

        //Enable all QtLoggingCategories with a lower severity than the DLT level
        switch (log_level) {
        case DLT_LOG_VERBOSE:
        case DLT_LOG_DEBUG: msgTypes << QtDebugMsg << QtWarningMsg << QtCriticalMsg << QtFatalMsg;
#if QT_VERSION >= 0x050500
            msgTypes << QtInfoMsg;
#endif
            break;
        case DLT_LOG_INFO: msgTypes << QtWarningMsg << QtCriticalMsg << QtFatalMsg;
#if QT_VERSION >= 0x050500
            msgTypes << QtInfoMsg;
#endif
            break;
        case DLT_LOG_WARN: msgTypes << QtCriticalMsg << QtFatalMsg;
#if QT_VERSION >= 0x050500
            msgTypes << QtInfoMsg;
#endif
            break;
        case DLT_LOG_ERROR: msgTypes << QtCriticalMsg << QtFatalMsg;  break;
        case DLT_LOG_FATAL: msgTypes << QtFatalMsg; break;
        case DLT_LOG_OFF: msgTypes = QList<QtMsgType>(); break;
        }

        QtMsgType end = QtFatalMsg;
#if QT_VERSION >= 0x050500
        end = QtInfoMsg;
#endif

        for (auto i = int(QtDebugMsg); i <= int(end); i++) {
            auto type = QtMsgType(i);
            bool enabled = true;
            if (!msgTypes.contains(type))
                enabled = !enabled;
            QLoggingCategory* category = it.value().m_category;
            if (category->isEnabled(type) != enabled) {
                category->setEnabled(type, enabled);
                emit q->logLevelChanged(category);
            }
        }
    }
}

DltLogLevelType QDltRegistrationPrivate::category2dltLevel(const QLoggingCategory *category)
{
    DltLogLevelType logLevel = DLT_LOG_OFF;

    if (category->isDebugEnabled())
        logLevel = DLT_LOG_DEBUG;
#if QT_VERSION >= 0x050500
    else if (category->isInfoEnabled())
        logLevel = DLT_LOG_INFO;
#endif
    else if (category->isWarningEnabled())
        logLevel = DLT_LOG_WARN;
    else if (category->isCriticalEnabled())
        logLevel = DLT_LOG_ERROR;

    return logLevel;
}

/*!
    \class QDltRegistration
    \inmodule QtGeniviExtras
    \brief The QDltRegistration class controls controls the mapping between QLoggingCategory and dlt context.

    The class talks to the dlt-daemon and provides a Qt messageHandler which forwards logging messages logged
    by the Qt Logging Framework to the dlt-daemon.

    Using dlt-daemon version 2.12 or higher it also reacts to DLT control messages and adapts the enabled msg
    types of a QLoggingCategory whenever the log level of a dlt context changes.
*/

QDltRegistration::QDltRegistration(QObject *parent)
    : QObject(parent)
    , d_ptr(new QDltRegistrationPrivate(this))
{
}

QDltRegistration::~QDltRegistration()
{
    unregisterApplication();
}

/*!
    Registers this application with the dlt-daemon using \a dltAppID and \a dltAppDescription.

    Per process only one application can be registered. Calling this function after an
    application is already registered, will register the application under the new \a dltAppID
    and \a dltAppDescription and also populating the already registered dlt contexts under this
    application. Calling the method with an empty \a dltAppID will update just the description
    if the app was already registered before.

    \sa QDLT_REGISTER_APPLICATION
*/
void QDltRegistration::registerApplication(const char *dltAppID, const char *dltAppDescription)
{
    Q_D(QDltRegistration);
    bool registerCategories = false;
    if (d->m_dltAppRegistered) {
        unregisterApplication();
        registerCategories = true;
    }

    if (dltAppID)
        d->m_dltAppID = QString::fromLatin1(dltAppID);
    d->m_dltAppDescription = QString::fromLatin1(dltAppDescription);

    if (!d->m_registerOnFirstUse)
        d->registerApplication();

    //Register all Contexts which already have been registered again under the new application
    if (registerCategories && !d->m_registerOnFirstUse) {
        for (auto it = d->m_categoryInfoHash.begin(); it != d->m_categoryInfoHash.end(); ++it) {
            if (it.value().m_registered)
                d->registerCategory(it.value());
        }
    }
}

/*!
    Registers \a category with the dlt-daemon using \a dltCtxName and \a dltCtxDescription.

    This function shouldn't be used directly, instead use the convenience macro.
    \sa QDLT_LOGGING_CATEGORY QDLT_REGISTER_LOGGING_CATEGORY
*/
void QDltRegistration::registerCategory(const QLoggingCategory *category, const char *dltCtxName, const char *dltCtxDescription)
{
    Q_D(QDltRegistration);
    Q_ASSERT(category);
    Q_ASSERT(strlen(category->categoryName()) != 0);
    d->registerCategory(category, new DltContext, dltCtxName, dltCtxDescription);
}

/*!
    Sets \a categoryName as the fallback logging category.

    \a categoryName needs to be the name of a valid QLoggingCategory which has been registered within the dlt-daemon.
    Either by using QDLT_LOGGING_CATEGORY or QDLT_REGISTER_LOGGING_CATEGORY.

    This function shouldn't be used directly, instead use the convenience macro.
    \sa QDLT_FALLBACK_CATEGORY
*/
void QDltRegistration::setDefaultContext(const char *categoryName)
{
    Q_D(QDltRegistration);
    d->setDefaultCategory(QString::fromLatin1(categoryName));
}

/*!
    When set to \a enabled the registration with the dlt-daemon is done on the first use of the category.

    Otherwise the registration is done directly when QDLT_LOGGING_CATEGORY or QDLT_REGISTER_LOGGING_CATEGORY
    is called.

    This function shouldn't be used directly, instead use the convenience macro.
    \sa QDLT_REGISTER_CONTEXT_ON_FIRST_USE
*/
void QDltRegistration::setRegisterContextOnFirstUseEnabled(bool enabled)
{
    Q_D(QDltRegistration);
    d->m_registerOnFirstUse = enabled;
}

/*!
    Registers all contexts with the dlt-daemon which are not yet registered.

    This function can be used in combination with setRegisterContextOnFirstUseEnabled to delay
    the registration with the dlt-daemon until it is really needed and once it is safe to register
    all context which haven't been registered yet, e.g. by using the associated logging category.

    \sa setRegisterContextOnFirstUseEnabled
*/
void QDltRegistration::registerUnregisteredContexts()
{
#if GENIVIEXTRAS_DEBUG
    std::cout << "REGISTERING UNREGISTERED CONTEXTS" << std::endl;
#endif
    Q_D(QDltRegistration);
    if (!d->m_dltAppRegistered)
        d->registerApplication();
    for (auto it = d->m_categoryInfoHash.begin(); it != d->m_categoryInfoHash.end(); ++it) {
        if (!it.value().m_registered) {
            d->registerCategory(it.value());
      }
    }
}

/*!
    Unregisters the application with the dlt-daemon.
    The registered application as well as all registered dlt context will be deleted.
*/
void QDltRegistration::unregisterApplication()
{
    Q_D(QDltRegistration);
    if (d->m_dltAppRegistered)
        DLT_UNREGISTER_APP();

    d->m_dltAppRegistered = false;
}

/*!
    The Qt message handler which forwards all the logging messages to the dlt-daemon.

    The function will map \a msgTypes to the appropriate dlt log level and forward the \a msg to
    the dlt context matching the category in \a msgCtx.

    If the category in \a msgCtx hasn't been registered with a dlt context, the fallback logging category
    will be used instead (if one is registered).

    This messageHandler needs to be installed using:
    \badcode
    qInstallMessageHandler(QDltRegistration::messageHandler);
    \endcode
*/
void QDltRegistration::messageHandler(QtMsgType msgTypes, const QMessageLogContext &msgCtx, const QString &msg)
{
    DltContext *dltCtx = globalDltRegistration()->d_ptr->context(msgCtx.category);
    if (!dltCtx)
        return;

    DltLogLevelType logLevel = DLT_LOG_OFF;

    switch (msgTypes) {
    case QtDebugMsg: logLevel = DLT_LOG_DEBUG; break;
#if QT_VERSION >= 0x050500
    case QtInfoMsg: logLevel = DLT_LOG_INFO; break;
#endif
    case QtWarningMsg: logLevel = DLT_LOG_WARN; break;
    case QtCriticalMsg: logLevel = DLT_LOG_ERROR; break;
    case QtFatalMsg: logLevel = DLT_LOG_FATAL; break;
    }

    DLT_LOG(*dltCtx, logLevel, DLT_STRING(qPrintable(qFormatLogMessage(msgTypes, msgCtx, msg))));
}

/*!
    \fn void QDltRegistration::logLevelChanged(const QLoggingCategory *category)

    This signal is emitted whenever the dlt-daemon changes the log level of a dlt context and
    the dlt context is registered with a QLoggingCategory. The updated QLoggingCategory is passed
    as \a category.

    \note This signal requires a dlt-daemon version equal to 2.12 or higher.
*/
