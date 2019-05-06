/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
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

#include "qiviremoteobjectpendingresult.h"

QT_BEGIN_NAMESPACE

namespace qtivi_private {
    Q_LOGGING_CATEGORY(qLcQtIviRoHelper, "qt.ivi.remoteobjects.helper", QtInfoMsg)
}

QIviRemoteObjectPendingResult::QIviRemoteObjectPendingResult()
    : m_id()
    , m_failed()
{}

QIviRemoteObjectPendingResult::QIviRemoteObjectPendingResult(quint64 id, bool failed)
    : m_id(id)
    , m_failed(failed)
{}

quint64 QIviRemoteObjectPendingResult::id() const
{
    return m_id;
}

void QIviRemoteObjectPendingResult::setId(quint64 id)
{
    if (id != m_id)
        m_id = id;
}

bool QIviRemoteObjectPendingResult::failed() const
{
    return m_failed;
}

void QIviRemoteObjectPendingResult::setFailed(bool failed)
{
    if (failed != m_failed)
        m_failed = failed;
}

QT_END_NAMESPACE
