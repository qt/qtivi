#!/usr/bin/env python3
# Copyright (C) 2019 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB)
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

import json
import inspect

from qface.idl.domain import Module, Interface, Property, Parameter, Field, Struct
from qface.helper.generic import lower_first, upper_first
from qface.helper.qtcpp import Filters

from .global_functions import jinja_error, jinja_warning
from . import builtin_config


def deprecated_filter(name=None):
    if not name:
        name = inspect.stack()[1][3]
    jinja_warning("The '{0}' filter is deprecated and will be removed in future Qt "
                  "versions".format(name))


def tag_by_path(symbol, path, default_value=False):
    """
    Find the tag given by its full path in the object hierarchy,
    like "property.config_sim.zones.right". If some part of the
    path is missing, return None
    """
    deprecated_filter()

    path_parts = path.split(".")
    cur_level_obj = symbol.tags
    for path_part in path_parts:
        if path_part in cur_level_obj:
            cur_level_obj = cur_level_obj[path_part]
        else:
            cur_level_obj = None
            break
    if cur_level_obj is None:
        cur_level_obj = default_value
    return cur_level_obj


def conf_sim_tag(symbol, path, default_value=False):
    """
    Find tag, given by its path, located under "config_simulator" sub-object.
    Returns None, of any of the path parts is missing
    """
    deprecated_filter()

    return tag_by_path(symbol, "config_simulator." + path, default_value)


def enum_value_to_cppliteral(value, module_name):
    value = value.strip().rsplit('.', 1)[-1]
    return '{0}{1}Module::{2}'.format(Filters.classPrefix, upper_first(module_name), value)


def enum_value(value, module_name):
    sub_values = value.split('|')
    sub_values = [enum_value_to_cppliteral(v, module_name) for v in sub_values]
    return "|".join(sub_values)


def default_type_value(symbol):
    """
    Find the default value for the type. Models are initialized as nullptr
    """
    prefix = Filters.classPrefix
    t = symbol.type
    if t.is_primitive:
        if t.is_int:
            return 'int(0)'
        if t.is_bool:
            return 'bool(false)'
        if t.is_string:
            return 'QString()'
        if t.is_real:
            return 'qreal(0.0)'
        if t.is_var:
            return 'QVariant()'
    elif t.is_void:
        return ''
    elif t.is_enum:
        module_name = t.reference.module.module_name
        value = next(iter(t.reference.members))
        return '{0}{1}Module::{2}'.format(prefix, upper_first(module_name), value)
    elif t.is_flag:
        module_name = t.reference.module.module_name
        return '{0}{1}Module::{2}()'.format(prefix, upper_first(module_name), flag_type(symbol))
    elif symbol.type.is_list:
        nested = Filters.returnType(symbol.type.nested)
        return 'QVariantList()'.format(nested)
    elif symbol.type.is_struct:
        return '{0}{1}()'.format(prefix, symbol.type.reference.name)
    elif symbol.type.is_model:
        return 'nullptr'
    jinja_error('default_type_value: Unknown parameter {0} of type {1}'.format(symbol, symbol.type))


def test_type_value(symbol):
    """
    Find a value different than the default value for the type. Models are initialized as nullptr
    """
    prefix = Filters.classPrefix
    t = symbol.type
    if t.is_primitive:
        if t.is_int:
            return '111'
        if t.is_bool:
            return 'true'
        if t.is_string:
            return 'QStringLiteral("TEST STRING")'
        if t.is_real:
            return '1234.5678'
        if t.is_var:
            return 'QVariant("TEST VARIANT")'
    elif t.is_void:
        return ''
    elif t.is_enum:
        module_name = t.reference.module.module_name
        value = list(iter(t.reference.members))[-1]
        return '{0}{1}Module::{2}'.format(prefix, upper_first(module_name), value)
    elif t.is_flag:
        module_name = t.reference.module.module_name
        value = next(iter(t.reference.members))
        return '{0}{1}Module::{2}'.format(prefix, upper_first(module_name), value)
    elif symbol.type.is_list:
        value = test_type_value(t.nested.type)
        if not (t.nested.type.is_primitive):
            value = 'QVariant::fromValue({0})'.format(value)
        return 'QVariantList({{{0}}})'.format(value)
    elif symbol.type.is_struct:
        values_string = ', '.join(test_type_value(e) for e in symbol.type.reference.fields)
        return '{0}{1}({2})'.format(prefix, symbol.type.reference.name, values_string)
    elif symbol.type.is_model:
        return 'new QIviPagingModel()'
    jinja_error('test_type_value: Unknown parameter {0} of type {1}'.format(symbol, symbol.type))


