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

#include "qivimediaplayer.h"
#include "qivimediaplayer_p.h"
#include "qiviplayqueue.h"
#include "qiviplayqueue_p.h"
#include "qiviqmlconversion_helper.h"
#include <QtIviCore/QIviServiceObject>
#include <QtDebug>
#include <QDataStream>
#include <QMetaEnum>

QT_BEGIN_NAMESPACE

QIviMediaPlayerPrivate::QIviMediaPlayerPrivate(const QString &interface, QIviMediaPlayer *parent)
    : QIviAbstractFeaturePrivate(interface, parent)
    , q_ptr(parent)
    , m_playQueue(nullptr)
    , m_playMode(QIviMediaPlayer::Normal)
    , m_playState(QIviMediaPlayer::Stopped)
    , m_currentTrack(nullptr)
    , m_position(-1)
    , m_duration(-1)
    , m_volume(0)
    , m_muted(false)
{
    qRegisterMetaType<QIviPlayQueue*>();
    qRegisterMetaType<QIviPlayableItem>();
    qRegisterMetaType<QIviAudioTrackItem>();
}

void QIviMediaPlayerPrivate::initialize()
{
    QIviAbstractFeaturePrivate::initialize();
    m_playQueue = new QIviPlayQueue(q_ptr);
}

void QIviMediaPlayerPrivate::clearToDefaults()
{
    m_playMode = QIviMediaPlayer::Normal;
    m_currentTrackData = QVariant();
    m_currentTrack = nullptr;
    m_position = -1;
    m_duration = -1;
    m_volume = 0;
    m_muted = false;
    m_playQueue->d_func()->clearToDefaults();
}

void QIviMediaPlayerPrivate::onPlayModeChanged(QIviMediaPlayer::PlayMode playMode)
{
    if (m_playMode == playMode)
        return;

    Q_Q(QIviMediaPlayer);
    m_playMode = playMode;
    emit q->playModeChanged(playMode);
}

void QIviMediaPlayerPrivate::onPlayStateChanged(QIviMediaPlayer::PlayState playState)
{
    if (m_playState == playState)
        return;

    Q_Q(QIviMediaPlayer);
    m_playState = playState;
    emit q->playStateChanged(playState);
}

void QIviMediaPlayerPrivate::onCurrentTrackChanged(const QVariant &currentTrack)
{
    if (m_currentTrackData == currentTrack)
        return;

    const QIviPlayableItem *currentItem = nullptr;

    if (currentTrack.isValid()) {
        currentItem = qtivi_gadgetFromVariant<QIviPlayableItem>(q_ptr, currentTrack);
        if (m_currentTrack == currentItem)
            return;
    }

    Q_Q(QIviMediaPlayer);
    m_currentTrackData = currentTrack;
    m_currentTrack = currentItem;
    emit q->currentTrackChanged(m_currentTrackData);
}

void QIviMediaPlayerPrivate::onPositionChanged(qint64 position)
{
    if (m_position == position)
        return;
    Q_Q(QIviMediaPlayer);
    m_position = position;
    emit q->positionChanged(position);
}

void QIviMediaPlayerPrivate::onDurationChanged(qint64 duration)
{
    if (m_duration == duration)
        return;
    Q_Q(QIviMediaPlayer);
    m_duration = duration;
    emit q->durationChanged(duration);
}

void QIviMediaPlayerPrivate::onVolumeChanged(int volume)
{
    if (m_volume == volume)
        return;
    Q_Q(QIviMediaPlayer);
    m_volume = volume;
    emit q->volumeChanged(volume);
}

void QIviMediaPlayerPrivate::onMutedChanged(bool muted)
{
    if (m_muted == muted)
        return;
    Q_Q(QIviMediaPlayer);
    m_muted = muted;
    emit q->mutedChanged(muted);
}

QIviMediaPlayerBackendInterface *QIviMediaPlayerPrivate::playerBackend() const
{
    return backend<QIviMediaPlayerBackendInterface*>();
}

