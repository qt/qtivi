{#
#############################################################################
##
## Copyright (C) 2021 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Copyright (C) 2018 Pelagicore AG
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
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}RoPlugin'.format(module.module_name) %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QVector>
#include <QtIviCore/QIviServiceInterface>

QT_BEGIN_NAMESPACE

class {{class}} : public QObject, QIviServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QIviServiceInterface_iid FILE "{{module.module_name|lower}}.json")
    Q_INTERFACES(QIviServiceInterface)

public:
    typedef QVector<QIviFeatureInterface *> (InterfaceBuilder)({{class}} *);

    explicit {{class}}(QObject *parent = nullptr);

    QStringList interfaces() const override;
    QIviFeatureInterface* interfaceInstance(const QString& interface) const override;

private:
    QVector<QIviFeatureInterface *> m_interfaces;
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
