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

#include "qiviplayqueue.h"
#include "qiviplayqueue_p.h"
#include "qivimediaplayer.h"

#include <QtDebug>

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

void QIviPlayQueuePrivate::init()
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
        q->dataChanged(q->index(start), q->index(start + items.count() -1));
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
    Q_UNUSED(data)
    Q_UNUSED(start)
    Q_UNUSED(count)

    //TODO Handle add/update/remove here.
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

    return playableItem(var);
}

const QIviPlayableItem *QIviPlayQueuePrivate::playableItem(const QVariant &item) const
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

QIviMediaPlayerBackendInterface *QIviPlayQueuePrivate::playerBackend() const
{
    return m_player->d_func()->playerBackend();
}

QIviPlayQueue::~QIviPlayQueue()
{
}

int QIviPlayQueue::currentIndex() const
{
    Q_D(const QIviPlayQueue);
    return d->m_currentIndex;
}

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

    if (loadingType == QIviPlayQueue::DataChanged && !d->playerBackend()->canReportListCount()) {
        qWarning("The backend doesn't support the DataChanged loading type. This call will have no effect");
        return;
    }

    d->m_loadingType = loadingType;
    emit loadingTypeChanged(loadingType);

    d->resetModel();
}

int QIviPlayQueue::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIviPlayQueue);
    if (parent.isValid())
        return 0;

    return d->m_itemList.count();
}

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

    const QIviSearchAndBrowseListItem *item = d->itemAt(row);
    if (!item)
        return QVariant();

    switch (role) {
        case NameRole: return item->name();
        case TypeRole: return item->type();
        case ItemRole: return d->m_itemList.at(row);
    }

    return QVariant();
}

QVariantMap QIviPlayQueue::get(int i) const
{
    QVariantMap map;
    map[QLatin1String("name")] = data(index(i,0), NameRole);
    map[QLatin1String("type")] = data(index(i,0), TypeRole);
    map[QLatin1String("item")] = data(index(i,0), ItemRole);

    return map;
}

void QIviPlayQueue::insert(int index, const QVariant &variant)
{
    Q_D(QIviPlayQueue);
    const QIviPlayableItem *item = d->playableItem(variant);
    if (!item)
        return;

    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't insert itmes without a connected backend");
        return;
    }

    //TODO should we use qBegin here ? instead of relying on dataChanged signal ?
    backend->insert(index, item);
}

void QIviPlayQueue::remove(int index)
{
    Q_D(QIviPlayQueue);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't remove itmes without a connected backend");
        return;
    }

    //TODO should we use qBegin here ? instead of relying on dataChanged signal ?
    backend->remove(index);
}

void QIviPlayQueue::move(int cur_index, int new_index)
{
    Q_D(QIviPlayQueue);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't move itmes without a connected backend");
        return;
    }

    //TODO should we use qBegin here ? instead of relying on dataChanged signal ?
    backend->move(cur_index, new_index);
}

bool QIviPlayQueue::canFetchMore(const QModelIndex &parent) const
{
    Q_D(const QIviPlayQueue);
    if (parent.isValid())
        return false;

    return d->m_moreAvailable;
}

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

QIviPlayQueue::QIviPlayQueue(QIviMediaPlayer *parent)
    : QAbstractListModel(*new QIviPlayQueuePrivate(parent, this), parent)
{
    Q_D(QIviPlayQueue);
    d->init();
}

#include "moc_qiviplayqueue.cpp"
