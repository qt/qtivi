{#
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
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
{% set class = '{0}Backend'.format(interface) %}
{% set zone_class = '{0}Zone'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
#include "{{class|lower}}.h"

#include <QDebug>
#include <QSettings>
#include "{{module.module_name|lower}}module.h"

Q_LOGGING_CATEGORY(qLcRO{{interface}}, "{{module|qml_type|lower}}.{{interface|lower}}backend.remoteobjects", QtInfoMsg)

{% for property in interface.properties %}
{%   if property.type.is_model %}
{% include "pagingmodel.cpp.tpl" %}
{%   endif %}
{% endfor %}

QT_BEGIN_NAMESPACE

{% if interface_zoned %}
{{zone_class}}::{{zone_class}}(const QString &zone, {{class}} *parent)
    : QObject(parent)
    , m_parent(parent)
    , m_zone(zone)
{% for property in interface.properties %}
{%   if not property.type.is_model %}
    , m_{{ property }}({{property|default_value}})
{%   endif %}
{% endfor %}
{
}

bool {{zone_class}}::isSyncing()
{
    return !m_propertiesToSync.isEmpty();
}

void {{zone_class}}::sync()
{
{% for property in interface.properties %}
    m_propertiesToSync.append(QStringLiteral("{{property}}"));
{% endfor %}

{% for property in interface.properties %}
    QRemoteObjectPendingReply<{{property|return_type}}> {{property}}Reply = m_parent->m_replica->{{property|getter_name}}(m_zone);
    auto {{property}}Watcher = new QRemoteObjectPendingCallWatcher({{property}}Reply);
    connect({{property}}Watcher, &QRemoteObjectPendingCallWatcher::finished, this, [this](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
            m_{{property}} = self->returnValue().value<{{property|return_type}}>();
            m_propertiesToSync.removeAll(QStringLiteral("{{property}}"));
            checkSync();
        }
        self->deleteLater();
    });
{% endfor %}
}

void {{zone_class}}::checkSync()
{
    if (m_propertiesToSync.isEmpty())
        emit syncDone();
}

void {{zone_class}}::emitCurrentState()
{
{% for property in interface.properties %}
    m_parent->{{property}}Changed(m_{{property}}, m_zone);
{% endfor %}
}

{% for property in interface.properties %}
{{ivi.prop_setter(property, zone_class, model_interface = true)}}
{
    m_{{property}} = {{property}};
    emit m_parent->{{property}}Changed({{property}}, m_zone);
}
{% endfor %}
{% endif %}

{{class}}::{{class}}(QObject *parent)
    : {{class}}Interface(parent)
{% for property in interface.properties %}
{%   if property.type.is_model %}
    , m_{{property}}(new {{property|upperfirst}}ModelBackend(this))
{%   endif %}
{% endfor %}
{% if interface_zoned %}
    , m_synced(false)
{% endif %}
{
    {{module.module_name|upperfirst}}Module::registerTypes();

    QString configPath(QStringLiteral("./server.conf"));
    if (qEnvironmentVariableIsSet("SERVER_CONF_PATH"))
        configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
    else
        qDebug() << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("{{module.module_name|lower}}"));
    m_url = QUrl(settings.value(QStringLiteral("Registry"), QStringLiteral("local:{{module.module_name|lower}}")).toString());
    m_node = new QRemoteObjectNode(m_url);
    connect(m_node, &QRemoteObjectNode::error, this, &{{class}}::onNodeError);
    m_replica.reset(m_node->acquire<{{interface}}Replica>(QStringLiteral("{{interface.qualified_name}}")));
    setupConnections();

{% if interface_zoned %}
    auto zoneObject = new {{zone_class}}(QString(), this);
    m_zoneMap.insert(QString(), zoneObject);
    connect(zoneObject, &{{zone_class}}::syncDone, this, &{{class}}::onZoneSyncDone);
{% endif %}
}

{{class}}::~{{class}}()
{
    delete m_node;
}

