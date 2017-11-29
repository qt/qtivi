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
{% set exportsymbol = 'Q_{0}_EXPORT'.format(module.module_name|upper) %}
{% set class = '{0}Module'.format(module.module_name) %}
{% set oncedefine = '{0}_H_'.format(class|upper) %}
{% include 'generated_comment.cpp.tpl' %}
{% import 'utils.tpl' as utils %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include "{{module.module_name|lower}}global.h"
#include <QObject>

QT_BEGIN_NAMESPACE

{% for struct in module.structs %}
class {{struct}}Model;
{% endfor %}

class {{exportsymbol}} {{class}} : public QObject {
    Q_OBJECT
public:
    {{class}}(QObject *parent=nullptr);

{% for enum in module.enums %}
    enum {{enum}} {
        {% for member in enum.members %}
        {{member.name}} = {{member.value}}, {{member.comment}}
        {% endfor %}
    };
{% if enum.is_flag %}
    Q_DECLARE_FLAGS({{enum|flag_type}}, {{enum}})
    Q_FLAG({{enum|flag_type}})
{% else %}
    Q_ENUM({{enum}})
{% endif %}
    static {{enum}} to{{enum}}(quint8 v, bool *ok);
{% endfor %}

    static void registerTypes();
    static void registerQmlTypes(const QString& uri, int majorVersion = 1, int minorVersion = 0);
};

{% for enum in module.enums %}
QDataStream &operator<<(QDataStream &out, {{class}}::{{enum|flag_type}} var);
QDataStream &operator>>(QDataStream &in, {{class}}::{{enum|flag_type}} &var);
{% endfor %}

QT_END_NAMESPACE

#endif // {{oncedefine}}
