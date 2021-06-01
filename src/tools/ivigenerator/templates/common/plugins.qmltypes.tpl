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
{% import 'common/qtivi_macros.j2' as ivi %}
{% set module_qml_name = (module|qml_type).split('.')[-1]|upperfirst %}

import QtQuick.tooling 1.2

// This file describes the plugin-supplied types contained in the library.
// It is used for QML tooling purposes only.
//
// This file was auto-generated with the QtIvi ivigenerator

Module {
    dependencies: ["QtIvi 1.0"]
{% for interface in module.interfaces %}
{% set class = '{0}'.format(interface) %}
{% if interface.tags.config.zoned %}
{%   set base_class = 'QIviAbstractZonedFeature' %}
{% else %}
{%   set base_class = 'QIviAbstractFeature' %}
{% endif %}
    Component {
        name: "{{class}}"
        prototype: "{{base_class}}"
        exports: ["{{module|qml_type}}/{{interface|qml_type}} {{module.majorVersion}}.{{module.minorVersion}}"]
        exportMetaObjectRevisions: [0]
{%   for property in interface.properties %}
{%     if property.readonly or property.const or property.type.is_model %}
{%       set readonly = 'true' %}
{%     else %}
{%       set readonly = 'false' %}
{%     endif %}
        Property { name: "{{property}}"; type: "{{property|qml_info_type}}"; isReadonly: {{readonly}} {% if property.type.is_model %}; isPointer: true {% endif %} }
{%   endfor %}
{%   for property in interface.properties %}
        Signal {
            name: "{{property}}Changed"
            Parameter { name: "{{property}}"; type: "{{property|qml_info_type}}" }
        }
{%   endfor %}
{%   for signal in interface.signals %}
        Signal {
            name: "{{signal}}"
{%     for parameter in signal.parameters %}
            Parameter { name: "{{parameter}}"; type: "{{parameter|qml_info_type}}" }
{%     endfor %}
        }
{%   endfor %}
{%   for operation in interface.operations %}
        Method {
            name: "{{operation}}"
{# TODO: Once QtCreator can also provide the completion for return types we need to reevaluate this #}
            type: "QIviPendingReply<{{operation|return_type}}>"
{%     for parameter in operation.parameters %}
            Parameter { name: "{{parameter}}"; type: "{{parameter|qml_info_type}}" }
{%     endfor %}
        }
{%   endfor %}
{%   for property in interface.properties %}
{%     if not (property.readonly or property.const or property.type.is_model) %}
        Method {
            name: "{{property|setter_name}}"
            Parameter { name: "{{property}}"; type: "{{property|qml_info_type}}" }
        }
{%     endif %}
{%   endfor %}
    }
{% endfor %}

    Component {
        name: "{{module.module_name|upperfirst}}"
        prototype: "QObject"
        exports: ["{{module|qml_type}}/{{module_qml_name}} {{module.majorVersion}}.{{module.minorVersion}}"]
        isCreatable: false
        isSingleton: true
        exportMetaObjectRevisions: [0]
{% for enum in module.enums %}
        Enum {
            name: "{{enum|flag_type}}"
            values: {
{%   for member in enum.members %}
                "{{member.name}}": {{member.value}}{% if not loop.last %},{% endif %}

{%   endfor %}
            }
        }
{% endfor %}
{% for struct in module.structs %}
        Method {
            name: "{{struct|lowerfirst}}"
            type: "{{struct}}"
{%   for fields in struct.fields %}
            Parameter { name: "{{fields}}"; type: "{{fields|qml_info_type}}" }
{%   endfor %}
        }
{% endfor %}
{% for struct in module.structs %}
        Method {
            name: "{{struct|lowerfirst}}"
            type: "{{struct}}"
        }
{% endfor %}
    }
}
