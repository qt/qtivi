/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
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

#ifndef SEARCHANDBROWSEMODEL_H
#define SEARCHANDBROWSEMODEL_H

#include <QtIviCore/QIviSearchAndBrowseModelInterface>
#include <QtIviMedia/QIviPlayableItem>
#include <QRemoteObjectNode>
#include "rep_qivisearchandbrowsemodel_replica.h"

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

QDataStream &operator<<(QDataStream &stream, const SearchAndBrowseItem &obj);
QDataStream &operator>>(QDataStream &stream, SearchAndBrowseItem &obj);

class SearchAndBrowseModel : public QIviSearchAndBrowseModelInterface
{
    Q_OBJECT
public:
    SearchAndBrowseModel(QRemoteObjectNode *node, QObject *parent = nullptr);

    void initialize() override;
    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;
    void fetchData(const QUuid &identifier, int start, int count) override;

    void setContentType(const QUuid &identifier, const QString &contentType) override;
    void setupFilter(const QUuid &identifier, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms) override;
    QIviPendingReply<QString> goBack(const QUuid &identifier) override;
    QIviPendingReply<QString> goForward(const QUuid &identifier, int index) override;
    QIviPendingReply<void> insert(const QUuid &identifier, int index, const QVariant &item) override;
    QIviPendingReply<void> remove(const QUuid &identifier, int index) override;
    QIviPendingReply<void> move(const QUuid &identifier, int currentIndex, int newIndex) override;
    QIviPendingReply<int> indexOf(const QUuid &identifier, const QVariant &item) override;

public Q_SLOTS:
    void onReplicaStateChanged(QRemoteObjectReplica::State newState,
                        QRemoteObjectReplica::State oldState);
    void onNodeError(QRemoteObjectNode::ErrorCode code);
    void onPendingResultAvailable(quint64 id, bool isSuccess, const QVariant &value);
private:
    QSharedPointer<QIviSearchAndBrowseModelReplica> m_replica;
    QHash<quint64, QIviPendingReplyBase> m_pendingReplies;
};

#endif // SEARCHANDBROWSEMODEL_H
