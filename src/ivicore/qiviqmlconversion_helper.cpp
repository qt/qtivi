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

#include <qiviqmlconversion_helper.h>

#include <QtQml>
#include <private/qv8engine_p.h>
#include <private/qv4engine_p.h>
#include <private/qv4scopedvalue_p.h>
#include <private/qv4errorobject_p.h>

QT_BEGIN_NAMESPACE

void qtivi_qmlOrCppWarning(const QObject *obj, const QString &errorString)
{
    //If the object is not part of a javascript engine, print a normal warning
    QJSEngine *jsEngine = qjsEngine(obj);
    if (Q_UNLIKELY(!jsEngine)) {
        qWarning("%s", qPrintable(errorString));
        return;
    }

    //Try to get more information about the current line of execution
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
    QV4::ExecutionEngine *v4 = jsEngine->handle();
#else
    QV4::ExecutionEngine *v4 = QV8Engine::getV4(jsEngine->handle());
#endif
    QV4::Scope scope(v4);
    QV4::Scoped<QV4::ErrorObject> error(scope);
    QV4::StackTrace trace = v4->stackTrace(1);
    if (!!error)
        trace = *error->d()->stackTrace;

    //If we don't have any information, let qmlWarning use its magic and find something
    if (trace.isEmpty()) {
        qmlWarning(obj) << errorString;
        return;
    }

    v4->throwError(errorString);
}

QT_END_NAMESPACE
