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

#include "qiviplayableitem.h"
#include <QDataStream>
#include <QDebug>

QT_BEGIN_NAMESPACE

class QIviPlayableItemPrivate : public QSharedData
{
public:
    QIviPlayableItemPrivate()
    {}

    QIviPlayableItemPrivate(const QIviPlayableItemPrivate &other)
        : QSharedData(other)
        , m_url(other.m_url)
    {}

    QUrl m_url;
};

class QIviAudioTrackItemPrivate : public QSharedData
{
public:
    QIviAudioTrackItemPrivate()
        : m_year(-1)
        , m_trackNumber(-1)
        , m_duration(-1)
        , m_rating(-1)
    {}

    QIviAudioTrackItemPrivate(const QIviAudioTrackItemPrivate &other)
        : QSharedData(other)
        , m_title(other.m_title)
        , m_artist(other.m_artist)
        , m_album(other.m_album)
        , m_genre(other.m_genre)
        , m_year(other.m_year)
        , m_trackNumber(other.m_trackNumber)
        , m_duration(other.m_duration)
        , m_coverArtUrl(other.m_coverArtUrl)
        , m_rating(other.m_rating)
    {}

    QString m_title;
    QString m_artist;
    QString m_album;
    QString m_genre;
    int m_year;
    int m_trackNumber;
    qint64 m_duration;
    QUrl m_coverArtUrl;
    int m_rating;
};

/*!
    \class QIviPlayableItem
    \inmodule QtIviMedia
    \brief The QIviPlayableItem is the base class of a row in the QIviPlayQueue model.
*/

/*!
    \qmltype PlayableItem
    \instantiates QIviPlayableItem
    \inqmlmodule QtIvi.Media
    \inherits StandardItem
    \brief The PlayableItem is the base class of a row in the PlayQueue model.

    \note This item is not creatable from QML.
*/

/*!
    \qmlproperty QUrl PlayableItem::url

    The url to the resource which should be played.

    E.g. a local file url to a audio track or a url to a web stream.
*/

/*!
    \property QIviPlayableItem::url

    The url to the resource which should be played.

    E.g. a local file url to a audio track or a url to a web stream.
*/

QIviPlayableItem::QIviPlayableItem()
    : QIviStandardItem()
    , d(new QIviPlayableItemPrivate)
{
}

QIviPlayableItem::QIviPlayableItem(const QIviPlayableItem &rhs)
    : QIviStandardItem(rhs)
    , d(rhs.d)
{
}

