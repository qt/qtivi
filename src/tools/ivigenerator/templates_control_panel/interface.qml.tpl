{#
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
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
{% set backend_obj = '{0}{1}Id'.format(interface, 'Object') %}
{% set backend_obj = backend_obj[0].lower() + backend_obj[1:] %}
import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtIvi.ControlPanel 1.0

Flickable {
    id: {{interface|lowerfirst}}
    flickableDirection: Flickable.VerticalFlick
    clip: true
    ScrollBar.vertical: ScrollBar {}
    contentHeight: layout.childrenRect.height

    {{interface|qml_type}} {
        id: {{backend_obj}}
    }

{% if interface.tags.config.zoned %}
    property {{interface|qml_type}} currentZoneObject: {{backend_obj}}.zoneAt[comboZones.displayText]
{% endif %}

    ColumnLayout {
        id: layout
        anchors.fill: parent
{% if interface.tags.config.zoned %}
        RowLayout {
            Text {
                text: "Zone: "
            }
            ComboBox {
                width: 250
                id: comboZones
                editable: true
                model: {{backend_obj}}.zones
                currentIndex: {{backend_obj}}.zones.indexOf("MainZone")
            }
            Button {
                text:"+"
                width: 20
                height: 20
                onClicked: {
                    var zoneName = comboZones.editText;
                    {{backend_obj}}.addZone(zoneName)
                }
            }
        }
{% endif %}

        ToolSeparator {
            Layout.fillWidth: true
            orientation: Qt.Horizontal
        }

{%     for property in interface.properties %}
{%     if interface.tags.config.zoned %}
{%       set backend_obj = 'currentZoneObject' %}
{%     endif %}
        RowLayout {
            height: 30
            Layout.fillWidth: true
            Text {
                text: "{{property|upperfirst}}: "
            }

            Connections {
                target: {{property|lowerfirst}}Control
                on{{property|qml_binding_property|upperfirst}}Changed: {
                    {{backend_obj}}.{{property}} = {{property|lowerfirst}}Control.{{property|qml_binding_property}}
                }
            }

            Binding {
                target: {{property|lowerfirst}}Control
                property: "{{property|qml_binding_property}}"
                value: {{backend_obj}}.{{property}}
            }

            {{property|qml_control(backend_obj)}}
        }
{%     endfor %}
{%     if interface.signals|count %}
        ToolSeparator {
            orientation: Qt.Horizontal
            Layout.fillWidth: true
        }

        Text {
            text: "Signals"
        }
{%       for signal in interface.signals  %}
        // Button for signal call
        RowLayout {
            spacing: 2
            height: 20
            Button {
                text: "{{signal}}"
                onClicked: {{backend_obj}}.{{signal}}({{signal|qml_control_signal_parameters}});
            }
{%         for param in signal.parameters %}
            Text {
                text: "{{param}}"
            }
            {{param|qml_control(backend_obj)}}

{%         endfor%}
        }
{%       endfor  %}
{%     endif  %}
{%     if interface.operations|count %}

        ToolSeparator {
            orientation: Qt.Horizontal
            Layout.fillWidth: true
        }

        Text {
            text: "Operations"
        }

        TextArea {
            id: operationArea
            readOnly: true
            placeholderText: "The called operations will be logged here"
            Layout.fillWidth: true
        }
{%       for operation in interface.operations %}
{%         if interface.tags.config.zoned %}
{%           set backend_obj = 'currentZoneObject' %}
{%         endif %}
        Connections {
            target: {{backend_obj}}
            on{{operation|upperfirst}}: {
                operationArea.text += "{{operation}}("
{%         for parameter in operation.parameters %}
                    + "{{parameter}}: " + {{parameter}}
{%         endfor %}
                    + ")\n"
            }
        }
{%       endfor %}
{%     endif  %}
        Item {
            //spacer
            Layout.fillHeight: true
        }
    }
}
