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
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QObject>
#include <QQmlPropertyMap>
{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{class}}Interface>
{% else %}
#include "{{class|lower}}interface.h"
{% endif %}

{% for property in interface.properties %}
{%   if property.type.is_model %}
{% include "pagingmodel.h.tpl" %}
{%   endif %}
{% endfor %}

QT_BEGIN_NAMESPACE

{% if 'simulator' in features %}
class QSimulatorConnection;
class QSimulatorConnectionWorker;
{% endif %}

class QIviSimulationEngine;

{% if interface_zoned %}
class {{zone_class}} : public QObject
{
    Q_OBJECT
{% for property in interface.properties %}
{%   if property.type.is_model %}
{%     set type = 'QIviPagingModelInterface *' %}
{%   else %}
{%     set type = property|return_type %}
{%   endif %}
    Q_PROPERTY({{type}} {{property}} READ {{property|getter_name}} WRITE {{property|setter_name}}  NOTIFY {{property.name}}Changed FINAL)
{% endfor %}
public:
    explicit {{zone_class}}(const QString &zone, {{class}}Interface *parent = nullptr);

{% for property in interface.properties %}
    {{ivi.prop_getter(property, model_interface = true)}};
{% endfor %}

public Q_SLOTS:
{% for property in interface.properties %}
    {{ivi.prop_setter(property, model_interface = true)}};
{% endfor %}

signals:
{% for property in interface.properties %}
    {{ivi.prop_notify(property, model_interface = true)}};
{% endfor %}

private:
    {{class}}Interface *m_parent;
    QString m_zone;
{% for property in interface.properties %}
{%   if property.type.is_model %}
    QIviPagingModelInterface *m_{{ property }};
{%   else %}
    {{ property|return_type }} m_{{ property }};
{%   endif %}
{% endfor %}
};
{% endif %}

class {{class}} : public {{class}}Interface
{
    Q_OBJECT

{% for property in interface.properties %}
{%   if property.type.is_model %}
{%     set type = 'QIviPagingModelInterface *' %}
{%   else %}
{%     set type = property|return_type %}
{%   endif %}
    Q_PROPERTY({{type}} {{property}} READ {{property|getter_name}} WRITE {{property|setter_name}}  NOTIFY {{property.name}}Changed FINAL)
{% endfor %}
    Q_PROPERTY(QQmlPropertyMap *zones READ zones CONSTANT)
public:
    explicit {{class}}(QObject *parent = nullptr);
    explicit {{class}}(QIviSimulationEngine *engine, QObject *parent = nullptr);
    ~{{class}}();

{%   if interface_zoned %}
    QStringList availableZones() const override;
{%   endif %}

    Q_INVOKABLE void initialize() override;
{% if interface_zoned %}
    void addZone(const QString &zone);
    {{zone_class}}* zoneAt(const QString &zone);
{% endif %}

{% for property in interface.properties %}
    {{ivi.prop_getter(property, model_interface = true)}};
{% endfor %}
    QQmlPropertyMap *zones() const { return m_zones; }

public Q_SLOTS:
{% for property in interface.properties %}
{%   if interface_zoned %}
    {{ivi.prop_setter(property)}} { {{property|setter_name}}({{property}}, QString()); }
{%   endif %}
{%   if not property.readonly and not property.const and not property.type.is_model %}
    virtual {{ivi.prop_setter(property, zoned = interface_zoned)}} override;
{%   else %}
    {{ivi.prop_setter(property, zoned = interface_zoned, model_interface = true)}};
{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
    virtual {{ivi.operation(operation, zoned = interface_zoned)}} override;
{% endfor %}

protected:
{% for property in interface.properties %}
{#{%   if not property.tags.config_simulator or not property.tags.config_simulator.zoned %}#}
{%     if property.type.is_model %}
    QIviPagingModelInterface *m_{{ property }};
{%     else %}
    {{ property|return_type }} m_{{ property }};
{%     endif %}
{#{%   endif %}#}
{% endfor %}
    QQmlPropertyMap *m_zones;

{% if 'simulator' in features %}
    QSimulatorConnection *mConnection;
    QSimulatorConnectionWorker *mWorker;
{% endif %}
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