void {{class}}::initialize()
{
{% for property in interface.properties %}
{%   if property.type.is_model %}
    emit {{property}}Changed(m_{{property}});
{%   endif %}
{% endfor %}

{% if interface_zoned %}
    if (m_synced)
        onZoneSyncDone();
{% else %}
    if (m_replica->isInitialized()) {
{%   for property in interface.properties %}
{%     if not property.is_model %}
        emit {{property}}Changed(m_replica->{{property}}());
{%     endif %}
{%   endfor %}
        emit initializationDone();
    }
{% endif %}
}

{% if interface_zoned %}
void {{class}}::syncZones()
{
    QRemoteObjectPendingReply<QStringList> zoneReply = m_replica->availableZones();
    auto zoneWatcher = new QRemoteObjectPendingCallWatcher(zoneReply);
    connect(zoneWatcher, &QRemoteObjectPendingCallWatcher::finished, this, [this, zoneReply](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
            if (!m_synced) {
                m_zones = zoneReply.returnValue();
                for (const QString& zone : qAsConst(m_zones)) {
                    if (m_zoneMap.contains(zone))
                        continue;
                    auto zoneObject = new {{zone_class}}(zone, this);
                    m_zoneMap.insert(zone, zoneObject);
                    connect(zoneObject, &{{zone_class}}::syncDone, this, &{{class}}::onZoneSyncDone);
                }
                emit availableZonesChanged(m_zones);

                for (const QString& zone : m_zoneMap.keys())
                    m_zoneMap.value(zone)->sync();
            } else {
                onZoneSyncDone();
            }
        }
        self->deleteLater();
    });
}

QStringList {{class}}::availableZones() const
{
    return m_zones;
}
{% endif %}

{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
{%     if not property.is_model %}
{{ivi.prop_setter(property, class, zoned=interface_zoned)}}
{
{%     if not property.type.is_model %}
{%     if interface_zoned %}
    m_replica->set{{property|upperfirst}}({{property}}, zone);
{%     else %}
    m_replica->push{{property|upperfirst}}({{property}});
{%     endif %}
{%     else %}
    qCritical() << "{{class}}::{{property}}, remote models not supported";
{%     endif %}
}

{%     endif %}
{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
{{ ivi.operation(operation, class, zoned=interface_zoned) }}
{
    if (static_cast<QRemoteObjectReplica*>(m_replica.get())->state() != QRemoteObjectReplica::Valid)
        return QIviPendingReply<{{operation|return_type}}>::createFailedReply();

    QIviPendingReply<{{operation|return_type}}> iviReply;
{% set function_parameters = operation.parameters|join(', ') %}
{% if interface_zoned %}
{%   if operation.parameters|length %}
{%     set function_parameters = function_parameters + ', ' %}
{%   endif %}
{%   set function_parameters = function_parameters + 'zone' %}
{% endif%}
    qCDebug(qLcRO{{interface}}) << "{{operation}} called";
    QRemoteObjectPendingReply<QVariant> reply = m_replica->{{operation}}({{function_parameters}});
    auto watcher = new QRemoteObjectPendingCallWatcher(reply);
    connect(watcher, &QRemoteObjectPendingCallWatcher::finished, this, [this, iviReply](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 1)
            QVariant value = self->returnValue();
#else
            QVariant value = self->returnValue().value<QVariant>();
#endif
            if (value.canConvert<{{interface}}PendingResult>()) {
                {{interface}}PendingResult result = value.value<{{interface}}PendingResult>();
                if (result.failed()) {
                    qCDebug(qLcRO{{interface}}) << "Pending Result with id:" << result.id() << "failed";
                    iviReply.setFailed();
                } else {
                    qCDebug(qLcRO{{interface}}) << "Result not available yet. Waiting for id:" << result.id();
                    m_pendingReplies.insert(result.id(), iviReply);
                }
            } else {
{% if operation.type.is_void %}
                qCDebug(qLcRO{{interface}}) << "Got the value right away: void";
                iviReply.setSuccess();
{% else %}
                qCDebug(qLcRO{{interface}}) << "Got the value right away:" << value.value<{{operation|return_type}}>();
                iviReply.setSuccess(value.value<{{operation|return_type}}>());
{% endif %}
            }
        } else {
            iviReply.setFailed();
            emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("{{class}}, remote call of method {{operation}} failed"));
        }
        self->deleteLater();
    });
    return iviReply;
}

