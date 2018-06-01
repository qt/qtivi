{#
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
# Copyright (C) 2018 Pelagicore AG
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
{% set class = '{0}Backend'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
#include "{{class|lower}}.h"

{% for property in interface.properties %}
{% if property.type.is_model %}
#include "{{property|model_type|lower}}.h"
{% endif %}
{% endfor %}

#include <QDebug>

{% if 'simulator' in features %}
#include <QtSimulator>
{% endif %}

QT_BEGIN_NAMESPACE

/*!
   \class {{class}}
   \inmodule {{module}}
{{ ivi.format_comments(interface.comment) }}
*/
{{class}}::{{class}}(QObject *parent)
    : {{class}}Interface(parent)
{% for property in interface.properties %}
{%   if not property.tags.config_simulator or not property.tags.config_simulator.zoned %}
    , m_{{ property }}({{property|default_value}})
{%   endif %}
{% endfor %}
{% if 'simulator' in features %}
    , mWorker(nullptr)
{% endif %}
{

    {{module.module_name|upperfirst}}Module::registerTypes();
{% set zones = interface.tags.config_simulator.zones if interface.tags.config_simulator else {} %}
{% for zone_name, zone_id in zones.items() %}
    ZoneBackend {{zone_name}}Zone;
{%   for property in interface.properties %}
{%     if property.tags.config_simulator and property.tags.config_simulator.zoned %}
    {{zone_name}}Zone.{{property}} = {{property|default_value(zone_name)}};
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

{% if 'simulator' in features %}
    mConnection = new QSimulatorConnection("{{interface}}", QVersionNumber(1, 0, 0));
    mConnection->addPeerInfo("versionInfo", "1.0.0");
    mConnection->addPeerInfo("name", "{{class}}");
    QString hostname = QSimulatorConnection::simulatorHostName(false);
    if (hostname.isEmpty()) {
        qWarning() << "SIMULATOR_HOSTNAME environment variable not set! Disabling the QtSimulator connection";
        return;
    }
    qWarning() << "Connecting to QtSimulator Device:" << hostname;
    mWorker = mConnection->connectToHost(hostname, 0xbeef+3);
    if (!mWorker) {
        qWarning() << "Couldn't connect to QtSimulator Device:" << hostname;
        return;
    }

    mWorker->addReceiver(this);
{% endif %}
    emit initializationDone();
}

{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
/*!
    \fn virtual {{ivi.prop_setter(property, class, interface_zoned)}}

{{ ivi.format_comments(property.comment) }}
*/
{{ivi.prop_setter(property, class, interface_zoned)}}
{
{%   if property.tags.config_simulator and property.tags.config_simulator.unsupported %}
    Q_UNUSED({{ property }});
{%     if interface_zoned %}
    Q_UNUSED(zone);
{%     endif %}
    qWarning() << "SIMULATION Setting {{ property | upperfirst }} is not supported!";

{%   else %}
{%     set zoned = property.tags.config_simulator and property.tags.config_simulator.zoned %}
{%     if zoned and interface_zoned %}
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].{{property}} == {{property}})
        return;
{% include "backend_range.cpp.tpl" %}
    qWarning() << "SIMULATION {{ property | upperfirst }} for Zone" << zone << "changed to" << {{property}};

    m_zoneMap[zone].{{property}} = {{property}};
    emit {{ property }}Changed({{property}}, zone);

{%       if 'simulator' in features %}
    if (mWorker)
        mWorker->call("{{property|setter_name}}", {{property}}, zone);
{%       endif %}
{%     else %}
    if ({% if interface_zoned %}!zone.isEmpty() || {%endif%}m_{{ property }} == {{property}})
        return;
{% include "backend_range.cpp.tpl" %}
    qWarning() << "SIMULATION {{ property | upperfirst }} changed to" << {{property}};

    m_{{property}} = {{property}};
    emit {{property}}Changed(m_{{property}}{% if interface_zoned%}, QString(){% endif %});
{%       if 'simulator' in features %}
    if (mWorker)
        mWorker->call("{{property|setter_name}}", {{property}}{% if interface_zoned%}, QString(){% endif %});
{%       endif %}
{%     endif %}
{%   endif %}
}

{% endif %}
{% endfor %}

{% for operation in interface.operations %}
/*!
    \fn virtual {{ivi.operation(operation, class, interface_zoned)}}

{{ ivi.format_comments(operation.comment) }}
*/
{{ivi.operation(operation, class, interface_zoned)}}
{
{% for operation_parameter in operation.parameters %}
    Q_UNUSED({{operation_parameter.name}});
{% endfor %}
{% if interface_zoned %}
    Q_UNUSED(zone);
{% endif %}
{%   set function_parameters = operation.parameters|join(', ') %}
{%   if interface_zoned %}
{%     if operation.parameters|length %}
{%       set function_parameters = function_parameters + ', ' %}
{%     endif %}
{%     set function_parameters = function_parameters + 'zone' %}
{%   endif%}

{% if 'simulator' in features %}
    if (mWorker)
        mWorker->call("{{operation}}" {% if function_parameters is not equalto "" %}, {{function_parameters}} {% endif %});
{% endif %}

    qWarning() << "Not implemented!";

    //Fake that the reply always succeeded
    QIviPendingReply<{{operation|return_type}}> successReply;
    successReply.setSuccess({{operation|default_value}});
    return successReply;
}

{% endfor %}

QT_END_NAMESPACE
