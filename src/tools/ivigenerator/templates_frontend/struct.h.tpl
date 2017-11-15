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
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
{% set exportsymbol = 'Q_{0}_EXPORT'.format(module.module_name|upper) %}
{% include 'generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include "{{module.module_name|lower}}module.h"
{% for inc in struct|struct_includes %}
{{inc}}
{% endfor %}
#include <QObject>
#include <QDataStream>
#include <QDebug>

QT_BEGIN_NAMESPACE

class  {{exportsymbol}} {{class}}
{
    Q_GADGET
{% for field in struct.fields %}
    Q_PROPERTY({{field|return_type}} {{field}} READ {{field}}{% if not field.readonly and not field.const %} WRITE set{{field|upperfirst}}{% endif %})
{% endfor %}
    Q_CLASSINFO("IviPropertyDomains", "{{ struct.fields|json_domain|replace("\"", "\\\"") }}")
public:
    {{class}}();
    {{class}}({% for field in struct.fields %}{% if not loop.first %}, {% endif %}{{field|return_type}} {{field}}{% endfor %});
    ~{{class}}();

{% for field in struct.fields %}
    {{field|return_type}} {{field}}() const;
{%   if not field.readonly and not field.const %}
    void set{{field|upperfirst}}({{field|parameter_type}});
{%   endif %}
{% endfor %}


private:
{% for field in struct.fields %}
    {{field|return_type}} m_{{field}};
{% endfor %}

    friend {{exportsymbol}} QDataStream &operator>>(QDataStream &stream, {{class}} &obj);
};

{{exportsymbol}} bool operator==(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW;
{{exportsymbol}} bool operator!=(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW;

{{exportsymbol}} QDataStream &operator<<(QDataStream &stream, const {{class}} &obj);
{{exportsymbol}} QDataStream &operator>>(QDataStream &stream, {{class}} &obj);

{{exportsymbol}} QDebug &operator<<(QDebug &dbg, const {{class}} &obj);

QT_END_NAMESPACE

Q_DECLARE_METATYPE({{class}})

#endif // {{oncedefine}}