/*!
    \class QIviMediaPlayer
    \inmodule QtIviMedia
    \brief Provides an interface to control a media player.

    The QIviMediaPlayer provides methods to control a media player. This media player can
    be local or even a remote device you are connected to e.g. over bluetooth.

    By default the autoDiscovery is turned to Automatic for this feature and most likely will connect to
    a local media player instance.
*/

/*!
    \qmltype MediaPlayer
    \instantiates QIviMediaPlayer
    \inqmlmodule QtIvi.Media
    \inherits AbstractFeature
    \brief Provides an interface to control a media player.

    The MediaPlayer provides methods to control a media player. This media player can
    be local or even a remote device you are connected to e.g. over bluetooth.

    By default the autoDiscovery is turned to Automatic for this feature and most likely will connect to
    a local media player instance.
*/

/*!
    \enum QIviMediaPlayer::PlayMode
    \value Normal
          Each item in the queue is played in sequential order. Usually the playback stops when the end
          of the queue is reached.
    \value RepeatTrack
          Always repeat the current item. It should still be possible to change the current item
          using next() and previous(), but this depends on the implementation of the backend.
    \value RepeatAll
          When the end of the queue is reached, the first item starts to play.
    \value Shuffle
          The item in the queue are played in an random order.
*/

/*!
    \enum QIviMediaPlayer::PlayState
    \value Playing
          The media player is currently playing an item.
    \value Paused
          The playback is paused and can be continued at the same position.
    \value Stopped
          The playback hasn't been started yet. Starting it, will always start from the beginning.
*/

/*!
    Constructs a QIviMediaPlayer.

    The \a parent argument is passed on to the \l QIviAbstractFeature base class.
*/
QIviMediaPlayer::QIviMediaPlayer(QObject *parent)
    : QIviAbstractFeature(*new QIviMediaPlayerPrivate(QLatin1String(QIviMediaPlayer_iid), this), parent)
{
}

/*!
    \qmlproperty PlayQueue MediaPlayer::playQueue
    \brief Holds the play queue of this media player.

    \sa PlayQueue
*/
/*!
    \property QIviMediaPlayer::playQueue
    \brief Holds the play queue of this media player.

    \sa QIviPlayQueue
*/
QIviPlayQueue *QIviMediaPlayer::playQueue() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_playQueue;
}

/*!
    \qmlproperty enumeration MediaPlayer::playMode
    \brief Holds the current playback mode of the media player.
    Available values are:
    \value Normal
           Each item in the queue is played in sequential order. Usually the playback stops when the end
           of the queue is reached.
    \value RepeatTrack
           Always repeat the current item. It should still be possible to change the current item
           using next() and previous(), but this depends on the implementation of the backend.
    \value RepeatAll
           When the end of the queue is reached, the first item starts to play.
    \value Shuffle
           The item in the queue are played in an random order.
*/
/*!
    \property QIviMediaPlayer::playMode
    \brief Holds the current playback mode of the media player.
*/
QIviMediaPlayer::PlayMode QIviMediaPlayer::playMode() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_playMode;
}

/*!
    \qmlproperty enumeration MediaPlayer::playState
    \brief Holds the current playback state of the media player.
    Available values are:
    \value Playing
           The media player is currently playing an item.
    \value Paused
           The playback is paused and can be continued at the same position.
    \value Stopped
           The playback hasn't been started yet. Starting it, will always start from the beginning.
*/
/*!
    \property QIviMediaPlayer::playState
    \brief Holds the current playback state of the media player.
*/
QIviMediaPlayer::PlayState QIviMediaPlayer::playState() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_playState;
}

/*!
    \qmlproperty object MediaPlayer::currentTrack
    \brief Holds the current track represented as QVariant.

    \note This will be replaced by soon.
*/
/*!
    \property QIviMediaPlayer::currentTrack
    \brief Holds the current track represented as QVariant.

    \note This will be replaced by soon.
*/
QVariant QIviMediaPlayer::currentTrack() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_currentTrackData;
}

