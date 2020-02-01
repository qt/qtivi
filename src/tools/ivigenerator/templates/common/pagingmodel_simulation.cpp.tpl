{#
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG.
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB)
# Contact: https://www.qt.io/licensing/
#
# This file is part of the QtIvi module of the Qt Toolkit.
#
# $QT_BEGIN_LICENSE:LGPL-QTAS$
# Commercial License Usage
# Licensees holding valid commercial Qt Automotive Suite licenses may use
# this file in accordance with the commercial license agreement provided
# with the Software or, alternatively, in accordance with the terms
# contained in a written agreement between you and The Qt Company.  For
# licensing terms and conditions see https://www.qt.io/terms-conditions.
# For further information use the contact form at https://www.qt.io/contact-us.
#
# GNU Lesser General Public License Usage
# Alternatively, this file may be used under the terms of the GNU Lesser
# General Public License version 3 as published by the Free Software
# Foundation and appearing in the file LICENSE.LGPL3 included in the
# packaging of this file. Please review the following information to
# ensure the GNU Lesser General Public License version 3 requirements
# will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 2.0 or (at your option) the GNU General
# Public license version 3 or any later version approved by the KDE Free
# Qt Foundation. The licenses are as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
# included in the packaging of this file. Please review the following
# information to ensure the GNU General Public License requirements will
# be met: https://www.gnu.org/licenses/gpl-2.0.html and
# https://www.gnu.org/licenses/gpl-3.0.html.
#
# $QT_END_LICENSE$
#
# SPDX-License-Identifier: LGPL-3.0
#}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
{% if interface_zoned %}
{% set class = 'Zoned{0}ModelBackend'.format(property|upperfirst) %}
{% else %}
{% set class = '{0}ModelBackend'.format(property|upperfirst) %}
{% endif %}

{{class}}::{{class}}(QObject* parent)
    : QIviPagingModelInterface(parent)
{
    qRegisterMetaType<QIviPagingModelInterface*>();
}

/*! \internal */
{{class}}::~{{class}}()
{
}

int {{class}}::count() const
{
    return m_list.count();
}

void {{class}}::initialize()
{
    QIVI_SIMULATION_TRY_CALL({{class}}, "initialize", void);

    Q_EMIT initializationDone();
}

void {{class}}::registerInstance(const QUuid &identifier)
{
    QIVI_SIMULATION_TRY_CALL({{class}}, "registerInstance", void, identifier);

    Q_EMIT countChanged(identifier, m_list.count());
}

void {{class}}::unregisterInstance(const QUuid &identifier)
{
    QIVI_SIMULATION_TRY_CALL({{class}}, "unregisterInstance", void, identifier);
}

void {{class}}::fetchData(const QUuid &identifier, int start, int count)
{
    QIVI_SIMULATION_TRY_CALL({{class}}, "fetchData", void, identifier, start, count);

    QVariantList list;
    int max = qMin(start + count, m_list.count());
    for(int i=start; i < max; i++)
        list.append(QVariant::fromValue(m_list.at(i)));

    Q_EMIT dataFetched(identifier, list, start, max <  m_list.count());
}

void {{class}}::insert(int index, const {{property.type.nested}} &item)
{
    m_list.insert(index, item);

    Q_EMIT dataChanged(QUuid(), { QVariant::fromValue(item) }, index, 0);
    Q_EMIT countChanged(QUuid(), m_list.count());
}

void {{class}}::remove(int index)
{
    m_list.removeAt(index);

    Q_EMIT dataChanged(QUuid(), QVariantList(), index, 1);
    Q_EMIT countChanged(QUuid(), m_list.count());
}

void {{class}}::move(int currentIndex, int newIndex)
{
    int min = qMin(currentIndex, newIndex);
    int max = qMax(currentIndex, newIndex);

    m_list.move(currentIndex, newIndex);
    QVariantList variantList;
    for (int i = min; i <= max; i++)
        variantList.append(QVariant::fromValue(m_list.at(i)));

    Q_EMIT dataChanged(QUuid(), variantList, min, max - min + 1);
}

void {{class}}::reset()
{
    Q_EMIT dataChanged(QUuid(), QVariantList(), 0, m_list.count());
    Q_EMIT countChanged(QUuid(), 0);
    m_list.clear();
}

void {{class}}::update(int index, const {{property.type.nested}} &item)
{
    m_list[index] = item;
    Q_EMIT dataChanged(QUuid(), { QVariant::fromValue(item) }, index, 1);
}

{{property.type.nested}} {{class}}::at(int index) const
{
    return m_list.at(index);
}
