/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
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

#include "mediaplayerbackend.h"

#include <QtDebug>

#include "mediaplayer2_interface.h"

MediaPlayerBackend::MediaPlayerBackend(const QString &dbusServiceName, const QDBusConnection &dbusConnection, QObject *parent)
    : QIviMediaPlayerBackendInterface(parent)
{
    qDBusRegisterMetaType<QList<QVariantMap> >();

    m_dbusPlayer = new OrgMprisMediaPlayer2PlayerInterface(dbusServiceName,
                                                           QStringLiteral("/org/mpris/MediaPlayer2"),
                                                           dbusConnection,
                                                           this);

    m_dbusPlayerProperties = new OrgFreedesktopDBusPropertiesInterface(dbusServiceName,
                                                                     QStringLiteral("/org/mpris/MediaPlayer2"),
                                                                     dbusConnection,
                                                                     this);

    m_dbusTrackList = new OrgMprisMediaPlayer2TrackListInterface(dbusServiceName,
                                                                 QStringLiteral("/org/mpris/MediaPlayer2"),
                                                                 dbusConnection,
                                                                 this);
    if (!m_dbusPlayer->isValid())
        qWarning() << "Failed to load Player interface on" << dbusServiceName;

    if (!m_dbusPlayerProperties->isValid())
        qWarning() << "Failed to load Player properties interface on" << dbusServiceName;

    if (!m_dbusTrackList->isValid())
        qWarning() << "Failed to load Tracklist properties interface on" << dbusServiceName;

    /* Player interface */
    connect(m_dbusPlayerProperties, &OrgFreedesktopDBusPropertiesInterface::PropertiesChanged,
            this, &MediaPlayerBackend::onPropertiesChanged);

    /* Properties interface */
    connect(m_dbusPlayer, &OrgMprisMediaPlayer2PlayerInterface::Seeked,
            this, &MediaPlayerBackend::onSeeked);

    /* Check for current position using timer */
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        onSeeked(m_dbusPlayer->position());
    });
    timer->start(1000);

}

QIviAudioTrackItem MediaPlayerBackend::audioTrackFromMPRIS2Object(const QVariantMap &variantMap)
{
    QIviAudioTrackItem audioItem;

    for (QVariantMap::const_iterator i = variantMap.cbegin(); i != variantMap.cend(); ++i) {
        if (i.key() == QLatin1String("mpris:trackid")) {
            const QString id = variantMap[QStringLiteral("mpris:trackid")].value<QDBusObjectPath>().path();
            audioItem.setId(id);
        } else if (i.key() == QLatin1String("xesam:url")) {
            const QString url = variantMap[QStringLiteral("xesam:url")].toString();
            audioItem.setUrl(url);
        } else if (i.key() == QLatin1String("mpris:artUrl")) {
            const QString url = variantMap[QStringLiteral("mpris:artUrl")].toString();
            audioItem.setCoverArtUrl(url);
        } else if (i.key() == QLatin1String("mpris:length")) {
            const qlonglong length = variantMap[QStringLiteral("mpris:length")].toLongLong();
            audioItem.setDuration(length);
        } else if (i.key() == QLatin1String("xesam:artist")) {
            const QStringList artists = variantMap[QStringLiteral("xesam:artist")].toStringList();
            if (artists.length() > 0)
                audioItem.setArtist(artists[0]);
        } else if (i.key() == QLatin1String("xesam:title")) {
            const QString title = variantMap[QStringLiteral("xesam:title")].toString();
            audioItem.setTitle(title);
        } else if (i.key() == QLatin1String("xesam:album")) {
            const QString title = variantMap[QStringLiteral("xesam:album")].toString();
            audioItem.setAlbum(title);
        }
    }

    return audioItem;
}

void MediaPlayerBackend::updateTrackIndex(const QVariantMap &newTrack)
{
    if (newTrack.contains(QStringLiteral("mpris:trackid"))) {
        auto newTrackId = newTrack[QStringLiteral("mpris:trackid")].value<QDBusObjectPath>();
        updateTrackIndex(newTrackId);
    }
}
void MediaPlayerBackend::updateTrackIndex(const QDBusObjectPath &newTrackId)
{
    int newIndex = m_tracks.indexOf(newTrackId);
    emit currentIndexChanged(newIndex);
    m_currentTrack = newTrackId;
}

