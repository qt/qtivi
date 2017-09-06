{#
# Copyright (C) 2017 Pelagicore AG.
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
{% set class = '{0}'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{% include 'generated_comment.cpp.tpl' %}
{% import 'utils.tpl' as utils %}

#include "{{class|lower}}.h"

#include <QQmlEngine>
#include <QDebug>

namespace {
const QString INITIAL_MAIN_ZONE = "MainZone";
}

QT_BEGIN_NAMESPACE

{% if interface.tags.config.zoned %}
{{class}}::{{class}}(const QString &zone, QObject *parent)
    : QObject(parent)
    , m_currentZone(zone)
{
    if (!zone.isEmpty()) {
        return;
    }

    //Add ourself to the available zones to make it available to the UI and still keep the code clean
    m_zoneHash.insert(INITIAL_MAIN_ZONE, this);
    m_zoneMap.insert(INITIAL_MAIN_ZONE, QVariant::fromValue(this));

    {% set zones = interface.tags.config_simulator.zones if interface.tags.config_simulator else {} %}
    {% for zone_name, zone_id in zones.items() %}
    addZone("{{zone_id}}");
    {% endfor %}
}
{% else %}
{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
{
}
{% endif %}

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

{{property|return_type}} {{class}}::{{property|getter_name}}() const
{
   return m_{{property}};
}

void {{class}}::{{property|setter_name}}({{ property|parameter_type }})
{
   if (m_{{property}} == {{property}})
       return;
   m_{{property}} = {{property}};
   emit {{property}}Changed({{property}});
}

{% endfor %}

{%- for operation in interface.operations %}

{{operation|return_type}} {{class}}::{{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}){% if operation.const %} const{% endif %}

{
    return {{operation|default_type_value}};
}

{% endfor %}

QT_END_NAMESPACE
