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
{% include 'generated_comment.cpp.tpl' %}

#include "{{class|lower}}.h"
#include "{{class|lower}}_p.h"
#include "{{class|lower}}backendinterface.h"

#include <QQmlEngine>
#include <QIviServiceObject>

QT_BEGIN_NAMESPACE

{% if module.tags.config.disablePrivateIVI %}
{{class}}Private::{{class}}Private({{class}} *q)
    : QObject(q)
{% else %}
{%   if interface.tags.config.zoned %}
{{class}}Private::{{class}}Private(const QString &interface, const QString &zone, {{class}} *parent)
    : QIviAbstractZonedFeaturePrivate(interface, zone, parent)
{%   else %}
{{class}}Private::{{class}}Private(const QString &interface, {{class}} *parent)
    : QIviAbstractFeaturePrivate(interface, parent)
{%   endif %}
    , q_ptr(parent)
{% endif %}
{% for property in interface.properties %}
    , m_{{property}}({{property|defaultValue}})
{% endfor %}
{
}

{{class}}Private *{{class}}Private::get({{class}} *v)
{
{% if module.tags.config.disablePrivateIVI %}
    return v->m_helper;
{% else %}
    return ({{class}}Private *) v->d_ptr.data();
{% endif %}
}

const {{class}}Private *{{class}}Private::get(const {{class}} *v)
{
{% if module.tags.config.disablePrivateIVI %}
    return v->m_helper;
{% else %}
    return (const {{class}}Private *) v->d_ptr.data();
{% endif %}
}

{{class}} *{{class}}Private::getParent()
{
{% if module.tags.config.disablePrivateIVI %}
    return qobject_cast<{{class}} *>(parent());
{% else %}
    return q_ptr;
{% endif %}
}

void {{class}}Private::clearToDefaults()
{
{% for property in interface.properties %}
    m_{{property}} = {{property|defaultValue}};
{% endfor %}
}

{% for property in interface.properties %}
{% if interface.tags.config.zoned %}
void {{class}}Private::on{{property|upperfirst}}Changed({{property|parameterType}}, const QString &zone)
{
    auto q = getParent();
    auto f = qobject_cast<{{class}}*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    {{class}}Private::get(f)->m_{{property}} = {{property}};
    emit f->{{property}}Changed({{property}});
}
{% else %}
void {{class}}Private::on{{property|upperfirst}}Changed({{property|parameterType}})
{
    if (m_{{property}} != {{property}}) {
        auto q = getParent();
        m_{{property}} = {{property}};
        emit q->{{property}}Changed({{property}});
    }
}
{% endif %}

{% endfor %}

{% if module.tags.config.disablePrivateIVI %}
{%   if interface.tags.config.zoned %}
{{class}}::{{class}}(const QString &zone, QObject *parent)
    : QIviAbstractZonedFeature({{module.module_name}}_{{interface}}_iid, zone, parent)
{%   else %}
{{class}}::{{class}}(QObject *parent)
    : QIviAbstractFeature({{module.module_name}}_{{interface}}_iid, parent)
{%   endif %}
    , m_helper(new {{class}}Private(this))
{% else %}
{%   if interface.tags.config.zoned %}
{{class}}::{{class}}(const QString &zone, QObject *parent)
    : QIviAbstractZonedFeature(*new {{class}}Private({{module.module_name}}_{{interface}}_iid, zone, this), parent)
{%   else %}
{{class}}::{{class}}(QObject *parent)
    : QIviAbstractFeature(*new {{class}}Private({{module.module_name}}_{{interface}}_iid, this), parent)
{%   endif %}
{% endif %}
{
}

{{class}}::~{{class}}()
{
{% if module.tags.config.disablePrivateIVI %}
    delete m_helper;
{% endif %}
}

void {{class}}::registerQmlTypes(const QString& uri, int majorVersion, int minorVersion)
{
{% if 'singleton' in interface.tags %}
    qmlRegisterSingletonType<{{class}}>(uri.toLatin1(), majorVersion, minorVersion, "{{interface}}", {{class|lower}}_singletontype_provider);
{% else %}
    qmlRegisterType<{{class}}>(uri.toLatin1(), majorVersion, minorVersion, "{{interface}}");
{% endif %}
}

{% for property in interface.properties %}
void {{class}}::set{{property|upperfirst}}({{ property|parameterType }})
{
    auto d = {{class}}Private::get(this);
    if (d->m_{{property}} == {{property}})
        return;
    d->m_{{property}} = {{property}};
    emit {{property}}Changed({{property}});
}

{{property|returnType}} {{class}}::{{property}}() const
{
    const auto d = {{class}}Private::get(this);
    return d->m_{{property}};
}
{% endfor %}

{%- for operation in interface.operations %}
{{operation|returnType}} {{class}}::{{operation}}({{operation.parameters|map('parameterType')|join(', ')}})
{
    if ({{class}}BackendInterface *backend = ({{class}}BackendInterface *) this->backend())
{% if interface.tags.config.zoned %}
{% if operation.parameters|length %}
        return backend->{{operation}}({{operation.parameters|join(', ')}}, zone());
{% else %}
        return backend->{{operation}}(zone());
{% endif %}
{% else %}
        return backend->{{operation}}({{operation.parameters|join(', ')}});
{% endif %}
    return {{operation|defaultValue}};
}
{% endfor %}

{% if interface.tags.config.zoned %}
QIviAbstractZonedFeature *{{class}}::createZoneFeature(const QString &zone)
{
    return new {{class}}(zone, this);
}
{% endif %}

void {{class}}::connectToServiceObject(QIviServiceObject *serviceObject)
{
{% if interface.tags.config.zoned %}
    QIviAbstractZonedFeature::connectToServiceObject(serviceObject);
{% else %}
    Q_UNUSED(serviceObject);
{% endif %}
    auto d = {{class}}Private::get(this);

    auto *backend = qobject_cast<{{class}}BackendInterface*>(this->backend());
    if (!backend)
        return;

{% for property in interface.properties %}
{% if module.tags.config.disablePrivateIVI %}{% set Connect = 'QObject::connect' %}
{% else %}{% set Connect = 'QObjectPrivate::connect' %}{% endif %}
    {{Connect}}(backend, &{{class}}BackendInterface::{{property}}Changed,
        d, &{{class}}Private::on{{property|upperfirst}}Changed);
{% endfor %}
}

void {{class}}::clearServiceObject()
{
    auto d = {{class}}Private::get(this);
    d->clearToDefaults();
{% if interface.tags.config.zoned %}
    QIviAbstractZonedFeature::clearServiceObject();
{% endif %}
}

{% if interface.tags.config.zoned != True %}
{{class}}BackendInterface *{{class}}::backend() const
{
    if (QIviServiceObject *so = serviceObject())
        return qobject_cast<{{class}}BackendInterface*>(so->interfaceInstance(interfaceName()));
    return nullptr;
}
{% endif %}

QT_END_NAMESPACE

#include "moc_{{class|lower}}.cpp"
