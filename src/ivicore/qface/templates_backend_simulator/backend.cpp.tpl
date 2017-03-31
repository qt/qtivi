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

#include "{{class|lower}}.h"

#include <QDebug>

{{class}}::{{class}}(QObject *parent) :
    {{class}}Interface(parent)
{% for property in interface.properties %}
{%   if not property.tags.config or not property.tags.config.zoned %}
    , m_{{ property }}({{property|sim_default_value}})
{%   endif %}
{% endfor %}
{

{% set zones = interface.tags.config_simulator.zones.split('.') if interface.tags.config_simulator else [] %}
{% for zone in zones %}
{%   set zone_name, zone_id = zone.split('_')%}
    ZoneBackend {{zone_name}}Zone;
{%   for property in interface.properties %}
{%     if property.tags.config_simulator and property.tags.config_simulator.zoned %}
    {{zone_name}}Zone.{{property}} = {{property|sim_default_value}};
{%     endif %}
{%   endfor %}
    m_zoneMap.insert("{{zone_id}}", {{zone_name}}Zone);

{% endfor %}
}

{{class}}::~{{class}}()
{
}

QStringList {{class}}::availableZones() const
{
    return m_zoneMap.keys();
}

void {{class}}::initializeAttributes()
{
{% for property in interface.properties %}
{%   if not interface.tags.config.zoned or not property.tags.config_simulator or not property.tags.config_simulator.zoned %}
    emit {{property}}Changed(m_{{property}});
{%   endif %}
{% endfor %}

{% if interface.tags.config.zoned %}
    const auto zones = availableZones();
    for (const QString &zone : zones) {
{%   for property in interface.properties %}
{%     if property.tags.config_simulator and property.tags.config_simulator.zoned %}
        emit {{property}}Changed(m_zoneMap[zone].{{property}}, zone);
{%     endif %}
{%   endfor %}
    }
{% endif %}
}

{% for property in interface.properties %}
{%   if interface.tags.config and interface.tags.config.zoned %}
void {{class}}::set{{property|upperfirst}}({{ property|parameter_type }}, const QString &zone);
{%   else %}
void {{class}}::set{{property|upperfirst}}({{ property|parameter_type }});
{%   endif %}
{
{%   if property.tags.config_simulator and property.tags.config_simulator.unsupported %}
    Q_UNUSED({ property|parameter_type }});
    Q_UNUSED(zone);
    qWarning() << "SIMULATION Setting {{ property | upperfirst }} is not supported!";

{%   else %}
{%     set range_low_val = property|range_low %}
{%     set range_high_val = property|range_high %}
{%     set zoned = property.tags.config and property.tags.config.zoned %}
{%     if zoned %}
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].m_{{property}} == val)
        return;
{%       if range_low_val and range_high_val %}

    if (val < {{range_low}} || val > {{range_high}}) {
        qWarning() << "SIMULATION {{ property | upperfirst }} change out of range ({{range_low}}-{{range_high}})" << val;
        emit {{property}}Changed(m_{{property, zone}});
        return;
    }
{%       endif %}

    qWarning() << "SIMULATION {{ property | upperfirst }} for Zone" << zone << "changed to" << val;

    m_zoneMap[zone].m_{{property}} = val;
    emit {{ property }}Changed(val, zone);

{%     else %}
    if (!zone.isEmpty() || m_{{ property }} == val)
        return;

{%       if range_low_val != None and range_high_val != None %}
    if (val < {{range_low_val}} || val > {{range_high_val}}) {
        qWarning() << "SIMULATION {{ property | upperfirst }} change out of range ({{range_low}}-{{range_high}})" << val;
        emit {{property}}Changed(m_{{property}});
        return;
    }
{%       endif %}
    qWarning() << "SIMULATION {{ property | upperfirst }} changed to" << val;

    m_{{property}} = val;
    emit {{property}}Changed(val);
{%     endif %}
{%   endif %}
}

{% endfor %}

{% for operation in interface.operations %}
{%   if interface.tags.config and interface.tags.config.zoned == 'true' %}
{%     if operation.parameters|length %}
    {{operation|return_type}} {{class}}::{{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}, const QString &zone)
{%     else %}
    {{operation|return_type}} {{class}}::{{operation}}(const QString &zone)
{%     endif %}
{%   else %}
    {{operation|return_type}} {{class}}::{{operation}}({{operation.parameters|map('parameter_type')|join(', ')}})
{%   endif %}
    {
        qWarning() << "Not implemented!";
    }
{% endfor %}