void MediaPlayerBackend::handlePlaybackStatus(const QString &playbackStatus){
    if (playbackStatus == QLatin1String("Playing"))
        emit playStateChanged(QIviMediaPlayer::PlayState::Playing);
    else if (playbackStatus == QLatin1String("Paused"))
        emit playStateChanged(QIviMediaPlayer::PlayState::Paused);
    else if (playbackStatus == QLatin1String("Stopped"))
        emit playStateChanged(QIviMediaPlayer::PlayState::Stopped);
    else
        qWarning() << "Unhandled PlaybackStatus: " << playbackStatus;
}

void MediaPlayerBackend::handlePosition(qlonglong position)
{
    /* QtIVI needs position to be in seconds, while MPRIS2 uses milliseconds */
    emit positionChanged(position/1000);
}

void MediaPlayerBackend::onPropertiesChanged(const QString &interface, const QVariantMap &changed_properties, const QStringList &invalidated_properties)
{
    if (interface == QLatin1String("org.mpris.MediaPlayer2.Player")) {
        for (QVariantMap::const_iterator i = changed_properties.constBegin(); i != changed_properties.constEnd(); ++i) {
            if (i.key() == QLatin1String("PlaybackStatus")) {
                handlePlaybackStatus(i.value().toString());
            } else if (i.key() == QLatin1String("Metadata")) { /* Triggered when the current track changes */
                QVariantMap variantMap;
                i.value().value<QDBusArgument>() >> variantMap;

                /* Update current duration */
                if (variantMap.contains(QStringLiteral("mpris:length")))
                    emit durationChanged(variantMap[QStringLiteral("mpris:length")].toLongLong() / 1000);

                /* Adjust current playing index according to track ID */
                updateTrackIndex(variantMap);
            } else if (i.key() == QLatin1String("Volume")) {
                emit volumeChanged(qRound(i.value().toDouble() * 100));
            } else if (i.key() == QLatin1String("Rate")) {
                ; /* Not handled */
            } else if (i.key() == QLatin1String("CanPlay")) {
                ; /* Not handled */
            } else if (i.key() == QLatin1String("CanPause")) {
                ; /* Not handled */
            } else if (i.key() == QLatin1String("CanSeek")) {
                ; /* Not handled */
            } else {
                qWarning() << "Unhandled property: " << i.key() << ": " << i.value() << endl;
            }
        }
    } else if (interface == QLatin1String("org.mpris.MediaPlayer2.TrackList")) {
        for (QStringList::const_iterator i = invalidated_properties.constBegin(); i != invalidated_properties.constEnd(); ++i) {
            if (*i == QLatin1String("Tracks"))
                fetchOrUpdate(0     /* Update from first element */,
                              -1    /* Update until end of list (all elements)*/,
                              false /* Do an update, not a fetch */);
        }
    } else
        qWarning("Property changed on unknown interface");
}

void MediaPlayerBackend::onSeeked(qlonglong position)
{
    handlePosition(position);
}

void MediaPlayerBackend::initialize()
{
    /* Run in separate thread so we can start fast even if track list is very long */
    QtConcurrent::run(&m_threadPool, [=]() {
        /* Fetch current track list from service, and emit its length so updates can be requested */
        m_tracks = m_dbusTrackList->tracks();
        QVariantMap metadata = m_dbusPlayer->metadata();
        emit countChanged(m_tracks.length());
        updateTrackIndex(metadata);

        /* Simulate property change to get the latest metadata on start-up */
        onPropertiesChanged(QStringLiteral("org.mpris.MediaPlayer2.Player"), metadata, QStringList());

        /* Position is not in metadata, so handle separately */
        handlePosition(m_dbusPlayer->position());

        /* Playback status is not in metadata, so handle separately */
        handlePlaybackStatus(m_dbusPlayer->playbackStatus());

        emit initializationDone();
    });
}

void MediaPlayerBackend::play()
{
    m_dbusPlayer->Play();
}

void MediaPlayerBackend::pause()
{
    m_dbusPlayer->Pause();
}

void MediaPlayerBackend::stop()
{
    m_dbusPlayer->Stop();
}

void MediaPlayerBackend::seek(qint64 offset)
{
    m_dbusPlayer->Seek(offset);
}

