{#
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
{% include "generated_comment.cpp.tpl" %}
{% set class = '{0}Backend'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
#include "{{class|lower}}.h"

{% for property in interface.properties %}
{% if property.type.is_model %}
#include "{{property|model_type|lower}}.h"
{% endif %}
{% endfor %}

#include <QDebug>
#include <QSettings>
#include "{{module.module_name|lower}}module.h"

QT_BEGIN_NAMESPACE

{{class}}::{{class}}(QObject *parent)
    : {{class}}Interface(parent)
{
    {{module.module_name|upperfirst}}Module::registerTypes();
}

{{class}}::~{{class}}()
{
    delete m_node;
}

void {{class}}::initialize()
{
    QString configPath = "./server.conf";
    if (qEnvironmentVariableIsSet("SERVER_CONF_PATH"))
        configPath = QString::fromLatin1(qgetenv("SERVER_CONF_PATH"));
    else
        qDebug() << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup("{{module.module_name|lower}}");
    QUrl url = QUrl(settings.value("Registry", "local:{{module.module_name|lower}}").toString());
    m_node = new QRemoteObjectNode(url);
    connect(m_node, &QRemoteObjectNode::error, this, &{{class}}::onNodeError);
    m_replica.reset(m_node->acquire<{{interface}}Replica>("{{interface.qualified_name}}"));
    setupConnections();

{% for property in interface.properties %}
{%   if not property.is_model %}
    emit {{property}}Changed(m_replica->{{property}}());
{%   endif %}
{% endfor %}
    if (m_replica->isInitialized())
        emit initializationDone();
}

{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
void {{class}}::set{{property|upperfirst}}({{ property|parameter_type }})
{
{%     if not property.type.is_model %}
    m_replica->push{{property|upperfirst}}({{property}});
{%     else %}
    qCritical() << "{{class}}::{{property}}, remote models not supported";
{%     endif %}
}

{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
{%   set operation_parameters = operation.parameters|map('parameter_type')|join(', ') %}
QIviPendingReply<{{operation|return_type}}> {{class}}::{{operation}}({{operation_parameters}}){%if operation.const %} const{% endif %}
{
    if (m_replica->state() != QRemoteObjectReplica::Valid)
        return QIviPendingReply<{{operation|return_type}}>::createFailedReply();

    QIviPendingReply<{{operation|return_type}}> iviReply;
{% if not operation.type.is_void %}
    QRemoteObjectPendingReply<{{operation|return_type}}> reply = m_replica->{{operation}}({{operation.parameters|join(', ')}});
    auto watcher = new QRemoteObjectPendingCallWatcher(reply);
    connect(watcher, &QRemoteObjectPendingCallWatcher::finished, this, [this, iviReply](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
            iviReply.setSuccess(self->returnValue().value<{{operation|return_type}}>());
        } else {
            iviReply.setFailed();
            emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("{{class}}, remote call of method {{operation}} failed"));
        }
        self->deleteLater();
    });
{%   else %}
    m_replica->{{operation}}({{operation.parameters|join(', ')}});
    iviReply.setSuccess();
{%   endif %}
    return iviReply;
}

{% endfor %}

void {{class}}::setupConnections()
{
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIviFeatureInterface::initializationDone);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, this, &{{class}}::onReplicaStateChanged);
{% for property in interface.properties if not property.type.is_model %}
    connect(m_replica.data(), &{{interface}}Replica::{{property}}Changed, this, &{{class}}::{{property}}Changed);
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

QT_END_NAMESPACE
