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
{% import 'common/qtivi_macros.j2' as ivi %}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}Backend'.format(interface) %}
{% set zone_class = '{0}Zone'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
#include "{{class|lower}}.h"

#include <QDebug>
#include <QSettings>
#include <QTimer>
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
{%   if property.type.is_model %}
    , m_{{property}}(new Zoned{{property|upperfirst}}ModelBackend(QStringLiteral("{{interface.qualified_name}}.{{property}}.") + m_zone, this))
{%   else %}
    , m_{{ property }}({{property|default_type_value}})
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
    if (m_parent->m_replica.isNull())
        return;

{% for property in interface.properties %}
{%   if not property.type.is_model %}
    m_propertiesToSync.append(QStringLiteral("{{property}}"));
{%   endif %}
{% endfor %}

{% for property in interface.properties %}
{%   if not property.type.is_model %}
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
{%   endif %}
{% endfor %}
}

void {{zone_class}}::checkSync()
{
    if (m_propertiesToSync.isEmpty())
        Q_EMIT syncDone();
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
    Q_EMIT m_parent->{{property}}Changed({{property}}, m_zone);
}
{% endfor %}
{% endif %}

{{class}}::{{class}}(const QString &remoteObjectsLookupName, QObject *parent)
    : {{class}}Interface(parent)
    , m_node(nullptr)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_helper(new QIviRemoteObjectReplicaHelper(qLcRO{{interface}}(), this))
{% for property in interface.properties %}
{%   if property.type.is_model %}
{%     if interface_zoned %}
    , m_{{property}}(new Zoned{{property|upperfirst}}ModelBackend(QStringLiteral("{{interface.qualified_name}}.{{property}}"), this))
{%     else %}
    , m_{{property}}(new {{property|upperfirst}}ModelBackend(QStringLiteral("{{interface.qualified_name}}.{{property}}"), this))
{%     endif %}
{%   endif %}
{% endfor %}
{% if interface_zoned %}
    , m_synced(false)
{% endif %}
{
    {{module.module_name|upperfirst}}Module::registerTypes();

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
    Q_EMIT {{property}}Changed(m_{{property}});
{%   endif %}
{% endfor %}

    if (!connectToNode())
        return;

{% if interface_zoned %}
    if (m_synced)
        onZoneSyncDone();
{% else %}
    if (m_replica->isInitialized()) {
{%   for property in interface.properties %}
{%     if not property.is_model %}
        Q_EMIT {{property}}Changed(m_replica->{{property}}());
{%     endif %}
{%   endfor %}
        Q_EMIT initializationDone();
    }
{% endif %}

    QTimer::singleShot(3000, this, [this](){
        if(!m_replica->isInitialized())
            qCCritical(qLcRO{{interface}}) << "{{interface.qualified_name}} wasn't initialized within the timeout period. Please make sure the server is running.";
    });
}

{% if interface_zoned %}
void {{class}}::syncZones()
{
    if (m_replica.isNull())
        return;
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
                Q_EMIT availableZonesChanged(m_zones);

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
    if (m_replica.isNull())
        return;
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
    if (m_replica.isNull())
        return QIviPendingReply<{{operation|return_type}}>::createFailedReply();
    else if (static_cast<QRemoteObjectReplica*>(m_replica.get())->state() != QRemoteObjectReplica::Valid)
        return QIviPendingReply<{{operation|return_type}}>::createFailedReply();

{% set function_parameters = operation.parameters|join(', ') %}
{% if interface_zoned %}
{%   if operation.parameters|length %}
{%     set function_parameters = function_parameters + ', ' %}
{%   endif %}
{%   set function_parameters = function_parameters + 'zone' %}
{% endif%}
    qCDebug(qLcRO{{interface}}) << "{{operation}} called";
    QRemoteObjectPendingReply<QVariant> reply = m_replica->{{operation}}({{function_parameters}});
    auto iviReply = m_helper->toQIviPendingReply<{{operation|return_type}}>(reply);

    //Pass an empty std::function to only handle errors.
    iviReply.then(std::function<void({{operation|return_type}})>(), [this]() {
        Q_EMIT errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("{{class}}, remote call of method {{operation}} failed"));
    });
    return iviReply;
}

{% endfor %}

bool {{class}}::connectToNode()
{
    static QString configPath;
    if (configPath.isEmpty()) {
        if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
            configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
        } else {
            configPath = QStringLiteral("./server.conf");
            qCInfo(qLcRO{{interface}}) << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
        }
    }

    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("{{module.module_name|lower}}"));
    QUrl registryUrl = QUrl(settings.value(QStringLiteral("Registry"), QStringLiteral("local:{{module.module_name|lower}}")).toString());
    if (m_url != registryUrl) {
        m_url = registryUrl;
        // QtRO doesn't allow to change the URL without destroying the Node
        delete m_node;
        m_node = new QRemoteObjectNode();
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcRO{{interface}}) << "Connection to" << m_url << "failed!";
            m_replica.reset();
{% if interface_zoned %}
            m_synced = false;
{% endif %}
            return false;
        }
        qCInfo(qLcRO{{interface}}) << "Connecting to" << m_url;
        m_replica.reset(m_node->acquire<{{interface}}Replica>(m_remoteObjectsLookupName));
        setupConnections();
    }
    return true;
}

void {{class}}::setupConnections()
{
    connect(m_node, &QRemoteObjectNode::error, m_helper, &QIviRemoteObjectReplicaHelper::onNodeError);
    connect(m_helper, &QIviRemoteObjectReplicaHelper::errorChanged, this, &QIviFeatureInterface::errorChanged);

    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIviRemoteObjectReplicaHelper::onReplicaStateChanged);
    connect(m_replica.data(), &{{interface}}Replica::pendingResultAvailable, m_helper, &QIviRemoteObjectReplicaHelper::onPendingResultAvailable);
{% if interface_zoned %}
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &{{class}}::syncZones);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, this, [this](QRemoteObjectReplica::State newState, QRemoteObjectReplica::State oldState){
        Q_UNUSED(oldState)
        if (newState == QRemoteObjectReplica::Suspect)
            m_synced = false;
    });
{% else %}
    //As the Replica is now initialized, this will trigger an update of all properties (not just the changed ones)
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &{{class}}::initialize);
{% endif %}
{% for property in interface.properties if not property.type.is_model %}
{%   if interface_zoned %}
    connect(m_replica.data(), &{{interface}}Replica::{{property}}Changed, this, [this]({{property|parameter_type}}, const QString &zone) {
        auto zoneObject = m_zoneMap.value(zone);
        if (!zoneObject) {
            qCCritical(qLcRO{{interface}}) << "Backend got changed signal for a zone which doesn't exist. Ignoring it.";
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
    Q_EMIT initializationDone();
}
{% endif %}

QT_END_NAMESPACE
