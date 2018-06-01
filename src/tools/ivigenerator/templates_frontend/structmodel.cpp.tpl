{#
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
{% set class = '{0}Model'.format(struct) %}
{% include 'generated_comment.cpp.tpl' %}

#include "{{class|lower}}.h"
#include "{{class|lower}}_p.h"

QT_BEGIN_NAMESPACE

/*! \internal */
{{class}}Private::{{class}}Private()
    : QAbstractItemModelPrivate()
{
}

/*! \internal */
{{class}}Private::~{{class}}Private()
{
}


{{class}}::{{class}}(QObject* parent)
    : QAbstractListModel(parent)
{
}

/*! \internal */
{{class}}::~{{class}}()
{
}

QHash<int, QByteArray> {{class}}::roleNames() const
{
    auto res = QAbstractItemModel::roleNames();
{% for field in struct.fields %}
    res.insert({{field|upperfirst}}, "{{field}}");
{% endfor %}
    return res;
}

int {{class}}::count() const
{
    Q_D(const {{class}});

    return d->m_data.count();
}

int {{class}}::rowCount(const QModelIndex &parent) const
{
    Q_D(const {{class}});

    return parent.isValid() ? 0 : d->m_data.count();
}

QVariant {{class}}::data(const QModelIndex &index, int role) const
{
    Q_D(const {{class}});
    if (index.row() < 0 || index.row() >= d->m_data.count())
        return {};

    const {{struct}} &data = d->m_data.at(index.row());
    switch(role) {
{% for field in struct.fields %}
        case {{field|upperfirst}}: return qVariantFromValue<{{field|return_type}}>(data.{{field}}());
{% endfor %}
        default: break;
    }

    return {};
}

bool {{class}}::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_D({{class}});
    if (index.row() < 0 || index.row() >= d->m_data.count())
        return false;
    {{struct}} &data = d->m_data[index.row()];
    switch(role) {
{% for field in struct.fields %}
        case {{field|upperfirst}}:
{%   if field.readonly or field.const %}
            return false;
{%   else %}
            data.set{{field|upperfirst}}(value.value<{{field|return_type}}>());
            break;
{%   endif %}
{% endfor %}
        default: return false;
    }
    Q_EMIT dataChanged(index, index, QVector<int>() << role);
    return true;
}

bool {{class}}::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_D({{class}});
    if (row < 0 || row >= d->m_data.size() || count < 0 || parent.isValid())
        return false;
    beginInsertRows(parent, row, row + count - 1);
    d->m_data.insert(row, count, {});
    endInsertRows();
    Q_EMIT countChanged(d->m_data.count());
    return true;
}

bool {{class}}::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_D({{class}});
    if (row < 0 || row >= d->m_data.size() || count < 0 || parent.isValid())
        return false;
    beginInsertRows(parent, row, row + count - 1);
    while (count) {
        d->m_data.remove(row);
        --count;
    }
    endInsertRows();
    Q_EMIT countChanged(d->m_data.count());
    return true;
}

bool {{class}}::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    Q_D({{class}});
    if (sourceRow < 0 || sourceRow >= d->m_data.size() || count < 0 || sourceParent.isValid())
        return false;
    if (destinationChild < 0 || destinationChild > d->m_data.size() || destinationParent.isValid())
        return false;
    if (!beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild))
        return false;
    QVector<{{struct}}> data;
    for (int i = 0; i < count; i++) {
        data << d->m_data.at(sourceRow);
        d->m_data.remove(sourceRow);
    }
    if (destinationChild >= d->m_data.size())
        d->m_data << data;
    else {
        d->m_data.insert(destinationChild, count, {});
        for (int i = 0; i < count; i++)
            d->m_data[destinationChild + 1] = data[i];
    }
    endMoveRows();
    return true;
}

{{struct}} {{class}}::at(int index) const
{
    Q_D(const {{class}});
    if (index < 0 || index >= d->m_data.count())
        return {};
    return d->m_data.at(index);
}

void {{class}}::append(const {{struct}}& data)
{
    Q_D({{class}});
    beginInsertRows({}, d->m_data.count(), d->m_data.count());
    d->m_data << data;
    endInsertRows();
    Q_EMIT countChanged(d->m_data.count());
}

{{struct}} {{class}}::append({% for field in struct.fields %}{% if not loop.first %}, {% endif %}{{field|return_type}} {{field}}{% endfor %})
{
    auto data = {{struct}}({% for field in struct.fields %}{% if not loop.first %}, {% endif %}{{field}}{% endfor %});
    append(data);
    return data;
}

void {{class}}::remove(int index)
{
    Q_D({{class}});
    if (index < 0 || index >= d->m_data.count()) {
        beginRemoveRows({}, index, index);
        d->m_data.remove(index);
        endRemoveRows();
        Q_EMIT countChanged(d->m_data.count());
    }
}


QT_END_NAMESPACE

#include "moc_{{class|lower}}.cpp"