/*!
    \qmlproperty int MediaPlayer::position
    \brief Holds the position of the current song of the media player in seconds.
*/
/*!
    \property QIviMediaPlayer::position
    \brief Holds the position of the current song of the media player in seconds.
*/
qint64 QIviMediaPlayer::position() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_position;
}

/*!
    \qmlproperty int MediaPlayer::duration
    \brief Holds the total duration of the current song in seconds.
*/
/*!
    \property QIviMediaPlayer::duration
    \brief Holds the total duration of the current song in seconds.
*/
qint64 QIviMediaPlayer::duration() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_duration;
}

/*!
    \qmlproperty int MediaPlayer::volume
    \brief Holds the sound volume level (0..100)
    \sa muted
*/
/*!
    \property QIviMediaPlayer::volume
    \brief Holds the sound volume level (0..100)
    \sa muted
*/
int QIviMediaPlayer::volume() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_volume;
}


/*!
    \qmlproperty bool MediaPlayer::muted
    \brief This property holds whether the audio output is muted.
    \sa volume
*/
/*!
    \property QIviMediaPlayer::muted
    \brief This property holds whether the audio output is muted.
    \sa volume
*/
bool QIviMediaPlayer::isMuted() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_muted;
}

void QIviMediaPlayer::setPlayMode(QIviMediaPlayer::PlayMode playMode)
{
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't set the play mode without a connected backend");
        return;
    }

    backend->setPlayMode(playMode);
}

void QIviMediaPlayer::setPosition(qint64 position)
{
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't set the position without a connected backend");
        return;
    }

    backend->setPosition(position);
}

/*!
    \qmlmethod MediaPlayer::play()

    Starts to play the current track. If the playQueue is empty
    it's up to the backend to decide what to do.

    \sa pause() stop()
*/

/*!
    \fn void QIviMediaPlayer::play()

    Starts to play the current track. If the playQueue is empty
    it's up to the backend to decide what to do.

    \sa pause() stop()
*/
void QIviMediaPlayer::play()
{
    Q_IVI_BACKEND(QIviMediaPlayer, d->playerBackend(), "Can't start playing without a connected backend");

    backend->play();
}

/*!
    \qmlmethod MediaPlayer::pause()

    Pauses the currently ongoing playback.

    \sa play() stop()
*/

/*!
    \fn void QIviMediaPlayer::pause()

    Pauses the currently ongoing playback.

    \sa play() stop()
*/
void QIviMediaPlayer::pause()
{
    Q_IVI_BACKEND(QIviMediaPlayer, d->playerBackend(), "Can't pause playing without a connected backend");

    backend->pause();
}

/*!
    \qmlmethod MediaPlayer::stop()

    Stops the currently ongoing playback.

    \sa play() pause()
*/

/*!
    \fn void QIviMediaPlayer::stop()

    Stops the currently ongoing playback.

    \sa play() pause()
*/
void QIviMediaPlayer::stop()
{
    Q_IVI_BACKEND(QIviMediaPlayer, d->playerBackend(), "Can't stop playing without a connected backend");

    backend->stop();
}

/*!
    \qmlmethod MediaPlayer::seek(offset)

    Seeks into the current track using \a offset.

    The offset can be positive or negative to either seek forward
    or backward. A successful seek will result in a change of the
    position property.
*/

/*!
    \fn void QIviMediaPlayer::seek(qint64 offset)

    Seeks into the current track using \a offset.

    The offset can be positive or negative to either seek forward
    or backward. A successful seek will result in a change of the
    position property.
*/
void QIviMediaPlayer::seek(qint64 offset)
{
    Q_IVI_BACKEND(QIviMediaPlayer, d->playerBackend(), "Can't seek without a connected backend");

    backend->seek(offset);
}

/*!
    \qmlmethod MediaPlayer::next()

    Skips to the next track in the playQueue.

    \sa playMode
*/

/*!
    \fn void QIviMediaPlayer::next()

    Skips to the next track in the playQueue.

    \sa playMode
*/
void QIviMediaPlayer::next()
{
    Q_IVI_BACKEND(QIviMediaPlayer, d->playerBackend(), "Can't skip to the next track without a connected backend");

    backend->next();
}

