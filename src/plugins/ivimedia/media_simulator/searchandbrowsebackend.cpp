/****************************************************************************
**
** Copyright (C) 2018 Pelagicore AG
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

#include "searchandbrowsebackend.h"
#include "logging.h"

#include <QtConcurrent/QtConcurrent>

#include <QFuture>
#include <QSqlError>
#include <QSqlQuery>
#include <QtDebug>

static const QString artistLiteral = QStringLiteral("artist");
static const QString albumLiteral = QStringLiteral("album");
static const QString trackLiteral = QStringLiteral("track");

SearchAndBrowseBackend::SearchAndBrowseBackend(const QSqlDatabase &database, QObject *parent)
    : QIviSearchAndBrowseModelInterface(parent)
    , m_threadPool(new QThreadPool(this))
{
    m_threadPool->setMaxThreadCount(1);

    qRegisterMetaType<SearchAndBrowseItem>();
    registerContentType<SearchAndBrowseItem>(artistLiteral);
    registerContentType<SearchAndBrowseItem>(albumLiteral);
    registerContentType<QIviAudioTrackItem>(trackLiteral);

    m_db = database;
    m_db.open();
}

void SearchAndBrowseBackend::initialize()
{
    emit initializationDone();
}

//TODO fix the growing state map in one of the following commits
void SearchAndBrowseBackend::setContentType(const QUuid &identifier, const QString &contentType)
{
    auto &state = m_state[identifier];
    state.contentType = contentType;
}

void SearchAndBrowseBackend::setupFilter(const QUuid &identifier, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms)
{
    auto &state = m_state[identifier];
    state.queryTerm = term;
    state.orderTerms = orderTerms;
}

void SearchAndBrowseBackend::fetchData(const QUuid &identifier, int start, int count)
{
    emit supportedCapabilitiesChanged(identifier, QtIviCoreModule::ModelCapabilities(
                                          QtIviCoreModule::SupportsFiltering |
                                          QtIviCoreModule::SupportsSorting |
                                          QtIviCoreModule::SupportsAndConjunction |
                                          QtIviCoreModule::SupportsOrConjunction |
                                          QtIviCoreModule::SupportsStatelessNavigation |
                                          QtIviCoreModule::SupportsGetSize
                                          ));

    if (!m_state.contains(identifier)) {
        qCCritical(media) << "INTERNAL ERROR: No state available for this uuid";
        return;
    }
    auto state = m_state[identifier];

    qCDebug(media) << "FETCH" << identifier << state.contentType << start << count;

    //Determine the current type and which items got selected previously to define the base filter.
    QStringList where_clauses;
    QStringList types = state.contentType.split('/');
    for (const QString &filter_type : types) {
        QStringList parts = filter_type.split('?');
        if (parts.count() != 2)
            continue;

        QString filter = QString::fromUtf8(QByteArray::fromBase64(parts.at(1).toUtf8(), QByteArray::Base64UrlEncoding));
        where_clauses.append(QStringLiteral("%1 = \"%2\"").arg(mapIdentifiers(parts.at(0), QStringLiteral("name")), filter));
    }
    QString current_type = types.last();

    QString order;
    if (!state.orderTerms.isEmpty())
        order = QStringLiteral("ORDER BY %1").arg(createSortOrder(current_type, state.orderTerms));

    QString columns;
    QString groupBy;
    if (current_type == artistLiteral) {
        columns = QStringLiteral("artistName, coverArtUrl");
        groupBy = QStringLiteral("artistName");
    } else if (current_type == albumLiteral) {
        columns = QStringLiteral("artistName, albumName, coverArtUrl");
        groupBy = QStringLiteral("artistName, albumName");
    } else {
        columns = QStringLiteral("artistName, albumName, trackName, genre, number, file, id, coverArtUrl");
    }

    QString filterClause = createWhereClause(current_type, state.queryTerm);
    if (!filterClause.isEmpty())
        where_clauses.append(filterClause);

    QString whereClause = where_clauses.join(" AND ");

    QString countQuery = QStringLiteral("SELECT count() FROM (SELECT %1 FROM track %2 %3)")
            .arg(columns,
                 whereClause.isEmpty() ? QString() : QStringLiteral("WHERE ") + whereClause,
                 groupBy.isEmpty() ? QString() : QStringLiteral("GROUP BY ") + groupBy);

    QtConcurrent::run(m_threadPool, [this, countQuery, identifier]() {
        QSqlQuery query(m_db);
        if (query.exec(countQuery)) {
            while (query.next()) {
                emit countChanged(identifier, query.value(0).toInt());
            }
        } else {
            sqlError(this, query.lastQuery(), query.lastError().text());
        }
    });

    QString queryString = QStringLiteral("SELECT %1 FROM track %2 %3 %4 LIMIT %5, %6")
            .arg(columns,
            whereClause.isEmpty() ? QString() : QStringLiteral("WHERE ") + whereClause,
            order,
            groupBy.isEmpty() ? QString() : QStringLiteral("GROUP BY ") + groupBy,
            QString::number(start),
            QString::number(count));

    QtConcurrent::run(m_threadPool,
                      this,
                      &SearchAndBrowseBackend::search,
                      identifier,
                      queryString,
                      current_type,
                      start,
                      count);
}

void SearchAndBrowseBackend::search(const QUuid &identifier, const QString &queryString, const QString &type, int start, int count)
{
    QVariantList list;
    QSqlQuery query(m_db);

    if (query.exec(queryString)) {
        while (query.next()) {
            QString artist = query.value(0).toString();
            QString album = query.value(1).toString();

            if (type == trackLiteral) {
                QIviAudioTrackItem item;
                item.setId(query.value(6).toString());
                item.setTitle(query.value(2).toString());
                item.setArtist(artist);
                item.setAlbum(album);
                item.setUrl(QUrl::fromLocalFile(query.value(5).toString()));
                item.setCoverArtUrl(QUrl::fromLocalFile(query.value(7).toString()));
                list.append(QVariant::fromValue(item));
            } else {
                SearchAndBrowseItem item;
                item.setType(type);
                if (type == artistLiteral) {
                    item.setName(artist);
                    item.setData(QVariantMap{{"coverArtUrl", QUrl::fromLocalFile(query.value(1).toString())}});
                } else if (type == albumLiteral) {
                    item.setName(album);
                    item.setData(QVariantMap{{"artist", artist},
                                             {"coverArtUrl", QUrl::fromLocalFile(query.value(2).toString())}
                                             });
                }
                list.append(QVariant::fromValue(item));
            }

//            if (type == "artist") {
//                DiskArtistItem* artistItem = new DiskArtistItem();
//                artistItem->m_artist = artist;
//                list.append(artistItem);
//            } else if (type == "album") {
//                DiskAlbumItem* albumItem = new DiskAlbumItem();
//                albumItem->m_album = album;
//                albumItem->m_artist = artist;
//                list.append(albumItem);
//            } else if (type == "track") {
//                DiskTrackItem* trackItem = new DiskTrackItem();
//                trackItem->m_artist = artist;
//                trackItem->m_album = album;
//                trackItem->m_track = query.value(2).toString();
//                trackItem->m_genres.append(query.value(3).toString());
//                trackItem->m_number = query.value(4).toUInt();
//                trackItem->m_url = QUrl::fromLocalFile(query.value(5).toString()).toString();
//                list.append(trackItem);
//            }
        }
    } else {
        qCWarning(media) << query.lastError().text();
    }

    emit dataFetched(identifier, list, start, list.count() >= count);
}

QString SearchAndBrowseBackend::createSortOrder(const QString &type, const QList<QIviOrderTerm> &orderTerms)
{
    QStringList order;
    int i = 0;
    for (const QIviOrderTerm & term : orderTerms) {
        if (i)
            order.append(QStringLiteral(","));

        order.append(mapIdentifiers(type, term.propertyName()));
        if (term.isAscending())
            order.append(QStringLiteral("ASC"));
        else
            order.append(QStringLiteral("DESC"));

        i++;
    }

    return order.join(' ');
}

QString SearchAndBrowseBackend::mapIdentifiers(const QString &type, const QString &identifer)
{
    if (identifer == QLatin1String("name")) {
        if (type == artistLiteral)
            return QStringLiteral("artistName");
        else if (type == albumLiteral)
            return QStringLiteral("albumName");
        else if (type == trackLiteral)
            return QStringLiteral("trackName");
    }

    return identifer;
}

QString SearchAndBrowseBackend::createWhereClause(const QString &type, QIviAbstractQueryTerm *term)
{
    if (!term)
        return QString();

    switch (term->type()) {
    case QIviAbstractQueryTerm::ScopeTerm: {
        auto *scope = static_cast<QIviScopeTerm*>(term);
        return QStringLiteral("%1 (%2)").arg(scope->isNegated() ? QStringLiteral("NOT") : QStringLiteral(""),createWhereClause(type, scope->term()));
    }
    case QIviAbstractQueryTerm::ConjunctionTerm: {
        auto *conjunctionTerm = static_cast<QIviConjunctionTerm*>(term);
        QString conjunction = QStringLiteral("AND");
        if (conjunctionTerm->conjunction() == QIviConjunctionTerm::Or)
            conjunction = QStringLiteral("OR");

        QString string;
        QListIterator<QIviAbstractQueryTerm*> it(conjunctionTerm->terms());
        while (it.hasNext()) {
            string += createWhereClause(type, it.next());
            if (it.hasNext())
                string += QStringLiteral(" ") + conjunction + QStringLiteral(" ");
        }
        return string;
    }
    case QIviAbstractQueryTerm::FilterTerm: {
        auto *filter = static_cast<QIviFilterTerm*>(term);
        QString operatorString;
        bool negated = filter->isNegated();
        QString value;
        if (filter->value().type() == QVariant::String)
            value = QStringLiteral("'%1'").arg(filter->value().toString().replace('*', '%'));
        else
            value = filter->value().toString();

        switch (filter->operatorType()){
            case QIviFilterTerm::Equals: operatorString = QStringLiteral("="); break;
            case QIviFilterTerm::EqualsCaseInsensitive: operatorString = QStringLiteral("LIKE"); break;
            case QIviFilterTerm::Unequals: operatorString = QStringLiteral("="); negated = !negated; break;
            case QIviFilterTerm::GreaterThan: operatorString = QStringLiteral(">"); break;
            case QIviFilterTerm::GreaterEquals: operatorString = QStringLiteral(">="); break;
            case QIviFilterTerm::LowerThan: operatorString = QStringLiteral("<"); break;
            case QIviFilterTerm::LowerEquals: operatorString = QStringLiteral("<="); break;
        }

        QStringList clause;
        if (negated)
            clause.append(QStringLiteral("NOT"));
        clause.append(mapIdentifiers(type, filter->propertyName()));
        clause.append(operatorString);
        clause.append(value);

        return clause.join(QStringLiteral(" "));
    }
    }

    return QString();
}

bool SearchAndBrowseBackend::canGoBack(const QUuid &identifier, const QString &type)
{
    Q_UNUSED(identifier)
    return type.split('/').count() >= 2;
}

QString SearchAndBrowseBackend::goBack(const QUuid &identifier, const QString &type)
{
    Q_UNUSED(identifier)
    QStringList types = type.split('/');

    if (types.count() < 2)
        return QString();

    types.removeLast();
    types.replace(types.count() - 1, types.at(types.count() - 1).split('?').at(0));

    return types.join('/');
}

bool SearchAndBrowseBackend::canGoForward(const QUuid &identifier, const QString &type, const QString &itemId)
{
    return !goForward(identifier, type, itemId).isEmpty();
}

QString SearchAndBrowseBackend::goForward(const QUuid &identifier, const QString &type, const QString &itemId)
{
    Q_UNUSED(identifier)
    QStringList types = type.split('/');
    QString current_type = types.last();
    QString new_type = type + QStringLiteral("?%1").arg(QLatin1String(itemId.toUtf8().toBase64(QByteArray::Base64UrlEncoding)));

    if (current_type == artistLiteral)
        new_type += QLatin1String("/album");
    else if (current_type == albumLiteral)
        new_type += QLatin1String("/track");
    else
        return QString();

    return new_type;
}

QIviPendingReply<void> SearchAndBrowseBackend::insert(const QUuid &identifier, const QString &type, int index, const QIviSearchAndBrowseModelItem *item)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(index)
    Q_UNUSED(item)

    return QIviPendingReply<void>::createFailedReply();
}

QIviPendingReply<void> SearchAndBrowseBackend::remove(const QUuid &identifier, const QString &type, int index)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(index)

    return QIviPendingReply<void>::createFailedReply();
}

QIviPendingReply<void> SearchAndBrowseBackend::move(const QUuid &identifier, const QString &type, int currentIndex, int newIndex)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(currentIndex)
    Q_UNUSED(newIndex)

    return QIviPendingReply<void>::createFailedReply();
}

QIviPendingReply<int> SearchAndBrowseBackend::indexOf(const QUuid &identifier, const QString &type, const QIviSearchAndBrowseModelItem *item)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(item)

    return QIviPendingReply<int>::createFailedReply();
}
