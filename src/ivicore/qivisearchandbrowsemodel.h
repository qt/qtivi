/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
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

#ifndef QIVISEARCHANDBROWSEMODEL_H
#define QIVISEARCHANDBROWSEMODEL_H

#include <QtIviCore/QIviPagingModel>
#include <QtIviCore/QtIviCoreModule>
#include <QtIviCore/QIviPendingReply>
#include <QtIviCore/QIviServiceObject>
#include <QtQml/QJSValue>

QT_BEGIN_NAMESPACE

class QIviSearchAndBrowseModelPrivate;

class Q_QTIVICORE_EXPORT QIviSearchAndBrowseModel : public QIviPagingModel
{
    Q_OBJECT

    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QString contentType READ contentType WRITE setContentType NOTIFY contentTypeChanged)
    Q_PROPERTY(QStringList availableContentTypes READ availableContentTypes NOTIFY availableContentTypesChanged)
    Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY canGoBackChanged)

public:

    enum Roles {
        CanGoForwardRole = QIviPagingModel::LastRole + 1,
        LastRole = CanGoForwardRole
    };

    //TODO fix naming
    enum NavigationType {
        InModelNavigation,
        OutOfModelNavigation
    };
    Q_ENUM(NavigationType)

    explicit QIviSearchAndBrowseModel(QObject *parent = nullptr);

    QString query() const;
    void setQuery(const QString &query);

    QString contentType() const;
    void setContentType(const QString &contentType);

    QStringList availableContentTypes() const;

    bool canGoBack() const;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void goBack();
    Q_INVOKABLE bool canGoForward(int index) const;
    Q_INVOKABLE QIviSearchAndBrowseModel *goForward(int index, QIviSearchAndBrowseModel::NavigationType navigationType);
    Q_INVOKABLE QIviPendingReply<void> insert(int index, const QVariant &variant);
    Q_INVOKABLE QIviPendingReply<void> remove(int index);
    Q_INVOKABLE QIviPendingReply<void> move(int cur_index, int new_index);
    Q_INVOKABLE QIviPendingReply<int> indexOf(const QVariant &variant);

Q_SIGNALS:
    void queryChanged(const QString &query);
    void contentTypeChanged(const QString &contentType);
    void availableContentTypesChanged(const QStringList &availableContentTypes);
    void canGoBackChanged(bool canGoBack);

protected:
    QIviSearchAndBrowseModel(QIviServiceObject *serviceObject, QObject *parent = nullptr);
    QIviSearchAndBrowseModel(QIviSearchAndBrowseModelPrivate &dd, QObject *parent);
    void connectToServiceObject(QIviServiceObject *serviceObject) override;
    void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIviSearchAndBrowseModel)
    Q_PRIVATE_SLOT(d_func(), void onCanGoForwardChanged(const QUuid &identifier, const QVector<bool> &indexes, int start))
    Q_PRIVATE_SLOT(d_func(), void onCanGoBackChanged(const QUuid &identifier, bool canGoBack))
    Q_PRIVATE_SLOT(d_func(), void onQueryIdentifiersChanged(const QUuid &identifier, const QSet<QString> &queryIdentifiers))
    Q_PRIVATE_SLOT(d_func(), void onContentTypeChanged(const QUuid &identifier, const QString &contentType))
    Q_PRIVATE_SLOT(d_func(), void onAvailableContentTypesChanged(const QStringList &contentTypes))
};

QT_END_NAMESPACE

#endif // QIVISEARCHANDBROWSEMODEL_H
