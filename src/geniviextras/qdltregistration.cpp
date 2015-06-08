#include "qdltregistration.h"
/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

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
