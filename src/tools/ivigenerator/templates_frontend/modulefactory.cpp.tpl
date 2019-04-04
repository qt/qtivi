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
{% set class = '{0}ModuleFactory'.format(module.module_name|upperfirst) %}
{% set qml_name = (module|qml_type).split('.')[-1]|upperfirst %}
{% include 'common/generated_comment.cpp.tpl' %}

#include "{{class|lower}}.h"

QT_BEGIN_NAMESPACE

/*!
    \class {{class}}
    \inmodule {{module}}

    \brief The {{class}} class provides factory methods for all structs defined in the {{module}} module.
*/
/*!
    \qmltype {{qml_name}}
    \instantiates {{class}}
    \inqmlmodule {{module|qml_type}}

    \brief The {{qml_name}} singleton holds all the enums defined in the {{module}} module and
    provides factory methods for all structs.

    The following enums are exported from this object:

{% for enum in module.enums %}
    \section3 {{enum}}
    \include {{module|lower}}module_enum.qdocinc {{enum}}
{% endfor %}
*/
{{class}}::{{class}}(QObject *parent)
    : {{module.module_name|upperfirst}}Module(parent)
{
}

{% for struct in module.structs %}
/*!
    \qmlmethod {{struct}} {{module|qml_type}}::{{qml_name}}()

    Returns a default instance of {{struct}}

    \sa {{struct}}
*/
/*!
    Returns a default instance of {{struct}}.

    \sa {{struct}}
*/
{{struct}} {{class}}::{{struct|lowerfirst}}() const
{
    return {{struct}}();
}

/*!
    \qmlmethod {{struct}} {{module|qml_type}}::{{qml_name}}({{struct.fields|map('parameter_type')|join(', ')}})

    Returns a default instance of {{struct}}

    \sa {{struct}}
*/
/*!
    Returns a instance of {{struct}} using the passed arguments.

{% for field in struct.fields %}
{%   if field.type.is_enum or field.type.is_flag %}
    Available values for {{field}} are:
    \include {{module|lower}}module_enum.qdocinc {{field.type}}
{%   endif %}

{% endfor %}

    \sa {{struct}}
*/
{{struct}} {{class}}::{{struct|lowerfirst}}({{struct.fields|map('parameter_type')|join(', ')}}) const
{
    return {{struct}}({{struct.fields|join(', ')}});
}

{% endfor %}

QT_END_NAMESPACE
