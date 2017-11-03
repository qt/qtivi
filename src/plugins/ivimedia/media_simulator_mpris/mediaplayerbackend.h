/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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

#ifndef MEDIAPLAYERBACKEND_H
#define MEDIAPLAYERBACKEND_H

#include <QtConcurrent/QtConcurrent>
#include <QtMultimedia/QMediaPlayer>
#include <QtIviMedia/QIviMediaPlayerBackendInterface>
#include "mediaplayer2_interface.h"

class MediaPlayerBackend : public QIviMediaPlayerBackendInterface
{
    Q_OBJECT

public:
    MediaPlayerBackend(const QString &dbusServiceName, const QDBusConnection &dbusConnection, QObject *parent = nullptr);

    virtual void initialize() override;
    virtual void play() override;
    virtual void pause() override;
    virtual void stop() override;
    virtual void seek(qint64 offset) override;
    virtual void next() override;
    virtual void previous() override;
    virtual void setPlayMode(QIviMediaPlayer::PlayMode playMode) override;
    virtual void setPosition(qint64 position) override;
    virtual void setCurrentIndex(int index) override;

    virtual bool canReportCount() override;
    virtual void fetchData(int start, int count) override;

    virtual void insert(int index, const QIviPlayableItem *item) override;
    virtual void remove(int index) override;
    virtual void move(int cur_index, int new_index) override;

signals:
    void initializationDone();

private Q_SLOTS:
    void onPropertiesChanged(const QString &interface, const QVariantMap &changed_properties, const QStringList &invalidated_properties);
    void onSeeked(qlonglong position);

private:
    QThreadPool m_threadPool;
    OrgMprisMediaPlayer2PlayerInterface *m_dbusPlayer;
    OrgFreedesktopDBusPropertiesInterface *m_dbusPlayerProperties;
    OrgMprisMediaPlayer2TrackListInterface *m_dbusTrackList;
    QList<QDBusObjectPath> m_tracks;
    QDBusObjectPath m_currentTrack;

    void updateTrackIndex(const QDBusObjectPath &newTrackId);
    void updateTrackIndex(const QVariantMap &newTrack);
    QIviAudioTrackItem audioTrackFromMPRIS2Object(const QVariantMap &metadata);
    QFuture<QVariantList> itemsForMPRIS2Object(QList<QDBusObjectPath> obj);
    void fetchOrUpdate(int start, int count, bool fetch);
    void handlePlaybackStatus(const QString &playbackStatus);
    void handlePosition(qlonglong position);
};

#endif // MEDIAPLAYERBACKEND_H
