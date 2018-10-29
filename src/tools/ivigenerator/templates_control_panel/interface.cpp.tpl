{#
# Copyright (C) 2018 Pelagicore AG.
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
{% set class = '{0}'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{% include 'generated_comment.cpp.tpl' %}

#include "{{class|lower}}.h"

#include <QQmlEngine>
#include <QDebug>
#include <QtSimulator>

namespace {
const QString INITIAL_MAIN_ZONE = "MainZone";
}

QT_BEGIN_NAMESPACE

{% if interface.tags.config.zoned %}
{{class}}::{{class}}(const QString &zone, QObject *parent)
    : QObject(parent)
    {% for property in interface.properties %}
    , m_{{ property }}({{property|default_value}})
    {% endfor %}
    , m_currentZone(zone)
{
    if (!zone.isEmpty()) {
        return;
    }

    //Add ourself to the available zones to make it available to the UI and still keep the code clean
    m_zoneHash.insert(INITIAL_MAIN_ZONE, this);
    m_zoneMap.insert(INITIAL_MAIN_ZONE, QVariant::fromValue(this));

    {% set zones = interface.tags.config_simulator.zones if interface.tags.config_simulator else {} %}
    {% for zone_id in zones %}
    addZone(QLatin1String("{{zone_id}}"));
    {%   for property in interface.properties %}
    {%     if not property.type.is_model %}
    {%     if property.tags.config_simulator and property.tags.config_simulator.zoned %}
    m_zoneHash[QLatin1String("{{zone_id}}")]->m_{{property}} = {{property|default_value(zone_id)}};
    {%     endif %}
    {%     endif %}
    {%   endfor %}
    {% endfor %}
{% else %}
{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
{
{% endif %}
    QVariantMap peerInfo{{ '{{' }}"control_panel",""{{ '}}' }};
    QSimulatorServer *server = {{module.module_name}}Module::simulationServer();
    server->registerServer("{{interface}}", QVersionNumber(1, 0, 0), peerInfo, [=](QSimulatorConnectionWorker *client) {
        m_worker = client;
        connect(m_worker, &QSimulatorConnectionWorker::disconnected, this, [=]() {
            qDebug() << "DISCONNECTED";
            //Deleting it on the mainZone is enough as QPointer does the rest of the work for us.
            m_worker = 0;
        });
        m_worker->addReceiver(this);
        qDebug() << "connected: " << client->peerInfo();
{% if interface.tags.config.zoned %}
        auto i = m_zoneHash.constBegin();
        while (i != m_zoneHash.constEnd()) {
            i.value()->m_worker = client;
{%   for property in interface.properties %}
{%   if not property.type.is_model %}
{%     set function_name = property|setter_name %}
{%     if property.readonly or property.const %}
{%       set function_name = property.name + 'Changed' %}
{%     endif %}
            m_worker->call("{{function_name}}", i.value()->m_{{property}}, i.value()->m_currentZone);
{%   endif %}
{%   endfor %}
            ++i;
        }
{% else %}
{%   for property in interface.properties %}
{%   if not property.type.is_model %}
{%     set function_name = property|setter_name %}
{%     if property.readonly or property.const %}
{%       set function_name = property.name + 'Changed' %}
{%     endif %}
        m_worker->call("{{function_name}}", m_{{property}});
{%   endif %}
{%   endfor %}
{% endif %}
    });
}

{{class}}::~{{class}}()
{
}

void {{class}}::registerQmlTypes(const QString& uri, int majorVersion, int minorVersion, const QString& qmlName)
{
{% if 'singleton' in interface.tags %}
    qmlRegisterSingletonType<{{class}}>(uri.toLatin1(), majorVersion, minorVersion, "{{interface}}", {{class|lower}}_singletontype_provider);
{% else %}
    qmlRegisterType<{{class}}>(uri.toLatin1(), majorVersion, minorVersion, qmlName.toLatin1());
{% endif %}
}

{% if interface.tags.config.zoned %}
void {{class}}::addZone(const QString &newZone)
{
    if (!m_currentZone.isEmpty()) {
        qWarning("Adding a new zone is only possible from the root zone.");
        return;
    }

    if (m_zoneHash.contains(newZone))
        return;

    {{class}} *zoneObject = new {{class}}(newZone, this);
    zoneObject->m_worker = m_worker;
    m_zoneHash.insert(newZone, zoneObject);
    m_zoneMap.insert(newZone, QVariant::fromValue(zoneObject));

    emit zonesChanged();
}

QStringList {{class}}::zones() const
{
     return m_zoneMap.keys();
}


QString {{class}}::currentZone() const
{
    return m_currentZone;
}

QVariantMap {{class}}::zoneAt() const
{
    return m_zoneMap;
}
{% endif %}

{% for property in interface.properties %}
{%   if not property.type.is_model %}
{{ivi.prop_getter(property, class)}}
{
   return m_{{property}};
}

{{ivi.prop_setter(property, class)}}
{
   if (m_{{property}} == {{property}})
       return;
    m_{{property}} = {{property}};
    emit {{property}}Changed({{property}});
    auto w = worker();
    if (w)
{% set function_name = property|setter_name %}
{% if property.readonly or property.const %}
{%   set function_name = property.name + 'Changed' %}
{% endif %}
{% if interface.tags.config.zoned %}
        w->call("{{function_name}}", {{property}}, m_currentZone);
{% else %}
        w->call("{{function_name}}", {{property}});
{% endif %}
}

{% if interface.tags.config.zoned %}
{{ivi.prop_setter(property, class, true)}}
{
    QString z = zone;
    if (z.isEmpty())
        z = INITIAL_MAIN_ZONE;

    if (!m_zoneMap.contains(z)) {
        return;
    }

    // the setter cannot be used as it would update the simulation connection
    // and by this create a async loop
    if (m_zoneHash[z]->m_{{property}} == {{property}})
        return;
     m_zoneHash[z]->m_{{property}} = {{property}};
     emit m_zoneHash[z]->{{property}}Changed({{property}});
}
{% endif %}

{% endif %}
{% endfor %}

{% for signal in interface.signals %}
{{ivi.signal(signal, class)}}
{
    auto w = worker();
    if (w)
        w->call("{{signal}}", {{signal.parameters|join(', ')}}{% if interface.tags.config.zoned %}, m_currentZone{% endif %});
}

{% endfor %}

{% if interface_zoned %}
{%   for operation in interface.operations %}
void {{class}}::{{operation}}({{ivi.join_params(operation, zoned=true)}})
{
    QString z = zone;
    if (z.isEmpty())
        z = INITIAL_MAIN_ZONE;

    if (!m_zoneMap.contains(z)) {
        return;
    }

    emit m_zoneHash[z]->{{operation}}({{operation.parameters|join(', ')}});
}
{%   endfor %}
{% endif %}

QSimulatorConnectionWorker *{{class}}::worker()
{
{% if interface.tags.config.zoned %}
    if (m_currentZone.isEmpty())
        return m_worker;
    {{class}}* globalZone = qobject_cast<{{class}}*>(parent());
    if (globalZone)
        return globalZone->m_worker;
    return nullptr;
{% else %}
    return m_worker;
{% endif %}
}

QT_END_NAMESPACE
