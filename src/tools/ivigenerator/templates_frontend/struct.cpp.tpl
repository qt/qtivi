{#
# Copyright (C) 2017 Pelagicore AG.
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
{% set class = '{0}'.format(struct) %}
{% include 'generated_comment.cpp.tpl' %}
{% import 'utils.tpl' as utils %}

#include "{{class|lower}}.h"

QT_BEGIN_NAMESPACE

/*!
    \class {{struct}}
    \inmodule {{module}}
{{ utils.format_comments(struct.comment) }}
*/

{{class}}::{{class}}()
{% for field in struct.fields %}
    {% if loop.first %}:{% else %},{% endif %} m_{{field}}({{field|default_type_value}})
{% endfor %}
{
}

{{class}}::{{class}}({% for field in struct.fields %}{% if not loop.first %}, {% endif %}{{field|return_type}} {{field}}{% endfor %})
{% for field in struct.fields %}
    {% if loop.first %}:{% else %},{% endif %} m_{{field}}({{field}})
{% endfor %}
{
}

/*! \internal */
{{class}}::~{{class}}()
{
}

{% for field in struct.fields %}

/*!
    \property {{class}}::{{field}}
{{ utils.format_comments(field.comment) }}
{% if field.const %}
    \note This property is constant and the value will not change once an instance has been created.
{% endif %}
*/
{{field|return_type}} {{class}}::{{field}}() const
{
    return m_{{field}};
}
{%   if not field.readonly and not field.const %}

void {{class}}::set{{field|upperfirst}}({{ field|parameter_type }})
{
    m_{{field}} = {{field}};
}
{%   endif %}

{% endfor %}

bool operator==(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW
{
    return (
{% for field in struct.fields %}
        left.{{field}}() == right.{{field}}() {% if not loop.last %}&&{% endif %}

{% endfor %}
    );
}

bool operator!=(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW
{
    return !(left == right);
}

QDataStream &operator<<(QDataStream &stream, const {{class}} &obj)
{
{% for field in struct.fields %}
    stream << obj.{{field}}();
{% endfor %}
    return stream;
}

QDataStream &operator>>(QDataStream &stream, {{class}} &obj)
{
{% for field in struct.fields %}
    stream >> obj.m_{{field}};
{% endfor %}
    return stream;
}

QDebug &operator<<(QDebug &dbg, const {{class}} &obj)
{
    Q_UNUSED(obj);
    dbg << "{{class}}";
    return dbg;
}

QT_END_NAMESPACE
