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

import inspect
import click

from jinja2 import TemplateAssertionError
from jinja2 import contextfunction

currentSrcFile = ""

def jinjaTrace():
    """
    Collects all jinja template files and the line numbers from the current calltrace
    """
    frame = inspect.currentframe()
    infos = []
    while frame:
        template = frame.f_globals.get('__jinja_template__')
        if template is not None:
            infos.append((inspect.getsourcefile(frame),
                          template.get_corresponding_lineno(frame.f_lineno)))
        frame = frame.f_back
    return infos

def jinja_error(msg):
    """
    Throws an error for the current jinja template and the templates this is included from
    This can be used inside a filter to indicate problems with the passed arguments or direclty
    inside an template
    """
    infos = jinjaTrace()
    if len(infos):
        message = msg
        if len(infos) > 1:
            for info in infos[1:]:
                message = message + "\n{0}:{1}: instantiated from here".format(info[0], info[1])
        message = message + "\n{0}: instantiated from here".format(currentSrcFile)
        raise TemplateAssertionError(message, infos[0][1], "", infos[0][0])
    raise TemplateAssertionError(msg, -1, "", "unknown")


def jinja_warning(msg):
    """
    Reports an warning in the current jinja template.
    This can be used inside a filter to indicate problems with the passed arguments or direclty
    inside an template
    """
    file, lineno = jinjaTrace()[0]
    if file:
        message = '{0}:{1}: warning: {2}'.format(file, lineno, msg)
    else:
        message = '<unknown-file>: warning: {0}'.format(msg)
    click.secho(message, fg='yellow', err=True)


def register_global_functions(generator):
    generator.env.globals["error"] = jinja_error
    generator.env.globals["warning"] = jinja_warning
