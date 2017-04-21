{#
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
# Copyright (C) 2017 Pelagicore AG
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
{% include "generated_comment.cpp.tpl" %}
{% set class = '{0}Backend'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QObject>
{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{class}}Interface>
{% else %}
#include "{{class|lower}}interface.h"
{% endif %}

class {{class}} : public {{class}}Interface
{
public:
    explicit {{class}}(QObject *parent = nullptr);
    ~{{class}}();

public:
{%   if interface_zoned %}
    QStringList availableZones() const override;
{%   endif %}

    void initialize() override;
{% for property in interface.properties %}
{%   if interface_zoned %}
    virtual void set{{property|upperfirst}}({{ property|parameter_type }}, const QString &zone) override;
{%   else %}
    virtual void set{{property|upperfirst}}({{ property|parameter_type }}) override;
{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
{%   if interface_zoned %}
{%     if operation.parameters|length %}
    virtual {{operation|return_type}} {{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}, const QString &zone) override;
{%     else %}
    virtual {{operation|return_type}} {{operation}}(const QString &zone) override;
{%     endif %}
{%   else %}
    virtual {{operation|return_type}} {{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}) override;
{%   endif %}
{% endfor %}

private:
{% for property in interface.properties %}
{%   if not property.tags.config_simulator or not property.tags.config_simulator.zoned %}
    {{ property|return_type }} m_{{ property }};
{%   endif %}
{% endfor %}

{% if interface_zoned %}
    struct ZoneBackend {
{%   for property in interface.properties %}
{%     if property.tags.config and property.tags.config.zoned %}
        {{ property|return_type }} {{ property }};
{%     endif %}
{%   endfor %}
    };
    QMap<QString,ZoneBackend> m_zoneMap;
{% endif %}

};

#endif // {{oncedefine}}
