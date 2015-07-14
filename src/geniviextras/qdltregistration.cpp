/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.pelagicore.com/
**
** This file is part of QtGeniviExtras.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial QtGeniviExtras licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions see http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, Pelagicore gives you certain additional
** rights. These rights are described in Pelagicore LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdltregistration.h"
#include "qdltregistration_p.h"

#include <QDebug>

Q_GLOBAL_STATIC(QDltRegistration, dltRegistration)

QDltRegistration *globalDltRegistration()
{
    return dltRegistration();
}

QDltRegistrationPrivate::QDltRegistrationPrivate()
    : m_defaultContext(nullptr)
{
}

void QDltRegistrationPrivate::registerCategory(const char *categoryName, DltContext *dltContext, const char *dltCtxName, const char *dltCtxDescription)
{
    DLT_REGISTER_CONTEXT(*dltContext, dltCtxName, dltCtxDescription);
    m_map.insert(QString::fromLatin1(categoryName), dltContext);
}

void QDltRegistrationPrivate::setDefaultContext(DltContext *dltContext)
{
    m_defaultContext = dltContext;
}

DltContext *QDltRegistrationPrivate::context(const char *categoryName)
{
    const QString category = QString::fromLatin1(categoryName);
    if (!m_map.contains(category) && m_defaultContext)
        return m_defaultContext;

    return m_map.value(category);
}



QDltRegistration::QDltRegistration()
    : d_ptr(new QDltRegistrationPrivate())
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

void QDltRegistration::registerCategory(const char *categoryName, const char *dltCtxName, const char *dltCtxDescription)
{
    Q_D(QDltRegistration);
    //TODO memory leak
    d->registerCategory(categoryName, new DltContext, dltCtxName, dltCtxDescription);
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
    case QtWarningMsg: logLevel = DLT_LOG_INFO; break;
    case QtCriticalMsg: logLevel = DLT_LOG_ERROR; break;
    case QtFatalMsg: logLevel = DLT_LOG_FATAL; break;
    }

    DLT_LOG(*dltCtx, logLevel, DLT_STRING(qPrintable(qFormatLogMessage(msgTypes, msgCtx, msg))));
}
