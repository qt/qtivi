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

#include "qivimediaplayerqtroadapter.h"
#include "qiviqmlconversion_helper.h"

QIviMediaPlayerQtRoAdapter::QIviMediaPlayerQtRoAdapter(MediaPlayerBackend *parent)
    : QIviMediaPlayerQtRoAdapter(QStringLiteral("QtIviMedia.QIviMediaPlayer"), parent)
{
}

QIviMediaPlayerQtRoAdapter::QIviMediaPlayerQtRoAdapter(const QString& remoteObjectsLookupName, MediaPlayerBackend *parent)
    : QIviMediaPlayerSource(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_backend(parent)
    , m_helper(this)
{
    connect(m_backend, &MediaPlayerBackend::playModeChanged, this, &QIviMediaPlayerQtRoAdapter::playModeChanged);
    connect(m_backend, &MediaPlayerBackend::playStateChanged, this, &QIviMediaPlayerQtRoAdapter::playStateChanged);
    connect(m_backend, &MediaPlayerBackend::positionChanged, this, &QIviMediaPlayerQtRoAdapter::positionChanged);
    connect(m_backend, &MediaPlayerBackend::durationChanged, this, &QIviMediaPlayerQtRoAdapter::durationChanged);
    connect(m_backend, &MediaPlayerBackend::currentIndexChanged, this, &QIviMediaPlayerQtRoAdapter::currentIndexChanged);
    connect(m_backend, &MediaPlayerBackend::currentTrackChanged, this, [this](const QVariant &currentTrack) {
        emit currentTrackChanged(QVariant(QMetaType(QMetaType::QVariant), &currentTrack));
    });
    connect(m_backend, &MediaPlayerBackend::volumeChanged, this, &QIviMediaPlayerQtRoAdapter::volumeChanged);
    connect(m_backend, &MediaPlayerBackend::mutedChanged, this, &QIviMediaPlayerQtRoAdapter::mutedChanged);
    connect(m_backend, &MediaPlayerBackend::dataFetched, this, &QIviMediaPlayerQtRoAdapter::dataFetched);
    connect(m_backend, &MediaPlayerBackend::dataChanged, this, &QIviMediaPlayerQtRoAdapter::dataChanged);
    connect(m_backend, &MediaPlayerBackend::countChanged, this, &QIviMediaPlayerQtRoAdapter::countChanged);
}


QString QIviMediaPlayerQtRoAdapter::remoteObjectsLookupName() const
{
    return m_remoteObjectsLookupName;
}

QIviMediaPlayer::PlayMode QIviMediaPlayerQtRoAdapter::playMode() const
{
    return m_backend->playMode();
}

QIviMediaPlayer::PlayState QIviMediaPlayerQtRoAdapter::playState() const
{
    return m_backend->playState();
}

qint64 QIviMediaPlayerQtRoAdapter::position() const
{
    return m_backend->position();
}

qint64 QIviMediaPlayerQtRoAdapter::duration() const
{
    return m_backend->duration();
}

QVariant QIviMediaPlayerQtRoAdapter::currentTrack() const
{
    return m_helper.toRemoteObjectVariant(m_backend->currentTrack());
}

int QIviMediaPlayerQtRoAdapter::currentIndex() const
{
    return m_backend->currentIndex();
}

int QIviMediaPlayerQtRoAdapter::volume() const
{
    return m_backend->volume();
}

bool QIviMediaPlayerQtRoAdapter::muted() const
{
    return m_backend->isMuted();
}

bool QIviMediaPlayerQtRoAdapter::canReportCount() const
{
    return m_backend->canReportCount();
}

void QIviMediaPlayerQtRoAdapter::setPlayMode(QIviMediaPlayer::PlayMode playMode)
{
    m_backend->setPlayMode(playMode);
}

void QIviMediaPlayerQtRoAdapter::setPosition(qint64 position)
{
    m_backend->setPosition(position);
}

void QIviMediaPlayerQtRoAdapter::setCurrentIndex(int currentIndex)
{
    m_backend->setCurrentIndex(currentIndex);
}

void QIviMediaPlayerQtRoAdapter::setVolume(int volume)
{
    m_backend->setVolume(volume);
}

void QIviMediaPlayerQtRoAdapter::setMuted(bool muted)
{
    m_backend->setMuted(muted);
}

void QIviMediaPlayerQtRoAdapter::play()
{
    m_backend->play();
}

void QIviMediaPlayerQtRoAdapter::pause()
{
    m_backend->pause();
}

void QIviMediaPlayerQtRoAdapter::stop()
{
    m_backend->stop();
}

void QIviMediaPlayerQtRoAdapter::seek(qint64 offset)
{
    m_backend->seek(offset);
}

void QIviMediaPlayerQtRoAdapter::next()
{
    m_backend->next();
}

void QIviMediaPlayerQtRoAdapter::previous()
{
    m_backend->previous();
}

void QIviMediaPlayerQtRoAdapter::fetchData(const QUuid &identifier, int start, int count)
{
    m_backend->fetchData(identifier, start, count);
}

void QIviMediaPlayerQtRoAdapter::insert(int index, const QVariant &item)
{
    m_backend->insert(index, item);
}

void QIviMediaPlayerQtRoAdapter::remove(int index)
{
    m_backend->remove(index);
}

void QIviMediaPlayerQtRoAdapter::move(int currentIndex, int newIndex)
{
    m_backend->move(currentIndex, newIndex);
}
