{#
# Copyright (C) 2017 Pelagicore AG
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
{% include "generated_comment.cpp.tpl" %}
{% set class = '{0}Backend'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QRemoteObjectNode>
{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{class}}Interface>
{% else %}
#include "{{class|lower}}interface.h"
{% endif %}
#include "{{interface|lower}}replica.h"

QT_BEGIN_NAMESPACE

class {{class}} : public {{class}}Interface
{
    Q_OBJECT

public:
    explicit {{class}}(QObject *parent = nullptr);
    ~{{class}}();

    void initialize() override;

public Q_SLOTS:
{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
    virtual void set{{property|upperfirst}}({{ property|parameter_type }}) override;
{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
    virtual {{operation|return_type}} {{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}){%if operation.const %} const{% endif %} override;
{% endfor %}

    void onError(QRemoteObjectNode::ErrorCode code);

protected:
    void setupConnections();

    QSharedPointer<{{interface}}Replica> m_replica;
    QRemoteObjectNode* m_node= nullptr;
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
