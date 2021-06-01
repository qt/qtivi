/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
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

#include "qivisearchandbrowsemodelqtroadapter.h"
#include "qiviqmlconversion_helper.h"

Q_LOGGING_CATEGORY(qLcROQIviSearchAndBrowseModel, "qt.ivi.qivisearchandbrowsemodel.remoteobjects", QtInfoMsg)

QIviSearchAndBrowseModelQtRoAdapter::QIviSearchAndBrowseModelQtRoAdapter(QIviSearchAndBrowseModelInterface *parent)
    : QIviSearchAndBrowseModelQtRoAdapter(QStringLiteral("QIviSearchAndBrowseModel"), parent)
{
}

QIviSearchAndBrowseModelQtRoAdapter::QIviSearchAndBrowseModelQtRoAdapter(const QString &remoteObjectsLookupName, QIviSearchAndBrowseModelInterface *parent)
    : QIviSearchAndBrowseModelSource(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_backend(parent)
    , m_helper(this, qLcROQIviSearchAndBrowseModel())
{
    connect(m_backend, &SearchAndBrowseBackend::dataFetched, this, &QIviSearchAndBrowseModelQtRoAdapter::dataFetched);
    connect(m_backend, &SearchAndBrowseBackend::dataChanged, this, &QIviSearchAndBrowseModelQtRoAdapter::dataChanged);
    connect(m_backend, &SearchAndBrowseBackend::countChanged, this, &QIviSearchAndBrowseModelQtRoAdapter::countChanged);
    connect(m_backend, &SearchAndBrowseBackend::canGoBackChanged, this, &QIviSearchAndBrowseModelQtRoAdapter::canGoBackChanged);
    connect(m_backend, &SearchAndBrowseBackend::canGoForwardChanged, this, &QIviSearchAndBrowseModelQtRoAdapter::canGoForwardChanged);
    connect(m_backend, &SearchAndBrowseBackend::supportedCapabilitiesChanged, this, &QIviSearchAndBrowseModelQtRoAdapter::supportedCapabilitiesChanged);
    connect(m_backend, &SearchAndBrowseBackend::availableContentTypesChanged, this, &QIviSearchAndBrowseModelQtRoAdapter::availableContentTypesChanged);
    connect(m_backend, &SearchAndBrowseBackend::contentTypeChanged, this, &QIviSearchAndBrowseModelQtRoAdapter::contentTypeChanged);
    connect(m_backend, &SearchAndBrowseBackend::queryIdentifiersChanged, this, &QIviSearchAndBrowseModelQtRoAdapter::queryIdentifiersChanged);
}

QString QIviSearchAndBrowseModelQtRoAdapter::remoteObjectsLookupName() const
{
    return m_remoteObjectsLookupName;
}

QStringList QIviSearchAndBrowseModelQtRoAdapter::availableContentTypes() const
{
    return m_backend->property("availableContentTypes").toStringList();
}

void QIviSearchAndBrowseModelQtRoAdapter::setContentType(const QUuid &identifier, const QString &contentType)
{
    m_backend->setContentType(identifier, contentType);
}

void QIviSearchAndBrowseModelQtRoAdapter::setupFilter(const QUuid &identifier, const QVariant &term, const QList<QIviOrderTerm> &orderTerms)
{
    QByteArray data = term.toByteArray();
    QDataStream stream(data);
    QIviAbstractQueryTerm *t = nullptr;
    if (!data.isEmpty())
        stream >> &t;
    m_backend->setupFilter(identifier, t, orderTerms);
}

QVariant QIviSearchAndBrowseModelQtRoAdapter::goBack(const QUuid &identifier)
{
    QIviPendingReplyBase pendingReply = m_backend->goBack(identifier);
    qCDebug(qLcROQIviSearchAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}

QVariant QIviSearchAndBrowseModelQtRoAdapter::goForward(const QUuid &identifier, int index)
{
    QIviPendingReplyBase pendingReply = m_backend->goForward(identifier, index);
    qCDebug(qLcROQIviSearchAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}

void QIviSearchAndBrowseModelQtRoAdapter::registerInstance(const QUuid &identifier)
{
    qCDebug(qLcROQIviSearchAndBrowseModel) << Q_FUNC_INFO;
    m_backend->registerInstance(identifier);
}

void QIviSearchAndBrowseModelQtRoAdapter::unregisterInstance(const QUuid &identifier)
{
    qCDebug(qLcROQIviSearchAndBrowseModel) << Q_FUNC_INFO;
    m_backend->unregisterInstance(identifier);
}

void QIviSearchAndBrowseModelQtRoAdapter::fetchData(const QUuid &identifier, int start, int count)
{
    qCDebug(qLcROQIviSearchAndBrowseModel) << Q_FUNC_INFO;
    m_backend->fetchData(identifier, start, count);
}

QVariant QIviSearchAndBrowseModelQtRoAdapter::insert(const QUuid &identifier, int index, const QVariant &item)
{
    QIviPendingReplyBase pendingReply = m_backend->insert(identifier, index, item);
    qCDebug(qLcROQIviSearchAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}

QVariant QIviSearchAndBrowseModelQtRoAdapter::remove(const QUuid &identifier, int index)
{
    QIviPendingReplyBase pendingReply = m_backend->remove(identifier, index);
    qCDebug(qLcROQIviSearchAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}

QVariant QIviSearchAndBrowseModelQtRoAdapter::move(const QUuid &identifier, int currentIndex, int newIndex)
{
    QIviPendingReplyBase pendingReply = m_backend->move(identifier, currentIndex, newIndex);
    qCDebug(qLcROQIviSearchAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}

QVariant QIviSearchAndBrowseModelQtRoAdapter::indexOf(const QUuid &identifier, const QVariant &item)
{
    QIviPendingReplyBase pendingReply = m_backend->indexOf(identifier, item);
    qCDebug(qLcROQIviSearchAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}
