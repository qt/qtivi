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

#ifndef QIVIREMOTEOBJECTREPLICAHELPER_H
#define QIVIREMOTEOBJECTREPLICAHELPER_H

#include <QtIviCore/QIviPendingReply>
#include <QtIviCore/QIviAbstractFeature>
#include <QtRemoteObjects/QRemoteObjectNode>
#include <QtRemoteObjects/QRemoteObjectReplica>
#include <QtRemoteObjects/QRemoteObjectPendingCall>

#include <QtIviRemoteObjectsHelper/qiviremoteobjectpendingresult.h>

QT_BEGIN_NAMESPACE

class QIviRemoteObjectReplicaHelper : public QObject
{
    Q_OBJECT

public:
    QIviRemoteObjectReplicaHelper(const QLoggingCategory &category = qtivi_private::qLcQtIviRoHelper(), QObject *parent = nullptr);

    QVariant fromRemoteObjectVariant(const QVariant &variant) const;

    template <class T> QIviPendingReply<T> toQIviPendingReply(const QRemoteObjectPendingCall &reply)
    {
        qCDebug(m_category) << "Analyzing QRemoteObjectPendingCall";
        QIviPendingReply<T> iviReply;
        auto watcher = new QRemoteObjectPendingCallWatcher(reply);
        connect(watcher, &QRemoteObjectPendingCallWatcher::finished, watcher, [this, iviReply] (QRemoteObjectPendingCallWatcher *self) mutable {
            if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 1)
                QVariant value = self->returnValue();
#else
                QVariant value = self->returnValue().value<QVariant>();
#endif
                if (value.canConvert<QIviRemoteObjectPendingResult>()) {
                    auto result = value.value<QIviRemoteObjectPendingResult>();
                    if (result.failed()) {
                        qCDebug(m_category) << "Pending Result with id:" << result.id() << "failed";
                        iviReply.setFailed();
                    } else {
                        qCDebug(m_category) << "Result not available yet. Waiting for id:" << result.id();
                        m_pendingReplies.insert(result.id(), iviReply);
                    }
                } else {
                    qCDebug(m_category) << "Got the value right away:" << value;
                    iviReply.setSuccess(value);
                }
            } else {
                iviReply.setFailed();
            }
            self->deleteLater();
        });
        return iviReply;
    }

public Q_SLOTS:
    void onPendingResultAvailable(quint64 id, bool isSuccess, const QVariant &value);
    void onReplicaStateChanged(QRemoteObjectReplica::State newState, QRemoteObjectReplica::State oldState);
    void onNodeError(QRemoteObjectNode::ErrorCode code);

Q_SIGNALS:
    void errorChanged(QIviAbstractFeature::Error error, const QString &message = QString());

private:
    QHash<quint64, QIviPendingReplyBase> m_pendingReplies;
    const QLoggingCategory &m_category;
};

QT_END_NAMESPACE

#endif //QIVIREMOTEOBJECTREPLICAHELPER_H
