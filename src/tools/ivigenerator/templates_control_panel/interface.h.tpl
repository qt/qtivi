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
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
{% set exportsymbol = 'Q_QT{0}_EXPORT'.format(module.module_name|upper) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{% include 'generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include "{{module.module_name|lower}}module.h"

#include <QObject>
#include <QHash>


QT_BEGIN_NAMESPACE

class {{class}}Private;
class {{class}}BackendInterface;

{% if interface.tags.config.zoned %}
class {{exportsymbol}} {{class}} : public QObject {
{% else %}
class {{exportsymbol}} {{class}} : public QObject {
{% endif %}
    Q_OBJECT
    Q_PROPERTY(QString currentZone READ currentZone WRITE setCurrentZone NOTIFY currentZoneChanged)
    Q_PROPERTY(QVariantList zones READ zones NOTIFY zonesChanged)
{% for property in interface.properties %}
    Q_PROPERTY({{property|return_type}} {{property}} READ {{property}}{% if not property.readonly and not property.const %} WRITE set{{property|upperfirst}}{% endif %} NOTIFY {{property}}Changed)
{% endfor %}
    Q_CLASSINFO("IviPropertyDomains", "{{ interface.properties|json_domain|replace("\"", "\\\"") }}")
public:
{% if interface.tags.config.zoned %}
    explicit {{class}}(const QString &zone = QString(), QObject *parent = nullptr);
{% else %}
    explicit {{class}}(QObject *parent = nullptr);
{% endif %}
    ~{{class}}();

    static void registerQmlTypes(const QString& uri, int majorVersion=1, int minorVersion=0, const QString& qmlName = "");
    Q_INVOKABLE void addZone(const QString& newZone);

    QString currentZone() const;
    QVariantList zones() const;
{% for property in interface.properties %}
    {{property|return_type}} {{property}}() const;
{% endfor %}

public Q_SLOTS:
    void setCurrentZone(const QString& zone);
{% for operation in interface.operations %}
    {{operation|return_type}} {{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}){% if operation.const %} const{% endif %};
{% endfor %}
{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
    void set{{property|upperfirst}}({{property|parameter_type}});
{%   endif %}
{% endfor %}

Q_SIGNALS:
    void currentZoneChanged();
    void zonesChanged();
{% for signal in interface.signals %}
    void {{signal}}({{signal.parameters|map('parameter_type')|join(', ')}});
{% endfor %}
{% for property in interface.properties %}
    void {{property}}Changed({{property|parameter_type}});
{% endfor %}

private:
    struct ZoneBackend {
{%   for property in interface.properties %}
        {{ property|return_type }} {{ property }};
{%   endfor %}
    };
    ZoneBackend m_currentZoneBackend ;
{% if interface_zoned %}
    QHash<QString,ZoneBackend> m_zoneMap;
{% endif %}
    QVariantList m_zones;
    QString m_currentZone;
    ZoneBackend createZoneBackend();
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
