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
{% include 'generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include "{{module.module_name|lower}}module.h"

{% if interface.tags.config.zoned %}
#include <QtIviCore/QIviAbstractZonedFeature>
{% else %}
#include <QtIviCore/QIviAbstractFeature>
{% endif %}

QT_BEGIN_NAMESPACE

class {{class}}Private;
class {{class}}BackendInterface;

{% if interface.tags.config.zoned %}
class {{exportsymbol}} {{class}} : public QIviAbstractZonedFeature {
{% else %}
class {{exportsymbol}} {{class}} : public QIviAbstractFeature {
{% endif %}
    Q_OBJECT
{% for property in interface.properties %}
    Q_PROPERTY({{property|return_type}} {{property}} READ {{property}}{% if not property.readonly and not property.const %} WRITE set{{property|upperfirst}}{% endif %} NOTIFY {{property}}Changed)
{% endfor %}
public:
{% if interface.tags.config.zoned %}
    explicit {{class}}(const QString &zone = QString(), QObject *parent = nullptr);
{% else %}
    explicit {{class}}(QObject *parent = nullptr);
{% endif %}
    ~{{class}}();

    static void registerQmlTypes(const QString& uri, int majorVersion=1, int minorVersion=0);

{% for property in interface.properties %}
    {{property|return_type}} {{property}}() const;
{% endfor %}

public Q_SLOTS:
{% for operation in interface.operations %}
    {{operation|return_type}} {{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}){% if operation.const %} const{% endif %};
{% endfor %}
{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
    void set{{property|upperfirst}}({{property|parameter_type}});
{%   endif %}
{% endfor %}

Q_SIGNALS:
{% for signal in interface.signals %}
    void {{signal}}({{signal.parameters|map('parameter_type')|join(', ')}});
{% endfor %}
{% for property in interface.properties %}
    void {{property}}Changed({{property|parameter_type}});
{% endfor %}

protected:
{% if interface.tags.config.zoned %}
    QIviAbstractZonedFeature *createZoneFeature(const QString &zone) Q_DECL_OVERRIDE;
{% else %}
    {{class}}BackendInterface *backend() const;
{% endif %}
    void connectToServiceObject(QIviServiceObject *service) Q_DECL_OVERRIDE;
    void clearServiceObject() Q_DECL_OVERRIDE;

private:
{% if module.tags.config.disablePrivateIVI %}
    friend class {{class}}Private;
    {{class}}Private *m_helper;
{% else %}
{% for property in interface.properties %}
{%   if interface.tags.config.zoned %}
    Q_PRIVATE_SLOT(d_func(), void on{{property|upperfirst}}Changed({{property|parameter_type}}, const QString &))
{%   else %}
    Q_PRIVATE_SLOT(d_func(), void on{{property|upperfirst}}Changed({{property|parameter_type}}))
{%   endif %}
{% endfor %}
    Q_DECLARE_PRIVATE({{class}})
{% endif %}
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
