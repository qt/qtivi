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
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
{% if interface_zoned %}
{% set class = 'Zoned{0}RoModelBackend'.format(property|upperfirst) %}
{% else %}
{% set class = '{0}RoModelBackend'.format(property|upperfirst) %}
{% endif %}

#include <QIviPagingModelInterface>
#include "{{property.type.nested|lower}}.h"

#include "rep_qivipagingmodel_replica.h"

class {{class}} : public QIviPagingModelInterface
{
    Q_OBJECT
public:
    explicit {{class}}(const QString &remoteObjectsLookupName = QStringLiteral("{{interface.qualified_name}}.{{property}}"), QObject *parent = nullptr);
    ~{{class}}();

    void initialize() override;
    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;

    void fetchData(const QUuid &identifier, int start, int count) override;

private:
    bool connectToNode();
    void setupConnections();

    QSharedPointer<QIviPagingModelReplica> m_replica;
    QIviRemoteObjectReplicaHelper *m_helper;
    QRemoteObjectNode *m_node= nullptr;
    QString m_remoteObjectsLookupName;
    QUrl m_url;
    QVariantList m_list;
};

