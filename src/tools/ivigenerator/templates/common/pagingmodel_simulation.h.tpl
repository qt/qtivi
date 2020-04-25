{#
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG.
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
#}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
{% if interface_zoned %}
{% set class = 'Zoned{0}ModelBackend'.format(property|upperfirst) %}
{% else %}
{% set class = '{0}ModelBackend'.format(property|upperfirst) %}
{% endif %}

#include <QIviPagingModelInterface>
#include "{{property.type.nested|lower}}.h"

class {{class}} : public QIviPagingModelInterface
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit {{class}}(QObject *parent = nullptr);
    ~{{class}}();

    int count() const;

    Q_INVOKABLE void initialize() override;
    Q_INVOKABLE void registerInstance(const QUuid &identifier) override;
    Q_INVOKABLE void unregisterInstance(const QUuid &identifier) override;

    Q_INVOKABLE void fetchData(const QUuid &identifier, int start, int count) override;

public Q_SLOTS:
    void insert(int index, const {{property.type.nested}} &item);
    void remove(int index);
    void move(int currentIndex, int newIndex);
    void reset();
    void update(int index, const {{property.type.nested}} &item);
    {{property.type.nested}} at(int index) const;

private:
    QList<{{property.type.nested}}> m_list;
};

