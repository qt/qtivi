/****************************************************************************
**
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

#include "logging.h"
#include "mediaplayerbackend.h"
#include "searchandbrowsebackend.h"

#include <QtConcurrent/QtConcurrent>

#include <QFuture>
#include <QSqlError>
#include <QSqlQuery>
#include <QThreadPool>
#include <QtDebug>

MediaPlayerBackend::MediaPlayerBackend(const QSqlDatabase &database, QObject *parent)
    : QIviMediaPlayerBackendInterface(parent)
    , m_count(0)
    , m_currentIndex(-1)
    , m_playMode(QIviMediaPlayer::Normal)
    , m_requestedState(QIviMediaPlayer::Stopped)
    , m_state(QIviMediaPlayer::Stopped)
    , m_threadPool(new QThreadPool(this))
    , m_player(new QMediaPlayer(this))
{
    m_threadPool->setMaxThreadCount(1);
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &MediaPlayerBackend::onDurationChanged);
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &MediaPlayerBackend::onPositionChanged);
    connect(m_player, &QMediaPlayer::stateChanged,
            this, &MediaPlayerBackend::onStateChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &MediaPlayerBackend::onMediaStatusChanged);
    connect(m_player, &QMediaPlayer::volumeChanged,
            this, &MediaPlayerBackend::volumeChanged);
    connect(m_player, &QMediaPlayer::mutedChanged,
            this, &MediaPlayerBackend::mutedChanged);
    connect(this, &MediaPlayerBackend::playTrack,
            this, &MediaPlayerBackend::onPlayTrack,
            Qt::QueuedConnection);

    m_db = database;
}

void MediaPlayerBackend::initialize()
{
    emit durationChanged(0);
    emit positionChanged(0);
    emit volumeChanged(m_player->volume());
    emit mutedChanged(m_player->isMuted());
    emit initializationDone();
}

void MediaPlayerBackend::play()
{
    qCDebug(media) << Q_FUNC_INFO;
    qCDebug(media) << m_player->media().canonicalUrl();
    m_requestedState = QIviMediaPlayer::Playing;
    m_player->play();
}

void MediaPlayerBackend::pause()
{
    qCDebug(media) << Q_FUNC_INFO;
    m_requestedState = QIviMediaPlayer::Paused;
    m_player->pause();
}

void MediaPlayerBackend::stop()
{
    qCDebug(media) << Q_FUNC_INFO;
    m_requestedState = QIviMediaPlayer::Stopped;
    m_player->stop();
}

void MediaPlayerBackend::seek(qint64 offset)
{
    qCDebug(media) << Q_FUNC_INFO << offset;
    m_player->setPosition(m_player->position() + offset);
}

void MediaPlayerBackend::next()
{
    qCDebug(media) << Q_FUNC_INFO;
    int nextIndex = m_currentIndex + 1;
    if (m_playMode == QIviMediaPlayer::Shuffle)
        nextIndex = qrand() % m_count;
    else if (m_playMode == QIviMediaPlayer::RepeatTrack)
        nextIndex = m_currentIndex;
    else if (m_playMode == QIviMediaPlayer::RepeatAll && nextIndex >= m_count)
        nextIndex = 0;

    setCurrentIndex(nextIndex);
}

void MediaPlayerBackend::previous()
{
    qCDebug(media) << Q_FUNC_INFO;
    int nextIndex = m_currentIndex - 1;
    if (m_playMode == QIviMediaPlayer::Shuffle)
        nextIndex = qrand() % m_count;
    else if (m_playMode == QIviMediaPlayer::RepeatTrack)
        nextIndex = m_currentIndex;
    else if (m_playMode == QIviMediaPlayer::RepeatAll && nextIndex < 0)
        nextIndex = m_count - 1;

    setCurrentIndex(nextIndex);
}

void MediaPlayerBackend::setPlayMode(QIviMediaPlayer::PlayMode playMode)
{
    qCDebug(media) << Q_FUNC_INFO << playMode;
    m_playMode = playMode;
    emit playModeChanged(m_playMode);
}

void MediaPlayerBackend::setPosition(qint64 position)
{
    qCDebug(media) << Q_FUNC_INFO << position;
    m_player->setPosition(position);
}

bool MediaPlayerBackend::canReportCount()
{
    return true;
}

void MediaPlayerBackend::fetchData(int start, int count)
{
    QString queryString = QStringLiteral("SELECT track.id, artistName, albumName, trackName, genre, number, file, coverArtUrl FROM track JOIN queue ON queue.track_index=track.id ORDER BY queue.qindex LIMIT %4, %5")
            .arg(start)
            .arg(count);

    QStringList queries;
    queries.append(queryString);
    QtConcurrent::run(m_threadPool, this,
                      &MediaPlayerBackend::doSqlOperation,
                      MediaPlayerBackend::Select,
                      queries,
                      start,
                      count);
}

void MediaPlayerBackend::insert(int index, const QIviPlayableItem *item)
{
    QString queryString;
    if (item->type() == QStringLiteral("audiotrack")) {
        int track_index = item->id().toInt();
        queryString = QStringLiteral("UPDATE queue SET qindex = qindex + 1 WHERE qindex >= %1;"
                                     "INSERT INTO queue(qindex, track_index) VALUES( %1, %2);"
                                     "SELECT track.id, artistName, albumName, trackName, genre, number, file, coverArtUrl FROM track JOIN queue ON queue.track_index=track.id WHERE qindex=%1")
                .arg(index)
                .arg(track_index);
    } else {
        QString whereClause;
        if (item->type() == QStringLiteral("artist")) {
            whereClause = QStringLiteral("artistName == \"%1\"").arg(item->name());
        } else if (item->type() == QStringLiteral("album")) {
            whereClause = QStringLiteral("albumName == \"%1\"").arg(item->name());
        } else {
            qCWarning(media) << "Can't insert item: Given type is not supported.";
            emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("Can't insert item: Given type is not supported."));
            return;
        }
        queryString = QStringLiteral("UPDATE queue SET qindex = qindex + (SELECT count(*) from track WHERE %2) WHERE qindex >= %1;"
                                     "INSERT INTO queue(qindex, track_index) SELECT (SELECT COUNT(*) FROM track t1 WHERE t1.id <= t2.id AND %2)"
                                     "+ %1 - 1, id from track t2 WHERE %2;"
                                     "SELECT track.id, artistName, albumName, trackName, genre, number, file, coverArtUrl FROM track JOIN queue ON queue.track_index=track.id ORDER BY queue.qindex LIMIT %1, (SELECT count(*) from track WHERE %2)")
                .arg(index)
                .arg(whereClause);
    }
    QStringList queries = queryString.split(';');

    QtConcurrent::run(m_threadPool, this,
                      &MediaPlayerBackend::doSqlOperation,
                      MediaPlayerBackend::Insert,
                      queries, index, 0);
}

void MediaPlayerBackend::remove(int index)
{
    QString queryString = QStringLiteral("DELETE FROM queue WHERE qindex=%1;"
                                         "UPDATE queue SET qindex = qindex - 1 WHERE qindex >= %1")
            .arg(index);
    QStringList queries = queryString.split(';');

    QtConcurrent::run(m_threadPool, this,
                      &MediaPlayerBackend::doSqlOperation,
                      MediaPlayerBackend::Remove,
                      queries, index, 1);
}

void MediaPlayerBackend::move(int cur_index, int new_index)
{
    int delta = new_index - cur_index;
    if (delta == 0)
        return;

    QString queryString = QStringLiteral("UPDATE queue SET qindex = ( SELECT MAX(qindex) + 1 FROM queue) WHERE qindex=%1;"
                                         "UPDATE queue SET qindex = qindex %5 1 WHERE qindex >= %3 AND qindex <= %4;"
                                         "UPDATE queue SET qindex = %2 WHERE qindex= ( SELECT MAX(qindex) FROM queue);"
                                         "SELECT track.id, artistName, albumName, trackName, genre, number, file, coverArtUrl FROM track JOIN queue ON queue.track_index=track.id WHERE qindex >= %3 AND qindex <= %4 ORDER BY qindex")
            .arg(cur_index)
            .arg(new_index)
            .arg(qMin(cur_index, new_index))
            .arg(qMax(cur_index, new_index))
            .arg(delta > 0 ? QStringLiteral("-") : QStringLiteral("+"));
    QStringList queries = queryString.split(';');

    QtConcurrent::run(m_threadPool, this,
                      &MediaPlayerBackend::doSqlOperation,
                      MediaPlayerBackend::Move,
                      queries, cur_index, new_index);
}

void MediaPlayerBackend::doSqlOperation(MediaPlayerBackend::OperationType type, const QStringList &queries, int start, int count)
{
    m_db.transaction();
    QSqlQuery query(m_db);
    QVariantList list;

    for (const QString& queryString : queries) {
        if (query.exec(queryString)) {
            while (query.next()) {
                QString id = query.value(0).toString();
                QString artist = query.value(1).toString();
                QString album = query.value(2).toString();

                //Creating the TrackItem in an factory with would make this more performant
                QIviAudioTrackItem item;
                item.setId(id);
                item.setTitle(query.value(3).toString());
                item.setArtist(artist);
                item.setAlbum(album);
                item.setUrl(QUrl::fromLocalFile(query.value(6).toString()));
                item.setCoverArtUrl(QUrl::fromLocalFile(query.value(7).toString()));
                list.append(QVariant::fromValue(item));
            }
        } else {
            sqlError(this, query.lastQuery(), query.lastError().text());
            m_db.rollback();
            break;
        }
    }

    query.clear();
    if (query.exec(QStringLiteral("SELECT COUNT(*) FROM queue"))) {
        query.next();
        m_count = query.value(0).toInt();
        emit countChanged(m_count);
    } else {
        sqlError(this, query.lastQuery(), query.lastError().text());
    }

    if (type == MediaPlayerBackend::Select) {
        emit dataFetched(list, start, list.count() >= count);
    } else if (type == MediaPlayerBackend::SetIndex) {
        if (list.isEmpty()) {
            emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("SIMULATION: Can't set index in an empty queue"));
            return;
        }

        auto item = list.at(0).value<QIviAudioTrackItem>();
        emit playTrack(item.url());
        emit currentIndexChanged(start);
        emit currentTrackChanged(list.at(0));
    } else if (type == MediaPlayerBackend::Insert && start <= m_currentIndex) {
        // A new Item has been inserted before currentIndex
        // The currentIndex needs to be incremented to remain valid
        // It is safe to increment it by one as the backend only supports
        // adding/removing/moving one item
        emit currentIndexChanged(++m_currentIndex);
        emit dataChanged(list, start, count);
    } else if (type == MediaPlayerBackend::Remove && start <= m_currentIndex) {
        // A new Item has been removed before currentIndex
        // The currentIndex needs to be decremented to remain valid
        // It is safe to increment it by one as the backend only supports
        // adding/removing/moving one item

        if (start == m_currentIndex) {
            // If the currentIndex gets removed try updating it to the
            // Item before the currentIndex. If that is not possible fallback
            // to the item after it.
            int new_index = m_currentIndex - 1;
            if (m_currentIndex == 0 && m_count > 2)
                new_index = m_currentIndex + 1;
            setCurrentIndex(new_index);
            emit dataChanged(list, start, count);
            m_db.commit();
            return;
        }

        emit currentIndexChanged(--m_currentIndex);
        emit dataChanged(list, start, count);
    } else if (type == MediaPlayerBackend::Move) {
        // As we need to know the old index and the new index of the moved item
        // we reuse start and count as cur_index and new_index instead of their
        // original meaning
        int cur_index = start;
        int new_index = count;
        int delta = cur_index - new_index;

        //We only need to update the currentIndex if the currentIndex itself has been
        //moved or a item has changed its position from before the currentIndex to after it or vice-versa.
        if ((cur_index < m_currentIndex && new_index >= m_currentIndex) || (cur_index == m_currentIndex && delta > 0))
            emit currentIndexChanged(--m_currentIndex);
        else if ((cur_index > m_currentIndex && new_index < m_currentIndex) || (cur_index == m_currentIndex && delta < 0))
            emit currentIndexChanged(++m_currentIndex);

        emit dataChanged(list, qMin(cur_index, new_index), qAbs(delta) + 1);
    } else {
        emit dataChanged(list, start, count);
    }

    m_db.commit();
}

void MediaPlayerBackend::setCurrentIndex(int index)
{
    qCDebug(media) << Q_FUNC_INFO << index;
    if (m_currentIndex == index)
        return;
    //If we the list is empty the current Index needs to updated to an invalid track
    if (m_count == 0 && index == -1) {
        m_currentIndex = index;
        m_player->setMedia(QUrl());
        emit currentTrackChanged(QVariant());
        emit currentIndexChanged(m_currentIndex);
        emit durationChanged(0);
        emit positionChanged(0);
        return;
    }

    if (index >= m_count || index < 0)
        return;

    m_currentIndex = index;
    QString queryString = QStringLiteral("SELECT track.id, artistName, albumName, trackName, genre, number, file, coverArtUrl FROM track JOIN queue ON queue.track_index=track.id WHERE queue.qindex=%1 ORDER BY queue.qindex")
            .arg(m_currentIndex);

    QStringList queries;
    queries.append(queryString);

    QtConcurrent::run(m_threadPool, this,
                      &MediaPlayerBackend::doSqlOperation,
                      MediaPlayerBackend::SetIndex,
                      queries, m_currentIndex, 0);
}

void MediaPlayerBackend::setVolume(int volume)
{
    qCDebug(media) << Q_FUNC_INFO << volume;
    if (volume != m_player->volume()) {
        m_player->setVolume(volume);
        emit volumeChanged(volume);
    }
}

void MediaPlayerBackend::setMuted(bool muted)
{
    qCDebug(media) << Q_FUNC_INFO << muted;
    if (muted != m_player->isMuted()) {
        m_player->setMuted(muted);
        emit mutedChanged(muted);
    }
}

void MediaPlayerBackend::onStateChanged(QMediaPlayer::State state)
{
    qCDebug(media) << Q_FUNC_INFO << state;
    if (state == QMediaPlayer::PlayingState)
        m_state = QIviMediaPlayer::Playing;
    else if (state == QMediaPlayer::PausedState)
        m_state = QIviMediaPlayer::Paused;

    emit playStateChanged(m_state);
}

void MediaPlayerBackend::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qCDebug(media) << Q_FUNC_INFO << status;
    if (status == QMediaPlayer::EndOfMedia)
        next();
    if (status == QMediaPlayer::LoadedMedia && m_requestedState == QIviMediaPlayer::Playing)
        m_player->play();
}

void MediaPlayerBackend::onPositionChanged(qint64 position)
{
    qCDebug(media) << Q_FUNC_INFO << position;
    emit positionChanged(position);
}

void MediaPlayerBackend::onDurationChanged(qint64 duration)
{
    qCDebug(media) << Q_FUNC_INFO << duration;
    emit durationChanged(duration);
}

void MediaPlayerBackend::onPlayTrack(const QUrl &url)
{
    bool playing = m_player->state() == QMediaPlayer::PlayingState || m_player->mediaStatus() == QMediaPlayer::EndOfMedia;
    m_player->setMedia(url);
    if (playing)
        m_player->play();
}
