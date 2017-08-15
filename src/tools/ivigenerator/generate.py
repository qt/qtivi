#!/usr/bin/env python3
# Copyright (C) 2017 Pelagicore AG
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

import re
import os
import click
import logging.config
import yaml
import json
from path import Path

from qface.generator import FileSystem, Generator
from qface.helper.qtcpp import Filters
from qface.helper.doc import parse_doc
from qface.watch import monitor
from qface.idl.domain import Property, Parameter, Field, Struct

here = Path(__file__).dirname()

log = logging.getLogger(__file__)

Filters.classPrefix = ''

QT_AS_VERSION = 2.0
IVI_DEFAULT_TEMPLATES = ['frontend', 'backend_simulator', 'generation_validator', 'control_panel']

def tag_by_path(symbol, path, default_value=False):
    """
    Find the tag given by its full path in the object hierarchy,
    like "property.config_sim.zones.right". If some part of the
    path is missing, return None
    """
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
    return tag_by_path(symbol, "config_simulator." + path, default_value)


def enum_value_to_cppliteral(value, module_name):
    value = value.strip().rsplit('.', 1)[-1]
    return '{0}{1}Module::{2}'.format(Filters.classPrefix, module_name, value)


def enum_value(value, module_name):
    sub_values = value.split('|')
    sub_values = [enum_value_to_cppliteral(v, module_name) for v in sub_values]
    return "|".join(sub_values)

def default_type_value(symbol):
    """
    Find the default value for the type. Models are initialized as nullptr
    """
    prefix = Filters.classPrefix
    t = symbol.type  # type: qface.domain.TypeSymbol
    if t.is_primitive:
        if t.is_int:
            return 'int(0)'
        if t.is_bool:
            return 'bool(false)'
        if t.is_string:
            return 'QString()'
        if t.is_real:
            return 'qreal(0.0)'
        if t.is_variant:
            return 'QVariant()'
    elif t.is_void:
        return ''
    elif t.is_enum:
        module_name = t.reference.module.module_name
        value = next(iter(t.reference.members))
        return '{0}{1}Module::{2}'.format(prefix, module_name, value)
    elif t.is_flag:
        return '0'
    elif symbol.type.is_list:
        nested = Filters.returnType(symbol.type.nested)
        return 'QVariantList()'.format(nested)
    elif symbol.type.is_struct:
        return '{0}{1}()'.format(prefix, symbol.type)
    elif symbol.type.is_model:
        return 'nullptr'
    return 'XXX'


def default_value(symbol):
    """
    Find the default value used by the simulator backend
    """
    res = default_type_value(symbol)
    if symbol.type.is_model:
        nested = symbol.type.nested
        # TODO: find a way of passing parent object
        return 'new {0}Model(parent)'.format(nested)
    if 'config_simulator' in symbol.tags and 'default' in symbol.tags['config_simulator']:
        res = symbol.tags['config_simulator']['default']
        t = symbol.type
        if t.is_enum:
            module_name = t.reference.module.module_name
            return enum_value(res, module_name)
        # in case it's bool, Python True is sent to the C++ as "True", let's take care of that
        if t.is_bool:
            if res:
                return 'true'
            else:
                return 'false'
        if t.is_string:
            return '"{0}"'.format(re.escape(res))
    return res


def parameter_type(symbol):
    """
    Return the parameter declaration for properties, handle camel case module name
    """
    prefix = Filters.classPrefix
    if symbol.type.is_enum or symbol.type.is_flag:
        return '{0}{1}Module::{2} {3}'.format(prefix, symbol.module.module_name, flag_type(symbol), symbol)
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
            return '{0}{1}Model *{2}'.format(prefix, nested, symbol)
    else:
        return 'const {0}{1} &{2}'.format(prefix, symbol.type, symbol)
    return 'QFace Error: Unknown parameter {0} of type {1}'.format(symbol, symbol.type)


def return_type(symbol):
    """
    Return the type declaration for properties, handle camel case module name
    """
    prefix = Filters.classPrefix
    if symbol.type.is_enum or symbol.type.is_flag:
        return('{0}{1}Module::{2}'.format(prefix, symbol.module.module_name, flag_type(symbol)))
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
            return '{0}{1}Model *'.format(prefix, nested)
    else:
        return '{0}{1}'.format(prefix, symbol.type)
    return 'QFace Error: Unknown symbol {0} of type {1}'.format(symbol, symbol.type)


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
    return range_value(symbol, 1, 'maximum')


def range_low(symbol):
    """
    Returns minimum value for property (if defined by @range or @minimum)
    """
    return range_value(symbol, 0, 'minimum')


