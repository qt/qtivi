/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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
#include "searchandbrowsebackend.h"

#include <QtMultimedia/QMediaPlayer>
#include <QSqlQuery>
#include <QSqlError>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QtDebug>

MediaPlayerBackend::MediaPlayerBackend(QObject *parent)
    : QIviMediaPlayerBackendInterface(parent)
    , m_player(new QMediaPlayer(this))
{
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &MediaPlayerBackend::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &MediaPlayerBackend::positionChanged);

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    const QByteArray database = qgetenv("QTIVIMEDIA_SIMULATOR_DATABASE");
    if (database.isEmpty()) {
        qCritical() << "QTIVIMEDIA_SIMULATOR_DATABASE environment variable needs to be set to a valid database file location.";
    }
    m_db.setDatabaseName(database);
    m_db.open();

    QSqlQuery query = m_db.exec(QLatin1String("CREATE TABLE IF NOT EXISTS \"queue\" (\"id\" INTEGER PRIMARY KEY, \"qindex\" INTEGER, \"track_index\" INTEGER)"));
    if (query.lastError().isValid())
        qWarning() << query.lastError().text();
}

void MediaPlayerBackend::initialize()
{
}

void MediaPlayerBackend::play()
{
    m_player->play();
}

void MediaPlayerBackend::pause()
{
    m_player->pause();
}

void MediaPlayerBackend::stop()
{
    m_player->stop();
}

void MediaPlayerBackend::seek(int offset)
{
    m_player->setPosition(m_player->position() + offset);
}

void MediaPlayerBackend::next()
{
}

void MediaPlayerBackend::previous()
{
}

bool MediaPlayerBackend::canReportListCount()
{
    return false;
}

void MediaPlayerBackend::fetchData(int start, int count)
{
    QString queryString = QString(QLatin1String("SELECT track.id, artistName, albumName, trackName, genre, number, file FROM track JOIN queue ON queue.track_index=track.id ORDER BY queue.qindex LIMIT %4, %5"))
            .arg(start)
            .arg(count);

    QStringList queries;
    queries.append(queryString);
    QtConcurrent::run(this,
                      &MediaPlayerBackend::doSqlOperation,
                      MediaPlayerBackend::Select,
                      queries,
                      start,
                      count);
}

void MediaPlayerBackend::insert(int index, const QIviPlayableItem *item)
{
    if (item->type() != "track")
        return;

    int track_index = item->id().toInt();

    QString queryString = QString(QLatin1String("UPDATE queue SET qindex = qindex + 1 WHERE qindex >= %1;"
                                                "INSERT INTO queue(qindex, track_index) VALUES( %1, %2)"))
            .arg(index)
            .arg(track_index);
    QStringList queries = queryString.split(';');

    QtConcurrent::run(this,
                      &MediaPlayerBackend::doSqlOperation,
                      MediaPlayerBackend::Insert,
                      queries, 0, 0);
}

void MediaPlayerBackend::remove(int index)
{
    QString queryString = QString(QLatin1String("DELETE FROM queue WHERE qindex=%1;"
                                                "UPDATE queue SET qindex = qindex - 1 WHERE qindex >= %1"))
            .arg(index);
    QStringList queries = queryString.split(';');

    QtConcurrent::run(this,
                      &MediaPlayerBackend::doSqlOperation,
                      MediaPlayerBackend::Remove,
                      queries, 0, 0);
}

void MediaPlayerBackend::move(int cur_index, int new_index)
{
    QString queryString = QString(QLatin1String("INSERT INTO queue(qindex, track_index) VALUES( %1, %2);"
                                                "DELETE FROM queue WHERE qindex=%1;"
                                                "UPDATE queue SET qindex = qindex + 1 WHERE qindex >= %1 AND qindex <= %2;"))
            .arg(cur_index)
            .arg(new_index);
    QStringList queries = queryString.split(';');

    QtConcurrent::run(this,
                      &MediaPlayerBackend::doSqlOperation,
                      MediaPlayerBackend::Move,
                      queries, 0, 0);
}

void MediaPlayerBackend::doSqlOperation(MediaPlayerBackend::OperationType type, const QStringList &queries, int start, int count)
{
    m_db.transaction();
    QSqlQuery query(m_db);

    for (const QString& queryString : queries) {
        if (query.exec(queryString)) {
            if (type == MediaPlayerBackend::Select) {
                QVariantList list;
                while (query.next()) {
                    QString id = query.value(0).toString();
                    QString artist = query.value(1).toString();
                    QString album = query.value(2).toString();

                    //Creating the TrackItem in an factory with would make this more performant
                    TrackItem item;
                    item.setId(id);
                    item.setTitle(query.value(3).toString());
                    item.setArtist(artist);
                    item.setAlbum(album);
                    item.setUrl(QUrl::fromLocalFile(query.value(6).toString()));
                    list.append(QVariant::fromValue(item));
                }

                emit dataFetched(list, start, list.count() >= count);
            } else if (type == MediaPlayerBackend::Insert) {

            }
        } else {
            qDebug() << queries;
            qDebug() << query.lastError().text();
            m_db.rollback();
            break;
        }
    }
    m_db.commit();
}
