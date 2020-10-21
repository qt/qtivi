{#
# Copyright (C) 2020 The Qt Company Ltd.
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
{% import 'common/qtivi_macros.j2' as ivi %}
{% set module_qml_name = (module|qml_type).split('.')[-1]|upperfirst %}
{% set default_category_name = module_qml_name %}
{% if module.tags.designer and module.tags.designer.categoryName %}
{%   set default_category_name = module.tags.designer.categoryName %}
{% endif %}

MetaInfo {
{% for interface in module.interfaces %}
{%   set category_name = default_category_name %}
{%   if interface.tags.designer and interface.tags.designer.categoryName %}
{%     set category_name = interface.tags.designer.categoryName %}
{%   endif %}
{%   set name = interface|qml_type %}
{%   if interface.tags.designer and interface.tags.designer.name %}
{%     set name = interface.tags.designer.name %}
{%   endif %}

   Type {
        name: "{{module|qml_type}}.{{interface|qml_type}}"
{%   if interface.tags.designer and interface.tags.designer.typeIcon %}
        icon: "{{interface.tags.designer.typeIcon}}"
{%   endif %}

        Hints {
            visibleInNavigator: true
            canBeDroppedInNavigator: true
            canBeDroppedInFormEditor: false
        }

        ItemLibraryEntry {
            name: "{{name}}"
            category: "{{category_name}}"
{%   if interface.tags.designer and interface.tags.designer.libraryIcon %}
             libraryIcon: "{{interface.tags.designer.libraryIcon}}"
{%   endif %}
            version: "{{module.majorVersion}}.{{module.minorVersion}}"
            requiredImport: "{{module|qml_type}}"
        }
    }

{% endfor %}
}
