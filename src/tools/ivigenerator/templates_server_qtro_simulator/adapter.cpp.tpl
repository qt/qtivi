{#
# Copyright (C) 2019 Luxoft Sweden AB
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
{% set class = '{0}QtRoAdapter'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
#include "{{interface|lower}}adapter.h"
Q_LOGGING_CATEGORY(qLcRO{{interface}}, "{{module|qml_type|lower}}.{{interface|lower}}backend.remoteobjects", QtInfoMsg)

/*
* A simple QtRO Adapter which is derived from the repc generated *Source class
* Every setter/getter and signal is simply forwarded to the original backend
* Every operation is adapted to use the special PendingResult class as a return value
* to inform the client that the return value is not yet ready. Once the QIviPendingReply is ready
* the value is send with the pendingResultAvailable value
*/
{{class}}::{{class}}({{interface}}Backend *parent)
    : {{interface}}Source(parent)
    , m_backend(parent)
    , m_replyCounter(0)
{
{% for property in interface.properties %}
{%   if not property.type.is_model %}
    connect(m_backend, &{{interface}}Backend::{{property.name}}Changed, this, &{{class}}::{{property.name}}Changed);
{%   endif %}
{% endfor %}
{% for signal in interface.signals %}
    connect(m_backend, &{{interface}}Backend::{{signal}}, this, &{{class}}::{{signal}});
{% endfor %}
}

{% if interface_zoned %}
QStringList {{class}}::availableZones()
{
    return m_backend->availableZones();
}
{% endif %}

{% for property in interface.properties %}
{%   if not property.type.is_model %}
{%     if interface_zoned %}
{{property|return_type}} {{class}}::{{property|getter_name}}(const QString &zone)
{
    return m_backend->{{property|getter_name}}(zone);
}
{%     else %}
{{ivi.prop_getter(property, class, model_interface = true)}}
{
    return m_backend->{{property|getter_name}}();
}
{%     endif %}
{%   endif %}
{% endfor %}

{% for property in interface.properties %}
{%   if not property.readonly and not property.const and not property.type.is_model %}
{{ivi.prop_setter(property, class, interface_zoned, model_interface = true)}}
{
{%     set parameters = property.name %}
{%     if interface_zoned %}
{%       set parameters = parameters + ', zone' %}
{%     endif%}
    m_backend->{{property|setter_name}}({{parameters}});
}

{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
QVariant {{class}}::{{operation}}({{ivi.join_params(operation, zoned = interface_zoned)}})
{
{%   set function_parameters = operation.parameters|join(', ') %}
{%   if interface_zoned %}
{%     if operation.parameters|length %}
{%       set function_parameters = function_parameters + ', ' %}
{%     endif %}
{%     set function_parameters = function_parameters + 'zone' %}
{%   endif%}
    QIviPendingReplyBase pendingReply = m_backend->{{operation}}({{function_parameters}});
    qCDebug(qLcRO{{interface}}) << "{{operation}} called";
    if (pendingReply.isSuccessful()) {
        qCDebug(qLcRO{{interface}}) << "Returning result right away";
        return pendingReply.value();
    } else { //reply not yet ready or failed
        const quint64 id = ++m_replyCounter;
        if (pendingReply.isResultAvailable()) { // the call failed
            qCDebug(qLcRO{{interface}}) << "Returning failed reply";
            return QVariant::fromValue({{interface}}PendingResult(id, true /* failed */));
        }
        {{interface}}PendingResult result = {{interface}}PendingResult(id, false /* failed */);
        qCDebug(qLcRO{{interface}}) << "Returning a pending result: id:" << id;
        connect(pendingReply.watcher(), &QIviPendingReplyWatcher::valueChanged, this, [this, pendingReply, id] (const QVariant &value) {
            qCDebug(qLcRO{{interface}}) << "Value for pending result available: id:" << id << "value:" << value;
            emit pendingResultAvailable(id, pendingReply.isSuccessful(), value);
        });
        return QVariant::fromValue(result);
    }

    return QVariant();
}
{% endfor %}
