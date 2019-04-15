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

#ifndef QIVISEARCHMODELINTERFACE_H
#define QIVISEARCHMODELINTERFACE_H

#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>
#include <QSet>
#include <QUuid>
#include <QtIviCore/QIviAbstractQueryTerm>
#include <QtIviCore/QtIviCoreModule>
#include <QtIviCore/QIviPagingModelInterface>
#include <QtIviCore/QIviSearchAndBrowseModel>
#include <QtIviCore/QIviStandardItem>
#include <QtIviCore/qiviqmlconversion_helper.h>

QT_BEGIN_NAMESPACE

class QIviSearchAndBrowseModelInterfacePrivate;

class Q_QTIVICORE_EXPORT QIviSearchAndBrowseModelInterface : public QIviPagingModelInterface
{
    Q_OBJECT

public:
    explicit QIviSearchAndBrowseModelInterface(QObject *parent = nullptr);

    virtual void setContentType(const QUuid &identifier, const QString &contentType) = 0;
    virtual void setupFilter(const QUuid &identifier, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms) = 0;

    virtual QIviPendingReply<QString> goBack(const QUuid &identifier) = 0;
    virtual QIviPendingReply<QString> goForward(const QUuid &identifier, int index) = 0;

    virtual QIviPendingReply<void> insert(const QUuid &identifier, int index, const QVariant &item) = 0;
    virtual QIviPendingReply<void> remove(const QUuid &identifier, int index) = 0;
    virtual QIviPendingReply<void> move(const QUuid &identifier, int currentIndex, int newIndex) = 0;
    virtual QIviPendingReply<int> indexOf(const QUuid &identifier, const QVariant &item) = 0;

Q_SIGNALS:
    void canGoForwardChanged(const QUuid &identifier, const QVector<bool> &indexes, int start);
    void canGoBackChanged(const QUuid &identifier, bool canGoBack);
    //does this really make sense ?
    void contentTypeChanged(const QUuid &identifier, const QString &contentType);
    void availableContentTypesChanged(const QStringList &availableContentTypes);
    void queryIdentifiersChanged(const QUuid &identifier, const QSet<QString> &queryIdentifiers);

protected:
    template <typename T>
    QSet<QString> identifiersFromItem() {
        return identifiersFromItem(T::staticMetaObject);
    }
    QSet<QString> identifiersFromItem(const QMetaObject &object);

private:
    Q_DECLARE_PRIVATE(QIviSearchAndBrowseModelInterface)
};

#define QIviSearchAndBrowseModel_iid "org.qt-project.qtivi.SearchAndBrowseModel/1.0"

QT_END_NAMESPACE

#endif // QIVISEARCHMODELINTERFACE_H