{% endfor %}

void {{class}}::setupConnections()
{
    connect(m_replica.data(), &{{interface}}Replica::pendingResultAvailable, this, &{{class}}::onPendingResultAvailable);
{% if interface_zoned %}
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &{{class}}::syncZones);
{% else %}
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIviFeatureInterface::initializationDone);
{% endif %}
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, this, &{{class}}::onReplicaStateChanged);
{% for property in interface.properties if not property.type.is_model %}
{%   if interface_zoned %}
    connect(m_replica.data(), &{{interface}}Replica::{{property}}Changed, this, [this]({{property|parameter_type}}, const QString &zone) {
        auto zoneObject = m_zoneMap.value(zone);
        if (!zoneObject) {
            qCritical() << "{{class}}: Backend got changed signal for a zone which doesn't exist. Ignoring it.";
            return;
        }
        zoneObject->{{property|setter_name}}({{property}});
    });
{%   else %}
    connect(m_replica.data(), &{{interface}}Replica::{{property}}Changed, this, &{{class}}::{{property}}Changed);
{%   endif %}
{% endfor %}
{% for signal in interface.signals %}
    connect(m_replica.data(), &{{interface}}Replica::{{signal}}, this, &{{class}}::{{signal}});
{% endfor %}
}

void {{class}}::onReplicaStateChanged(QRemoteObjectReplica::State newState,
                               QRemoteObjectReplica::State oldState)
{
    if (newState == QRemoteObjectReplica::Suspect) {
        qDebug() << "{{class}}, QRemoteObjectReplica error, connection to the source lost";
        emit errorChanged(QIviAbstractFeature::Unknown,
                        "QRemoteObjectReplica error, connection to the source lost");
{% if interface_zoned %}
        m_synced = false;
{% endif %}
    } else if (newState == QRemoteObjectReplica::SignatureMismatch) {
        qDebug() << "{{class}}, QRemoteObjectReplica error, signature mismatch";
        emit errorChanged(QIviAbstractFeature::Unknown,
                        "QRemoteObjectReplica error, signature mismatch");
    } else if (newState==QRemoteObjectReplica::Valid) {
        emit errorChanged(QIviAbstractFeature::NoError, "");
    }
}

void {{class}}::onNodeError(QRemoteObjectNode::ErrorCode code)
{
    qDebug() << "{{class}}, QRemoteObjectNode error, code: " << code;
    emit errorChanged(QIviAbstractFeature::Unknown, "QRemoteObjectNode error, code: " + code);
}

void {{class}}::onPendingResultAvailable(quint64 id, bool isSuccess, const QVariant &value)
{
    qCDebug(qLcRO{{interface}}) << "pending result available" << id;
    if (!m_pendingReplies.contains(id)) {
        qCDebug(qLcRO{{interface}}) << "Received a result for an unexpected id:" << id << ". Ignoring!";
        return;
    }

    QIviPendingReplyBase iviReply = m_pendingReplies.take(id);

    if (isSuccess)
        iviReply.setSuccess(value);
    else
        iviReply.setFailed();
}

{% if interface_zoned %}
void {{class}}::onZoneSyncDone()
{
    const QStringList zones = m_zoneMap.keys();

    for (const QString& zone : zones) {
        if (m_zoneMap.value(zone)->isSyncing())
            return;
    }

    m_synced = true;

    for (const QString& zone : zones)
        m_zoneMap.value(zone)->emitCurrentState();
    emit initializationDone();
}
{% endif %}

QT_END_NAMESPACE
