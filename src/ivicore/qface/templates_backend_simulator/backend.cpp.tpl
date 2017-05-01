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
{% import 'utils.tpl' as utils %}
{% set class = '{0}Backend'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
#include "{{class|lower}}.h"

#include <QDebug>

/*!
   \class {{class}}
   \inmodule {{module}}
{{ utils.format_comments(interface.comment) }}
*/
{{class}}::{{class}}(QObject *parent) :
    {{class}}Interface(parent)
{% for property in interface.properties %}
{%   if not property.tags.config_simulator or not property.tags.config_simulator.zoned %}
    , m_{{ property }}({{property|sim_default_value}})
{%   endif %}
{% endfor %}
{

{% set zones = interface.tags.config_simulator.zones if interface.tags.config_simulator else {} %}
{% for zone_name, zone_id in zones.items() %}
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

{% if interface_zoned %}
/*!
    \fn QStringList {{class}}::availableZones() const

    Returns a list of supported zone names. This is called from the client
    after having connected.

    The returned names must be valid QML property names, i.e. \c {[a-z_][A-Za-z0-9_]*}.

    \sa {Providing Available Zones}
*/
QStringList {{class}}::availableZones() const
{
{%   if interface.tags.config_simulator and interface.tags.config_simulator.zoned %}
    return m_zoneMap.keys();
{%   else %}
    return QStringList();
{%   endif%}
}
{% endif %}

/*!
    \fn void {{class}}::initialize()

    Initializes the backend and informs about its current state by
    emitting signals with the current status (property values).

*/
void {{class}}::initialize()
{
{% for property in interface.properties %}
{%   if not interface_zoned  %}
    emit {{property}}Changed(m_{{property}});
{%   elif not property.tags.config_simulator or not property.tags.config_simulator.zoned%}
    emit {{property}}Changed(m_{{property}}, QString());
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
{%   if not property.readonly and not property.const %}
/*!
    \fn virtual void {{class}}::set{{property|upperfirst}}({{ property|parameter_type }}{% if interface_zoned %}, const QString &zone){%endif%})

{{ utils.format_comments(property.comment) }}
*/
{%   if interface_zoned %}
void {{class}}::set{{property|upperfirst}}({{ property|parameter_type }}, const QString &zone)
{%   else %}
void {{class}}::set{{property|upperfirst}}({{ property|parameter_type }})
{%   endif %}
{
{%   if property.tags.config_simulator and property.tags.config_simulator.unsupported %}
    Q_UNUSED({{ property }});
{%     if interface_zoned %}
    Q_UNUSED(zone);
{%     endif %}
    qWarning() << "SIMULATION Setting {{ property | upperfirst }} is not supported!";

{%   else %}
{%     set range_low_val = property|range_low %}
{%     set range_high_val = property|range_high %}
{%     set zoned = property.tags.config_simulator and property.tags.config_simulator.zoned %}
{%     if zoned and interface_zoned %}
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].{{property}} == {{property}})
        return;
{%       if range_low_val and range_high_val %}

    if ({{property}} < {{range_low}} || {{property}} > {{range_high}}) {
        qWarning() << "SIMULATION {{ property | upperfirst }} change out of range ({{range_low}}-{{range_high}})" << {{property}};
        emit {{property}}Changed({{property}}, zone);
        return;
    }
{%       endif %}

    qWarning() << "SIMULATION {{ property | upperfirst }} for Zone" << zone << "changed to" << {{property}};

    m_zoneMap[zone].{{property}} = {{property}};
    emit {{ property }}Changed({{property}}, zone);

{%     else %}
    if ({% if interface_zoned %}!zone.isEmpty() || {%endif%}m_{{ property }} == {{property}})
        return;

{%       if range_low_val != None and range_high_val != None %}
    if ({{property}} < {{range_low_val}} || {{property}} > {{range_high_val}}) {
        qWarning() << "SIMULATION {{ property | upperfirst }} change out of range ({{range_low}}-{{range_high}})" << {{property}};
        emit {{property}}Changed(m_{{property}}{% if interface_zoned%}, QString(){% endif %});
        return;
    }
{%       endif %}
    qWarning() << "SIMULATION {{ property | upperfirst }} changed to" << {{property}};

    m_{{property}} = {{property}};
    emit {{property}}Changed(m_{{property}}{% if interface_zoned%}, QString(){% endif %});
{%     endif %}
{%   endif %}
}

{% endif %}
{% endfor %}

{% for operation in interface.operations %}
{%   set operation_parameters = operation.parameters|map('parameter_type')|join(', ') %}
{%   if interface_zoned %}
{%     if operation.parameters|length %}
{%       set operation_parameters = operation_parameters + ', ' %}
{%     endif %}
{%     set operation_parameters = operation_parameters + 'const QString &zone' %}
{%   endif%}
/*!
    \fn virtual void {{class}}::{{operation}}({{operation_parameters}})

{{ utils.format_comments(operation.comment) }}
*/
{{operation|return_type}} {{class}}::{{operation}}({{operation_parameters}}){%if operation.const %} const{% endif %}

{
    qWarning() << "Not implemented!";
    return {{operation|default_value}};
}

{% endfor %}
