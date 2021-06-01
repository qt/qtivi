/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qivipagingmodelqtroadapter.h"

QIviPagingModelQtRoAdapter::QIviPagingModelQtRoAdapter(const QString &remoteObjectsLookupName, QIviPagingModelInterface *parent)
    : QIviPagingModelSource(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_backend(parent)
{
    connect(m_backend, &QIviPagingModelInterface::supportedCapabilitiesChanged, this, &QIviPagingModelQtRoAdapter::supportedCapabilitiesChanged);
    connect(m_backend, &QIviPagingModelInterface::countChanged, this, &QIviPagingModelQtRoAdapter::countChanged);
    connect(m_backend, &QIviPagingModelInterface::dataFetched, this, &QIviPagingModelQtRoAdapter::dataFetched);
    connect(m_backend, &QIviPagingModelInterface::dataChanged, this, &QIviPagingModelQtRoAdapter::dataChanged);
}

QString QIviPagingModelQtRoAdapter::remoteObjectsLookupName() const
{
    return m_remoteObjectsLookupName;
}

void QIviPagingModelQtRoAdapter::registerInstance(const QUuid &identifier)
{
    m_backend->registerInstance(identifier);
}

void QIviPagingModelQtRoAdapter::unregisterInstance(const QUuid &identifier)
{
    m_backend->unregisterInstance(identifier);
}

void QIviPagingModelQtRoAdapter::fetchData(const QUuid &identifier, int start, int count)
{
    m_backend->fetchData(identifier, start, count);
}
