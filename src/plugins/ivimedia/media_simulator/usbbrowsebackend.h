/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
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
****************************************************************************/

#ifndef USBBROWSEBACKEND_H
#define USBBROWSEBACKEND_H

#include "searchandbrowsebackend.h"

class UsbBrowseBackend : public QIviSearchAndBrowseModelInterface
{
    Q_OBJECT

    Q_PROPERTY(QStringList availableContentTypes READ availableContentTypes CONSTANT)
public:
    UsbBrowseBackend(const QString &path, QObject *parent = nullptr);

    QStringList availableContentTypes() const;

    void initialize() override;
    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;
    void setContentType(const QUuid &identifier, const QString &contentType) override;
    void setupFilter(const QUuid &identifier, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms) override;
    void fetchData(const QUuid &identifier, int start, int count) override;
//    bool canGoBack(const QUuid &identifier, const QString &type) override;
    QIviPendingReply<QString> goBack(const QUuid &identifier) override;
//    bool canGoForward(const QUuid &identifier, const QString &type, const QString &itemId) override;
    QIviPendingReply<QString> goForward(const QUuid &identifier, int index) override;

    QIviPendingReply<void> insert(const QUuid &identifier, int index, const QVariant &item) override;
    QIviPendingReply<void> remove(const QUuid &identifier, int index) override;
    QIviPendingReply<void> move(const QUuid &identifier, int currentIndex, int newIndex) override;
    QIviPendingReply<int> indexOf(const QUuid &identifier, const QVariant &item) override;

private:
    QString m_rootFolder;
    struct State {
        QString contentType;
        QVariantList items;
    };
    QMap<QUuid, State> m_state;
};

#endif // USBBROWSEBACKEND_H
