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
{% include "common/generated_comment.cpp.tpl" %}
#include <QCoreApplication>
#include <QDir>
#include <QLockFile>

{% for interface in module.interfaces %}
#include "{{interface|lower}}backend.h"
#include "{{interface|lower}}adapter.h"
{% endfor %}

#include "core.h"
#include <QtIviCore/QIviSimulationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // single instance guard
    QLockFile lockFile(QStringLiteral("%1/%2.lock").arg(QDir::tempPath(), app.applicationName()));
    if (!lockFile.tryLock(100)) {
        qCritical("%s already running, aborting...", qPrintable(app.applicationName()));
        return EXIT_FAILURE;
    }

    auto simulationEngine = new QIviSimulationEngine(QStringLiteral("{{module.name|lower}}"));

{% for interface in module.interfaces %}
    auto {{interface|lowerfirst}}Instance = new {{interface}}Backend(simulationEngine);
    //Register the types for the SimulationEngine
    {{module.module_name|upperfirst}}Module::registerQmlTypes(QStringLiteral("{{module|qml_type}}.simulation"), {{module.majorVersion}}, {{module.minorVersion}});
    simulationEngine->registerSimulationInstance({{interface|lowerfirst}}Instance, "{{module|qml_type}}.simulation", {{module.majorVersion}}, {{module.minorVersion}}, "{{interface}}Backend");
{% endfor %}
{% if module.tags.config_simulator and module.tags.config_simulator.simulationFile %}
{%   set simulationFile = module.tags.config_simulator.simulationFile %}
{% else %}
{%   set simulationFile = "qrc:///simulation/" + module.module_name|lower + '_simulation.qml' %}
{% endif %}
    simulationEngine->loadSimulationData(QStringLiteral(":/simulation/{{module.module_name|lower}}_simulation_data.json"));
    simulationEngine->loadSimulation(QUrl(QStringLiteral("{{simulationFile}}")));

    //initialize all our backends
{% for interface in module.interfaces %}
    {{interface|lowerfirst}}Instance->initialize();
{%   for property in interface.properties %}
{%     if property.type.is_model %}
    {{interface|lowerfirst}}Instance->{{property|getter_name}}()->initialize();
{%     endif %}
{%   endfor %}
{% endfor %}

    //Start Remoting the backends
{% for interface in module.interfaces %}
    auto {{interface|lowerfirst}}Adapter = new {{interface}}QtRoAdapter({{interface|lowerfirst}}Instance);
    {{interface|lowerfirst}}Adapter->enableRemoting(Core::instance()->host());
{% endfor %}


    return app.exec();
}
