/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
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

#ifndef QIVISEARCHANDBROWSEMODELQTROADAPTER_H
#define QIVISEARCHANDBROWSEMODELQTROADAPTER_H

#include <QIviRemoteObjectSourceHelper>

#include "searchandbrowsebackend.h"
#include "rep_qivisearchandbrowsemodel_source.h"

template <class ObjectType>
struct QIviSearchAndBrowseModelAddressWrapper: public QIviSearchAndBrowseModelSourceAPI<ObjectType> {
    QIviSearchAndBrowseModelAddressWrapper(ObjectType *object)
        : QIviSearchAndBrowseModelSourceAPI<ObjectType>(object, object->remoteObjectsLookupName())
    {}
};

class QIviSearchAndBrowseModelQtRoAdapter : public QIviSearchAndBrowseModelSource
{
public:
    QIviSearchAndBrowseModelQtRoAdapter(QIviSearchAndBrowseModelInterface *parent);
    QIviSearchAndBrowseModelQtRoAdapter(const QString& remoteObjectsLookupName, QIviSearchAndBrowseModelInterface *parent);

    QString remoteObjectsLookupName() const;
    QStringList availableContentTypes() const override;

public Q_SLOTS:
    void setContentType(const QUuid &identifier, const QString &contentType) override;
    void setupFilter(const QUuid &identifier, const QVariant &term, const QList<QIviOrderTerm> &orderTerms) override;
    QVariant goBack(const QUuid &identifier) override;
    QVariant goForward(const QUuid &identifier, int index) override;
    QVariant insert(const QUuid &identifier, int index, const QVariant &item) override;
    QVariant remove(const QUuid &identifier, int index) override;
    QVariant move(const QUuid &identifier, int currentIndex, int newIndex) override;
    QVariant indexOf(const QUuid &identifier, const QVariant &item) override;

    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;
    void fetchData(const QUuid &identifier, int start, int count) override;

private:
    QString m_remoteObjectsLookupName;
    QIviSearchAndBrowseModelInterface *m_backend;
    QIviRemoteObjectSourceHelper<QIviSearchAndBrowseModelQtRoAdapter> m_helper;
};

#endif // QIVISEARCHANDBROWSEMODELQTROADAPTER_H
