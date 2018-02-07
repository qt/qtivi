/****************************************************************************
**
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

#include "qiviplayqueue.h"
#include "qiviplayqueue_p.h"
#include "qivimediaplayer.h"
#include "qiviqmlconversion_helper.h"

#include <QtDebug>

QT_BEGIN_NAMESPACE

QIviPlayQueuePrivate::QIviPlayQueuePrivate(QIviMediaPlayer *player, QIviPlayQueue *model)
    : QAbstractItemModelPrivate()
    , q_ptr(model)
    , m_player(player)
    , m_currentIndex(-1)
    , m_chunkSize(30)
    , m_moreAvailable(false)
    , m_fetchMoreThreshold(10)
    , m_fetchedDataCount(0)
    , m_loadingType(QIviPlayQueue::FetchMore)
{
    qRegisterMetaType<QIviPlayableItem>();
}

QIviPlayQueuePrivate::~QIviPlayQueuePrivate()
{
}

void QIviPlayQueuePrivate::initialize()
{
    Q_Q(QIviPlayQueue);

    QObject::connect(q, &QAbstractListModel::rowsInserted,
                     q, &QIviPlayQueue::countChanged);
    QObject::connect(q, &QAbstractListModel::rowsRemoved,
                     q, &QIviPlayQueue::countChanged);
    QObject::connect(q, &QAbstractListModel::modelReset,
                     q, &QIviPlayQueue::countChanged);
    QObjectPrivate::connect(q, &QIviPlayQueue::fetchMoreThresholdReached,
                            this, &QIviPlayQueuePrivate::onFetchMoreThresholdReached);
}

void QIviPlayQueuePrivate::onCurrentIndexChanged(int currentIndex)
{
    if (m_currentIndex == currentIndex)
        return;
    Q_Q(QIviPlayQueue);
    m_currentIndex = currentIndex;
    emit q->currentIndexChanged(currentIndex);
}

void QIviPlayQueuePrivate::onDataFetched(const QList<QVariant> &items, int start, bool moreAvailable)
{
    if (!items.count())
        return;

    Q_Q(QIviPlayQueue);
    m_moreAvailable = moreAvailable;

    if (m_loadingType == QIviPlayQueue::FetchMore) {
        q->beginInsertRows(QModelIndex(), m_itemList.count(), m_itemList.count() + items.count() -1);
        m_itemList += items;
        m_fetchedDataCount = m_itemList.count();
        q->endInsertRows();
    } else {
        if (m_itemList.count() < start + items.count()) {
            qWarning() << "countChanged signal needs to be emitted before the dataFetched signal";
            return;
        }

        m_fetchedDataCount = start + items.count();

        for (int i = 0; i < items.count(); i++)
            m_itemList.replace(start + i, items.at(i));
        emit q->dataChanged(q->index(start), q->index(start + items.count() -1));
    }
}

void QIviPlayQueuePrivate::onCountChanged(int new_length)
{
    if (m_loadingType != QIviPlayQueue::DataChanged || m_itemList.count() == new_length)
        return;

    Q_Q(QIviPlayQueue);
    q->beginInsertRows(QModelIndex(), m_itemList.count(), m_itemList.count() + new_length -1);
    for (int i = 0; i < new_length; i++)
        m_itemList.append(QVariant());
    q->endInsertRows();
}

void QIviPlayQueuePrivate::onDataChanged(const QList<QVariant> &data, int start, int count)
{
    if (start < 0 || start > m_itemList.count()) {
        qWarning("provided start argument is out of range");
        return;
    }

    if (count < 0 || count > m_itemList.count() - start) {
        qWarning("provided count argument is out of range");
        return;
    }

    Q_Q(QIviPlayQueue);

    //delta > 0 insert rows
    //delta < 0 remove rows
    int delta = data.count() - count;
    //find data overlap for updates
    int updateCount = qMin(data.count(), count);
    int updateCountEnd = updateCount > 0 ? updateCount + 1 : 0;
    //range which is either added or removed
    int insertRemoveStart = start + updateCountEnd;
    int insertRemoveCount = qMax(data.count(), count) - updateCount;

    if (updateCount > 0) {
        for (int i = start, j=0; j < updateCount; i++, j++)
            m_itemList.replace(i, data.at(j));
        emit q->dataChanged(q->index(start), q->index(start + updateCount -1));
    }

    if (delta < 0) { //Remove
        q->beginRemoveRows(QModelIndex(), insertRemoveStart, insertRemoveStart + insertRemoveCount -1);
        for (int i = insertRemoveStart; i < insertRemoveStart + insertRemoveCount; i++)
            m_itemList.removeAt(i);
        q->endRemoveRows();
    } else if (delta > 0) { //Insert
        q->beginInsertRows(QModelIndex(), insertRemoveStart, insertRemoveStart + insertRemoveCount -1);
        for (int i = insertRemoveStart, j = updateCountEnd; i < insertRemoveStart + insertRemoveCount; i++, j++)
            m_itemList.insert(i, data.at(j));
        q->endInsertRows();
    }
}

void QIviPlayQueuePrivate::onFetchMoreThresholdReached()
{
    Q_Q(QIviPlayQueue);
    q->fetchMore(QModelIndex());
}

void QIviPlayQueuePrivate::resetModel()
{
    Q_Q(QIviPlayQueue);
    q->beginResetModel();
    m_itemList.clear();
    q->endResetModel();
    m_fetchedDataCount = 0;

    m_moreAvailable = false;
    q->fetchMore(QModelIndex());
}

void QIviPlayQueuePrivate::clearToDefaults()
{
    m_currentIndex = -1;
    m_chunkSize = 30;
    m_moreAvailable = false;
    m_fetchMoreThreshold = 10;
    m_fetchedDataCount = 0;
    m_loadingType = QIviPlayQueue::FetchMore;
}

const QIviPlayableItem *QIviPlayQueuePrivate::itemAt(int i) const
{
    QVariant var = m_itemList.at(i);
    if (!var.isValid())
        return nullptr;

    return qtivi_gadgetFromVariant<QIviPlayableItem>(var);
}

QIviMediaPlayerBackendInterface *QIviPlayQueuePrivate::playerBackend() const
{
    return m_player->d_func()->playerBackend();
}

/*!
    \class QIviPlayQueue
    \inmodule QtIviMedia
    \brief Provides a play queue for the QIviMediaPlayer

    The QIviPlayQueue is a model which is used by the QIviMediaPlayer to control the
    play order of QIviPlayableItems.

    It provides mechanisms for adding new items and managing the existing ones by removing
    or moving them around.

    The QIviPlayQueue can't be instantiated by its own and can only be retrieved through the QIviMediaPlayer.

    The following roles are available in this model:

    \table
    \header
        \li Role name
        \li Type
        \li Description
    \row
        \li \c name
        \li string
        \li The name of the playable item. E.g. The track name or the name of the web-stream.
    \row
        \li \c type
        \li string
        \li The type of the playable item. E.g. \e "track" or \e "web-stream"
    \row
        \li \c item
        \li QIviPlayableItem
        \li The playable item instance. This can be used to access type specific properties like the artist.
    \endtable

*/

