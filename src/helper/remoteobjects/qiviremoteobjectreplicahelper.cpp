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

#include "qiviremoteobjectreplicahelper.h"

QT_BEGIN_NAMESPACE

QIviRemoteObjectReplicaHelper::QIviRemoteObjectReplicaHelper(const QLoggingCategory &category, QObject *parent)
    : QObject(parent)
    , m_category(category)
{
    qRegisterMetaType<QIviRemoteObjectPendingResult>();
    qRegisterMetaTypeStreamOperators<QIviRemoteObjectPendingResult>();
}

QVariant QIviRemoteObjectReplicaHelper::fromRemoteObjectVariant(const QVariant &variant) const
{
    return variant.value<QVariant>();
}

void QIviRemoteObjectReplicaHelper::onPendingResultAvailable(quint64 id, bool isSuccess, const QVariant &value)
{
    qCDebug(m_category) << "pending result available for id:" << id;
    if (!m_pendingReplies.contains(id)) {
        qCDebug(m_category) << "Received a result for an unexpected id:" << id << ". Ignoring!";
        return;
    }

    QIviPendingReplyBase iviReply = m_pendingReplies.take(id);

    if (isSuccess)
        iviReply.setSuccess(value);
    else
        iviReply.setFailed();
}

void QIviRemoteObjectReplicaHelper::onReplicaStateChanged(QRemoteObjectReplica::State newState, QRemoteObjectReplica::State oldState)
{
    Q_UNUSED(oldState)

    if (newState == QRemoteObjectReplica::Suspect) {
        qCWarning(m_category) << "QRemoteObjectReplica error, connection to the source lost";
        emit errorChanged(QIviAbstractFeature::Unknown,
                          QStringLiteral("QRemoteObjectReplica error, connection to the source lost"));
    } else if (newState == QRemoteObjectReplica::SignatureMismatch) {
        qCWarning(m_category) << "QRemoteObjectReplica error, signature mismatch";
        emit errorChanged(QIviAbstractFeature::Unknown,
                          QStringLiteral("QRemoteObjectReplica error, signature mismatch"));
    } else if (newState == QRemoteObjectReplica::Valid) {
        emit errorChanged(QIviAbstractFeature::NoError);
    }
}

void QIviRemoteObjectReplicaHelper::onNodeError(QRemoteObjectNode::ErrorCode code)
{
    qCWarning(m_category) << "QRemoteObjectNode error, code: " << code;
    QMetaEnum metaEnum = QMetaEnum::fromType<QRemoteObjectNode::ErrorCode>();
    emit errorChanged(QIviAbstractFeature::Unknown, QStringLiteral("QRemoteObjectNode error, code: ") + QLatin1String(metaEnum.valueToKey(code)));
}

QT_END_NAMESPACE
