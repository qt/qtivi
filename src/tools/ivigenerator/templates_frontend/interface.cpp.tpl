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
{% import 'utils.tpl' as utils %}

#include "{{class|lower}}.h"
#include "{{class|lower}}_p.h"
#include "{{class|lower}}backendinterface.h"

{% for property in interface.properties %}
{% if property.type.is_model %}
#include "{{property|model_type|lower}}.h"
{% endif %}
{% endfor %}

#include <QQmlEngine>
#include <QIviServiceObject>

QT_BEGIN_NAMESPACE

/*!
    \class {{interface}}
    \inmodule {{module}}
{{ utils.format_comments(interface.comment) }}
*/

/*!
    \qmltype {{interface|qml_type}}
    \instantiates {{interface}}
{% if module.tags.config.qml_name is defined %}
    \inqmlmodule {{module.tags.config.qml_name}}
{% endif %}
{% if interface.tags.config.zoned %}
    \inherits AbstractZonedFeature
{% else %}
    \inherits AbstractFeature
{% endif %}

{{ utils.format_comments(interface.comment) }}
*/

/*! \internal */
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
    , m_{{property}}({{property|default_type_value}})
{% endfor %}
{
{% if not module.tags.config.disablePrivateIVI %}
    m_supportsPropertyOverriding = true;
{% endif %}
}

/*! \internal */
{{class}}Private *{{class}}Private::get({{class}} *v)
{
{% if module.tags.config.disablePrivateIVI %}
    return v->m_helper;
{% else %}
    return ({{class}}Private *) v->d_ptr.data();
{% endif %}
}

/*! \internal */
const {{class}}Private *{{class}}Private::get(const {{class}} *v)
{
{% if module.tags.config.disablePrivateIVI %}
    return v->m_helper;
{% else %}
    return (const {{class}}Private *) v->d_ptr.data();
{% endif %}
}

/*! \internal */
{{class}} *{{class}}Private::getParent()
{
{% if module.tags.config.disablePrivateIVI %}
    return qobject_cast<{{class}} *>(parent());
{% else %}
    return q_ptr;
{% endif %}
}

/*! \internal */
void {{class}}Private::clearToDefaults()
{
{% for property in interface.properties %}
    m_{{property}} = {{property|default_type_value}};
{% endfor %}
}

{% for property in interface.properties %}
/*! \internal */
{%   if interface.tags.config.zoned %}
void {{class}}Private::on{{property|upperfirst}}Changed({{property|parameter_type}}, const QString &zone)
{
    auto q = getParent();
    auto f = qobject_cast<{{class}}*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
{% if not module.tags.config.disablePrivateIVI %}
    if (Q_UNLIKELY(m_propertyOverride)) {
        const int pi = f->metaObject()->indexOfProperty("{{property}}");
        if (m_propertyOverride->isOverridden(pi)) {
            QVariant v = qVariantFromValue<{{property|return_type}}>({{property}});
            m_propertyOverride->setProperty(pi, v);
            return;
        }
    }
{% endif %}
    if ({{class}}Private::get(f)->m_{{property}} != {{property}}) {
        {{class}}Private::get(f)->m_{{property}} = {{property}};
        emit f->{{property}}Changed({{property}});
    }
}
{%   else %}
void {{class}}Private::on{{property|upperfirst}}Changed({{property|parameter_type}})
{
    if (m_{{property}} != {{property}}) {
        auto q = getParent();
        m_{{property}} = {{property}};
        emit q->{{property}}Changed({{property}});
    }
}
{%   endif %}

{% endfor %}
{% for signal in interface.signals %}
/*! \internal */
{%   if interface.tags.config.zoned %}
void {{class}}Private::on{{signal|upperfirst}}({{signal.parameters|map('parameter_type')|join(', ')}}, const QString &zone)
{
    auto q = getParent();
    auto f = qobject_cast<{{class}}*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    emit f->{{signal}}({{signal.parameters|join(', ')}});
}
{%   else %}
void {{class}}Private::on{{signal|upperfirst}}({{signal.parameters|map('parameter_type')|join(', ')}})
{
    auto q = getParent();
    emit q->{{signal}}({{signal.parameters|join(', ')}});
}
{%   endif %}