def has_domains(properties):
    """
    Returns true if any property has range or domain tags
    """
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
    data = {}
    if len(properties):
        data["iviVersion"] = QT_AS_VERSION
    for property in properties:
        if 'config_simulator' in property.tags:
            for p in ['range', 'domain', 'minimum', 'maximum']:
                if p in property.tags['config_simulator']:
                    if not property.name in data:
                        data[property.name] = {}
                    data[property.name][p] = property.tags['config_simulator'][p]
    return json.dumps(data, separators=(',', ':'))


def jsonify(obj):
    """
    This is copied from QFace (qface/filters.py), should be replaced with
    original when it's released
    """
    try:
        # all symbols have a toJson method, try it
        return json.dumps(obj.toJson(), indent='  ')
    except AttributeError:
        pass
    return json.dumps(obj, indent='  ')


def lower_first_filter(s):
    s = str(s)
    return s[0].lower() + s[1:]

def qml_control_properties(symbol, backend_object):
    """
    Returns properties of the QML control matching to this
    IDL type (e.g. min/max properties)
    """
    prop_str = lower_first_filter(symbol)
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
        if values is None and symbol.type.is_enum:
            values = symbol.type.reference.members
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
        return 'id: {1}_{0}'.format(prop_str, lower_first_filter(symbol.struct))


def qml_meta_control_name(symbol):
    """
    Returns name of the QML control needed to display this type based on the meta
    data of  the symbol -- if symbol has some meta data (e.g. value limits or domain)
    then control name is taken based on these constraints. Otherwise returns None.
    """
    top = range_high(symbol)
    bottom = range_low(symbol)
    if top is not None and bottom is not None:
        return 'Slider'

    if top is not None or bottom is not None:
        return 'SpinBox'

    values = domain_values(symbol)
    if values is not None:
        return "ComboBox"


def qml_flag_control(symbol):
    """
    Returns QML code for creation of group of check-boxes for
    the flag property
    """
    # First try to calculate control name based on the tags
    result = qml_meta_control_name(symbol)
    # If nothing is defined, calculate it based on its type
    if result is None and symbol.type.reference.members:
        # form a group of checkboxes
        values = symbol.type.reference.members
        result = ""
        for value in values:
            result+="Text{{ text:'{0}'}} CheckBox {{ id: flag{0}; }}\n".format(value)
    return result


def qml_type_control_name(symbol):
    """
    Returns name of the QML control inferred based on the type of the symbol.
    """
    t = symbol.type
    if t.is_string or t.is_int or t.is_real:
        return "TextField"
    elif t.is_bool:
        return "CheckBox"
    elif t.is_enum:
        if t.reference.is_enum:
            return "ComboBox"
        elif t.reference.is_flag:
            return qml_flag_control(symbol)
    return "TextField"


def qml_control_name(symbol):
    """
    Returns name of the QML control for the symbol. First it checks meta data (as it may
    influence the control type) and if nothing is  defined there, it falls back to the
    symbol actual type.
    """
    # First try to calculate control name based on the tags
    control_name = qml_meta_control_name(symbol)
    # If nothing is defined, calculate it based on its type
    if control_name is None:
        control_name = qml_type_control_name(symbol)
    return control_name


def qml_control(symbol, backend_object):
    """
    Returns QML code for the control (or group of controls) to represent the editing UI for the symbol.
    """

    if symbol.type.is_enum and symbol.type.reference.is_flag:
        return qml_flag_control(symbol)
    if symbol.type.is_struct:
        return qml_struct_control(symbol)

    return "{0} {{ {1} }}".format(qml_control_name(symbol), qml_control_properties(symbol, backend_object))


def qml_binding_property(symbol):
    """
    :param symbol: property which is being bound by the control
    :return: name of the property of the QML control to be bound with
    """
    control_name = qml_control_name(symbol)
    if control_name == "CheckBox":
        return "checked"
    elif control_name == "Slider" or control_name == "TextField" or control_name == "SpinBox":
        return "value"
    elif control_name == "ComboBox":
        return "currentText"

def qml_struct_control(symbol):
    if symbol.type.is_struct and symbol.type.reference.fields:
        result = "Rectangle { ColumnLayout { "
        for field in symbol.type.reference.fields:
            result += qml_control(field)
        result += "}}"
        return result


def qml_type(interface):
    """
    :param interface:
    :return: Returns the name of the interface for using in QML. This name is defined in the IDL under
    the "config" tag as "qml_type". This annotation is optional, if not provided, the interface name is
    used.
    """
    result = interface.name
    if 'qml_type' in interface.tags['config']:
        result = interface.tags['config']['qml_type']
    return result


