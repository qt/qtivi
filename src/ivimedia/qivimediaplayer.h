/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

#ifndef QIVIMEDIAPLAYER_H
#define QIVIMEDIAPLAYER_H

#include <QtIviCore/QIviAbstractFeature>
#include <QtIviMedia/QIviPlayableItem>
#include <QtIviMedia/qtivimediaglobal.h>

QT_BEGIN_NAMESPACE

class QIviMediaPlayerPrivate;
class QIviPlayQueue;

class Q_QTIVIMEDIA_EXPORT QIviMediaPlayer : public QIviAbstractFeature
{
    Q_OBJECT

    //TODO Status
    Q_PROPERTY(QIviPlayQueue *playQueue READ playQueue CONSTANT)
    Q_PROPERTY(QIviMediaPlayer::PlayMode playMode READ playMode WRITE setPlayMode NOTIFY playModeChanged)
    Q_PROPERTY(QIviMediaPlayer::PlayState playState READ playState NOTIFY playStateChanged)
    Q_PROPERTY(QVariant currentTrack READ currentTrack NOTIFY currentTrackChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged)

public:
    enum PlayMode {
        Normal,
        RepeatTrack,
        RepeatAll,
        Shuffle
    };
    Q_ENUM(PlayMode)

    enum PlayState {
        Playing,
        Paused,
        Stopped
    };
    Q_ENUM(PlayState)

    explicit QIviMediaPlayer(QObject *parent = nullptr);

    QIviPlayQueue *playQueue() const;
    PlayMode playMode() const;
    PlayState playState() const;
    QVariant currentTrack() const;
    qint64 position() const;
    qint64 duration() const;
    int volume() const;
    bool isMuted() const;

public Q_SLOTS:
    void setPlayMode(QIviMediaPlayer::PlayMode playMode);
    void setPosition(qint64 position);
    void play();
    void pause();
    void stop();
    void seek(qint64 offset);
    void next();
    void previous();
    void setVolume(int volume);
    void setMuted(bool muted);

Q_SIGNALS:
    void playModeChanged(QIviMediaPlayer::PlayMode playMode);
    void playStateChanged(QIviMediaPlayer::PlayState playState);
    void currentTrackChanged(const QVariant &currentTrack);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void volumeChanged(int volume);
    void mutedChanged(bool muted);

protected:
    QIviMediaPlayer(QIviMediaPlayerPrivate &dd, QObject *parent = nullptr);

    virtual void connectToServiceObject(QIviServiceObject *serviceObject) override;
    virtual void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIviMediaPlayer)
    Q_PRIVATE_SLOT(d_func(), void onPlayModeChanged(QIviMediaPlayer::PlayMode playMode))
    Q_PRIVATE_SLOT(d_func(), void onPlayStateChanged(QIviMediaPlayer::PlayState playState))
    Q_PRIVATE_SLOT(d_func(), void onCurrentTrackChanged(const QVariant &currentTrack))
    Q_PRIVATE_SLOT(d_func(), void onPositionChanged(qint64 position))
    Q_PRIVATE_SLOT(d_func(), void onDurationChanged(qint64 duration))
    Q_PRIVATE_SLOT(d_func(), void onVolumeChanged(int volume))
    Q_PRIVATE_SLOT(d_func(), void onMutedChanged(bool muted))

    friend class QIviPlayQueuePrivate;
};

Q_QTIVIMEDIA_EXPORT QDataStream &operator<<(QDataStream &out, QIviMediaPlayer::PlayMode var);
Q_QTIVIMEDIA_EXPORT QDataStream &operator>>(QDataStream &in, QIviMediaPlayer::PlayMode &var);
Q_QTIVIMEDIA_EXPORT QDataStream &operator<<(QDataStream &out, QIviMediaPlayer::PlayState var);
Q_QTIVIMEDIA_EXPORT QDataStream &operator>>(QDataStream &in, QIviMediaPlayer::PlayState &var);

QT_END_NAMESPACE

#endif // QIVIMEDIAPLAYER_H
