{#
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
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
{% import 'common/qtivi_macros.j2' as ivi %}
{% set class = '{0}BackendInterface'.format(interface) %}
{% include 'common/generated_comment.cpp.tpl' %}

#include "{{class|lower}}.h"

QT_BEGIN_NAMESPACE

/*!
    \class {{class}}
    \inmodule {{module}}
    \ingroup backends
{% if interface.tags.config.zoned %}
    \inherits QIviZonedFeatureInterface
{% else %}
    \inherits QIviFeatureInterface
{% endif %}
    \keyword {{interface.tags.config.id | default(interface.qualified_name)}}

    \brief Backend interface for {{interface}}.

    The {{class}} is the interface used by \l {{interface}}.

    The interface is discovered by a \l {{interface}} object, which connects to it and sets up
    the connections to it.

    \sa {{interface}}
 */
{{class}}::{{class}}(QObject *parent)
{% if interface.tags.config.zoned %}
    : QIviZonedFeatureInterface(parent)
{% else %}
    : QIviFeatureInterface(parent)
{% endif %}
{
}

{{class}}::~{{class}}()
{
}

{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
/*!
    \fn {{ivi.prop_setter(property, class, interface.tags.config.zoned)}};

    Setter for {{interface}}::{{property}}.
    Sets the property \e {{property}} to the new value passed by \a {{property}}.

{%     if interface.tags.config.zoned %}
    The value of \a zone indicates the zone this property should be changed in.
{%     endif %}

    This method is expected to emit a \l {{property}}Changed() signal when the internal state changes
    due to this function call. The signal is even expected to be emitted if the given \a {{property}} is out of range and no
    actual change takes place.

    \sa {{property}}Changed()
*/
{%   endif %}
{% endfor %}
{% for operation in interface.operations %}
/*!
    \fn {{ivi.operation(operation, class, interface.tags.config.zoned)}};

{{ ivi.format_comments(operation.comment) }}

{%   if interface.tags.config.zoned %}
    The value of \a zone indicates the zone this operation should be done in.
{%   endif %}
*/
{% endfor %}

{% for signal in interface.signals %}
/*!
    \fn {{ivi.signal(signal, class, interface.tags.config.zoned)}};

{{ ivi.format_comments(signal.comment) }}

{%   if interface.tags.config.zoned %}
    The value of \a zone indicates the zone this operation should be done in.
{%   endif %}
*/
{% endfor %}
{% for property in interface.properties %}
/*!
    \fn {{ivi.prop_notify(property, class, interface.tags.config.zoned)}};

    The signal is emitted when the \e {{property}} property changed to \a {{property}}.

{%   if interface.tags.config.zoned %}
    The value of \a zone indicates the zone this property should be changed in.
{%   endif %}

{%   if not property.readonly and not property.const %}
    \sa {{property|setter_name}}
{%   endif %}
    \sa {{interface}}::{{property}}
*/
{% endfor %}

QT_END_NAMESPACE