/*!
    \enum QIviPlayQueue::Roles
    \value NameRole
          The name of the playable item. E.g. The track name or the name of the web-stream.
    \value TypeRole
          The type of the playable item. E.g. \e "track" or \e "web-stream"
    \value ItemRole
          The playable item instance. This can be used to access type specific properties like the artist.
*/

/*!
    \enum QIviPlayQueue::LoadingType
    \value FetchMore
           This is the default and can be used if you don't know the final size of the list (e.g. a infinite list).
           The list will detect that it is near the end (fetchMoreThreshold) and then fetch the next chunk of data using canFetchMore and fetchMore.
           The drawback of this method is that, because the final size of the data is not known, you can't display a dynamic scroll-bar indicator which is resized depending on the content of the list.
           The other problem could be fast scrolling, as the data might not come in-time and scrolling stops. This can be tweaked by the fetchMoreThreshold property.

    \value DataChanged
           For this loading type you need to know how many items are in the list, as dummy items are created and the user can already start scrolling even though the data is not yet ready to be displayed.
           Similar to FetchMore the data is also loaded in chunks. You can safely use a scroll indicator here.
           The delegate needs to support this approach, as it doesn't have a content when it's first created.
*/

/*!
    \qmltype PlayQueue
    \instantiates QIviPlayQueue
    \inqmlmodule QtIvi.Media
    \inherits QAbstractListModel
    \brief Provides a play queue for the MediaPlayer

    The PlayQueue is a model which is used by the MediaPlayer to control the
    play order of PlayableItems.

    It provides mechanisms for adding new items and managing the existing ones by removing
    or moving them around.

    The PlayQueue can't be instantiated by its own and can only be retrieved through the MediaPlayer.
*/


QIviPlayQueue::~QIviPlayQueue()
{
}

