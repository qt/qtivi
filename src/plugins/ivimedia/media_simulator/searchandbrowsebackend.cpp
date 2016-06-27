/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#include <QSqlQuery>
#include <QSqlError>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QtDebug>

SearchAndBrowseBackend::SearchAndBrowseBackend(const QSqlDatabase &database, QObject *parent)
    : QIviSearchAndBrowseModelInterface(parent)
{
    qRegisterMetaType<SearchAndBrowseItem>();
    registerContentType<SearchAndBrowseItem>("artist");
    registerContentType<SearchAndBrowseItem>("album");
    registerContentType<QIviAudioTrackItem>("track");

    m_db = database;
    m_db.open();
}


QIviSearchAndBrowseModelInterface::Flags SearchAndBrowseBackend::supportedFlags() const
{
    return QIviSearchAndBrowseModelInterface::Flags(QIviSearchAndBrowseModelInterface::SupportsFiltering |
           QIviSearchAndBrowseModelInterface::SupportsSorting |
           QIviSearchAndBrowseModelInterface::SupportsAndConjunction |
           QIviSearchAndBrowseModelInterface::SupportsOrConjunction |
           QIviSearchAndBrowseModelInterface::SupportsStatelessNavigation |
           QIviSearchAndBrowseModelInterface::SupportsGetSize);
}