/*!
    \qmlmethod MediaPlayer::previous()

    Skips to the previous track in the playQueue.

    \sa playMode
*/

/*!
    \fn void QIviMediaPlayer::previous()

    Skips to the previous track in the playQueue.

    \sa playMode
*/
void QIviMediaPlayer::previous()
{
    Q_IVI_BACKEND(QIviMediaPlayer, d->playerBackend(), "Can't skip to the previous track without a connected backend");

    backend->previous();
}

void QIviMediaPlayer::setVolume(int volume)
{
    Q_IVI_BACKEND(QIviMediaPlayer, d->playerBackend(), "Can't set the volume without a connected backend");

    backend->setVolume(volume);
}

void QIviMediaPlayer::setMuted(bool muted)
{
    Q_IVI_BACKEND(QIviMediaPlayer, d->playerBackend(), "Can't set muted without a connected backend");

    backend->setMuted(muted);
}

/*!
    \internal
*/
QIviMediaPlayer::QIviMediaPlayer(QIviMediaPlayerPrivate &dd, QObject *parent)
    : QIviAbstractFeature(dd, parent)
{
}

/*!
    \reimp
*/
void QIviMediaPlayer::connectToServiceObject(QIviServiceObject *serviceObject)
{
    Q_UNUSED(serviceObject);

    Q_D(QIviMediaPlayer);

    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::playModeChanged,
                            d, &QIviMediaPlayerPrivate::onPlayModeChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::playStateChanged,
                            d, &QIviMediaPlayerPrivate::onPlayStateChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::positionChanged,
                            d, &QIviMediaPlayerPrivate::onPositionChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::currentTrackChanged,
                            d, &QIviMediaPlayerPrivate::onCurrentTrackChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::durationChanged,
                            d, &QIviMediaPlayerPrivate::onDurationChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::volumeChanged,
                            d, &QIviMediaPlayerPrivate::onVolumeChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::mutedChanged,
                            d, &QIviMediaPlayerPrivate::onMutedChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::canReportCountChanged,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onCanReportCountChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::dataFetched,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onDataFetched);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::countChanged,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onCountChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::dataChanged,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onDataChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::currentIndexChanged,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onCurrentIndexChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::initializationDone,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onInitializationDone);

    QIviAbstractFeature::connectToServiceObject(serviceObject);
}

/*!
    \reimp
*/
void QIviMediaPlayer::clearServiceObject()
{
    Q_D(QIviMediaPlayer);
    d->clearToDefaults();
}

QDataStream &operator <<(QDataStream &out, QIviMediaPlayer::PlayMode var)
{
    out << int(var);
    return out;
}

QDataStream &operator>>(QDataStream &in, QIviMediaPlayer::PlayMode &var)
{
    int val;
    in >> val;
    QMetaEnum metaEnum = QMetaEnum::fromType<QIviMediaPlayer::PlayMode>();
    if (metaEnum.valueToKey(val) == nullptr)
        qWarning() << "Received an invalid enum value for type QIviMediaPlayer::PlayMode, value =" << val;
    var = QIviMediaPlayer::PlayMode(val);
    return in;
}

QDataStream &operator <<(QDataStream &out, QIviMediaPlayer::PlayState var)
{
    out << int(var);
    return out;
}

QDataStream &operator>>(QDataStream &in, QIviMediaPlayer::PlayState &var)
{
    int val;
    in >> val;
    QMetaEnum metaEnum = QMetaEnum::fromType<QIviMediaPlayer::PlayState>();
    if (metaEnum.valueToKey(val) == nullptr)
        qWarning() << "Received an invalid enum value for type QIviMediaPlayer::PlayState, value =" << val;
    var = QIviMediaPlayer::PlayState(val);
    return in;
}

QT_END_NAMESPACE

#include "moc_qivimediaplayer.cpp"