def default_value_helper(symbol, res):
    t = symbol.type
    if t.is_struct:
        if not (isinstance(res, dict) or isinstance(res, list)):
            jinja_error('default_value: value in annotation for symbol {0} is supposed to be a dict or list'.format(symbol.qualified_name))
        if len(res) != len(t.reference.fields):
            jinja_error('default_value: argument count in annotation for symbol {0} and number of struct fields '
                        'does not match: Expected {1} instead of {2}'.format(symbol.qualified_name, len(t.reference.fields), len(res)))
        values = []
        for idx, property in enumerate(res):
            values.append(default_value_helper(list(t.reference.fields)[idx], property))
        return '{0}({{{1}}})'.format(t.type, ', '.join(values))
    if t.is_model or t.is_list:
        if not isinstance(res, list):
            jinja_error('default_value: value in annotation for symbol {0} is supposed to be a list'.format(symbol.qualified_name))
        row_string = ''
        if t.nested.is_struct and t.is_list:
            row_string = ', '.join(('QVariant::fromValue({0})'.format(default_value_helper(t.nested, row))) for row in res)
        else:
            row_string = ', '.join(default_value_helper(t.nested, row) for row in res)
        return '{{{0}}}'.format(row_string)
    if t.is_enum or t.is_flag:
        module_name = t.reference.module.module_name
        return enum_value(res, module_name)
    # in case it's bool, Python True is sent to the C++ as "True", let's take care of that
    if t.is_bool:
        if res:
            return 'true'
        else:
            return 'false'
    if t.is_string:
        return 'QStringLiteral("{0}")'.format(res.replace("\\", "\\\\"))
    if t.is_var:
        if isinstance(res, str):
            res = 'QStringLiteral("{0}")'.format(res)
        return 'QVariant({0})'.format(res)

    return '{0}'.format(res)


def default_value(symbol, zone='='):
    """
    Find the default value used by the simulator backend
    """
    deprecated_filter()

    res = default_type_value(symbol)
    if symbol.type.is_model:
        res = '{}'
    if 'config_simulator' in symbol.tags and 'default' in symbol.tags['config_simulator']:
        res = symbol.tags['config_simulator']['default']
        if isinstance(res, dict):
            if zone in res:
                res = res[zone]
            elif '=' in res:
                res = res['=']
        return default_value_helper(symbol, res)

    return res


def parameter_type_default(symbol):
    """
    Return the parameter declaration for properties, handle camel case module name
    """
    prefix = Filters.classPrefix
    if symbol.type.is_enum or symbol.type.is_flag:
        return '{0}{1}Module::{2} {3}={4}'.format(prefix, upper_first(symbol.type.reference.module.module_name), flag_type(symbol), symbol, default_type_value(symbol))
    if symbol.type.is_void or symbol.type.is_primitive:
        if symbol.type.name == 'string':
            return 'const QString &{0}=QString()'.format(symbol)
        if symbol.type.name == 'var':
            return 'const QVariant &{0}=QVariant()'.format(symbol)
        if symbol.type.name == 'real':
            return 'qreal {0}=qreal()'.format(symbol)
        return '{0} {1}={2}'.format(symbol.type, symbol, default_type_value(symbol))
    elif symbol.type.is_list:
        nested = return_type(symbol.type.nested)
        return 'const QVariantList &{1}=QVariantList()'.format(nested, symbol)
    elif symbol.type.is_model:
        nested = symbol.type.nested
        if nested.is_primitive:
            return '{0}VariantModel *{1}=QVariantModel'.format(prefix, symbol)
        elif nested.is_complex:
            return 'QIviPagingModel *{0}=nullptr'.format(symbol)
    else:
        return 'const {0}{1} &{2}={0}{1}()'.format(prefix, symbol.type.reference.name, symbol)
    jinja_error('parameter_type_default: Unknown parameter {0} of type {1}'.format(symbol,
                                                                                   symbol.type))


