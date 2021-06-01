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
{% set class = "Core" %}

#ifndef {{class|upper}}_H
#define {{class|upper}}_H

#include <QRemoteObjectNode>

QT_BEGIN_NAMESPACE

class {{class}} : public QObject
{
    Q_OBJECT
private:
    {{class}}(QObject *parent=nullptr);

public:
    virtual ~{{class}}();
    void init();
    static {{class}}* instance();
    QRemoteObjectRegistryHost* host() const;

private:
    void reportError(QRemoteObjectNode::ErrorCode code);

    static {{class}}* s_instance;
    QRemoteObjectRegistryHost* m_host;
};

QT_END_NAMESPACE

#endif
