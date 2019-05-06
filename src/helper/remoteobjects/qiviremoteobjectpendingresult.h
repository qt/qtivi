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

#ifndef QIVIREMOTEOBJECTPENDINGRESULT_H
#define QIVIREMOTEOBJECTPENDINGRESULT_H

#include <QtCore/QDataStream>
#include <QtCore/QObject>
#include <QtRemoteObjects/qtremoteobjectglobal.h>

QT_BEGIN_NAMESPACE

namespace qtivi_private {
    Q_DECLARE_LOGGING_CATEGORY(qLcQtIviRoHelper)
}

class QIviRemoteObjectPendingResult
{
    Q_GADGET

    Q_PROPERTY(quint64 id READ id WRITE setId)
    Q_PROPERTY(bool failed READ failed WRITE setFailed)
public:
    QIviRemoteObjectPendingResult();
    explicit QIviRemoteObjectPendingResult(quint64 id, bool failed);
    quint64 id() const;
    void setId(quint64 id);
    bool failed() const;
    void setFailed(bool failed);
private:
    quint64 m_id;
    bool m_failed;
};

//POD, passing by value should be fine
inline bool operator==(QIviRemoteObjectPendingResult left, QIviRemoteObjectPendingResult right) Q_DECL_NOTHROW {
    return left.id() == right.id() && left.failed() == right.failed();
};;
inline bool operator!=(QIviRemoteObjectPendingResult left, QIviRemoteObjectPendingResult right) Q_DECL_NOTHROW {
    return !(left == right);
}

inline QDataStream &operator<<(QDataStream &ds, QIviRemoteObjectPendingResult obj) {
    QtRemoteObjects::copyStoredProperties(&obj, ds);
    return ds;
}

inline QDataStream &operator>>(QDataStream &ds, QIviRemoteObjectPendingResult &obj) {
    QtRemoteObjects::copyStoredProperties(ds, &obj);
    return ds;
}

QT_END_NAMESPACE

#endif //QIVIREMOTEOBJECTPENDINGRESULT_H