def parameter_type(symbol):
    """
    Return the parameter declaration for properties, handle camel case module name
    """
    prefix = Filters.classPrefix
    if symbol.type.is_enum or symbol.type.is_flag:
        return '{0}{1}Module::{2} {3}'.format(prefix, upper_first(symbol.type.reference.module.module_name), flag_type(symbol), symbol)
    if symbol.type.is_void or symbol.type.is_primitive:
        if symbol.type.name == 'string':
            return 'const QString &{0}'.format(symbol)
        if symbol.type.name == 'var':
            return 'const QVariant &{0}'.format(symbol)
        if symbol.type.name == 'real':
            return 'qreal {0}'.format(symbol)
        return '{0} {1}'.format(symbol.type, symbol)
    elif symbol.type.is_list:
        nested = return_type(symbol.type.nested)
        return 'const QVariantList &{1}'.format(nested, symbol)
    elif symbol.type.is_model:
        nested = symbol.type.nested
        if nested.is_primitive:
            return '{0}VariantModel *{1}'.format(prefix, symbol)
        elif nested.is_complex:
            return 'QIviPagingModel *{0}'.format(symbol)
    else:
        return 'const {0}{1} &{2}'.format(prefix, symbol.type.reference.name, symbol)
    jinja_error('parameter_type: Unknown parameter {0} of type {1}'.format(symbol, symbol.type))


def return_type(symbol):
    """
    Return the type declaration for properties, handle camel case module name
    """
    prefix = Filters.classPrefix
    if symbol.type.is_enum or symbol.type.is_flag:
        return('{0}{1}Module::{2}'.format(prefix,
                                          upper_first(symbol.type.reference.module.module_name),
                                          flag_type(symbol)))
    if symbol.type.is_void or symbol.type.is_primitive:
        if symbol.type.name == 'string':
            return 'QString'
        if symbol.type.name == 'var':
            return 'QVariant'
        if symbol.type.name == 'real':
            return 'qreal'
        return symbol.type.name
    elif symbol.type.is_list:
        nested = return_type(symbol.type.nested)
        return 'QVariantList'.format(nested)
    elif symbol.type.is_model:
        nested = symbol.type.nested
        if nested.is_primitive:
            return '{0}VariantModel *'.format(prefix)
        elif nested.is_complex:
            return 'QIviPagingModel *'
    else:
        return '{0}{1}'.format(prefix, symbol.type.reference.name)
    jinja_error('return_type: Unknown symbol {0} of type {1}'.format(symbol, symbol.type))


def flag_type(symbol):
    """
    Return the annotation for the flag type if available, the plural otherwise
    """
    actualType = symbol
    if symbol.type.reference:
        actualType = symbol.type.reference
    if actualType.is_flag:
        if 'config' in actualType.tags and 'type' in actualType.tags['config']:
            return actualType.tags['config']['type']
        return '{0}s'.format(actualType)
    return actualType


def domain_values(symbol):
    """
    Returns domain values for property (if defined by @domain)
    """
    deprecated_filter()

    if type(symbol) is Property:
        if 'config_simulator' in symbol.tags:
            if 'domain' in symbol.tags['config_simulator']:
                return symbol.tags['config_simulator']['domain']
    return None


def getter_name(symbol):
    """
    Returns the getter name of the property
    """
    if type(symbol) is Property:
        if 'config' in symbol.tags and 'getter_name' in symbol.tags['config']:
            return symbol.tags['config']['getter_name']
    return symbol


def setter_name(symbol):
    """
    Returns the setter name of the property
    """
    if type(symbol) is Property:
        if 'config' in symbol.tags and 'setter_name' in symbol.tags['config']:
            return symbol.tags['config']['setter_name']
    return 'set' + symbol.name[0].upper() + symbol.name[1:]


def range_value(symbol, index, key):
    """
    Returns value for property (if defined by @range index or key)
    """
    deprecated_filter()

    if type(symbol) is Property and symbol.type.is_int or symbol.type.is_real:
        if 'config_simulator' in symbol.tags:
            if 'range' in symbol.tags['config_simulator']:
                return symbol.tags['config_simulator']['range'][index]
            if key in symbol.tags['config_simulator']:
                return symbol.tags['config_simulator'][key]
    return None


def range_high(symbol):
    """
    Returns maximum value for property (if defined by @range or @maximum)
    """
    deprecated_filter()

    return range_value(symbol, 1, 'maximum')


