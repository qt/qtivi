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
{% set exportsymbol = '{0}'.format(module|upper|replace('.', '_')) %}
{% set oncedefine = '{0}QMLPLUGIN_H_'.format(exportsymbol) %}
{% set module_name = module.module_name|upperfirst %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QtQml/qqmlextensionplugin.h>
#include <qqml.h>

{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{module.module_name|lower}}factory.h>
{% else %}
#include "{{module.module_name|lower}}factory.h"
{% endif %}

QT_BEGIN_NAMESPACE

class {{module_name}}QmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("{{module|qml_type}}"));
        Q_UNUSED(uri);

        {{module_name}}Factory::registerQmlTypes();
    }
};

QT_END_NAMESPACE

#include "plugin.moc"

#endif // {{oncedefine}}