{% endfor %}

{% if not module.tags.config.disablePrivateIVI %}
bool {{class}}Private::notify(const QByteArray &propertyName, const QVariant &value)
{
    auto q = getParent();
{%   for property in interface.properties %}
    if (propertyName == QByteArray("{{property}}")) {
        emit q->{{property}}Changed(value.value<{{property|return_type}}>());
        return true;
    }
{%   endfor %}
{%   if interface.tags.config.zoned %}
    return QIviAbstractZonedFeaturePrivate::notify(propertyName, value);
{%   else %}
    return QIviAbstractFeaturePrivate::notify(propertyName, value);
{%   endif %}
}
{% endif %}

{% if module.tags.config.disablePrivateIVI %}
{%   if interface.tags.config.zoned %}
/*!
   Default constructs an instance of {{class}} to the given \a zone.

   If \a zone is not provided the General zone will be created.

   The \a parent argument is passed on to the \l QIviAbstractZonedFeature base class.
*/
{{class}}::{{class}}(const QString &zone, QObject *parent)
    : QIviAbstractZonedFeature(QLatin1String({{module.module_name}}_{{interface}}_iid), zone, parent)
{%   else %}
/*!
    Default constructs an instance of {{class}}.
*/
{{class}}::{{class}}(QObject *parent)
    : QIviAbstractFeature({{module.module_name}}_{{interface}}_iid, parent)
{%   endif %}
    , m_helper(new {{class}}Private(this))
{% else %}
{%   if interface.tags.config.zoned %}
/*!
   Default constructs an instance of {{class}} to the given \a zone.

   If \a zone is not provided the General zone will be created.

   The \a parent argument is passed on to the \l QIviAbstractZonedFeature base class.
*/
{{class}}::{{class}}(const QString &zone, QObject *parent)
    : QIviAbstractZonedFeature(*new {{class}}Private(QLatin1String({{module.module_name}}_{{interface}}_iid), zone, this), parent)
{%   else %}
/*!
    Default constructs an instance of {{class}}.
*/
{{class}}::{{class}}(QObject *parent)
    : QIviAbstractFeature(*new {{class}}Private(QLatin1String({{module.module_name}}_{{interface}}_iid), this), parent)
{%   endif %}
{% endif %}
{
}

/*! \internal */
{{class}}::~{{class}}()
{
{% if module.tags.config.disablePrivateIVI %}
    delete m_helper;
{% endif %}
}

/*! \internal */
void {{class}}::registerQmlTypes(const QString& uri, int majorVersion, int minorVersion)
{
{% if 'singleton' in interface.tags %}
    qmlRegisterSingletonType<{{class}}>(uri.toLatin1(), majorVersion, minorVersion, "{{interface}}", {{class|lower}}_singletontype_provider);
{% else %}
    qmlRegisterType<{{class}}>(uri.toLatin1(), majorVersion, minorVersion, "{{interface|qml_type}}");
{% endif %}
}

{% for property in interface.properties %}

/*!
    \property {{class}}::{{property}}
{{ utils.format_comments(property.comment) }}
{% if property.const %}
    \note This property is constant and the value will not change once the plugin is initialized.
{% endif %}
*/

/*!
    \qmlproperty {{property|return_type}} {{interface|qml_type}}::{{property}}
{{ utils.format_comments(property.comment) }}
{% if property.const %}
    \note This property is constant and the value will not change once the plugin is initialized.
{% endif %}
*/
{{property|return_type}} {{class}}::{{property|getter_name}}() const
{
    const auto d = {{class}}Private::get(this);
{% if not module.tags.config.disablePrivateIVI %}
    if (Q_UNLIKELY(d->m_propertyOverride))
        return d->m_propertyOverride->property(metaObject()->indexOfProperty("{{property}}")).value<{{property|return_type}}>();
{% endif %}
    return d->m_{{property}};
}
{%   if not property.readonly and not property.const %}

