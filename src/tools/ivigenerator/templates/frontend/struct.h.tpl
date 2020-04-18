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
{% import 'common/qtivi_macros.j2' as ivi %}
{% set class = '{0}'.format(struct) %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
{% set exportsymbol = 'Q_{0}_EXPORT'.format(module|upper|replace('.', '_')) %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{module.module_name|lower}}module.h>
{% else %}
#include "{{module.module_name|lower}}module.h"
{% endif %}
{% for inc in struct|struct_includes %}
{{inc}}
{% endfor %}
#include <QObject>
#include <QDataStream>
#include <QDebug>
#include <QIviStandardItem>

QT_BEGIN_NAMESPACE

class {{class}}Private;

class {{exportsymbol}} {{class}} : public QIviStandardItem
{
    Q_GADGET
{% for field in struct.fields %}
    {{ivi.property(field, notify=false)}}
{% endfor %}

public:
    {{class}}();
    {{class}}(const {{class}} &rhs);
    {{class}} &operator=(const {{class}} &);
    {{class}}({{struct.fields|map('parameter_type')|join(', ')}});
    ~{{class}}();

    QString type() const override;

{% for field in struct.fields %}
{%   if field.name == "id" or field.name == "name" or field.name == "type" or field.name == "data" %}
    {{ivi.prop_getter(field)}} override;
{%   else %}
    {{ivi.prop_getter(field)}};
{%   endif %}
{%   if not field.readonly and not field.const %}
{%     if field.name == "id" or field.name == "data" %}
    {{ivi.prop_setter(field)}} override;
{%     else %}
    {{ivi.prop_setter(field)}};
{%   endif %}
{%   endif %}
{% endfor %}

protected:
    Q_INVOKABLE void fromJSON(const QVariant &variant);

private:
    QSharedDataPointer<{{class}}Private> d;
    friend {{exportsymbol}} bool operator==(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW;
    friend {{exportsymbol}} QDataStream &operator>>(QDataStream &stream, {{class}} &obj);
};
Q_DECLARE_TYPEINFO({{class}}, Q_MOVABLE_TYPE);

{{exportsymbol}} bool operator==(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW;
{{exportsymbol}} bool operator!=(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW;

{{exportsymbol}} QDataStream &operator<<(QDataStream &stream, const {{class}} &obj);
{{exportsymbol}} QDataStream &operator>>(QDataStream &stream, {{class}} &obj);

{{exportsymbol}} QDebug &operator<<(QDebug &dbg, const {{class}} &obj);

QT_END_NAMESPACE

Q_DECLARE_METATYPE({{class}})

#endif // {{oncedefine}}
