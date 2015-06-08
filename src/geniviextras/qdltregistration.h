/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#ifndef QDLTREGISTRATION_H
#define QDLTREGISTRATION_H

#include <QString>
#include <QMap>
#include <qgeniviextrasglobal.h>

class QDltRegistrationPrivate;

class Q_GENIVIEXTRAS_EXPORT QDltRegistration
{
public:
    QDltRegistration();
    ~QDltRegistration();

    void registerApplication(const char *dltAppID, const char *dltAppDescription);
    void unregisterApplication();

    void registerCategory(const char* categoryName, const char* dltCtxName, const char* dltCtxDescription);
    void setDefaultContext(const char* categoryName);

    static void messageHandler(QtMsgType msgTypes, const QMessageLogContext &msgCtx, const QString &msg);
private:
    QDltRegistrationPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(QDltRegistration)
};

Q_GENIVIEXTRAS_EXPORT extern QDltRegistration* globalDltRegistration();

#endif // QDLTREGISTRATION_H
