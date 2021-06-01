/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
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

#include "contactsmodelservice.h"

ContactsModelService::ContactsModelService(QObject* parent)
    : QIviPagingModelSimpleSource(parent)
{
}

void ContactsModelService::registerInstance(const QUuid &identifier)
{
    Q_UNUSED(identifier)
}

void ContactsModelService::unregisterInstance(const QUuid &identifier)
{
    Q_UNUSED(identifier)
}

void ContactsModelService::fetchData(const QUuid &identifier, int start, int count)
{
    QVariantList list;
    int max = qMin(start + count, m_list.count());
    for (int i=start; i < max; i++)
        list.append(QVariant::fromValue(m_list.at(i)));

    emit dataFetched(identifier, list, start, max <  m_list.count());
}

void ContactsModelService::insert(int index, const Contact &item)
{
    m_list.insert(index, item);

    emit dataChanged(QUuid(), { QVariant::fromValue(item) }, index, 0);
}

void ContactsModelService::remove(int index)
{
    m_list.removeAt(index);

    emit dataChanged(QUuid(), QVariantList(), index, 1);
}

void ContactsModelService::move(int currentIndex, int newIndex)
{
    int min = qMin(currentIndex, newIndex);
    int max = qMax(currentIndex, newIndex);

    m_list.move(currentIndex, newIndex);
    QVariantList variantList;
    for (int i = min; i <= max; i++)
        variantList.append(QVariant::fromValue(m_list.at(i)));

    emit dataChanged(QUuid(), variantList, min, max - min + 1);
}

void ContactsModelService::reset()
{
    emit dataChanged(QUuid(), QVariantList(), 0, m_list.count());
    m_list.clear();
}

void ContactsModelService::update(int index, const Contact &item)
{
    m_list[index] = item;
    emit dataChanged(QUuid(), { QVariant::fromValue(item) }, index, 1);
}

const Contact &ContactsModelService::at(int index) const
{
    return m_list.at(index);
}
