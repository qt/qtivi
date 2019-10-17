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

import re
import os
import fnmatch
import click
import logging.config
import yaml
import sys
from path import Path

from qface.generator import FileSystem, Generator
from qface.watch import monitor
from qface.idl.domain import Module, Interface, Property, Parameter, Field, Struct

from jinja2 import TemplateAssertionError
import inspect

import generator.builtin_config as builtin_config
from generator.global_functions import register_global_functions
from generator.filters import register_filters

here = Path(__file__).dirname()

log = logging.getLogger(__file__)

currentQFaceSrcFile = ''
builtinTemplatesPath = Path(here / 'templates')
builtinTemplates = [os.path.splitext(f)[0] for f in os.listdir(builtinTemplatesPath) if fnmatch.fnmatch(f, '*.yaml')]

def generate(tplconfig, moduleConfig, annotations, imports, src, dst):
    log.debug('run {0} {1}'.format(src, dst))
    FileSystem.strict = True
    Generator.strict = True

    # First run without imports to know the name of the modules we want to generate
    module_names = []
    system = FileSystem.parse(src)
    for module in system.modules:
        module_names.append(module.name)

    # Second run with imports to resolve all needed type information
    all_files = imports + src
    system = FileSystem.parse(all_files)
    for annotations_file in annotations:
        log.debug('{0}'.format(annotations_file))
        if not os.path.isabs(annotations_file):
            annotations_file = Path.getcwd() / str(annotations_file)
        if not Path(annotations_file).exists():
            print('no such annotation file: {0}'.format(annotations_file))
            exit(1)
        FileSystem.merge_annotations(system, Path(annotations_file))
    generator = Generator(search_path=[tplconfig, builtinTemplatesPath])
    generator.env.keep_trailing_newline = True

    register_global_functions(generator)
    register_filters(generator)

    srcFile = os.path.basename(src[0])
    srcBase = os.path.splitext(srcFile)[0]
    global currentQFaceSrcFile
    currentQFaceSrcFile = src[0]
    ctx = {'dst': dst, 'qtASVersion': builtin_config.config["VERSION"], 'srcFile':srcFile, 'srcBase':srcBase, 'features': builtin_config.config["FEATURES"]}
    gen_config = yaml.load(open(os.path.dirname(tplconfig) + '/{0}.yaml'.format(os.path.basename(tplconfig))), Loader=yaml.SafeLoader)

    #Make sure the config tag is available for all our symbols
    for module in system.modules:
        module.add_tag('config')
        for interface in module.interfaces:
            interface.add_tag('config')
            for property in interface.properties:
                property.add_tag('config')
            for operation in interface.operations:
                operation.add_tag('config')
            for signal in interface.signals:
                signal.add_tag('config')
        for struct in module.structs:
            struct.add_tag('config')
            for field in struct.fields:
                field.add_tag('config')
        for enum in module.enums:
            enum.add_tag('config')
            for member in enum.members:
                member.add_tag('config')

    ctx.update({'modules': system.modules})
    for module in system.modules:
        # Only generate files for the modules detected from the first parse run
        if not module.name in module_names:
            continue

        log.debug('generate code for module %s', module)

        # Report early if we cannot find the imported modules
        for imp in module._importMap:
            if imp not in system._moduleMap:
                sys.exit("{0}: Couldn't resolve import '{1}'".format(currentQFaceSrcFile, imp))

        for val, key in moduleConfig.items():
            module.add_attribute('config', val, key)
        ctx.update({'module': module})
        # TODO: refine that, probably just use plain output folder
        dst = generator.apply('{{dst}}', ctx)
        generator.destination = dst
        module_rules = gen_config['generate_rules']['module_rules']
        force = moduleConfig['force']
        if module_rules is None: module_rules = []
        for rule in module_rules:
            preserve = rule['preserve'] if 'preserve' in rule else False
            generator.write(rule['dest_file'], rule['template_file'], ctx, preserve, force)
        for interface in module.interfaces:
            log.debug('generate backend code for interface %s', interface)
            ctx.update({'interface': interface})
            interface_rules = gen_config['generate_rules']['interface_rules']
            if interface_rules is None: interface_rules = []
            for rule in interface_rules:
                preserve = rule['preserve'] if 'preserve' in rule else False
                generator.write(rule['dest_file'], rule['template_file'], ctx, preserve, force)
        if 'struct_rules' in gen_config['generate_rules'] and isinstance(gen_config['generate_rules']['struct_rules'], list):
            for struct in module.structs:
                log.debug('generate code for struct %s', struct)
                ctx.update({'struct': struct})
                for rule in gen_config['generate_rules']['struct_rules']:
                    preserve = rule['preserve'] if 'preserve' in rule else False
                    generator.write(rule['dest_file'], rule['template_file'], ctx, preserve, force)


def run(format, moduleConfig, annotations, imports, src, dst):
    templatePath = format
    if format in builtinTemplates:
        templatePath = builtinTemplatesPath / format

    if os.path.exists(templatePath):
        generate(templatePath, moduleConfig, annotations, imports, src, dst)
    else:
        print('Invalid Format: {0}. It either needs to be one of the builtin formats or an existing '
            'template folder. The following builtin formats are available: {1}'.format(format, builtinTemplates))
        exit(1)

@click.command()
@click.option('--reload/--no-reload', default=False, help='Specifies whether the generator should keep track of the changes in the IDL file and update output on the fly (--no-reload by default).')
@click.option('--format', '-f', multiple=False, help='The format the autogenerator should use for the generation. This can either be one of the builtin formats or a path to a template folder. Builtin formats are: \n' + '\n'.join(builtinTemplates))
@click.option('--module', default=False, help='The name of the Qt module the autogenerator is generating. This is automatically used by the qmake integration and passed directly to the qface templates.')
@click.option('--force', is_flag=True, default=False, help='Always write all output files')
@click.option('--annotations', '-A', multiple=True, default=False, help=
    'Merges the given annotation file with annotions already in the qface file and the '
    'implicit annotation file. The annotation files will be merged in the order they are passed '
    'to the generator. Providing a duplicate key in the YAML file will override the previously '
    'set value. This option can be used multiple times.')
@click.option('--import', '-I', 'imports' , multiple=True, default=False, help=
    'Adds the given path to the list of import paths. All directories in this list are '
    'scanned recursively for QFace files. The QFace files found are then used to resolve '
    'the information required when importing a module; this is similar to how C++ include '
    'paths work.')
@click.argument('src', nargs=-1, type=click.Path(exists=True))
@click.argument('dst', nargs=1, type=click.Path(exists=True))

def app(src, dst, format, reload, module, force, annotations, imports):
    """
    The QtIvi Autogenerator (ivigenerator)

    It takes several files or directories as src and generates the code
    in the given dst directory.
    """

    # Parse the .config file and throw an error in case it doesn't exist or it is invalid
    builtin_config.parse(here)

    if reload:
        script = '{0} {1} {2}'.format(Path(__file__).abspath(), ' '.join(src), dst)
        monitor(src, script)
    else:
        moduleConfig = {
            "module": module,
            "force": force
        }
        run(format, moduleConfig, annotations, imports, src, dst)


if __name__ == '__main__':
    app()
