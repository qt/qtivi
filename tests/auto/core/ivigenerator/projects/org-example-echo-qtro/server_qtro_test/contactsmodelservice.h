/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CONTACTSMODELSERVICE_H
#define CONTACTSMODELSERVICE_H

#include "contact.h"
#include "rep_pagingmodel_source.h"

class ContactsModelService : public PagingModelSimpleSource
{
public:
    ContactsModelService(QObject* parent = nullptr);

    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;

    void fetchData(const QUuid &identifier, int start, int count) override;

public Q_SLOTS:
    void insert(int index, const Contact &item);
    void remove(int index);
    void move(int currentIndex, int newIndex);
    void reset();
    void update(int index, const Contact &item);
    const Contact &at(int index) const;

private:
    QList<Contact> m_list;
};

#endif // CONTACTSMODELSERVICE_H