def range_low(symbol):
    """
    Returns minimum value for property (if defined by @range or @minimum)
    """
    deprecated_filter()

    return range_value(symbol, 0, 'minimum')


def has_domains(properties):
    """
    Returns true if any property has range or domain tags
    """
    deprecated_filter()

    for property in properties:
        if 'config_simulator' in property.tags:
            for p in ['range', 'domain', 'minimum', 'maximum']:
                if p in property.tags['config_simulator']:
                    return True
    return False


def strip_QT(s):
    """
    If the given string starts with QT, stip it away.
    """
    s = str(s)
    if s.startswith('QT'):
        return s[2:]
    return s


def json_domain(properties):
    """
    Returns property domains formated in json
    """
    deprecated_filter()

    data = {}
    if len(properties):
        data["iviVersion"] = builtin_config.config["VERSION"]
    for property in properties:
        if 'config_simulator' in property.tags:
            for p in ['range', 'domain', 'minimum', 'maximum']:
                if (property.tags['config_simulator'] is not None
                        and p in property.tags['config_simulator']):
                    if property.name not in data:
                        data[property.name] = {}
                    data[property.name][p] = property.tags['config_simulator'][p]
    return json.dumps(data, separators=(',', ':'))


def simulationData(module):
    data = {}
    for interface in module.interfaces:
        iData = {}
        if 'config_simulator' in interface.tags:
            iData = interface.tags['config_simulator']
        for property in interface.properties:
            if 'config_simulator' in property.tags:
                for p in ['range', 'domain', 'minimum', 'maximum', 'default']:
                    if (property.tags['config_simulator'] is not None
                            and p in property.tags['config_simulator']):
                        if property.name not in iData:
                            iData[property.name] = {}
                        if p not in iData[property.name]:
                            iData[property.name][p] = {}
                        zones = iData.get('zones', None)
                        res = property.tags['config_simulator'][p]
                        if isinstance(res, dict) and zones:
                            for zone in zones:
                                if zone in res:
                                    iData[property.name][p][zone] = symbolToJson(res[zone], property.type)
                            # Also check the entry for the general zone (=)
                            if "=" in res:
                                iData[property.name][p]["="] = symbolToJson(res["="], property.type)
                        else:
                            iData[property.name][p] = symbolToJson(res, property.type)
        data[interface.name] = iData
    return json.dumps(data, indent='  ')


def symbolToJson(data, symbol):
    if symbol.type.is_struct:
        t = symbol.type
        if not (isinstance(data, dict) or isinstance(data, list)):
            jinja_error('simulationData: value in annotation for symbol {0} is supposed to be a dict or list'.format(symbol.qualified_name))
        if len(data) != len(t.reference.fields):
            print(len(data), len(t.reference.fields))
            jinja_error('simulationData: argument count in annotation for symbol {0} and number of struct fields '
                        'does not match: Expected {1} instead of {2}'.format(symbol.qualified_name, len(t.reference.fields), len(data)))
        newList = list(symbolToJson(property, list(t.reference.fields)[idx]) for idx, property in enumerate(data))
        return {"type": symbol.type.name, "value": newList}
    elif symbol.type.is_enum or symbol.type.is_flag:
        module_name = symbol.type.reference.module.module_name
        return {"type": "enum", "value": enum_value(data, module_name)}
    elif symbol.type.is_list or symbol.type.is_model:
        nested = symbol.type.nested
        if nested.is_complex:
            newList = []
            for value in data:
                newList.append(symbolToJson(value, nested))
            return newList
    return data


