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
{% set class = '{0}'.format(interface) %}
{% set oncedefine = '{0}_{1}PRIVATE_H_'.format(module.module_name|upper, class|upper) %}
{% include 'generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "{{module.module_name|lower}}module.h"

{% if module.tags.config.disablePrivateIVI %}
#include <QObject>
{% else %}
{%   if interface.tags.config.zoned %}
#include <QtIviCore/private/qiviabstractzonedfeature_p.h>
{%   else %}
#include <QtIviCore/private/qiviabstractfeature_p.h>
{%   endif %}
{% endif %}

QT_BEGIN_NAMESPACE

class {{class}};

{% if module.tags.config.disablePrivateIVI %}
class {{class}}Private : public QObject
{% else %}
{%   if interface.tags.config.zoned %}
class {{class}}Private : public QIviAbstractZonedFeaturePrivate
{%   else %}
class {{class}}Private : public QIviAbstractFeaturePrivate
{%   endif %}
{% endif %}
{
public:
{% if module.tags.config.disablePrivateIVI %}
    {{class}}Private({{class}} *parent);
{% else %}
{%   if interface.tags.config.zoned %}
    {{class}}Private(const QString &interface, const QString &zone, {{class}} *parent);
{%   else %}
    {{class}}Private(const QString &interface, {{class}} *parent);
{%   endif %}
{% endif %}

    static {{class}}Private *get({{class}} *p);
    static const {{class}}Private *get(const {{class}} *p);
    {{class}} *getParent();

    void clearToDefaults();

{% for property in interface.properties %}
{%   if interface.tags.config.zoned %}
    void on{{property|upperfirst}}Changed({{property|parameter_type}}, const QString &zone);
{%   else %}
    void on{{property|upperfirst}}Changed({{property|parameter_type}});
{%   endif %}
{% endfor %}
{% for signal in interface.signals %}
{%   if interface.tags.config.zoned %}
    void on{{signal|upperfirst}}({{signal.parameters|map('parameter_type')|join(', ')}}, const QString &zone);
{%   else %}
    void on{{signal|upperfirst}}({{signal.parameters|map('parameter_type')|join(', ')}});
{%   endif %}
{% endfor %}

{% if not module.tags.config.disablePrivateIVI %}
    bool notify(const QByteArray &propertyName, const QVariant &value) override;

    {{class}} * const q_ptr;
{% endif %}
{% for property in interface.properties %}
    {{property|return_type}} m_{{property}};
{% endfor %}

{% if not module.tags.config.disablePrivateIVI %}
    Q_DECLARE_PUBLIC({{class}})
{% endif %}
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