void {{class}}::{{property|setter_name}}({{ property|parameter_type }})
{
    auto d = {{class}}Private::get(this);
    bool forceUpdate = false;
{% if not module.tags.config.disablePrivateIVI %}
    if (Q_UNLIKELY(d->m_propertyOverride)) {
        const int pi = metaObject()->indexOfProperty("{{property}}");
        if (d->m_propertyOverride->isOverridden(pi)) {
            emit {{property}}Changed(d->m_propertyOverride->property(pi).value<{{property|return_type}}>());
            return;
        }
        forceUpdate = property("{{property}}DirtyOverride").isValid();
        if (forceUpdate)
            setProperty("{{property}}DirtyOverride", {});
        QVariant v = qVariantFromValue<{{property|return_type}}>({{property}});
        d->m_propertyOverride->setProperty(pi, v);
    }
{% endif %}
    if (!forceUpdate && d->m_{{property}} == {{property}})
        return;
    d->m_{{property}} = {{property}};
    if ({{class}}BackendInterface *backend = qobject_cast<{{class}}BackendInterface *>(this->backend()))
        backend->{{property|setter_name}}({{property}}{% if interface.tags.config.zoned %}, zone(){% endif %});
    emit {{property}}Changed({{property}});
}
{%   endif %}

{% endfor %}

{%- for operation in interface.operations %}
/*!
    \qmlmethod {{interface|qml_type}}::{{operation}}({{operation.parameters|map('parameter_type')|join(', ')}})
{{ utils.format_comments(operation.comment) }}
*/
/*!
{{ utils.format_comments(operation.comment) }}
*/
{{operation|return_type}} {{class}}::{{operation}}({{operation.parameters|map('parameter_type')|join(', ')}}){% if operation.const %} const{% endif %}

{
    if ({{class}}BackendInterface *backend = ({{class}}BackendInterface *) this->backend())
{% if interface.tags.config.zoned %}
{%   if operation.parameters|length %}
        return backend->{{operation}}({{operation.parameters|join(', ')}}, zone());
{%   else %}
        return backend->{{operation}}(zone());
{%   endif %}
{% else %}
        return backend->{{operation}}({{operation.parameters|join(', ')}});
{% endif %}
    return {{operation|default_type_value}};
}

{% endfor %}

{% if interface.tags.config.zoned %}
/*! \internal */
QIviAbstractZonedFeature *{{class}}::createZoneFeature(const QString &zone)
{
    return new {{class}}(zone, this);
}
{% endif %}

/*! \internal */
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
{% for signal in interface.signals %}
{% if module.tags.config.disablePrivateIVI %}{% set Connect = 'QObject::connect' %}
{% else %}{% set Connect = 'QObjectPrivate::connect' %}{% endif %}
    {{Connect}}(backend, &{{class}}BackendInterface::{{signal}},
        d, &{{class}}Private::on{{signal|upperfirst}});
{% endfor %}

    backend->initialize();
}

/*! \internal */
void {{class}}::clearServiceObject()
{
    auto d = {{class}}Private::get(this);
    d->clearToDefaults();
{% if interface.tags.config.zoned %}
    QIviAbstractZonedFeature::clearServiceObject();
{% endif %}
}

{% if not interface.tags.config.zoned %}
/*! \internal */
{{class}}BackendInterface *{{class}}::backend() const
{
    if (QIviServiceObject *so = serviceObject())
        return qobject_cast<{{class}}BackendInterface*>(so->interfaceInstance(interfaceName()));
    return nullptr;
}
{% endif %}

QT_END_NAMESPACE

#include "moc_{{class|lower}}.cpp"
