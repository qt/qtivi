/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
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

#include "mediaindexerbackend.h"

#include <QTimer>

Q_LOGGING_CATEGORY(qLcROQIviMediaIndexer, "qtivi.media.qivimediaindexerbackend.remoteobjects", QtInfoMsg)

MediaIndexerBackend::MediaIndexerBackend(QRemoteObjectNode *node, QObject *parent)
    : QIviMediaIndexerControlBackendInterface(parent)
{
    m_replica.reset(node->acquire<QIviMediaIndexerReplica>(QStringLiteral("QtIviMedia.QIviMediaIndexer")));

    connect(node, &QRemoteObjectNode::error, this, &MediaIndexerBackend::onNodeError);
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIviFeatureInterface::initializationDone);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, this, &MediaIndexerBackend::onReplicaStateChanged);
    connect(m_replica.data(), &QIviMediaIndexerReplica::stateChanged, this, &MediaIndexerBackend::stateChanged);
    connect(m_replica.data(), &QIviMediaIndexerReplica::progressChanged, this, &MediaIndexerBackend::progressChanged);

    QTimer::singleShot(3000, this, [this](){
        if (!m_replica->isInitialized())
            qCCritical(qLcROQIviMediaIndexer) << "QtIviMedia.QIviMediaIndexer wasn't initialized within the timeout period. Please make sure the server is running.";
    });
}

void MediaIndexerBackend::initialize()
{
    if (m_replica->isInitialized()) {
        emit progressChanged(m_replica->progress());
        emit stateChanged(m_replica->state());
        emit initializationDone();
    }
}

void MediaIndexerBackend::pause()
{
    m_replica->pause();
}

void MediaIndexerBackend::resume()
{
    m_replica->resume();
}

void MediaIndexerBackend::onReplicaStateChanged(QRemoteObjectReplica::State newState, QRemoteObjectReplica::State oldState)
{
    Q_UNUSED(oldState)

    if (newState == QRemoteObjectReplica::Suspect) {
        qCWarning(qLcROQIviMediaIndexer) << "QRemoteObjectReplica error, connection to the source lost";
        emit errorChanged(QIviAbstractFeature::Unknown,
                        "QRemoteObjectReplica error, connection to the source lost");
    } else if (newState == QRemoteObjectReplica::SignatureMismatch) {
        qCWarning(qLcROQIviMediaIndexer) << "QRemoteObjectReplica error, signature mismatch";
        emit errorChanged(QIviAbstractFeature::Unknown,
                        "QRemoteObjectReplica error, signature mismatch");
    } else if (newState==QRemoteObjectReplica::Valid) {
        emit errorChanged(QIviAbstractFeature::NoError, "");
    }
}

void MediaIndexerBackend::onNodeError(QRemoteObjectNode::ErrorCode code)
{
    qCWarning(qLcROQIviMediaIndexer) << "QRemoteObjectNode error, code: " << code;
    emit errorChanged(QIviAbstractFeature::Unknown, "QRemoteObjectNode error, code: " + code);
}
