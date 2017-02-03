/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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

#include "qivimediaplayerbackendinterface.h"

/*!
    \class QIviMediaPlayerBackendInterface
    \inmodule QtIviMedia
    \ingroup backends
    \inherits QObject
    \brief The QIviMediaPlayerBackendInterface defines the interface for backends to the
    QIviMediaPlayer feature class.

    The QIviMediaPlayerBackendInterface is the interface used by \l QIviMediaPlayer

    The interface is discovered by a \l QIviMediaPlayer object, which connects to it and sets it up.

    <example of a fully featured backend>
*/

/*!
    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIviMediaPlayerBackendInterface::QIviMediaPlayerBackendInterface(QObject *parent)
    : QObject(parent)
{

}

/*!
    \fn QIviMediaPlayerBackendInterface::initialize()

    Initializes the backend. This function is called, after a feature connected to the backend.
    It is expected that this function will inform about the current state of the backend by emitting signals with the current status.
*/

/*!
    \fn QIviMediaPlayerBackendInterface::play()

    Starts playing the current playable item.

    \sa pause() stop()
*/

/*!
    \fn QIviMediaPlayerBackendInterface::pause()

    Pauses the playback of the current playable item.

    \sa play() stop()
*/

/*!
    \fn QIviMediaPlayerBackendInterface::stop()

    Stops playing the current playable item.

    \sa play() stop()
*/

/*!
    \fn QIviMediaPlayerBackendInterface::seek(qint64 offset)

    Seeks the current playable item using \a offset in ms.

    The offset can be positive or negative to either seek forward
    or backward. A successful seek will result in a change of the
    position property.
*/

/*!
    \fn QIviMediaPlayerBackendInterface::next()

    Skips to the next playable item.
*/

/*!
    \fn QIviMediaPlayerBackendInterface::previous()

    Skips to the previous playable item.
*/

/*!
    \fn QIviMediaPlayerBackendInterface::setCurrentIndex(int currentIndex)

    Sets the \a currentIndex of the play-queue to the given index. If a valid index is passed, the
    function should update the current playable item and emit the corresponding change signals

    \sa currentIndexChanged currentTrackChanged durationChanged
*/

/*!
    \fn QIviMediaPlayerBackendInterface::canReportCount()

    Returns \e true if the backend can return the final number of items for a specific request. This makes it possible to support the QIviPlayQueue::DataChanged loading
    type. The number of items can be returned by emitting the countChanged signal.
*/

/*!
    \fn QIviMediaPlayerBackendInterface::fetchData(int start, int count)

    This function is called whenever new playable items needs to be retrieved by the QIviPlayQueue.

    The parameters \a start and \a count define the range of data which should be fetched. This method is expected to emit the dataFetched() signal once
    the new data is ready.

    \sa dataFetched()
*/

/*!
    \fn QIviMediaPlayerBackendInterface::insert(int index, const QIviPlayableItem *item)

    Adds the playable item identitifed by \a item into the play queue at \a index. The provided item is owned by the QIviMediaPlayer and it's expected
    that the backend stores its internal representation.

    \sa dataChanged()
*/

/*!
    \fn QIviMediaPlayerBackendInterface::remove(int index)

    Removes the playable item at position \a index from the play queue.

    \sa dataChanged()
*/

/*!
    \fn QIviMediaPlayerBackendInterface::move(int currentIndex, int newIndex)

    Moves the playable item at position \a currentIndex of the play queue to the new position \a newIndex.

    \sa dataChanged()
*/

/*!
    \fn QIviMediaPlayerBackendInterface::currentTrackChanged(const QVariant &currentTrack)

    Emitted when the currently played playable item changed. The new playable item will be passed as \a currentTrack.
*/

/*!
    \fn QIviMediaPlayerBackendInterface::positionChanged(qint64 position)

    Emitted when the position of the currently playing playable item changed. The new position will be passed as \a position in ms.
*/

/*!
    \fn QIviMediaPlayerBackendInterface::durationChanged(qint64 duration)

    Emitted when the duration of the currently playing playable item changed. The new duration will be passed as \a duration in ms.
*/

/*!
    \fn QIviMediaPlayerBackendInterface::currentIndexChanged(int currentIndex)

    Emitted when the currently played playable item in the play queue changed. The index of the new current playable item will be passed as \a currentIndex.
*/

/*!
    \fn QIviMediaPlayerBackendInterface::countChanged(int newLength)

    This signal is emitted once the backend knows about the new size of the play queue.
    The new number of items is returned as \a newLength.

    This signal is expected to be emitted before the data is returned by emitting the dataFetched() signal.

    \sa fetchData() dataFetched()
*/

/*!
    \fn QIviMediaPlayerBackendInterface::dataFetched(const QList<QVariant> &data, int start, bool moreAvailable)

    This signal is emitted as a result of a call of fetchData() and returns the requested data in the argument \a data to the QIviPlayQueue.
    The arguments \a start holds the index where the data starts and \a moreAvailable holds whether there is more data available and a new fetchData() call can be used to retrieve this data.

    \sa fetchData() dataFetched()
*/

/*!
    \fn QIviMediaPlayerBackendInterface::dataChanged(const QList<QVariant> &data, int start, int count)

    This signal is emitted whenever the play queue changes, e.g. by a insert, remove or a move operation.
    The \a data argument holds the new data which will replace the data starting at \a start until \a count.

    If \a data is empty the rows identified by the range of \a start and \a count will be removed.
    If \a count is 0, \a data will be inserted at \a start.

    For inserting a new item, the item is passed in \a data and \a start is used for where the item should be inserted, the \a count argument needs to be 0 as we don't want to replace existing data:

    \code
    QList<ExampleItem> list;
    ExampleItem item = ExampleItem();
    list.insert(index, item);
    QVariantList items = { QVariant::fromValue(item) };
    emit dataChanged(items, index, 0);
    \endcode

    Removing an item is very similar, \a start is used to indicate which item and \a count to indicate how much:

    \code
    list.removeAt(index);
    emit dataChanged(QVariantList(), index, 1);
    \endcode

    \sa insert() remove() move()
*/
