/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
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
** SPDX-License-Identifier: LGPL-2.1
**
****************************************************************************/

#include <QString>
#include <QHash>

#include <dlt/dlt.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

QT_BEGIN_NAMESPACE

void qtGeniviLogLevelChangedHandler(char context_id[], uint8_t log_level, uint8_t trace_status);

class QDltRegistration;

class QDltRegistrationPrivate
{
public:
    QDltRegistrationPrivate(QDltRegistration* parent);

    void registerCategory(const QLoggingCategory* category, DltContext *dltContext, const char *dltCtxName, const char *dltCtxDescription);
    void setDefaultContext(DltContext* dltContext);

    DltContext* context(const char* categoryName);
    void dltLogLevelChanged(char context_id[], uint8_t log_level, uint8_t trace_status);

private:
    QDltRegistration* const q_ptr;
    Q_DECLARE_PUBLIC(QDltRegistration)
    QString m_dltAppID;
    DltContext* m_defaultContext;
    QHash<QString, DltContext*> m_categoryName2DltContext;
    QHash<QString, QLoggingCategory*> m_ctxName2Category;
};

QT_END_NAMESPACE
