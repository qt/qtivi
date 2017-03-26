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

import click
import logging
import logging.config
import yaml
from path import Path

from qface.generator import FileSystem, Generator
from qface.helper.qtcpp import Filters
from qface.helper.doc import parse_doc
from qface.watch import monitor

here = Path(__file__).dirname()

log = logging.getLogger(__file__)

Filters.classPrefix = ''

def run_cpp(src, dst):
    log.debug('run {0} {1}'.format(src, dst))
    system = FileSystem.parse(src)
    generator = Generator(search_path=here / 'templates_frontend')
    generator.register_filter('returnType', Filters.returnType)
    generator.register_filter('parameterType', Filters.parameterType)
    generator.register_filter('defaultValue', Filters.defaultValue)
    generator.register_filter('parse_doc', parse_doc)
    ctx = {'dst': dst, 'qtASVersion': 1.2}
    for module in system.modules:
        log.debug('generate code for module %s', module)
        module.add_tag('config')
        ctx.update({'module': module})
        dst = generator.apply('{{dst}}/{{module|lower|replace(".", "-")}}', ctx)
        generator.destination = dst
        generator.write('{{module.module_name|lower}}global.h', 'global.h.tpl', ctx)
        generator.write('{{module.module_name|lower}}module.cpp', 'module.cpp.tpl', ctx)
        generator.write('{{module.module_name|lower}}module.h', 'module.h.tpl', ctx)
        generator.write('{{module|lower|replace(".", "-")}}.pro', 'module.pro.tpl', ctx)
        for interface in module.interfaces:
            log.debug('generate code for interface %s', interface)
            interface.add_tag('config')
            ctx.update({'interface': interface})
            generator.write('{{interface|lower}}.h', 'interface.h.tpl', ctx)
            generator.write('{{interface|lower}}_p.h', 'interface_p.h.tpl', ctx)
            generator.write('{{interface|lower}}.cpp', 'interface.cpp.tpl', ctx)
            generator.write('{{interface|lower}}backendinterface.h', 'backendinterface.h.tpl', ctx)
            generator.write('{{interface|lower}}backendinterface.cpp', 'backendinterface.cpp.tpl', ctx)

def wrong_format(src, dst):
    log.debug('run {0} {1}'.format(src, dst))
def run(formats, src, dst):
    for f in formats:
        switcher = {
            'cpp': run_cpp,
        }
        funct = switcher.get(f, wrong_format)
        funct(src, dst)

@click.command()
@click.option('--reload/--no-reload', default=False)
@click.option('--format', '-f', multiple=True, type=click.Choice(['cpp']))
@click.argument('src', nargs=-1, type=click.Path(exists=True))
@click.argument('dst', nargs=1, type=click.Path(exists=True))
def app(src, dst, format, reload):
    """Takes several files or directories as src and generates the code
    in the given dst directory."""
    if reload:
        script = '{0} {1} {2}'.format(Path(__file__).abspath(), ' '.join(src), dst)
        monitor(src, script)
    else:
        run(format, src, dst)


if __name__ == '__main__':
    app()
