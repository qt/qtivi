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
{% set zone_class = '{0}Zone'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
#include "{{class|lower}}.h"

#include <QDebug>
#include <QtIviCore/QIviSimulationEngine>

{% if 'simulator' in features %}
#include <QtSimulator>
{% endif %}

{% for property in interface.properties %}
{%   if property.type.is_model %}
{% include "pagingmodel.cpp.tpl" %}
{%   endif %}
{% endfor %}

QT_BEGIN_NAMESPACE

{% if interface_zoned %}
{{zone_class}}::{{zone_class}}(const QString &zone, {{class}}Interface *parent)
    : QObject(parent)
    , m_parent(parent)
    , m_zone(zone)
{% for property in interface.properties %}
{%   if not property.type.is_model %}
    , m_{{ property }}({{property|default_value}})
{%   endif %}
{% endfor %}
{
}

{% for property in interface.properties %}
{{ivi.prop_getter(property, zone_class, model_interface = true)}}
{
    return m_{{property}};
}
{% endfor %}

{% for property in interface.properties %}
{{ivi.prop_setter(property, zone_class, model_interface = true)}}
{
    if (m_{{property}} == {{property}})
        return;
    m_{{property}} = {{property}};
    emit {{property}}Changed({{property}});
    emit m_parent->{{property}}Changed({{property}}, m_zone);
}
{% endfor %}
{% endif %}

/*!
   \class {{class}}
   \inmodule {{module}}
{{ ivi.format_comments(interface.comment) }}
*/
{{class}}::{{class}}(QObject *parent)
    : {{class}}(nullptr, parent)
{
}

{{class}}::{{class}}(QIviSimulationEngine *engine, QObject *parent)
    : {{class}}Interface(parent)
{% for property in interface.properties %}
{%       if not property.type.is_model %}
    , m_{{ property }}({{property|default_value}})
{%       endif %}
{% endfor %}
    , m_zones(new QQmlPropertyMap(this))
{% if 'simulator' in features %}
    , mWorker(nullptr)
{% endif %}
{
    //In some cases the engine is unused, this doesn't do any harm if it is still used
    Q_UNUSED(engine)
    qRegisterMetaType<QQmlPropertyMap*>();

{% for property in interface.properties %}
{%   if not property.tags.config_simulator or not property.tags.config_simulator.zoned %}
{%       if property.type.is_model %}
    auto {{ property }}Model = (new {{property|upperfirst}}ModelBackend(this));
    m_{{ property }} = {{ property }}Model;
    engine->registerSimulationInstance({{ property }}Model, "{{module.name|lower}}.simulation", 1, 0, "{{property|upperfirst}}ModelBackend");
{%       endif %}
{%   endif %}
{% endfor %}

    {{module.module_name|upperfirst}}Module::registerTypes();
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
    QStringList zones;
    QIVI_SIMULATION_TRY_CALL_FUNC({{class}}, "availableZones", zones = return_value.toStringList());

    for (const QString &zone : zones)
        const_cast<{{class}}*>(this)->addZone(zone);
    return zones;
}
{% endif %}

/*!
    \fn void {{class}}::initialize()

    Initializes the backend and informs about its current state by
    emitting signals with the current status (property values).

*/
void {{class}}::initialize()
{
    QIVI_SIMULATION_TRY_CALL({{class}}, "initialize", void);
{% for property in interface.properties %}
{%   if not interface_zoned  %}
    emit {{property}}Changed(m_{{property}});
{%   elif not property.tags.config_simulator or not property.tags.config_simulator.zoned%}
    emit {{property}}Changed(m_{{property}}, QString());
{%   endif %}
{% endfor %}

{% if interface.tags.config.zoned %}
    for (const QString &zone : m_zones->keys()) {
        {{interface}}Zone *zo = zoneAt(zone);
{%   for property in interface.properties %}
        emit {{property}}Changed(zo->{{property|getter_name}}(), zone);
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

{% if interface_zoned %}
void {{class}}::addZone(const QString &zone)
{
    m_zones->insert(zone, QVariant::fromValue(new {{zone_class}}(zone, this)));
}

{{zone_class}}* {{class}}::zoneAt(const QString &zone)
{
    return m_zones->value(zone).value<{{zone_class}}*>();
}
{% endif %}

{% for property in interface.properties %}
{{ivi.prop_getter(property, class, model_interface = true)}}
{
    return m_{{property}};
}
{% endfor %}

{% for property in interface.properties %}
/*!
    \fn virtual {{ivi.prop_setter(property, class, interface_zoned)}}

{{ ivi.format_comments(property.comment) }}
*/
{{ivi.prop_setter(property, class, interface_zoned, model_interface = true)}}
{
    QIVI_SIMULATION_TRY_CALL({{class}}, "{{property|setter_name}}", void, {{property}});

{% if interface_zoned %}
    if (zone.isEmpty()) {
        if (m_{{property}} == {{property}})
            return;
        m_{{property}} = {{property}};
        emit {{property}}Changed({{property}}, QString());
    } else {
        {{interface}}Zone *zo = zoneAt(zone);
        if (zo)
            zo->{{property|setter_name}}({{property}});
        else
            qWarning() << "No such Zone";
    }
{% else %}
    if (m_{{property}} == {{property}})
        return;
    m_{{property}} = {{property}};
    emit {{property}}Changed(m_{{property}});
{% endif %}
}

{% endfor %}

{% for operation in interface.operations %}
/*!
    \fn virtual {{ivi.operation(operation, class, interface_zoned)}}

{{ ivi.format_comments(operation.comment) }}
*/
{{ivi.operation(operation, class, interface_zoned)}}
{
{%   set function_parameters = operation.parameters|join(', ') %}
{%   if interface_zoned %}
{%     if operation.parameters|length %}
{%       set function_parameters = function_parameters + ', ' %}
{%     endif %}
{%     set function_parameters = function_parameters + 'zone' %}
{%   endif%}
    QIviPendingReply<{{operation|return_type}}> pendingReply;
    QIVI_SIMULATION_TRY_CALL_FUNC({{class}}, "{{operation}}", return pendingReply, QIviPendingReplyBase(pendingReply){% if function_parameters is not equalto "" %}, {{function_parameters}} {% endif %});

{% if interface_zoned %}
    Q_UNUSED(zone);
{% endif %}

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
