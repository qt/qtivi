{#
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
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
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}Backend'.format(interface) %}
{% set zone_class = '{0}Zone'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QRemoteObjectNode>
#include <QIviRemoteObjectReplicaHelper>
{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{class}}Interface>
{% else %}
#include "{{class|lower}}interface.h"
{% endif %}
#include "rep_{{interface|lower}}_replica.h"

{% for property in interface.properties %}
{%   if property.type.is_model %}
{% include "pagingmodel.h.tpl" %}
{%   endif %}
{% endfor %}

QT_BEGIN_NAMESPACE

{% if interface_zoned %}
class {{class}};

class {{zone_class}} : public QObject
{
    Q_OBJECT

public:
    explicit {{zone_class}}(const QString &zone, {{class}} *parent = nullptr);

    bool isSyncing();
    void sync();

public Q_SLOTS:
{% for property in interface.properties %}
    {{ivi.prop_setter(property, model_interface = true)}};
{% endfor %}
    void emitCurrentState();

Q_SIGNALS:
    void syncDone();

private:
    void checkSync();

    {{class}} *m_parent;
    QString m_zone;
{% for property in interface.properties %}
{%   if property.type.is_model %}
    QIviPagingModelInterface *m_{{ property }};
{%   else %}
    {{ property|return_type }} m_{{ property }};
{%   endif %}
{% endfor %}
    QStringList m_propertiesToSync;
};
{% endif %}

class {{class}} : public {{class}}Interface
{
    Q_OBJECT

public:
    explicit {{class}}(const QString &remoteObjectsLookupName = QStringLiteral("{{interface.qualified_name}}"),
                       QObject *parent = nullptr);
    ~{{class}}();

    void initialize() override;

public Q_SLOTS:
{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
{%     if not property.is_model %}
    virtual {{ivi.prop_setter(property, zoned=interface_zoned)}} override;
{%     endif %}
{%   endif %}
{% endfor %}

{% if interface_zoned %}
    virtual QStringList availableZones() const override;
{% endif %}

{% for operation in interface.operations %}
    virtual {{ivi.operation(operation, zoned=interface_zoned)}} override;
{% endfor %}

protected Q_SLOTS:
{% if interface_zoned %}
    void syncZones();
    void onZoneSyncDone();
{% endif %}

protected:
    bool connectToNode();
    void setupConnections();

    QSharedPointer<{{interface}}Replica> m_replica;
    QRemoteObjectNode* m_node= nullptr;
    QUrl m_url;
    QString m_remoteObjectsLookupName;
    QHash<quint64, QIviPendingReplyBase> m_pendingReplies;
    QIviRemoteObjectReplicaHelper *m_helper;
{% for property in interface.properties %}
{%   if property.type.is_model %}
    QIviPagingModelInterface *m_{{property}};
{%   endif %}
{% endfor %}
{% if interface_zoned %}
    bool m_synced;
    QHash<QString, {{zone_class}}*> m_zoneMap;
    QStringList m_zones;

    friend class {{zone_class}};
{% endif %}
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
