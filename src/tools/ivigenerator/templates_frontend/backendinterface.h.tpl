{#
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
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
{% set class = '{0}BackendInterface'.format(interface) %}
{% if interface.tags.config.zoned %}
{%   set base_class = 'QIviZonedFeatureInterface' %}
{% else %}
{%   set base_class = 'QIviFeatureInterface' %}
{% endif %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
{% set exportsymbol = 'Q_{0}_EXPORT'.format(module|upper|replace('.', '_')) %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

{% if interface.module.tags.config.module %}
#include <{{interface.module.tags.config.module}}/{{module.module_name|lower}}module.h>
{% else %}
#include "{{module.module_name|lower}}module.h"
{% endif %}

{% for inc in interface|struct_includes %}
{{inc}}
{% endfor %}

#include <QtIviCore/{{base_class}}>
#include <QtIviCore/QIviPendingReply>
#include <QtIviCore/QIviPagingModelInterface>

QT_BEGIN_NAMESPACE

class {{exportsymbol}} {{class}} : public {{base_class}}
{
    Q_OBJECT

public:
    explicit {{class}}(QObject *parent = nullptr);
    ~{{class}}();

{% for property in interface.properties %}
{%   if not property.readonly and not property.const and not property.type.is_model %}
    virtual {{ivi.prop_setter(property, zoned = interface.tags.config.zoned)}} = 0;
{%   endif %}
{% endfor %}
{% for operation in interface.operations %}
    virtual {{ivi.operation(operation, zoned = interface.tags.config.zoned)}} = 0;
{% endfor %}

Q_SIGNALS:
{% for signal in interface.signals %}
    {{ivi.signal(signal, zoned = interface.tags.config.zoned)}};
{% endfor %}
{% for property in interface.properties %}
    {{ivi.prop_notify(property, zoned = interface.tags.config.zoned, model_interface = true, default_values = true)}};
{% endfor %}
};

#define {{module.module_name|upperfirst}}_{{interface}}_iid ("{{interface.tags.config.id | default(interface.qualified_name)}}")

QT_END_NAMESPACE

#endif // {{oncedefine}}
