/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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
#include <QtIviCore/QIviServiceObject>
#include <QtDebug>

QIviMediaPlayerPrivate::QIviMediaPlayerPrivate(const QString &interface, QIviMediaPlayer *parent)
    : QIviAbstractFeaturePrivate(interface, parent)
    , q_ptr(parent)
    , m_playQueue(nullptr)
    , m_currentTrack(0)
    , m_position(-1)
    , m_duration(-1)
{
    qRegisterMetaType<QIviPlayQueue*>();
    qRegisterMetaType<QIviPlayableItem>();
    qRegisterMetaType<QIviAudioTrackItem>();
}

void QIviMediaPlayerPrivate::init()
{
    m_playQueue = new QIviPlayQueue(q_ptr);
}

void QIviMediaPlayerPrivate::clearToDefaults()
{
    m_currentTrackData = QVariant();
    m_currentTrack = 0;
    m_position = -1;
    m_duration = -1;
    m_playQueue->d_func()->clearToDefaults();
}

void QIviMediaPlayerPrivate::onCurrentTrackChanged(const QVariant &currentTrack)
{
    if (m_currentTrackData == currentTrack)
        return;

    const QIviPlayableItem *currentItem = playableItem(m_currentTrackData);
    if (m_currentTrack == currentItem)
        return;

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

//TODO replace me by a generic function for all classes
const QIviPlayableItem *QIviMediaPlayerPrivate::playableItem(const QVariant &item)
{
    const void *data = item.constData();

    QMetaType type(item.userType());
    if (!type.flags().testFlag(QMetaType::IsGadget)) {
        qCritical() << "The passed QVariant needs to use the Q_GADGET macro";
        return nullptr;
    }

    const QMetaObject *mo = type.metaObject();
    while (mo) {
        if (mo->className() == QIviPlayableItem::staticMetaObject.className())
            return reinterpret_cast<const QIviPlayableItem*>(data);
        mo = mo->superClass();
    }

    qCritical() << "The passed QVariant is not derived from QIviPlayableItem";
    return nullptr;
}

QIviMediaPlayerBackendInterface *QIviMediaPlayerPrivate::playerBackend() const
{
    Q_Q(const QIviMediaPlayer);
    QIviServiceObject *so = q->serviceObject();
    if (so)
        return qobject_cast<QIviMediaPlayerBackendInterface*>(so->interfaceInstance(QIviStringMediaPlayerInterfaceName));

    return nullptr;
}

/*!
    \class QIviMediaPlayer
    \inmodule QtIviMedia
    \brief Provides a interface to control a media player

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
    \brief Provides a interface to control a media player

    The MediaPlayer provides methods to control a media player. This media player can
    be local or even a remote device you are connected to e.g. over bluetooth.

    By default the autoDiscovery is turned to Automatic for this feature and most likely will connect to
    a local media player instance.
*/

/*!
   Constructs a QIviMediaPlayer.

   The \a parent argument is passed on to the \l QIviAbstractFeature base class.
*/
QIviMediaPlayer::QIviMediaPlayer(QObject *parent)
    : QIviAbstractFeature(*new QIviMediaPlayerPrivate(QIviStringMediaPlayerInterfaceName, this), parent)
{
    Q_D(QIviMediaPlayer);
    d->init();
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
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't start playing without a connected backend");
        return;
    }

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
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't pause playing without a connected backend");
        return;
    }

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
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't stop playing without a connected backend");
        return;
    }

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
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't seek without a connected backend");
        return;
    }

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
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't skip to the next track without a connected backend");
        return;
    }

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
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't skip to the next track without a connected backend");
        return;
    }

    backend->previous();
}

/*!
    \internal
 */
QIviMediaPlayer::QIviMediaPlayer(QIviMediaPlayerPrivate &dd, QObject *parent)
    : QIviAbstractFeature(dd, parent)
{
    Q_D(QIviMediaPlayer);
    d->init();
}

/*!
    \reimp
 */
bool QIviMediaPlayer::acceptServiceObject(QIviServiceObject *serviceObject)
{
    return serviceObject->interfaces().contains(QIviStringMediaPlayerInterfaceName);
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

    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::positionChanged,
                            d, &QIviMediaPlayerPrivate::onPositionChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::currentTrackChanged,
                            d, &QIviMediaPlayerPrivate::onCurrentTrackChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::durationChanged,
                            d, &QIviMediaPlayerPrivate::onDurationChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::dataFetched,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onDataFetched);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::countChanged,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onCountChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::dataChanged,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onDataChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::currentIndexChanged,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onCurrentIndexChanged);

    backend->initialize();
    d->m_playQueue->d_func()->resetModel();
}

/*!
    \reimp
 */
void QIviMediaPlayer::disconnectFromServiceObject(QIviServiceObject *serviceObject)
{
    QIviMediaPlayerBackendInterface *backend = qobject_cast<QIviMediaPlayerBackendInterface*>(serviceObject->interfaceInstance(QIviStringMediaPlayerInterfaceName));

    if (backend)
        disconnect(backend, 0, this, 0);
}

/*!
    \reimp
 */
void QIviMediaPlayer::clearServiceObject()
{
    Q_D(QIviMediaPlayer);
    d->clearToDefaults();
}

#include "moc_qivimediaplayer.cpp"