/*!
    \qmlproperty int PlayQueue::currentIndex
    \brief Holds the index of the currently active track.

    Use the get() method to retrieve more information about the active track.
*/

/*!
    \property QIviPlayQueue::currentIndex
    \brief Holds the index of the currently active track.

    Use the get() method to retrieve more information about the active track.
*/
int QIviPlayQueue::currentIndex() const
{
    Q_D(const QIviPlayQueue);
    return d->m_currentIndex;
}

void QIviPlayQueue::setCurrentIndex(int currentIndex)
{
    Q_IVI_BACKEND(QIviPlayQueue, d->playerBackend(), "Can't set the current index without a connected backend");

    backend->setCurrentIndex(currentIndex);
}

/*!
    \qmlproperty int PlayQueue::chunkSize
    \brief Holds the number of rows which are requested from the backend interface.

    This property can be used to fine tune the loading performance.

    Bigger chunks means less calls to the backend and to a potential IPC underneath, but more data
    to be transferred and probably longer waiting time until the request was finished.
*/

/*!
    \property QIviPlayQueue::chunkSize
    \brief Holds the number of rows which are requested from the backend interface.

    This property can be used to fine tune the loading performance.

    Bigger chunks means less calls to the backend and to a potential IPC underneath, but more data
    to be transferred and probably longer waiting time until the request was finished.
*/
int QIviPlayQueue::chunkSize() const
{
    Q_D(const QIviPlayQueue);
    return d->m_chunkSize;
}

void QIviPlayQueue::setChunkSize(int chunkSize)
{
    Q_D(QIviPlayQueue);
    if (d->m_chunkSize == chunkSize)
        return;

    d->m_chunkSize = chunkSize;
    emit chunkSizeChanged(chunkSize);
}

/*!
    \qmlproperty int PlayQueue::fetchMoreThreshold
    \brief Holds the row delta to the end before the next chunk is loaded

    This property can be used to fine tune the loading performance. When the
    threshold is reached the next chunk of rows are requested from the backend. How many rows are fetched
    can be defined by using the chunkSize property.

    The threshold defines the number of rows before the cached rows ends.

    \note This property is only used when loadingType is set to FetchMore.
*/

/*!
    \property QIviPlayQueue::fetchMoreThreshold
    \brief Holds the row delta to the end before the next chunk is loaded

    This property can be used to fine tune the loading performance. When the
    threshold is reached the next chunk of rows are requested from the backend. How many rows are fetched
    can be defined by using the chunkSize property.

    The threshold defines the number of rows before the cached rows ends.

    \note This property is only used when loadingType is set to FetchMore.
*/
int QIviPlayQueue::fetchMoreThreshold() const
{
    Q_D(const QIviPlayQueue);
    return d->m_fetchMoreThreshold;
}

void QIviPlayQueue::setFetchMoreThreshold(int fetchMoreThreshold)
{
    Q_D(QIviPlayQueue);
    if (d->m_fetchMoreThreshold == fetchMoreThreshold)
        return;

    d->m_fetchMoreThreshold = fetchMoreThreshold;
    emit fetchMoreThresholdChanged(fetchMoreThreshold);
}

/*!
    \qmlproperty enumeration PlayQueue::loadingType
    \brief Holds the currently used loading type used for loading the data.

    \note When changing this property the content will be reset.
*/

/*!
    \property QIviPlayQueue::loadingType
    \brief Holds the currently used loading type used for loading the data.

    \note When changing this property the content will be reset.
*/
QIviPlayQueue::LoadingType QIviPlayQueue::loadingType() const
{
    Q_D(const QIviPlayQueue);
    return d->m_loadingType;
}

void QIviPlayQueue::setLoadingType(QIviPlayQueue::LoadingType loadingType)
{
    Q_D(QIviPlayQueue);
    if (d->m_loadingType == loadingType)
        return;

    if (loadingType == QIviPlayQueue::DataChanged && !d->playerBackend()->canReportCount()) {
        qWarning("The backend doesn't support the DataChanged loading type. This call will have no effect");
        return;
    }

    d->m_loadingType = loadingType;
    emit loadingTypeChanged(loadingType);

    d->resetModel();
}

/*!
    \qmlproperty int PlayQueue::count
    \brief Holds the current number of rows in this model.
*/
/*!
    \property QIviPlayQueue::count
    \brief Holds the current number of rows in this model.
*/
int QIviPlayQueue::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIviPlayQueue);
    if (parent.isValid())
        return 0;

    return d->m_itemList.count();
}