QIviPlayableItem &QIviPlayableItem::operator=(const QIviPlayableItem &rhs)
{
    QIviStandardItem::operator=(rhs);
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QIviPlayableItem::~QIviPlayableItem()
{
}

QUrl QIviPlayableItem::url() const
{
    return d->m_url;
}

void QIviPlayableItem::setUrl(const QUrl &url)
{
    d->m_url = url;
}

/*!
    \reimp
*/
QString QIviPlayableItem::type() const
{
    return QLatin1String("playableitem");
}

/*!
    Returns \e true if this item is equal to \a other; otherwise returns false.

    \sa operator!=()
*/
bool QIviPlayableItem::operator==(const QIviPlayableItem &other)
{
    return (QIviStandardItem::operator==(other) &&
            d->m_url == other.d->m_url);
}

/*!
    \fn bool QIviPlayableItem::operator!=(const QIviPlayableItem &other)

    Returns \e true if this item is not equal to \a other; otherwise returns false.

    \sa operator==()
*/

/*!
    \class QIviAudioTrackItem
    \inmodule QtIviMedia
    \brief The QIviAudioTrackItem represents a playable audio track.
*/

/*!
    \qmltype AudioTrackItem
    \instantiates QIviAudioTrackItem
    \inqmlmodule QtIvi.Media
    \inherits PlayableItem
    \brief The AudioTrackItem represents a playable audio track.

    \note This item is not creatable from QML.
*/

/*!
    \qmlproperty string AudioTrackItem::title

    Holds the title of the audio track.
*/

/*!
    \property QIviAudioTrackItem::title

    Holds the title of the audio track.
*/

/*!
    \qmlproperty string AudioTrackItem::album

    Holds the album name of the audio track.
*/

/*!
    \property QIviAudioTrackItem::album

    Holds the album name of the audio track.
*/

/*!
    \qmlproperty string AudioTrackItem::artist

    Holds the artist of the audio track.
*/

/*!
    \property QIviAudioTrackItem::artist

    Holds the artist of the audio track.
*/

/*!
    \qmlproperty string AudioTrackItem::genre

    Holds the genre of the audio track.
*/

/*!
    \property QIviAudioTrackItem::genre

    Holds the genre of the audio track.
*/

/*!
    \qmlproperty int AudioTrackItem::year

    Holds the release year of the audio track.
*/

/*!
    \property QIviAudioTrackItem::year

    Holds the release year of the audio track.
*/

/*!
    \qmlproperty int AudioTrackItem::trackNumber

    Holds the number this track has in the album.
*/

/*!
    \property QIviAudioTrackItem::trackNumber

    Holds the number this track has in the album.
*/

/*!
    \qmlproperty int AudioTrackItem::duration

    Holds the duration of the audio track in ms.
*/

/*!
    \property QIviAudioTrackItem::duration

    Holds the duration of the audio track.
*/

/*!
    \qmlproperty string AudioTrackItem::coverArt

    Holds the cover art of the audio track.

    This can be a url to a local image file or a link to a qml image provider
*/

/*!
    \property QIviAudioTrackItem::coverArtUrl

    Holds the cover art of the audio track.

    This can be a url to a local image file or a link to a qml image provider
*/

/*!
    \qmlproperty int AudioTrackItem::rating

    Holds the rating of the audio track.

    This is usually a value between \e 0 and \e 5.
*/

/*!
    \property QIviAudioTrackItem::rating

    Holds the rating of the audio track.

    This is usually a value between \e 0 and \e 5.
*/
QIviAudioTrackItem::QIviAudioTrackItem()
    : QIviPlayableItem()
    , d(new QIviAudioTrackItemPrivate)
{
}

QIviAudioTrackItem::QIviAudioTrackItem(const QIviAudioTrackItem &rhs)
    : QIviPlayableItem(rhs)
    , d(rhs.d)
{
}

QIviAudioTrackItem &QIviAudioTrackItem::operator=(const QIviAudioTrackItem &rhs)
{
    QIviPlayableItem::operator=(rhs);
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QIviAudioTrackItem::~QIviAudioTrackItem()
{
}

QString QIviAudioTrackItem::title()
{
    return d->m_title;
}

void QIviAudioTrackItem::setTitle(const QString &title)
{
    d->m_title = title;
}

QString QIviAudioTrackItem::artist()
{
    return d->m_artist;
}

void QIviAudioTrackItem::setArtist(const QString &artist)
{
    d->m_artist = artist;
}

QString QIviAudioTrackItem::album()
{
    return d->m_album;
}

void QIviAudioTrackItem::setAlbum(const QString &album)
{
    d->m_album = album;
}

QString QIviAudioTrackItem::genre()
{
    return d->m_genre;
}

void QIviAudioTrackItem::setGenre(const QString &genre)
{
    d->m_genre = genre;
}

int QIviAudioTrackItem::year()
{
    return d->m_year;
}

void QIviAudioTrackItem::setYear(int year)
{
    d->m_year = year;
}

int QIviAudioTrackItem::trackNumber()
{
    return d->m_trackNumber;
}

void QIviAudioTrackItem::setTrackNumber(int trackNumber)
{
    d->m_trackNumber = trackNumber;
}

qint64 QIviAudioTrackItem::duration()
{
    return d->m_duration;
}

void QIviAudioTrackItem::setDuration(qint64 duration)
{
    d->m_duration = duration;
}

QUrl QIviAudioTrackItem::coverArtUrl()
{
    return d->m_coverArtUrl;
}

void QIviAudioTrackItem::setCoverArtUrl(const QUrl &url)
{
    d->m_coverArtUrl = url;
}

int QIviAudioTrackItem::rating()
{
    return d->m_rating;
}


void QIviAudioTrackItem::setRating(int rating)
{
    d->m_rating = rating;
}

/*!
    \reimp
*/
QString QIviAudioTrackItem::name() const
{
    return d->m_title;
}

/*!
    \reimp
*/
QString QIviAudioTrackItem::type() const
{
    return QLatin1String("audiotrack");
}

/*!
    Returns \e true if this item is equal to \a other; otherwise returns false.

    \sa operator!=()
*/
bool QIviAudioTrackItem::operator==(const QIviAudioTrackItem &other)
{
    return (QIviPlayableItem::operator==(other) &&
            d->m_title == other.d->m_title &&
            d->m_artist == other.d->m_artist &&
            d->m_album == other.d->m_album &&
            d->m_genre == other.d->m_genre &&
            d->m_year == other.d->m_year &&
            d->m_trackNumber == other.d->m_trackNumber &&
            d->m_duration == other.d->m_duration &&
            d->m_coverArtUrl == other.d->m_coverArtUrl &&
            d->m_rating == other.d->m_rating);
}

/*!
    \fn bool QIviAudioTrackItem::operator!=(const QIviAudioTrackItem &other)

    Returns \e true if this item is not equal to \a other; otherwise returns false.

    \sa operator==()
*/

QT_END_NAMESPACE
