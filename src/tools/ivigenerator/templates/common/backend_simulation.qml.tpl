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


import QtQuick 2.10
import {{module|qml_type}}.simulation {{module.majorVersion}}.{{module.minorVersion}}

{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}

QtObject {
    property var settings : IviSimulator.findData(IviSimulator.simulationData, "{{interface}}")
    property bool defaultInitialized: false
    property LoggingCategory qLc{{interface|upperfirst}}: LoggingCategory {
        name: "{{module|qml_type|lower}}.simulation.{{interface|lower}}backend"
    }
    property var backend : {{interface|upperfirst}}Backend {

        function initialize() {
            console.log(qLc{{interface|upperfirst}}, "INITIALIZE")
            if (!defaultInitialized) {
                IviSimulator.initializeDefault(settings, backend)
                defaultInitialized = true
            }
            Base.initialize()
        }

{% if interface_zoned %}
        function availableZones() {
            return settings.zones;
        }
{% endif %}
{% for property in interface.properties %}

{% if interface_zoned %}
        function {{property|setter_name}}({{property}}, zone) {
            if ("{{property}}" in settings && !IviSimulator.checkSettings(settings["{{property}}"], {{property}}, zone)) {
                console.error(qLc{{interface|upperfirst}}, "SIMULATION changing {{property}} is not possible: provided: " + {{property}} + " constraint: " + IviSimulator.constraint(settings["{{property}}"]));
                return;
            }

            if (zone) {
                console.log(qLc{{interface|upperfirst}}, "SIMULATION {{ property }} for zone: " + zone + " changed to: " + {{property}});
                backend.zones[zone].{{property}} = {{property}}
            } else {
                console.log(qLc{{interface|upperfirst}}, "SIMULATION {{ property }} changed to: " + {{property}});
                backend.{{property}} = {{property}}
            }
        }
{% else %}
        function {{property|setter_name}}({{property}}) {
            if ("{{property}}" in settings && !IviSimulator.checkSettings(settings["{{property}}"], {{property}})) {
                console.error(qLc{{interface|upperfirst}}, "SIMULATION changing {{property}} is not possible: provided: " + {{property}} + " constraint: " + IviSimulator.constraint(settings["{{property}}"]));
                return;
            }

            console.log(qLc{{interface|upperfirst}}, "SIMULATION {{ property }} changed to: " + {{property}});
            backend.{{property}} = {{property}}
        }
{% endif %}
{% endfor %}
    }
}