def model_type(symbol):
    if symbol.type.is_model:
        nested = symbol.type.nested
        return '{0}Model'.format(nested)
    return None


def generate(tplconfig, moduleConfig, src, dst):
    log.debug('run {0} {1}'.format(src, dst))
    system = FileSystem.parse(src)
    generator = Generator(search_path=tplconfig)
    generator.register_filter('return_type', return_type)
    generator.register_filter('parameter_type', parameter_type)
    generator.register_filter('getter_name', getter_name)
    generator.register_filter('setter_name', setter_name)
    generator.register_filter('default_type_value', default_type_value)
    generator.register_filter('default_value', default_value)
    generator.register_filter('model_type', model_type)
    generator.register_filter('flag_type', flag_type)
    generator.register_filter('parse_doc', parse_doc)
    generator.register_filter('lowerfirst', lower_first_filter)
    generator.register_filter('range_low', range_low)
    generator.register_filter('range_high', range_high)
    generator.register_filter('strip_QT', strip_QT)
    generator.register_filter('domain_values', domain_values)
    generator.register_filter("enum_value", enum_value)
    generator.register_filter("tag_by_path", tag_by_path)
    generator.register_filter("conf_sim_tag", conf_sim_tag)
    generator.register_filter('jsonify', jsonify)
    generator.register_filter('has_domains', has_domains)
    generator.register_filter('json_domain', json_domain)
    generator.register_filter('qml_type', qml_type)
    generator.register_filter('qml_control', qml_control)
    generator.register_filter('qml_binding_property', qml_binding_property)

    srcFile = os.path.basename(src[0])
    srcBase = os.path.splitext(srcFile)[0]
    ctx = {'dst': dst, 'qtASVersion': QT_AS_VERSION, 'srcFile':srcFile, 'srcBase':srcBase}
    gen_config = yaml.load(open(here / '{0}.yaml'.format(os.path.basename(tplconfig))))
    for module in system.modules:
        log.debug('generate code for module %s', module)
        module.add_tag('config')
        for val, key in moduleConfig.items():
            module.add_attribute('config', val, key)
        ctx.update({'module': module})
        # TODO: refine that, probably just use plain output folder
        dst = generator.apply('{{dst}}', ctx)
        generator.destination = dst
        module_rules = gen_config['generate_rules']['module_rules']
        if module_rules is None: module_rules = []
        for rule in module_rules:
            preserve = rule['preserve'] if 'preserve' in rule else False
            generator.write(rule['dest_file'], rule['template_file'], ctx, preserve)
        for interface in module.interfaces:
            log.debug('generate backend code for interface %s', interface)
            interface.add_tag('config')
            ctx.update({'interface': interface})
            interface_rules = gen_config['generate_rules']['interface_rules']
            if interface_rules is None: interface_rules = []
            for rule in interface_rules:
                preserve = rule['preserve'] if 'preserve' in rule else False
                generator.write(rule['dest_file'], rule['template_file'], ctx, preserve)
        if 'struct_rules' in gen_config['generate_rules'] and isinstance(gen_config['generate_rules']['struct_rules'], list):
            for struct in module.structs:
                log.debug('generate code for struct %s', struct)
                struct.add_tag('config')
                ctx.update({'struct': struct})
                for rule in gen_config['generate_rules']['struct_rules']:
                    preserve = rule['preserve'] if 'preserve' in rule else False
                    generator.write(rule['dest_file'], rule['template_file'], ctx, preserve)


def run(format, moduleConfig, src, dst):
    if format in IVI_DEFAULT_TEMPLATES:
        tplConfig = 'templates_{0}'.format(format)
        generate(here / tplConfig, moduleConfig, src, dst)
    else:
        if os.path.exists(format):
            generate(format, moduleConfig, src, dst)
        else:
            print('Format "{0}" is invalid. Should be one of {1} or an existing template folder'.format(format, IVI_DEFAULT_TEMPLATES))


@click.command()
@click.option('--reload/--no-reload', default=False)
@click.option('--format', '-f', multiple=False)
@click.option('--module', default=False)
@click.option('--validation_info', default=False)
@click.argument('src', nargs=-1, type=click.Path(exists=True))
@click.argument('dst', nargs=1, type=click.Path(exists=True))

def app(src, dst, format, reload, module, validation_info):
    """Takes several files or directories as src and generates the code
    in the given dst directory."""
    if reload:
        script = '{0} {1} {2}'.format(Path(__file__).abspath(), ' '.join(src), dst)
        monitor(src, script)
    else:
        moduleConfig = {
            "module": module,
            "validation_info": validation_info
        }
        run(format, moduleConfig, src, dst)


if __name__ == '__main__':
    app()
