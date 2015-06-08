/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#include <QString>
#include <QMap>

#include <dlt.h>

class QDltRegistrationPrivate {
public:
    QDltRegistrationPrivate();

    void registerCategory(const char* categoryName, DltContext* dltContext, const char* dltCtxName, const char* dltCtxDescription);
    void setDefaultContext(DltContext* dltContext);

    DltContext* context(const char* categoryName);

    QString m_dltAppID;
    DltContext* m_defaultContext;
    QMap<QString, DltContext*> m_map;
};
