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
{% set class = '{0}BackendInterface'.format(interface) %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
{% set exportsymbol = 'Q_{0}_EXPORT'.format(module.module_name|upper) %}
{% include 'generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include "{{module.module_name|lower}}module.h"

{% for inc in interface|struct_includes %}
{{inc}}
{% endfor %}

{% if interface.tags.config.zoned %}
#include <QtIviCore/QIviZonedFeatureInterface>
{% else %}
#include <QtIviCore/QIviFeatureInterface>
{% endif %}
#include <QtIviCore/QIviPendingReply>

QT_BEGIN_NAMESPACE

{% if interface.tags.config.zoned %}
class {{exportsymbol}} {{class}} : public QIviZonedFeatureInterface
{% else %}
class {{exportsymbol}} {{class}} : public QIviFeatureInterface
{% endif %}
{
    Q_OBJECT
public:
    explicit {{class}}(QObject *parent = nullptr);
    ~{{class}}();

{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
{%     if interface.tags.config.zoned %}
    virtual void {{property|setter_name}}({{ property|parameter_type }}, const QString &zone) = 0;
{%     else %}
    virtual void {{property|setter_name}}({{ property|parameter_type }}) = 0;
{%     endif %}
{%   endif %}
{% endfor %}
{% for operation in interface.operations %}
{%   if interface.tags.config.zoned %}
{%     if operation.parameters|length %}
    virtual QIviPendingReply<{{operation|return_type}}> {{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}, const QString &zone){%if operation.const %} const{% endif %} = 0;
{%     else %}
    virtual QIviPendingReply<{{operation|return_type}}> {{operation}}(const QString &zone){%if operation.const %} const{% endif %} = 0;
{%     endif %}
{%   else %}
    virtual QIviPendingReply<{{operation|return_type}}> {{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}){%if operation.const %} const{% endif %} = 0;
{%   endif %}
{% endfor %}

Q_SIGNALS:
{% for signal in interface.signals %}
{%   if interface.tags.config.zoned %}
{%     if signal.parameters|length %}
    void {{signal}}({{signal.parameters|map('parameter_type')|join(', ')}}, const QString &zone = QString());
{%     else %}
    void {{signal}}(const QString &zone = QString());
{%     endif %}
{%   else %}
    void {{signal}}({{signal.parameters|map('parameter_type')|join(', ')}});
{%   endif %}
{% endfor %}
{% for property in interface.properties %}
{%   if interface.tags.config.zoned %}
    void {{property}}Changed({{ property|parameter_type }}, const QString &zone);
{%   else %}
    void {{property}}Changed({{ property|parameter_type }});
{%   endif %}
{% endfor %}
};

#define {{module.module_name|upperfirst}}_{{interface}}_iid ("{{interface.tags.config.id}}")

QT_END_NAMESPACE

#endif // {{oncedefine}}
