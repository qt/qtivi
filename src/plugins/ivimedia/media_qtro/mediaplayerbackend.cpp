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

#include "mediaplayerbackend.h"
#include "qiviqmlconversion_helper.h"

#include <QTimer>

Q_LOGGING_CATEGORY(qLcROQIviMediaPlayer, "qtivi.media.qivimediaplayerbackend.remoteobjects", QtInfoMsg)

MediaPlayerBackend::MediaPlayerBackend(QRemoteObjectNode *node, QObject *parent)
    : QIviMediaPlayerBackendInterface(parent)
    , m_helper(new QIviRemoteObjectReplicaHelper(qLcROQIviMediaPlayer(), this))
{
    qRegisterMetaType<QIviPlayableItem>();
    qRegisterMetaType<QIviAudioTrackItem>();
    qRegisterMetaTypeStreamOperators<QIviAudioTrackItem>();

    m_replica.reset(node->acquire<QIviMediaPlayerReplica>(QStringLiteral("QtIviMedia.QIviMediaPlayer")));

    connect(node, &QRemoteObjectNode::error, m_helper, &QIviRemoteObjectReplicaHelper::onNodeError);
    connect(m_helper, &QIviRemoteObjectReplicaHelper::errorChanged, this, &QIviFeatureInterface::errorChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIviRemoteObjectReplicaHelper::onReplicaStateChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIviFeatureInterface::initializationDone);
    connect(m_replica.data(), &QIviMediaPlayerReplica::playModeChanged, this, &MediaPlayerBackend::playModeChanged);
    connect(m_replica.data(), &QIviMediaPlayerReplica::playStateChanged, this, &MediaPlayerBackend::playStateChanged);
    connect(m_replica.data(), &QIviMediaPlayerReplica::positionChanged, this, &MediaPlayerBackend::positionChanged);
    connect(m_replica.data(), &QIviMediaPlayerReplica::durationChanged, this, &MediaPlayerBackend::durationChanged);
    connect(m_replica.data(), &QIviMediaPlayerReplica::currentTrackChanged, this, [this] (const QVariant &currentTrack) {
        emit currentTrackChanged(m_helper->fromRemoteObjectVariant(currentTrack));
    });
    connect(m_replica.data(), &QIviMediaPlayerReplica::currentIndexChanged, this, &MediaPlayerBackend::currentIndexChanged);
    connect(m_replica.data(), &QIviMediaPlayerReplica::volumeChanged, this, &MediaPlayerBackend::volumeChanged);
    connect(m_replica.data(), &QIviMediaPlayerReplica::mutedChanged, this, &MediaPlayerBackend::mutedChanged);
    connect(m_replica.data(), &QIviMediaPlayerReplica::countChanged, this, &MediaPlayerBackend::countChanged);
    connect(m_replica.data(), &QIviMediaPlayerReplica::canReportCountChanged, this, &MediaPlayerBackend::canReportCountChanged);
    connect(m_replica.data(), &QIviMediaPlayerReplica::dataFetched, this, &MediaPlayerBackend::dataFetched);
    connect(m_replica.data(), &QIviMediaPlayerReplica::dataChanged, this, &MediaPlayerBackend::dataChanged);

    QTimer::singleShot(3000, this, [this](){
        if (!m_replica->isInitialized())
            qCCritical(qLcROQIviMediaPlayer) << "QtIviMedia.QIviMediaPlayer wasn't initialized within the timeout period. Please make sure the server is running.";
    });
}

void MediaPlayerBackend::initialize()
{
    if (m_replica->isInitialized()) {
        emit canReportCountChanged(m_replica->canReportCount());
        emit playModeChanged(m_replica->playMode());
        emit playStateChanged(m_replica->playState());
        emit positionChanged(m_replica->position());
        emit durationChanged(m_replica->duration());
        emit currentTrackChanged(m_replica->currentTrack());
        emit currentIndexChanged(m_replica->currentIndex());
        emit volumeChanged(m_replica->volume());
        emit mutedChanged(m_replica->muted());
        emit initializationDone();
    }
}

void MediaPlayerBackend::play()
{
    m_replica->play();
}

void MediaPlayerBackend::pause()
{
    m_replica->pause();
}

void MediaPlayerBackend::stop()
{
    m_replica->stop();
}

void MediaPlayerBackend::seek(qint64 offset)
{
    m_replica->seek(offset);
}

void MediaPlayerBackend::next()
{
    m_replica->next();
}

void MediaPlayerBackend::previous()
{
    m_replica->previous();
}

void MediaPlayerBackend::setPlayMode(QIviMediaPlayer::PlayMode playMode)
{
    m_replica->pushPlayMode(playMode);
}

void MediaPlayerBackend::setPosition(qint64 position)
{
    m_replica->pushPosition(position);
}

void MediaPlayerBackend::setCurrentIndex(int currentIndex)
{
    m_replica->pushCurrentIndex(currentIndex);
}

void MediaPlayerBackend::setVolume(int volume)
{
    m_replica->pushVolume(volume);
}

void MediaPlayerBackend::setMuted(bool muted)
{
    m_replica->pushMuted(muted);
}

void MediaPlayerBackend::fetchData(const QUuid &identifier, int start, int count)
{
    m_replica->fetchData(identifier, start, count);
}

void MediaPlayerBackend::insert(int index, const QVariant &item)
{
    m_replica->insert(index, item);
}

void MediaPlayerBackend::remove(int index)
{
    m_replica->remove(index);
}

void MediaPlayerBackend::move(int currentIndex, int newIndex)
{
    m_replica->move(currentIndex, newIndex);
}
