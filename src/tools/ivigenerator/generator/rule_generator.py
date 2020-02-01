#!/usr/bin/env python3
# Copyright (C) 2019 The Qt Company Ltd.
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

import click
import logging.config
import sys
import yaml
from path import Path

from qface.generator import RuleGenerator
from qface.idl.domain import Module, Interface, Property, Parameter, Field, Struct

log = logging.getLogger(__file__)


class CustomRuleGenerator(RuleGenerator):
    """Extended RuleGenerator to only generate the wanted modules instead of all"""

    def __init__(self, search_path: str, destination: Path, modules: [], context: dict = {},
                 features: set = set(), force=False):
        super().__init__(search_path, destination, context, features, force)
        self.module_names = modules

    def process_rules(self, path: Path, system):
        gen_config = yaml.load(open(path), Loader=yaml.SafeLoader)
        if 'generate_rules' in gen_config:
            self._process_legacy(gen_config, system)
        else:
            super().process_rules(path, system)

    def _process_rules(self, rules: dict, system):
        self._source = None  # reset the template source
        if not self._shall_proceed(rules):
            return
        self.context.update(rules.get('context', {}))
        self.path = rules.get('path', '')
        self.source = rules.get('source', None)
        self._process_rule(rules.get('system', None), {'system': system})
        for module in system.modules:
            # Only generate files for the modules detected from the first parse run
            if module.name not in self.module_names:
                continue

            log.debug('generate code for module %s', module)

            # Report early if we cannot find the imported modules
            for imp in module._importMap:
                if imp not in system._moduleMap:
                    sys.exit("Couldn't resolve import '{0}'".format(imp))

            self._process_rule(rules.get('module', None), {'module': module})
            for interface in module.interfaces:
                self._process_rule(rules.get('interface', None), {'interface': interface})
            for struct in module.structs:
                self._process_rule(rules.get('struct', None), {'struct': struct})
            for enum in module.enums:
                self._process_rule(rules.get('enum', None), {'enum': enum})

    def _process_legacy(self, gen_config, system):

        click.secho('Using the legacy Generation YAML Parser. Please consider porting to the new '
                    'Rule based Generation provided by QFace 2.0', fg='yellow')

        for module in system.modules:
            # Only generate files for the modules detected from the first parse run
            if module.name not in self.module_names:
                continue

            log.debug('generate code for module %s', module)

            # Report early if we cannot find the imported modules
            for imp in module._importMap:
                if imp not in system._moduleMap:
                    sys.exit("Couldn't resolve import '{0}'".format(imp))

            self.context.update({'module': module})
            dst = self.apply('{{dst}}', self.context)
            self.destination = dst
            module_rules = gen_config['generate_rules']['module_rules']
            force = True  # moduleConfig['force']
            if module_rules is None:
                module_rules = []

            for rule in module_rules:
                preserve = rule['preserve'] if 'preserve' in rule else False
                self.write(rule['dest_file'], rule['template_file'], self.context, preserve, force)
            for interface in module.interfaces:
                log.debug('generate backend code for interface %s', interface)
                self.context.update({'interface': interface})
                interface_rules = gen_config['generate_rules']['interface_rules']
                if interface_rules is None:
                    interface_rules = []
                for rule in interface_rules:
                    preserve = rule['preserve'] if 'preserve' in rule else False
                    self.write(rule['dest_file'], rule['template_file'], self.context, preserve,
                               force)
            if ('struct_rules' in gen_config['generate_rules']
                    and isinstance(gen_config['generate_rules']['struct_rules'], list)):
                for struct in module.structs:
                    log.debug('generate code for struct %s', struct)
                    self.context.update({'struct': struct})
                    for rule in gen_config['generate_rules']['struct_rules']:
                        preserve = rule['preserve'] if 'preserve' in rule else False
                        self.write(rule['dest_file'], rule['template_file'], ctx, preserve, force)
