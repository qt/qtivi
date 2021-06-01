{#
#############################################################################
##
## Copyright (C) 2021 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Copyright (C) 2018 Pelagicore AG
## Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
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
{% set class = '{0}SimulatorPlugin'.format(module.module_name|upperfirst) %}

#include "{{class|lower}}.h"

{% for interface in module.interfaces %}
#include "{{interface|lower}}backend.h"
{% endfor %}

#include <QStringList>
#include <QtIviCore/QIviSimulationEngine>

QT_BEGIN_NAMESPACE

{% if module.tags.config.interfaceBuilder %}
extern {{class}}::InterfaceBuilder {{module.tags.config.interfaceBuilder}};
{% endif %}

/*!
   \class {{class}}
   \inmodule {{module}}

*/
/*! \internal */
{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
    , m_simulationEngine(new QIviSimulationEngine(QStringLiteral("{{module.name|lower}}"), this))
{
{% if module.tags.config.interfaceBuilder %}
    m_interfaces = {{module.tags.config.interfaceBuilder}}(this);
    Q_ASSERT(m_interfaces.size() == interfaces().size());
{% else %}
{%   for interface in module.interfaces %}
    auto {{interface}}Instance = new {{interface}}Backend(m_simulationEngine, this);
    //Register the types for the SimulationEngine
    {{module.module_name|upperfirst}}::registerQmlTypes(QStringLiteral("{{module|qml_type}}.simulation"), {{module.majorVersion}}, {{module.minorVersion}});
    m_simulationEngine->registerSimulationInstance({{interface}}Instance, "{{module|qml_type}}.simulation", {{module.majorVersion}}, {{module.minorVersion}}, "{{interface}}Backend");
    m_interfaces << {{interface}}Instance;
{%   endfor %}
{% if module.tags.config_simulator and module.tags.config_simulator.simulationFile %}
{%   set simulationFile = module.tags.config_simulator.simulationFile %}
{% else %}
{%   set simulationFile = "qrc:///simulation/" + module.module_name|lower + '_simulation.qml' %}
{% endif %}
    m_simulationEngine->loadSimulationData(QStringLiteral(":/simulation/{{module.module_name|lower}}_simulation_data.json"));
    m_simulationEngine->loadSimulation(QUrl(QStringLiteral("{{simulationFile}}")));
{% endif %}
}

/*! \internal */
QStringList {{class}}::interfaces() const
{
    QStringList list;
{% for iface in module.interfaces %}
{%   if loop.first %}    list{% endif %} << {{module.module_name|upperfirst}}_{{iface}}_iid{% if loop.last %};{% endif %}
{% endfor %}

    return list;
}

/*! \internal */
QIviFeatureInterface *{{class}}::interfaceInstance(const QString &interface) const
{
     int index = interfaces().indexOf(interface);
     return index < 0 ? nullptr : m_interfaces.at(index);
}

QT_END_NAMESPACE
