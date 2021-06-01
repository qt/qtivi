/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
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
****************************************************************************/

#ifndef QDLT_H
#define QDLT_H

#include <QGlobalStatic>
#include <QtGlobal>
#include <QtGeniviExtras/qdltregistration.h>

QT_BEGIN_NAMESPACE

#define QDLT_REGISTER_CONTEXT_ON_FIRST_USE(ENABLED) \
    QT_BEGIN_NAMESPACE \
    struct QDltRegistrator { \
        QDltRegistrator() { globalDltRegistration()->setRegisterContextOnFirstUseEnabled(ENABLED); } \
    }; \
    static QDltRegistrator qdltRegistrator; \
    QT_END_NAMESPACE \

#define QDLT_REGISTER_APPLICATION(APP, DESCRIPTION) \
QT_BEGIN_NAMESPACE \
struct QDltAppRegistrator { \
    QDltAppRegistrator() { globalDltRegistration()->registerApplication(APP, DESCRIPTION); } \
}; \
static QDltAppRegistrator qdltAppRegistrator; \
QT_END_NAMESPACE \


#define QDLT_LOGGING_CATEGORY(CATEGORY, CATEGORYNAME, DLT_CTX_NAME, DLT_CTX_DESCRIPTION) \
Q_LOGGING_CATEGORY(CATEGORY, CATEGORYNAME) \
QDLT_REGISTER_LOGGING_CATEGORY(CATEGORY, CATEGORYNAME, DLT_CTX_NAME, DLT_CTX_DESCRIPTION) \

#define QDLT_REGISTER_LOGGING_CATEGORY(CATEGORY, CATEGORYNAME, DLT_CTX_NAME, DLT_CTX_DESCRIPTION) \
QT_BEGIN_NAMESPACE \
struct QDlt ## CATEGORY ## Registrator { \
    QDlt ## CATEGORY ## Registrator() { globalDltRegistration()->registerCategory(&CATEGORY() , DLT_CTX_NAME, DLT_CTX_DESCRIPTION); } \
}; \
static QDlt ## CATEGORY ## Registrator qdlt ## CATEGORY ## registrator; \
QT_END_NAMESPACE \

#define QDLT_FALLBACK_CATEGORY(CATEGORY) \
QT_BEGIN_NAMESPACE \
struct QDltDefaultRegistrator { \
    QDltDefaultRegistrator() { globalDltRegistration()->setDefaultContext(CATEGORY().categoryName()); } \
}; \
static QDltDefaultRegistrator qdltDefaultRegistrator; \
QT_END_NAMESPACE \

QT_END_NAMESPACE

#endif // QDLT_H

