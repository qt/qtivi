{#
#############################################################################
##
## Copyright (C) 2021 The Qt Company Ltd.
## Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
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
{% set class = '{0}'.format(interface) %}
{% if interface.tags.config.zoned %}
{%   set base_class = 'QIviAbstractZonedFeature' %}
{% else %}
{%   set base_class = 'QIviAbstractFeature' %}
{% endif %}
{% set oncedefine = '{0}_{1}PRIVATE_H_'.format(module.module_name|upper, class|upper) %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "{{module.module_name|lower}}.h"

{% if module.tags.config.disablePrivateIVI %}
#include <QObject>
{% else %}
#include <QtIviCore/private/{{base_class|lower}}_p.h>
{% endif %}
#include <QIviPagingModelInterface>

QT_BEGIN_NAMESPACE

class {{class}};

{% if module.tags.config.disablePrivateIVI %}
class {{class}}Private : public QObject
{% else %}
class {{class}}Private : public {{base_class}}Private
{% endif %}
{
public:
{% if module.tags.config.disablePrivateIVI %}
    {{class}}Private({{class}} *parent);
{% else %}
{%   if interface.tags.config.zoned %}
    {{class}}Private(const QString &interface, const QString &zone, {{class}} *parent);
{%   else %}
    {{class}}Private(const QString &interface, {{class}} *parent);
{%   endif %}
{% endif %}

    static {{class}}Private *get({{class}} *p);
    static const {{class}}Private *get(const {{class}} *p);
    {{class}} *getParent();

    void clearToDefaults();

{% for property in interface.properties %}
    {{ivi.on_prop_changed(property, zoned = interface.tags.config.zoned, model_interface = true)}};
{% endfor %}
{% for signal in interface.signals %}
    void on{{signal|upperfirst}}({{ivi.join_params(signal, zoned = interface.tags.config.zoned)}});
{% endfor %}

{% if not module.tags.config.disablePrivateIVI %}
    bool notify(const QByteArray &propertyName, const QVariant &value) override;

    {{class}} * const q_ptr;
{% endif %}
{% for property in interface.properties %}
    {{property|return_type}} m_{{property}};
{% endfor %}

{% if not module.tags.config.disablePrivateIVI %}
    Q_DECLARE_PUBLIC({{class}})
{% endif %}
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
