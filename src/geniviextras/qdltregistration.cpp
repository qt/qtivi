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

#include "qdltregistration.h"
#include "qdltregistration_p.h"

#include <QDebug>
#include <QLoggingCategory>

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
    , m_defaultContext(nullptr)
{
}

void QDltRegistrationPrivate::registerCategory(const QLoggingCategory* category, DltContext *dltContext, const char *dltCtxName, const char *dltCtxDescription)
{
    DLT_REGISTER_CONTEXT(*dltContext, dltCtxName, dltCtxDescription);
    m_categoryName2DltContext.insert(QString::fromLatin1(category->categoryName()), dltContext);
    m_ctxName2Category.insert(QString::fromLatin1(dltCtxName), const_cast<QLoggingCategory*>(category));

#ifdef DLT_VERSION_2_12
    //TODO move to lamda once c++11 is ok to be used
    DLT_REGISTER_LOG_LEVEL_CHANGED_CALLBACK(*dltContext, &qtGeniviLogLevelChangedHandler);
#endif
}

void QDltRegistrationPrivate::setDefaultContext(DltContext *dltContext)
{
    m_defaultContext = dltContext;
}

DltContext *QDltRegistrationPrivate::context(const char *categoryName)
{
    const QString category = QString::fromLatin1(categoryName);
    if (!m_categoryName2DltContext.contains(category) && m_defaultContext)
        return m_defaultContext;

    return m_categoryName2DltContext.value(category);
}

void QDltRegistrationPrivate::dltLogLevelChanged(char context_id[], uint8_t log_level, uint8_t trace_status)
{
    Q_Q(QDltRegistration);
    Q_UNUSED(trace_status)

    const QString contextName = QString::fromLatin1(context_id);
    if (m_ctxName2Category.contains(contextName))
    {
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

        for(int i = (int)QtDebugMsg; i <= (int)end; i++) {
            QtMsgType type = (QtMsgType)i;
            bool enabled = true;
            if (!msgTypes.contains(type))
                enabled = !enabled;
            QLoggingCategory* category = m_ctxName2Category.value(contextName);
            if (category->isEnabled(type) != enabled)
            {
                category->setEnabled(type, enabled);
                q->logLevelChanged(category);
            }
        }
    }
}

QDltRegistration::QDltRegistration()
    : d_ptr(new QDltRegistrationPrivate(this))
{
}

QDltRegistration::~QDltRegistration()
{
    unregisterApplication();
}

void QDltRegistration::registerApplication(const char *dltAppID, const char *dltAppDescription)
{
    Q_D(QDltRegistration);
    d->m_dltAppID = QString::fromLatin1(dltAppID);
    DLT_REGISTER_APP(dltAppID, dltAppDescription);
}

void QDltRegistration::registerCategory(const QLoggingCategory* category, const char *dltCtxName, const char *dltCtxDescription)
{
    Q_D(QDltRegistration);
    //TODO memory leak
    d->registerCategory(category, new DltContext, dltCtxName, dltCtxDescription);
}

void QDltRegistration::setDefaultContext(const char *categoryName)
{
    Q_D(QDltRegistration);
    d->setDefaultContext(d->context(categoryName));
}

void QDltRegistration::unregisterApplication()
{
    Q_D(QDltRegistration);
    if (!d->m_dltAppID.isEmpty())
        DLT_UNREGISTER_APP();
}

void QDltRegistration::messageHandler(QtMsgType msgTypes, const QMessageLogContext & msgCtx, const QString & msg)
{
    DltContext* dltCtx = globalDltRegistration()->d_ptr->context(msgCtx.category);
    if (!dltCtx)
    {
        return;
    }

    DltLogLevelType logLevel = DLT_LOG_INFO;

    switch (msgTypes) {
    case QtDebugMsg: logLevel = DLT_LOG_DEBUG; break;
    case QtWarningMsg: logLevel = DLT_LOG_WARN; break;
    case QtCriticalMsg: logLevel = DLT_LOG_ERROR; break;
    case QtFatalMsg: logLevel = DLT_LOG_FATAL; break;
#if QT_VERSION >= 0x050500
    case QtInfoMsg: logLevel = DLT_LOG_INFO; break;
#endif
    }

    DLT_LOG(*dltCtx, logLevel, DLT_STRING(qPrintable(qFormatLogMessage(msgTypes, msgCtx, msg))));
}
