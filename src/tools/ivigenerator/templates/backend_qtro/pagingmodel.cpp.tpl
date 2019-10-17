{#
# Copyright (C) 2019 Luxoft Sweden AB
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
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
{% if interface_zoned %}
{% set class = 'Zoned{0}ModelBackend'.format(property|upperfirst) %}
{% else %}
{% set class = '{0}ModelBackend'.format(property|upperfirst) %}
{% endif %}

Q_LOGGING_CATEGORY(qLcRO{{interface}}{{property|upper_first}}, "{{module|qml_type|lower}}.{{interface|lower}}backend.{{property|lower}}.remoteobjects", QtInfoMsg)

{{class}}::{{class}}(const QString &remoteObjectsLookupName, QObject* parent)
    : QIviPagingModelInterface(parent)
    , m_node(nullptr)
    , m_helper(new QIviRemoteObjectReplicaHelper(qLcRO{{interface}}{{property|upper_first}}(), this))
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
{
    qRegisterMetaType<QIviPagingModelInterface*>();
}

/*! \internal */
{{class}}::~{{class}}()
{
    delete m_node;
}

void {{class}}::initialize()
{
    if (!connectToNode())
        return;

    if (m_replica->isInitialized())
        Q_EMIT initializationDone();
}

void {{class}}::registerInstance(const QUuid &identifier)
{
    if (m_replica.isNull())
        return;
    m_replica->registerInstance(identifier);
}

void {{class}}::unregisterInstance(const QUuid &identifier)
{
    if (m_replica.isNull())
        return;
    m_replica->unregisterInstance(identifier);
}

void {{class}}::fetchData(const QUuid &identifier, int start, int count)
{
    if (m_replica.isNull())
        return;
    m_replica->fetchData(identifier, start, count);
}

bool {{class}}::connectToNode()
{
    static QString configPath;
    if (configPath.isEmpty()) {
        if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
            configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
        } else {
            configPath = QStringLiteral("./server.conf");
            qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
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
            qCCritical(qLcRO{{interface}}{{property|upper_first}}) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "Connecting to" << m_url;
        m_replica.reset(m_node->acquire<QIviPagingModelReplica>(m_remoteObjectsLookupName));
        setupConnections();
    }
    return true;
}

void {{class}}::setupConnections()
{
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIviFeatureInterface::initializationDone);
    connect(m_node, &QRemoteObjectNode::error, m_helper, &QIviRemoteObjectReplicaHelper::onNodeError);
    connect(m_helper, &QIviRemoteObjectReplicaHelper::errorChanged, this, &QIviFeatureInterface::errorChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIviRemoteObjectReplicaHelper::onReplicaStateChanged);

    connect(m_replica.data(), &QIviPagingModelReplica::supportedCapabilitiesChanged, this, &{{class}}::supportedCapabilitiesChanged);
    connect(m_replica.data(), &QIviPagingModelReplica::countChanged, this, &{{class}}::countChanged);
    connect(m_replica.data(), &QIviPagingModelReplica::dataFetched, this, &{{class}}::dataFetched);
    connect(m_replica.data(), &QIviPagingModelReplica::dataChanged, this, &{{class}}::dataChanged);
}
