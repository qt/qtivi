{#
#############################################################################
##
## Copyright (C) 2021 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtIvi module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:GPL-EXCEPT$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3 as published by the Free Software
## Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
#############################################################################
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = "Core" %}


#include "{{class|lower}}.h"
#include "{{module.module_name|lower}}.h"
#include <QCoreApplication>
#include <QSettings>

QT_BEGIN_NAMESPACE

{{class}} *{{class}}::s_instance(nullptr);

{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
    , m_host(nullptr)
{
    init();
}

{{class}}::~{{class}}()
{
}

void {{class}}::init()
{
    {{module.module_name|upperfirst}}::registerTypes();
    QString configPath(QStringLiteral("./server.conf"));
    if (qEnvironmentVariableIsSet("SERVER_CONF_PATH"))
        configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
    else
        qDebug() << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("{{module.module_name|lower}}"));
    QUrl url = QUrl(settings.value(QStringLiteral("Registry"), QStringLiteral("local:{{module.module_name|lower}}")).toString());
    m_host = new QRemoteObjectRegistryHost(url);
    qDebug() << "registry at: " << m_host->registryUrl().toString();
    connect(m_host, &QRemoteObjectNode::error, this, &{{class}}::reportError);
}

{{class}} *{{class}}::instance()
{
    if (!s_instance)
        s_instance = new {{class}}(QCoreApplication::instance());
    Q_ASSERT(s_instance);
    return s_instance;
}

QRemoteObjectRegistryHost *{{class}}::host() const
{
    Q_ASSERT(m_host);
    return m_host;
}

void {{class}}::reportError(QRemoteObjectNode::ErrorCode code) const
{
    qWarning() << "QRemoteObjects Error: " << code;
}

QT_END_NAMESPACE