/*!
    \reimp
*/
QVariant QIviPlayQueue::data(const QModelIndex &index, int role) const
{
    Q_D(const QIviPlayQueue);
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= d->m_itemList.count() || row < 0)
        return QVariant();

    if (row >= d->m_fetchedDataCount - d->m_fetchMoreThreshold && canFetchMore(QModelIndex()))
        emit fetchMoreThresholdReached();

    const QIviSearchAndBrowseModelItem *item = d->itemAt(row);
    if (!item)
        return QVariant();

    switch (role) {
        case NameRole: return item->name();
        case TypeRole: return item->type();
        case ItemRole: return d->m_itemList.at(row);
    }

    return QVariant();
}

/*!
    \fn T QIviPlayQueue::at(int i) const

    Returns the item at index \a i converted to the template type T.
*/
/*!
    \qmlmethod object PlayQueue::get(i)

    Returns the item at index \a i.
*/
/*!
    Returns the item at index \a i.

    This function is intended to be used from QML. For C++
    please use the at() instead.
*/
QVariant QIviPlayQueue::get(int i) const
{
    return data(index(i,0), ItemRole);
}

/*!
    \qmlmethod PlayQueue::insert(int index, PlayableItem item)

    Insert the \a item at the position \a index.

    If the backend doesn't accept the provided item, this operation will end in a no op.
*/

/*!
    \fn void QIviPlayQueue::insert(int index, const QVariant &variant)

    Insert the \a variant at the position \a index.

    If the backend doesn't accept the provided item, this operation will end in a no op.
*/
void QIviPlayQueue::insert(int index, const QVariant &variant)
{
    const QIviPlayableItem *item = qtivi_gadgetFromVariant<QIviPlayableItem>(variant);
    if (!item)
        return;

    Q_IVI_BACKEND(QIviPlayQueue, d->playerBackend(), "Can't insert itmes without a connected backend");

    backend->insert(index, item);
}

/*!
    \qmlmethod PlayQueue::remove(int index)

    Removes the item at position \a index from the play queue.
*/

/*!
    \fn void QIviPlayQueue::remove(int index)

    Removes the item at position \a index from the play queue.
*/
void QIviPlayQueue::remove(int index)
{
    Q_IVI_BACKEND(QIviPlayQueue, d->playerBackend(), "Can't remove itmes without a connected backend");

    backend->remove(index);
}

/*!
    \qmlmethod PlayQueue::move(int cur_index, int new_index)

    Moves the item at position \a cur_index to the new position \a new_index the play queue.
*/

/*!
    \fn void QIviPlayQueue::move(int cur_index, int new_index)

    Moves the item at position \a cur_index to the new position \a new_index the play queue.
*/
void QIviPlayQueue::move(int cur_index, int new_index)
{
    Q_IVI_BACKEND(QIviPlayQueue, d->playerBackend(), "Can't move itmes without a connected backend");

    backend->move(cur_index, new_index);
}

/*!
    \reimp
*/
bool QIviPlayQueue::canFetchMore(const QModelIndex &parent) const
{
    Q_D(const QIviPlayQueue);
    if (parent.isValid())
        return false;

    return d->m_moreAvailable;
}

/*!
    \reimp
*/
void QIviPlayQueue::fetchMore(const QModelIndex &parent)
{
    Q_D(QIviPlayQueue);
    if (parent.isValid())
        return;

    if (!d->playerBackend())
        return;

    d->m_moreAvailable = false;
    d->playerBackend()->fetchData(d->m_fetchedDataCount, d->m_chunkSize);
}

/*!
    \reimp
*/
QHash<int, QByteArray> QIviPlayQueue::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[ItemRole] = "item";
    }
    return roles;
}

/*!
    Creates a play queue for the QIviMediaPlayer instance \a parent.
*/
QIviPlayQueue::QIviPlayQueue(QIviMediaPlayer *parent)
    : QAbstractListModel(*new QIviPlayQueuePrivate(parent, this), parent)
{
    Q_D(QIviPlayQueue);
    d->initialize();
}

/*!
    \fn void QIviPlayQueue::fetchMoreThresholdReached() const

    This signal is emitted whenever the fetchMoreThreshold is reached and new data is requested from the backend.
*/

/*!
    \qmlsignal PlayQueue::fetchMoreThresholdReached()

    This signal is emitted whenever the fetchMoreThreshold is reached and new data is requested from the backend.
*/

QT_END_NAMESPACE

#include "moc_qiviplayqueue.cpp"
