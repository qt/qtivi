{#
# Copyright (C) 2019 Luxoft Sweden AB
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
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
{% set exportsymbol = 'Q_QT{0}_EXPORT'.format(module.module_name|upper) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{% include 'generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include "{{module.module_name|lower}}module.h"

#include <QObject>
#include <QHash>
#include <QVariantMap>

QT_BEGIN_NAMESPACE

class {{class}}Private;
class {{class}}BackendInterface;
class QSimulatorConnectionWorker;

class {{exportsymbol}} {{class}} : public QObject {
    Q_OBJECT
{% if interface_zoned %}
    Q_PROPERTY(QString currentZone READ currentZone NOTIFY currentZoneChanged)
    Q_PROPERTY(QStringList zones READ zones NOTIFY zonesChanged)
    Q_PROPERTY(QVariantMap zoneAt READ zoneAt NOTIFY zonesChanged)
{% endif %}
{% for property in interface.properties %}
{%   if not property.type.is_model %}
    Q_PROPERTY({{property|return_type}} {{property}} READ {{property|getter_name}} WRITE {{property|setter_name}} NOTIFY {{property}}Changed)
{%   endif %}
{% endfor %}
    Q_CLASSINFO("IviPropertyDomains", "{{ interface.properties|json_domain|replace("\"", "\\\"") }}")
public:
{% if interface_zoned %}
    explicit {{class}}(const QString &zone = QString(), QObject *parent = nullptr);
{% else %}
    explicit {{class}}(QObject *parent = nullptr);
{% endif %}
    ~{{class}}();

    static void registerQmlTypes(const QString& uri, int majorVersion=1, int minorVersion=0, const QString& qmlName = "");

{% if interface_zoned %}
    Q_INVOKABLE void addZone(const QString& newZone);
    QString currentZone() const;
    QStringList zones() const;
    QVariantMap zoneAt() const;
{% endif %}
{% for property in interface.properties %}
{%   if not property.type.is_model %}
    {{ivi.prop_getter(property)}};
{%   endif %}
{% endfor %}

public Q_SLOTS:
{% for property in interface.properties %}
{%   if not property.type.is_model %}
    {{ivi.prop_setter(property)}};
{%     if interface_zoned %}
    {{ivi.prop_setter(property, zoned = true)}};
{%     endif %}
{%   endif %}
{% endfor %}
{% for signal in interface.signals %}
    {{ivi.signal(signal)}};
{% endfor %}

Q_SIGNALS:
{% for operation in interface.operations %}
    void {{operation}}({{ivi.join_params(operation)}});
{% endfor %}
{% if interface_zoned %}
    void currentZoneChanged();
    void zonesChanged();
{% endif %}
{% for property in interface.properties %}
{%   if not property.type.is_model %}
    {{ivi.prop_notify(property)}};
{%   endif %}
{% endfor %}

private Q_SLOTS:
{% if interface_zoned %}
{%   for operation in interface.operations %}
    void {{operation}}({{ivi.join_params(operation, zoned=true)}});
{%   endfor %}
{% endif %}
private:
    QSimulatorConnectionWorker *worker();
{%   for property in interface.properties %}
{%     if not property.type.is_model %}
    {{ property|return_type }} m_{{ property }};
{%     endif %}
{%   endfor %}
{% if interface_zoned %}
    QHash<QString,{{class}}*> m_zoneHash;
    QVariantMap m_zoneMap;
    QString m_currentZone;
{% endif %}
    QSimulatorConnectionWorker *m_worker;
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
