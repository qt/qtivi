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


namespace {
const QString INITIAL_MAIN_ZONE = "MainZone";
}

QT_BEGIN_NAMESPACE

/*!
    \class {{interface}}
    \inmodule {{module}}
{{ utils.format_comments(interface.comment) }}
*/
{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
    , m_currentZoneBackend(createZoneBackend())
    , m_currentZone(INITIAL_MAIN_ZONE)
{
    m_zones << INITIAL_MAIN_ZONE;
    {% set zones = interface.tags.config_simulator.zones if interface.tags.config_simulator else {} %}
    {% for zone_name, zone_id in zones.items() %}
    m_zones << "{{zone_id}}";
    {% endfor %}
}

/*! \internal */
{{class}}::~{{class}}()
{
{% if module.tags.config.disablePrivateIVI %}
    delete m_helper;
{% endif %}
}

/*! \internal */
void {{class}}::registerQmlTypes(const QString& uri, int majorVersion, int minorVersion, const QString& qmlName)
{
{% if 'singleton' in interface.tags %}
    qmlRegisterSingletonType<{{class}}>(uri.toLatin1(), majorVersion, minorVersion, "{{interface}}", {{class|lower}}_singletontype_provider);
{% else %}
    qmlRegisterType<{{class}}>(uri.toLatin1(), majorVersion, minorVersion, qmlName.toLatin1());
{% endif %}
}

void {{class}}::addZone(const QString &newZone)
{
    bool exists = false;
    for (int i = 0; i < m_zones.count(); ++i) {
        if (m_zones.at(i).toString() == newZone) {
            exists = true;
            break;
        }
    }
    if (!exists) {
        m_zones.append(newZone);
        emit zonesChanged();

    }
}


QVariantList {{class}}::zones() const
{
     return m_zones;
}


QString {{class}}::currentZone() const
{
    return m_currentZone;
}


void {{class}}::setCurrentZone(const QString &zone)
{
    if (zone != m_currentZone) {
{% if interface_zoned %}
        m_zoneMap[m_currentZone]=m_currentZoneBackend;
        m_currentZone = zone;
        if(m_zoneMap.contains(m_currentZone)) {
            m_currentZoneBackend = m_zoneMap[m_currentZone];
        } else {
            m_currentZoneBackend = createZoneBackend();
        }
{% endif %}
        emit currentZoneChanged();
{% for property in interface.properties %}
        emit {{property}}Changed(m_currentZoneBackend.{{property}});
{% endfor %}
    }
}


{% for property in interface.properties %}

/*!
    \property {{class}}::{{property}}
{{ utils.format_comments(property.comment) }}
{% if property.const %}
    \note This property is constant and the value will not change once the plugin is initialized.
{% endif %}
*/
{{property|return_type}} {{class}}::{{property|getter_name}}() const
{
   return m_currentZoneBackend.{{property}};
}

void {{class}}::{{property|setter_name}}({{ property|parameter_type }})
{
    m_currentZoneBackend.{{property}} = {{property}};
    emit {{property}}Changed({{property}});
}

{% endfor %}

{%- for operation in interface.operations %}
/*!
{{ utils.format_comments(operation.comment) }}
*/
{{operation|return_type}} {{class}}::{{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}){% if operation.const %} const{% endif %}

{
    return {{operation|default_type_value}};
}

{% endfor %}

{{class}}::ZoneBackend {{class}}::createZoneBackend() {
    ZoneBackend zoneBackend;
{% for property in interface.properties %}
    zoneBackend.{{property}} = {{property|default_value}};
{%   endfor %}
    return zoneBackend;
}

QT_END_NAMESPACE

#include "moc_{{class|lower}}.cpp"