def qml_control_properties(symbol, backend_object):
    """
    Returns properties of the QML control matching to this
    IDL type (e.g. min/max properties)
    """
    deprecated_filter()

    prop_str = lower_first(symbol) + "Control"
    if isinstance(symbol, Property):
        top = range_high(symbol)
        bottom = range_low(symbol)
        binding = "value: {0}.{1};".format(backend_object, symbol.name)
        if top is not None and bottom is not None:
            return 'id: {0};  from: {1}; to: {2}; {3}'.format(prop_str, bottom, top, binding)

        if top is not None or bottom is not None:
            if top is None:
                return 'id: {0};  from: {1}; to:100000; {2}'.format(prop_str, bottom, binding)
            elif bottom is None:
                return 'id: {0};  from:-100000; to: {1}; {2}'.format(prop_str, top, binding)

        values = domain_values(symbol)
        if values is None and (symbol.type.is_enum or symbol.type.is_flag):
            values_string = ' '.join('ListElement {{ key: "{0}"; value: {1}.{0} }}'.format(e, qml_type(symbol.interface)) for e in symbol.type.reference.members)
            return 'id: {0}; textRole: "key"; {2} model: ListModel {{ {1} }}'.format(prop_str,
                                                                                     values_string,
                                                                                     binding)
        if values is not None:
            values_string = ','.join('"'+str(e)+'"' for e in values)
            return 'id: {0}; model: [ {1} ]; '.format(prop_str, values_string)
        if symbol.type.is_bool:
            binding = "checked: {0}.{1};".format(backend_object, symbol.name)
            return 'id: {0}; {1}'.format(prop_str, binding)
        if symbol.type.is_real or symbol.type.is_int or symbol.type.is_string:
            binding = "text: {0}.{1};".format(backend_object, symbol.name)
            return 'id: {0}; {1}'.format(prop_str, binding)

    if isinstance(symbol, Parameter):
        return 'id: {1}Param{0}'.format(prop_str, symbol.operation)
    if isinstance(symbol, Field):
        return 'id: {1}_{0}'.format(prop_str, lower_first(symbol.struct))


def qml_control_signal_parameters(symbol):
    """
    Returns the parameters for calling the signal using the values from the ui controls
    """
    deprecated_filter()

    return ', '.join('{0}Param{1}Control.{2}'.format(e.operation, lower_first(e),qml_binding_property(e)) for e in symbol.parameters)


def qml_meta_control_name(symbol):
    """
    Returns name of the QML control needed to display this type based on the meta
    data of  the symbol -- if symbol has some meta data (e.g. value limits or domain)
    then control name is taken based on these constraints. Otherwise returns None.
    """
    deprecated_filter()

    top = range_high(symbol)
    bottom = range_low(symbol)
    if top is not None and bottom is not None:
        return 'Slider'

    if top is not None or bottom is not None:
        return 'SpinBox'

    values = domain_values(symbol)
    if values is not None:
        return "ComboBox"


def qml_type_control_name(symbol):
    """
    Returns name of the QML control inferred based on the type of the symbol.
    """
    deprecated_filter()

    t = symbol.type
    if t.is_string or t.is_int or t.is_real:
        return "TextField"
    elif t.is_bool:
        return "CheckBox"
    elif t.is_enum:
        if t.reference.is_enum:
            return "EnumControl"
        elif t.reference.is_flag:
            return "FlagControl"
    elif t.is_flag:
        return "FlagControl"
    return "TextField"


def qml_control_name(symbol):
    """
    Returns name of the QML control for the symbol. First it checks meta data (as it may
    influence the control type) and if nothing is  defined there, it falls back to the
    symbol actual type.
    """
    deprecated_filter()

    # First try to calculate control name based on the tags
    control_name = qml_meta_control_name(symbol)
    # If nothing is defined, calculate it based on its type
    if control_name is None:
        control_name = qml_type_control_name(symbol)
    return control_name


def qml_control(symbol, backend_object):
    """
    Returns QML code for the control (or group of controls) to represent the editing UI for the
    symbol.
    """
    deprecated_filter()

    if symbol.type.is_struct:
        return qml_struct_control(symbol)

    return "{0} {{ {1} }}".format(qml_control_name(symbol),
                                  qml_control_properties(symbol, backend_object))


def qml_binding_property(symbol):
    """
    :param symbol: property which is being bound by the control
    :return: name of the property of the QML control to be bound with
    """
    deprecated_filter()

    control_name = qml_control_name(symbol)
    if control_name == "CheckBox":
        return "checked"
    elif (control_name == "Slider" or control_name == "SpinBox" or control_name == "FlagControl"
            or control_name == "EnumControl"):
        return "value"
    elif control_name == "TextField":
        return "text"
    elif control_name == "ComboBox":
        return "currentIndex"


def qml_struct_control(symbol):
    deprecated_filter()

    if symbol.type.is_struct and symbol.type.reference.fields:
        result = "Rectangle { ColumnLayout { "
        for field in symbol.type.reference.fields:
            result += qml_control(field)
        result += "}}"
        return result


