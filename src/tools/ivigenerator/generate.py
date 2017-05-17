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
import click
import logging.config
import yaml
import json
from path import Path

from qface.generator import FileSystem, Generator
from qface.helper.qtcpp import Filters
from qface.helper.doc import parse_doc
from qface.watch import monitor
from qface.idl.domain import Property

here = Path(__file__).dirname()

log = logging.getLogger(__file__)

Filters.classPrefix = ''

QT_AS_VERSION = 1.2

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
    res = Filters.defaultValue(symbol)
    if symbol.type.is_model:
        return 'nullptr'
    return res


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


def domain_values(symbol):
    """
    Returns domain values for property (if defined by @domain)
    """
    if type(symbol) is Property:
        if 'config_simulator' in symbol.tags:
            if 'domain' in symbol.tags['config_simulator']:
                return symbol.tags['config_simulator']['domain']
    return None


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
    return json.dumps(data, separators=(',',':'))


def lower_first_filter(s):
    s = str(s)
    return s[0].lower() + s[1:]


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
    module_name = symbol.module.module_name
    if symbol.type.is_model:
        nested = symbol.type.nested
        return '{0}Model'.format(nested)
    return None


def generate(tplconfig, moduleConfig, src, dst):
    log.debug('run {0} {1}'.format(src, dst))
    system = FileSystem.parse(src)
    generator = Generator(search_path=here / tplconfig)
    generator.register_filter('return_type', Filters.returnType)
    generator.register_filter('parameter_type', Filters.parameterType)
    generator.register_filter('default_type_value', default_type_value)
    generator.register_filter('default_value', default_value)
    generator.register_filter('model_type', model_type)
    generator.register_filter('parse_doc', parse_doc)
    generator.register_filter('lowerfirst', lower_first_filter)
    generator.register_filter('range_low', range_low)
    generator.register_filter('range_high', range_high)
    generator.register_filter('domain_values', domain_values)
    generator.register_filter("enum_value", enum_value)
    generator.register_filter("tag_by_path", tag_by_path)
    generator.register_filter("conf_sim_tag", conf_sim_tag)
    generator.register_filter('has_domains', has_domains)
    generator.register_filter('json_domain', json_domain)
    generator.register_filter('qml_type', qml_type)

    ctx = {'dst': dst, 'qtASVersion': QT_AS_VERSION}
    gen_config = yaml.load(open(here / '{0}.yaml'.format(tplconfig)))
    for module in system.modules:
        log.debug('generate code for module %s', module)
        module.add_tag('config')
        for val, key in moduleConfig.items():
            module.add_attribute('config', val, key)
        ctx.update({'module': module})
        #TODO: refine that, probably just use plain output folder
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
            for rule in gen_config['generate_rules']['interface_rules']:
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


def run(formats, moduleConfig, src, dst):
    for f in formats:
        switcher = {
            'frontend': 'templates_frontend',
            'backend_simulator': 'templates_backend_simulator',
            'test': 'templates_test',
        }
        tplConfig = switcher.get(f, 'unknown')
        if tplConfig == 'unknown':
            log.debug('unknown format {0}'.format(f))
        else:
            generate(tplConfig, moduleConfig, src, dst)


@click.command()
@click.option('--reload/--no-reload', default=False)
@click.option('--format', '-f', multiple=True, type=click.Choice(['frontend', 'backend_simulator', 'test']))
@click.option('--module', default=False)
@click.argument('src', nargs=-1, type=click.Path(exists=True))
@click.argument('dst', nargs=1, type=click.Path(exists=True))
def app(src, dst, format, reload, module):
    """Takes several files or directories as src and generates the code
    in the given dst directory."""
    if reload:
        script = '{0} {1} {2}'.format(Path(__file__).abspath(), ' '.join(src), dst)
        monitor(src, script)
    else:
        moduleConfig = {
            "module": module
        }
        run(format, moduleConfig, src, dst)


if __name__ == '__main__':
    app()
