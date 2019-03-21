{#
# Copyright (C) 2019 Luxoft Sweden AB
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
{% import 'qtivi_macros.j2' as ivi %}
{% include "generated_comment.cpp.tpl" %}
{% set class = '{0}QtRoAdapter'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
#ifndef {{oncedefine}}
#define {{oncedefine}}

#include "{{interface|lower}}backend.h"
#include "rep_{{interface|lower}}_source.h"
#include "rep_pagingmodel_source.h"

QT_BEGIN_NAMESPACE

/*
* When using the templated enableRemoting method of QtRO, there is no way to specify
* the name this object is remoted as.
* This template fixes the name to the interface qualified name
*/
template <class ObjectType>
struct {{interface}}AddressWrapper: public {{interface}}SourceAPI<ObjectType> {
    {{interface}}AddressWrapper(ObjectType *object, const QString &name = QStringLiteral("{{interface.qualified_name}}"))
        : {{interface}}SourceAPI<ObjectType>(object, name)
    {}
};
{% for property in interface.properties %}
{%   if property.type.is_model %}
template <class ObjectType>
struct {{interface}}{{property}}ModelAddressWrapper: public PagingModelSourceAPI<ObjectType> {
    {{interface}}{{property}}ModelAddressWrapper(ObjectType *object, const QString &name = QStringLiteral("{{interface.qualified_name}}.{{property}}"))
        : PagingModelSourceAPI<ObjectType>(object, name)
    {}
};

{%   endif %}
{% endfor %}

class {{class}} : public {{interface}}Source
{
    Q_OBJECT
public:
    {{class}}({{interface}}Backend *parent = nullptr);

{% if interface_zoned %}
    Q_INVOKABLE QStringList availableZones() override;
{% endif %}

{% for property in interface.properties %}
{%   if not property.is_model %}
{%     if interface_zoned %}
    Q_INVOKABLE {{property|return_type}} {{property|getter_name}}(const QString &zone = QString()) override;
{%     else %}
    {{ivi.prop_getter(property)}} override;
{%     endif %}
{%   endif %}
{% endfor %}

public Q_SLOTS:
{% for property in interface.properties %}
{%   if not property.readonly and not property.const and not property.type.is_model %}
    {{ivi.prop_setter(property, zoned = interface_zoned, default_zone = true)}} override;
{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
    QVariant {{operation}}({{ivi.join_params(operation, zoned = interface_zoned)}}) override;
{% endfor %}

private:
    {{interface}}Backend *m_backend;
    quint64 m_replyCounter;
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
