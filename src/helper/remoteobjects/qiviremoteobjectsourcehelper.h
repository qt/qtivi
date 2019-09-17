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

#ifndef QIVIREMOTEOBJECTSOURCEHELPER_H
#define QIVIREMOTEOBJECTSOURCEHELPER_H

#include <QtCore/QObject>
#include <QtIviCore/QIviPendingReply>

#include <QtIviRemoteObjectsHelper/qiviremoteobjectpendingresult.h>

QT_BEGIN_NAMESPACE

template <class T> class QIviRemoteObjectSourceHelper
{
public:
    QIviRemoteObjectSourceHelper(T *adapter, const QLoggingCategory &category = qtivi_private::qLcQtIviRoHelper())
        : m_adapter(adapter)
        , m_replyCounter(0)
        , m_category(category)
    {
        qRegisterMetaType<QIviRemoteObjectPendingResult>();
        qRegisterMetaTypeStreamOperators<QIviRemoteObjectPendingResult>();
    }

    QVariant toRemoteObjectVariant(const QVariant &variant) const
    {
        // QtRemoteObjects always uses QVariant::data() and passes it directly to qt_metacall. This doesn't
        // work for methods/signals which take a QVariant.
        // The workaround is put the QVariant in a QVariant as a container.
        // QTBUG-75056
        return QVariant(QMetaType::QVariant, &variant);
    }

    QVariant fromPendingReply(const QIviPendingReplyBase &pendingReply)
    {
        if (pendingReply.isSuccessful()) {
            qCDebug(m_category) << "Returning result right away";
            return pendingReply.value();
        } else { //reply not yet ready or failed
            const quint64 id = ++m_replyCounter;
            if (pendingReply.isResultAvailable()) { // the call failed
                qCDebug(m_category) << "Returning failed reply";
                return QVariant::fromValue(QIviRemoteObjectPendingResult(id, true /* failed */));
            }
            QIviRemoteObjectPendingResult result = QIviRemoteObjectPendingResult(id, false /* failed */);
            qCDebug(m_category) << "Returning a pending result: id:" << id;
            QObject::connect(pendingReply.watcher(), &QIviPendingReplyWatcher::valueChanged, [this, pendingReply, id] (const QVariant &value) {
                qCDebug(m_category) << "Value for pending result available: id:" << id << "value:" << value;
                Q_EMIT m_adapter->pendingResultAvailable(id, pendingReply.isSuccessful(), value);
            });
            return QVariant::fromValue(result);
        }
    }

private:
    T *m_adapter;
    quint64 m_replyCounter;
    const QLoggingCategory &m_category;
};

QT_END_NAMESPACE

#endif //QIVIREMOTEOBJECTSOURCEHELPER_H
