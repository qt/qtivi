/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
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

#include "mediaindexerbackend.h"

#include <QTimer>
#include <QSettings>

Q_LOGGING_CATEGORY(qLcROQIviMediaIndexer, "qtivi.media.qivimediaindexerbackend.remoteobjects", QtInfoMsg)

MediaIndexerBackend::MediaIndexerBackend(QObject *parent)
    : QIviMediaIndexerControlBackendInterface(parent)
    , m_node(nullptr)
    , m_helper(new QIviRemoteObjectReplicaHelper(qLcROQIviMediaIndexer(), this))
{
}

void MediaIndexerBackend::initialize()
{
    if (!connectToNode())
        return;

    if (m_replica->isInitialized()) {
        emit progressChanged(m_replica->progress());
        emit stateChanged(m_replica->state());
        emit initializationDone();
    }

    QTimer::singleShot(3000, this, [this](){
        if (!m_replica->isInitialized())
            qCCritical(qLcROQIviMediaIndexer) << "QtIviMedia.QIviMediaIndexer wasn't initialized within the timeout period. Please make sure the server is running.";
    });
}

void MediaIndexerBackend::pause()
{
    m_replica->pause();
}

void MediaIndexerBackend::resume()
{
    m_replica->resume();
}

bool MediaIndexerBackend::connectToNode()
{
    static QString configPath;
    if (configPath.isEmpty()) {
        if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
            configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
        } else {
            configPath = QStringLiteral("./server.conf");
            qCInfo(qLcROQIviMediaIndexer) << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
        }
    }

    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("qtivimedia"));
    QUrl registryUrl = QUrl(settings.value(QStringLiteral("Registry"), QStringLiteral("local:qtivimedia")).toString());
    if (m_url != registryUrl) {
        m_url = registryUrl;
        // QtRO doesn't allow to change the URL without destroying the Node
        delete m_node;
        m_node = new QRemoteObjectNode(this);
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcROQIviMediaIndexer) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        qCInfo(qLcROQIviMediaIndexer) << "Connecting to" << m_url;
        m_replica.reset(m_node->acquire<QIviMediaIndexerReplica>(QStringLiteral("QtIviMedia.QIviMediaIndexer")));
        setupConnections();
    }
    return true;
}

void MediaIndexerBackend::setupConnections()
{
    connect(m_node, &QRemoteObjectNode::error, m_helper, &QIviRemoteObjectReplicaHelper::onNodeError);
    connect(m_helper, &QIviRemoteObjectReplicaHelper::errorChanged, this, &QIviFeatureInterface::errorChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIviRemoteObjectReplicaHelper::onReplicaStateChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIviFeatureInterface::initializationDone);
    connect(m_replica.data(), &QIviMediaIndexerReplica::stateChanged, this, &MediaIndexerBackend::stateChanged);
    connect(m_replica.data(), &QIviMediaIndexerReplica::progressChanged, this, &MediaIndexerBackend::progressChanged);
}