def qml_info_type(symbol):
    """
    Returns the correct type for the symbol, to be used inside the qmltype templates
    """
    prefix = Filters.classPrefix
    if symbol.type.is_enum or symbol.type.is_flag:
        return('{0}{1}Module::{2}'.format(prefix, upper_first(symbol.module.module_name),
               flag_type(symbol)))
    elif symbol.type.is_void or symbol.type.is_primitive:
        if symbol.type.is_real:
            return 'double'
        return symbol.type.name
    elif symbol.type.is_struct:
        return 'QVariant'
    elif symbol.type.is_list:
        return 'QVariantList'
    elif symbol.type.is_model:
        return 'QIviPagingModel'
    else:
        jinja_error('qml_info_type: Unknown symbol {0} of type {1}'.format(symbol, symbol.type))


def qml_type(symbol):
    """
    :param interface:
    :return: Returns the name of the interface for using in QML. This name is defined in the IDL
    under the "config" tag as "qml_type". This annotation is optional, if not provided, the
    interface name is used.
    """
    result = symbol.name
    if 'qml_type' in symbol.tags['config']:
        result = symbol.tags['config']['qml_type']
    elif 'qml_name' in symbol.tags['config']:
        result = symbol.tags['config']['qml_name']
    return result


def model_type(symbol):
    deprecated_filter()

    if symbol.type.is_model:
        nested = symbol.type.nested
        return '{0}Model'.format(nested)
    return None


def struct_includes(symbol):
    includesSet = set()
    tpl = '#include \"{0}.h\"'

    if isinstance(symbol, Struct):
        for val in symbol.fields:
            if val.type.is_struct:
                includesSet.add(tpl.format(val.type.reference.name).lower())
    elif isinstance(symbol, Interface):
        for val in symbol.properties:
            if val.type.is_struct:
                includesSet.add(tpl.format(val.type.reference.name).lower())
        for op in symbol.operations:
            for param in op.parameters:
                if param.type.is_struct:
                    includesSet.add(tpl.format(param.type.reference.name).lower())
            if op.type.is_struct:
                includesSet.add(tpl.format(op.type.reference.name).lower())
        for op in symbol.signals:
            for param in op.parameters:
                if param.type.is_struct:
                    includesSet.add(tpl.format(param.type.reference.name).lower())

    return includesSet


def comment_text(comment):
    """
    Returns the text of the passed comment without the leading/trailing comment tokens e.g. /**, *
    """
    comment_start = ['/**', '/*!', '/*']
    processed = []
    isComment = False

    # No comment is NOT a error
    if len(comment) == 0:
        return processed

    for token in comment_start:
        if (comment.startswith(token)):
            isComment = True
            break
    if isComment:
        comment = comment[3:-2]
    else:
        jinja_error("comment_text: The provided comment needs to be start with one of these "
                    "strings: {}".format(comment_start))

    for line in comment.splitlines():
        line = line.lstrip(" *")
        processed.append(line)
    return processed


def register_filters(generator):
    generator.env.keep_trailing_newline = True
    generator.register_filter('return_type', return_type)
    generator.register_filter('parameter_type_default', parameter_type_default)
    generator.register_filter('parameter_type', parameter_type)
    generator.register_filter('getter_name', getter_name)
    generator.register_filter('setter_name', setter_name)
    generator.register_filter('test_type_value', test_type_value)
    generator.register_filter('default_type_value', default_type_value)
    generator.register_filter('default_value', default_value)
    generator.register_filter('model_type', model_type)
    generator.register_filter('flag_type', flag_type)
    generator.register_filter('range_low', range_low)
    generator.register_filter('range_high', range_high)
    generator.register_filter('strip_QT', strip_QT)
    generator.register_filter('domain_values', domain_values)
    generator.register_filter("enum_value", enum_value)
    generator.register_filter("tag_by_path", tag_by_path)
    generator.register_filter("conf_sim_tag", conf_sim_tag)
    generator.register_filter('has_domains', has_domains)
    generator.register_filter('simulationData', simulationData)
    generator.register_filter('json_domain', json_domain)
    generator.register_filter('qml_info_type', qml_info_type)
    generator.register_filter('qml_type', qml_type)
    generator.register_filter('qml_control', qml_control)
    generator.register_filter('qml_binding_property', qml_binding_property)
    generator.register_filter('qml_control_signal_parameters', qml_control_signal_parameters)
    generator.register_filter('struct_includes', struct_includes)
    generator.register_filter('comment_text', comment_text)
