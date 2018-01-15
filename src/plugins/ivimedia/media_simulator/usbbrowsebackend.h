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

#ifndef USBBROWSEBACKEND_H
#define USBBROWSEBACKEND_H

#include "searchandbrowsebackend.h"

class UsbBrowseBackend : public QIviSearchAndBrowseModelInterface
{
    Q_OBJECT

public:
    UsbBrowseBackend(const QString &path, QObject *parent = nullptr);

    virtual void initialize() override;
    virtual void fetchData(const QUuid &identifier, const QString &type, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms, int start, int count) override;
    virtual bool canGoBack(const QUuid &identifier, const QString &type) override;
    virtual QString goBack(const QUuid &identifier, const QString &type) override;
    virtual bool canGoForward(const QUuid &identifier, const QString &type, const QString &itemId) override;
    virtual QString goForward(const QUuid &identifier, const QString &type, const QString &itemId) override;

    virtual void insert(const QUuid &identifier, const QString &type, int index, const QIviSearchAndBrowseModelItem *item) override;
    virtual void remove(const QUuid &identifier, const QString &type, int index) override;
    virtual void move(const QUuid &identifier, const QString &type, int currentIndex, int newIndex) override;
    virtual int indexOf(const QUuid &identifier, const QString &type, const QIviSearchAndBrowseModelItem *item) override;

private:
    QString m_rootFolder;
};

#endif // USBBROWSEBACKEND_H
