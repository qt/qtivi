/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#ifndef QDLTREGISTRATION_H
#define QDLTREGISTRATION_H

#include <QString>
#include <QMap>
#include <QObject>
#include <QtGeniviExtras/qgeniviextrasglobal.h>
#include <stdint.h>

QT_BEGIN_NAMESPACE

class QDltRegistrationPrivate;

class Q_GENIVIEXTRAS_EXPORT QDltRegistration : public QObject
{
    Q_OBJECT

public:
    QDltRegistration();
    ~QDltRegistration();

    void registerApplication(const char *dltAppID, const char *dltAppDescription);
    void unregisterApplication();

    void registerCategory(const QLoggingCategory *category, const char *dltCtxName, const char *dltCtxDescription);
    void setDefaultContext(const char *categoryName);

    static void messageHandler(QtMsgType msgTypes, const QMessageLogContext &msgCtx, const QString &msg);

Q_SIGNALS:
    void logLevelChanged(const QLoggingCategory *category);

private:
    QDltRegistrationPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QDltRegistration)

    friend void qtGeniviLogLevelChangedHandler(char context_id[], uint8_t log_level, uint8_t trace_status);
};

Q_GENIVIEXTRAS_EXPORT extern QDltRegistration *globalDltRegistration();

QT_END_NAMESPACE

#endif // QDLTREGISTRATION_H
