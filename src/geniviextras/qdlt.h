#ifndef QDLT_H
#define QDLT_H

#include <QGlobalStatic>
/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#include <QtGlobal>

#include "qdltregistration.h"

#define QDLT_REGISTER_APPLICATION(APP, DESCRIPTION) \
struct QDltAppRegistrator { \
    QDltAppRegistrator() { globalDltRegistration()->registerApplication(APP, DESCRIPTION); } \
}; \
static QDltAppRegistrator qdltAppRegistrator; \


#define QDLT_LOGGING_CATEGORY(CATEGORY, CATEGORYNAME, DLT_CTX_NAME, DLT_CTX_DESCRIPTION) \
Q_LOGGING_CATEGORY(CATEGORY, CATEGORYNAME) \
QDLT_REGISTER_LOGGING_CATEGORY(CATEGORY, CATEGORYNAME, DLT_CTX_NAME, DLT_CTX_DESCRIPTION) \

#define QDLT_REGISTER_LOGGING_CATEGORY(CATEGORY, CATEGORYNAME, DLT_CTX_NAME, DLT_CTX_DESCRIPTION) \
struct QDlt ## CATEGORY ## Registrator { \
    QDlt ## CATEGORY ## Registrator() { globalDltRegistration()->registerCategory(CATEGORYNAME , DLT_CTX_NAME, DLT_CTX_DESCRIPTION); } \
}; \
static QDlt ## CATEGORY ## Registrator qdlt ## CATEGORY ## registrator; \

#define QDLT_FALLBACK_CATEGORY(CATEGORY) \
struct QDltDefaultRegistrator { \
    QDltDefaultRegistrator() { globalDltRegistration()->setDefaultContext(CATEGORY().categoryName()); } \
}; \
static QDltDefaultRegistrator qdltDefaultRegistrator; \


#endif // QDLT_H

