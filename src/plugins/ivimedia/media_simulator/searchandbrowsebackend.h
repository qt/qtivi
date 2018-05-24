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

#ifndef SEARCHBACKEND_H
#define SEARCHBACKEND_H

#include <QtIviCore/QIviSearchAndBrowseModel>
#include <QtIviCore/QIviSearchAndBrowseModelInterface>
#include <QtIviMedia/QIviAudioTrackItem>

#include <QSqlDatabase>
#include <QStack>

QT_FORWARD_DECLARE_CLASS(QThreadPool);

class SearchAndBrowseItem : public QIviPlayableItem
{
    Q_GADGET

public:
    QString id() const override { return m_name; }
    QString name() const override { return m_name; }
    void setName(const QString &name) { m_name = name; }
    QString type() const override { return m_type; }
    void setType(const QString &type) { m_type = type; }

private:
    QString m_name;
    QString m_type;
};
Q_DECLARE_METATYPE(SearchAndBrowseItem)

class SearchAndBrowseBackend : public QIviSearchAndBrowseModelInterface
{
    Q_OBJECT
public:
    explicit SearchAndBrowseBackend(const QSqlDatabase &database, QObject *parent = nullptr);

    void initialize() override;
    void setContentType(const QUuid &identifier, const QString &contentType) override;
    void setupFilter(const QUuid &identifier, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms) override;
    void fetchData(const QUuid &identifier, int start, int count) override;
    bool canGoBack(const QUuid &identifier, const QString &type) override;
    QString goBack(const QUuid &identifier, const QString &type) override;
    bool canGoForward(const QUuid &identifier, const QString &type, const QString &itemId) override;
    QString goForward(const QUuid &identifier, const QString &type, const QString &itemId) override;

    QIviPendingReply<void> insert(const QUuid &identifier, const QString &type, int index, const QIviSearchAndBrowseModelItem *item) override;
    QIviPendingReply<void> remove(const QUuid &identifier, const QString &type, int index) override;
    QIviPendingReply<void> move(const QUuid &identifier, const QString &type, int currentIndex, int newIndex) override;
    QIviPendingReply<int> indexOf(const QUuid &identifier, const QString &type, const QIviSearchAndBrowseModelItem *item) override;

private slots:
    void search(const QUuid &identifier, const QString &queryString, const QString &type, int start, int count);
    QString createSortOrder(const QString &type, const QList<QIviOrderTerm> &orderTerms);
    QString createWhereClause(const QString &type, QIviAbstractQueryTerm *term);
private:
    QString mapIdentifiers(const QString &type, const QString &identifer);

    QSqlDatabase m_db;
    QThreadPool *m_threadPool;
    struct State {
        QString contentType;
        QIviAbstractQueryTerm *queryTerm = nullptr;
        QList<QIviOrderTerm> orderTerms;
    };
    QMap<QUuid, State> m_state;
};

#endif // SEARCHBACKEND_H
