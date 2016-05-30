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

#ifndef SEARCHBACKEND_H
#define SEARCHBACKEND_H

#include <QtIviCore/QIviSearchAndBrowseModelInterface>
#include <QtIviCore/QIviSearchAndBrowseModel>
#include <QtIviMedia/QIviAudioTrackItem>
#include <QSqlDatabase>
#include <QStack>

class SearchAndBrowseItem : public QIviSearchAndBrowseListItem
{
    Q_GADGET

public:
    virtual QString id() const { return m_name; }
    virtual QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }
    virtual QString type() const { return m_type; }
    void setType(const QString &type) { m_type = type; }

private:
    QString m_name;
    QString m_type;
};
Q_DECLARE_METATYPE(SearchAndBrowseItem)

class TrackItem : public QIviAudioTrackItem
{
    Q_GADGET

public:
    virtual QString id() const { return m_id; }
    void setId(const QString &id) { m_id = id; }
    virtual QString name() const { return m_title; }
    virtual QString type() const { return "track"; }
    virtual QString title() { return QString(); }
    void setTitle(const QString &title) { m_title = title; }
    virtual QString artist() { return QString(); }
    void setArtist(const QString &artist) { m_artist = artist; }
    virtual QString album() { return QString(); }
    void setAlbum(const QString &album) { m_artist = album; }
    virtual QUrl url() const { return m_url; }
    void setUrl(const QUrl &url) { m_url = url; }

private:
    QString m_id;
    QString m_title;
    QString m_artist;
    QString m_album;
    QUrl m_url;
};
Q_DECLARE_METATYPE(TrackItem)

class SearchAndBrowseBackend : public QIviSearchAndBrowseModelInterface
{
    Q_OBJECT
public:
    explicit SearchAndBrowseBackend(QObject *parent = Q_NULLPTR);

    virtual Flags supportedFlags() const Q_DECL_OVERRIDE;
    virtual void fetchData(const QUuid &identifier, const QString &type, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms, int start, int count) Q_DECL_OVERRIDE;
    virtual bool canGoBack(const QUuid &identifier, const QString &type) Q_DECL_OVERRIDE;
    virtual QString goBack(const QUuid &identifier, const QString &type) Q_DECL_OVERRIDE;
    virtual bool canGoForward(const QUuid &identifier, const QString &type, const QString &itemId) Q_DECL_OVERRIDE;
    virtual QString goForward(const QUuid &identifier, const QString &type, const QString &itemId) Q_DECL_OVERRIDE;

private slots:
    void search(const QUuid &identifier, const QString &queryString, const QString &type, int start, int count);
    QString createSortOrder(const QString &type, const QList<QIviOrderTerm> &orderTerms);
    QString createWhereClause(const QString &type, QIviAbstractQueryTerm *term);
private:
    QString mapIdentifiers(const QString &type, const QString &identifer);

    QSqlDatabase m_db;
};

#endif // SEARCHBACKEND_H
