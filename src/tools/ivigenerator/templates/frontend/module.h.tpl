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
{% set exportsymbol = 'Q_{0}_EXPORT'.format(module|upper|replace('.', '_')) %}
{% set class = '{0}Module'.format(module.module_name|upperfirst) %}
{% set oncedefine = '{0}_H_'.format(class|upper) %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{module.module_name|lower}}global.h>
{% else %}
#include "{{module.module_name|lower}}global.h"
{% endif %}

{% for import in module.imports %}
{# All imports are provided including their imported version e.g. "Common 1.0". Because we need
   to access the Module type, we first need to split the version and then search for the name.
   See https://github.com/Pelagicore/qface/issues/87
#}
{%   set name = import.split(' ')[0] %}
{%   for mod in system.modules %}
{%     if mod.name == name %}
#include <{{mod.module_name|lower}}module.h>
{%     endif %}
{%   endfor %}
{% endfor %}

#include <QObject>

QT_BEGIN_NAMESPACE

class {{exportsymbol}} {{class}} : public QObject
{
    Q_OBJECT

public:
    {{class}}(QObject *parent=nullptr);

{% for enum in module.enums %}
    enum {{enum}} {
        {% for member in enum.members %}
        {{member.name}} = {{member.value}},
        {% endfor %}
    };
{% if enum.is_flag %}
    Q_DECLARE_FLAGS({{enum|flag_type}}, {{enum}})
    Q_FLAG({{enum|flag_type}})
{% else %}
    Q_ENUM({{enum}})
{% endif %}
    static {{enum|flag_type}} to{{enum|flag_type}}(quint32 v, bool *ok);
{% endfor %}

    static void registerTypes();
    static void registerQmlTypes(const QString& uri = QStringLiteral("{{module|qml_type}}"), int majorVersion = {{module.majorVersion}}, int minorVersion = {{module.minorVersion}});
};

{% for enum in module.enums %}
{{exportsymbol}} QDataStream &operator<<(QDataStream &out, {{class}}::{{enum|flag_type}} var);
{{exportsymbol}} QDataStream &operator>>(QDataStream &in, {{class}}::{{enum|flag_type}} &var);
{% endfor %}

QT_END_NAMESPACE

#endif // {{oncedefine}}