void SearchAndBrowseBackend::fetchData(const QUuid &identifier, const QString &type, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms, int start, int count)
{
    qDebug() << "FETCH" << identifier << type << start << count;

    //Determine the current type and which items got selected previously to define the base filter.
    QStringList where_clauses;
    QStringList types = type.split('/');
    for (const QString &filter_type : types) {
        QStringList parts = filter_type.split('?');
        if (parts.count() != 2)
            continue;

        QString filter = QString::fromUtf8(QByteArray::fromBase64(parts.at(1).toUtf8(), QByteArray::Base64UrlEncoding));
        where_clauses.append(QString(QLatin1String("%1 = \"%2\"")).arg(mapIdentifiers(parts.at(0), "name"), filter));
    }
    QString current_type = types.last();

    QString order;
    if (!orderTerms.isEmpty())
        order = QString(QLatin1String("ORDER BY %1")).arg(createSortOrder(current_type, orderTerms));

    QString columns;
    if (current_type == QLatin1String("artist"))
        columns = QLatin1String("DISTINCT artistName");
    else if (current_type == QLatin1String("album"))
        columns = QLatin1String("DISTINCT artistName, albumName");
    else
        columns = QLatin1String("artistName, albumName, trackName, genre, number, file, id");

    QString filterClause = createWhereClause(current_type, term);
    if (!filterClause.isEmpty())
        where_clauses.append(filterClause);

    QString whereClause = where_clauses.join(" AND ");

    QString countQuery = QString(QLatin1String("SELECT count() FROM (SELECT %1 FROM track %2)"))
            .arg(columns)
            .arg(whereClause.isEmpty() ? QString() : QLatin1String("WHERE ") + whereClause);

    QSqlQuery query(m_db);

    qDebug() << countQuery;
    if (query.exec(countQuery)) {
        while (query.next()) {
            emit countChanged(identifier, query.value(0).toInt());
        }
    } else {
        qDebug() << query.lastError().text();
    }

    QString queryString = QString(QLatin1String("SELECT %1 FROM track %2 %3 LIMIT %4, %5"))
            .arg(columns)
            .arg(whereClause.isEmpty() ? QString() : QLatin1String("WHERE ") + whereClause)
            .arg(order)
            .arg(start)
            .arg(count);

    QtConcurrent::run(this,
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

            if (type == QLatin1String("track")) {
                QIviAudioTrackItem item;
                item.setId(query.value(6).toString());
                item.setTitle(query.value(2).toString());
                item.setArtist(artist);
                item.setAlbum(album);
                item.setUrl(QUrl::fromLocalFile(query.value(5).toString()));
                list.append(QVariant::fromValue(item));
            } else {
                SearchAndBrowseItem item;
                item.setType(type);
                if (type == QLatin1String("artist")) {
                    item.setName(artist);
                } else if (type == QLatin1String("album")) {
                    item.setName(album);
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
        qDebug() << query.lastError().text();
    }

    emit dataFetched(identifier, list, start, list.count() >= count);
}

QString SearchAndBrowseBackend::createSortOrder(const QString &type, const QList<QIviOrderTerm> &orderTerms)
{
    QStringList order;
    int i = 0;
    for (const QIviOrderTerm & term : orderTerms) {
        if (i)
            order.append(",");

        order.append(mapIdentifiers(type, term.propertyName()));
        if (term.isAscending())
            order.append("ASC");
        else
            order.append("DESC");

        i++;
    }

    return order.join(' ');
}

QString SearchAndBrowseBackend::mapIdentifiers(const QString &type, const QString &identifer)
{
    if (identifer == QLatin1String("name")) {
        if (type == QLatin1String("artist"))
            return QLatin1String("artistName");
        else if (type == QLatin1String("album"))
            return QLatin1String("albumName");
        else if (type == QLatin1String("track"))
            return QLatin1String("trackName");
    }

    return identifer;
}

QString SearchAndBrowseBackend::createWhereClause(const QString &type, QIviAbstractQueryTerm *term)
{
    if (!term)
        return QString();

    switch (term->type()) {
    case QIviAbstractQueryTerm::ScopeTerm: {
        QIviScopeTerm *scope = static_cast<QIviScopeTerm*>(term);
        return QString(QLatin1String("%1 (%2)")).arg(scope->isNegated() ? "NOT" : "",createWhereClause(type, scope->term()));
    }
    case QIviAbstractQueryTerm::ConjunctionTerm: {
        QIviConjunctionTerm *conjunctionTerm = static_cast<QIviConjunctionTerm*>(term);
        QString conjunction = QLatin1String("AND");
        if (conjunctionTerm->conjunction() == QIviConjunctionTerm::Or)
            conjunction = QLatin1String("OR");

        QString string;
        QListIterator<QIviAbstractQueryTerm*> it(conjunctionTerm->terms());
        while (it.hasNext()) {
            string += createWhereClause(type, it.next());
            if (it.hasNext())
                string += QLatin1Literal(" ") + conjunction + QLatin1Literal(" ");
        }
        return string;
    }
    case QIviAbstractQueryTerm::FilterTerm: {
        QIviFilterTerm *filter = static_cast<QIviFilterTerm*>(term);
        QString operatorString;
        bool negated = filter->isNegated();
        QString value;
        if (filter->value().type() == QVariant::String)
            value = QString(QLatin1String("'%1'")).arg(filter->value().toString().replace('*', '%'));
        else
            value = filter->value().toString();

        switch (filter->operatorType()){
            case QIviFilterTerm::Equals: operatorString = QLatin1String("="); break;
            case QIviFilterTerm::EqualsCaseInsensitive: operatorString = QLatin1String("LIKE"); break;
            case QIviFilterTerm::Unequals: operatorString = QLatin1String("="); negated = !negated; break;
            case QIviFilterTerm::GreaterThan: operatorString = QLatin1String(">"); break;
            case QIviFilterTerm::GreaterEquals: operatorString = QLatin1String(">="); break;
            case QIviFilterTerm::LowerThan: operatorString = QLatin1String("<"); break;
            case QIviFilterTerm::LowerEquals: operatorString = QLatin1String("<="); break;
        }

        QStringList clause;
        if (negated)
            clause.append(QLatin1String("NOT"));
        clause.append(mapIdentifiers(type, filter->propertyName()));
        clause.append(operatorString);
        clause.append(value);

        return clause.join(" ");
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
    QString new_type = type + QString(QLatin1String("?%1")).arg(QLatin1String(itemId.toUtf8().toBase64(QByteArray::Base64UrlEncoding)));

    if (current_type == "artist")
        new_type += QLatin1String("/album");
    else if (current_type == "album")
        new_type += QLatin1String("/track");
    else
        return QString();

    return new_type;
}