void MediaPlayerBackend::next()
{
    m_dbusPlayer->Next();
}

void MediaPlayerBackend::previous()
{
    m_dbusPlayer->Previous();
}

void MediaPlayerBackend::setPlayMode(QIviMediaPlayer::PlayMode playMode)
{
    bool shuffle = false;
    QString loopStatus = QStringLiteral("None");

    switch (playMode) {
        case QIviMediaPlayer::Normal:
            shuffle = false;
            loopStatus = QStringLiteral("None");
            break;
        case QIviMediaPlayer::RepeatTrack:
            shuffle = false;
            loopStatus = QStringLiteral("Track");
            break;
        case QIviMediaPlayer::RepeatAll:
            shuffle = false;
            loopStatus = QStringLiteral("Playlist");
            break;
        case QIviMediaPlayer::Shuffle:
            shuffle = true;
            loopStatus = QStringLiteral("None");
            break;
        default:
            qWarning() << "Illegal play mode: " << playMode;
            return;
    }

    m_dbusPlayer->setShuffle(shuffle);
    m_dbusPlayer->setLoopStatus(loopStatus);
}

void MediaPlayerBackend::setPosition(qint64 position)
{
    m_dbusPlayer->SetPosition(m_currentTrack, position * 1000);
}

bool MediaPlayerBackend::canReportCount()
{
    return true;
}

/* This doesn't need to be a Future right now, since it is only called from
 * fetchOrUpdate, but I keep this as a Future so that it can be used
 * asynchronously if needed.. in the future. */
QFuture<QVariantList> MediaPlayerBackend::itemsForMPRIS2Object(const QList<QDBusObjectPath> &objs)
{
    /* Get track metadata in own thread, since this may take a while */
    return QtConcurrent::run(&m_threadPool, [=]() -> QVariantList {
        auto reply = m_dbusTrackList->GetTracksMetadata(objs);

        reply.waitForFinished();
        QVariantList items;

        const QList<QMap<QString, QVariant> > objects = reply.argumentAt<0>();
        for (const QMap<QString, QVariant> &dbusItem : objects) {
            QIviAudioTrackItem item;
            item = audioTrackFromMPRIS2Object(dbusItem);
            items.append(QVariant::fromValue(item));
        }
        return items;
    });
}

void MediaPlayerBackend::fetchData(int start, int count)
{
    fetchOrUpdate(start, count, true /* Do a fetch, not an update */);
}

void MediaPlayerBackend::fetchOrUpdate(int start, int count, bool fetch)
{
    /* Do fetch or update operation in own thread, since it may take a while */
    QtConcurrent::run(&m_threadPool, [=]() {
        QList<QDBusObjectPath> newTracks = m_dbusTrackList->tracks();
        QVariantList list;
        if (start < newTracks.length())
            list = itemsForMPRIS2Object(newTracks.mid(start, count)).result();

        emit countChanged(newTracks.length());

        if (fetch)
            emit dataFetched(list, start, list.count() >= count);
        else {
            /* TODO: Don't invalidate the entire list, just reflect the modifications made */
            emit dataChanged(QVariantList(), 0, m_tracks.count());
            emit dataChanged(list, 0, 0);
        }

        m_tracks = newTracks;
    });
}

void MediaPlayerBackend::insert(int index, const QIviPlayableItem *item)
{
    Q_UNUSED(index)
    Q_UNUSED(item)
    qWarning() << "Insertion is not supported";
}

void MediaPlayerBackend::remove(int index)
{
    Q_UNUSED(index)
    qWarning() << "Removal is not supported";
}

void MediaPlayerBackend::move(int cur_index, int new_index)
{
    Q_UNUSED(cur_index)
    Q_UNUSED(new_index)
    qWarning() << "Moving is not supported";
}

void MediaPlayerBackend::setCurrentIndex(int index)
{
    if (index >= 0 && index < m_tracks.length()) {
        m_dbusTrackList->GoTo(m_tracks[index]);
        m_currentTrack = m_tracks[index];
        emit currentIndexChanged(index);
    } else
        qWarning() << "Supplied index is out of range";
}

void MediaPlayerBackend::setVolume(int volume)
{
    m_dbusPlayer->setVolume(volume/100);
}

void MediaPlayerBackend::setMuted(bool)
{
    qWarning() << "Mute is not supported";
}
