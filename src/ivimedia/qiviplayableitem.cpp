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

#include "qiviplayableitem.h"

/*!
    \class QIviPlayableItem
    \inmodule QtIviMedia
    \brief The QIviPlayableItem is the base class of a row in the QIviPlayQueue model.
*/

/*!
    \qmltype PlayableItem
    \qmlabstract
    \instantiates QIviPlayableItem
    \inqmlmodule QtIvi.Media
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
{
}

QIviPlayableItem::~QIviPlayableItem()
{
}

/*!
    \class QIviAudioTrackItem
    \inmodule QtIviMedia
    \brief The QIviAudioTrackItem represents a playable audio track.
*/

/*!
    \qmltype AudioTrackItem
    \instantiates QIviAudioTrackItem
    \inqmlmodule QtIvi.Media
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
    \property QIviAudioTrackItem::coverArt

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
{
}

QIviAudioTrackItem::~QIviAudioTrackItem()
{
}
