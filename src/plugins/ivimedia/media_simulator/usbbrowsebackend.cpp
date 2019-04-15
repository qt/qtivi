/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
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
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "usbbrowsebackend.h"

#include <QDir>
#include <QtDebug>

static const QString fileLiteral = QStringLiteral("file");

UsbBrowseBackend::UsbBrowseBackend(const QString &path, QObject *parent)
    : QIviSearchAndBrowseModelInterface(parent)
    , m_rootFolder(path)
{
    qRegisterMetaType<SearchAndBrowseItem>();
}

void UsbBrowseBackend::initialize()
{
    emit availableContentTypesChanged(QStringList(fileLiteral));
    emit initializationDone();
}

void UsbBrowseBackend::registerInstance(const QUuid &identifier)
{
    m_state.insert(identifier, {});
}

void UsbBrowseBackend::unregisterInstance(const QUuid &identifier)
{
    m_state.remove(identifier);
}

void UsbBrowseBackend::setContentType(const QUuid &identifier, const QString &contentType)
{
    auto &state = m_state[identifier];
    state.contentType = contentType;

    emit queryIdentifiersChanged(identifier, identifiersFromItem<SearchAndBrowseItem>());
    emit canGoBackChanged(identifier, contentType != fileLiteral);
    emit contentTypeChanged(identifier, contentType);
}

void UsbBrowseBackend::setupFilter(const QUuid &identifier, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms)
{
    Q_UNUSED(identifier)
    Q_UNUSED(term)
    Q_UNUSED(orderTerms)
}

void UsbBrowseBackend::fetchData(const QUuid &identifier, int start, int count)
{
    emit supportedCapabilitiesChanged(identifier, QtIviCoreModule::ModelCapabilities(
                                          QtIviCoreModule::SupportsSorting |
                                          QtIviCoreModule::SupportsStatelessNavigation |
                                          QtIviCoreModule::SupportsGetSize
                                          ));

    auto &state = m_state[identifier];
    QVariantList list;
    QString folder = m_rootFolder;
    if (state.contentType != fileLiteral)
        folder += QDir::separator() + state.contentType;
    QDir dir(folder);
    QFileInfoList infoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    emit countChanged(identifier, infoList.count());

    for (int i = start; i < infoList.count() && i < count; i++) {
        SearchAndBrowseItem item;
        item.setType(fileLiteral);
        item.setName(infoList.at(i).fileName());
        list.append(QVariant::fromValue(item));
    }

    emit dataFetched(identifier, list, start, list.count() >= count);

    for (int i=0; i < list.count(); i++) {
        if (start + i >= state.items.count())
            state.items.append(list.at(i));
        else
            state.items.replace(start + i, list.at(i));
    }
    emit canGoForwardChanged(identifier, QVector<bool>(infoList.count(), true), 0);
}

QIviPendingReply<QString> UsbBrowseBackend::goBack(const QUuid &identifier)
{
    auto &state = m_state[identifier];
    QStringList types = state.contentType.split('/');

    if (types.count() < 2 && state.contentType != fileLiteral)
        return fileLiteral;

    types.removeLast();
    types.replace(types.count() - 1, types.at(types.count() - 1).split('?').at(0));

    return QIviPendingReply<QString>(types.join('/'));
}

QIviPendingReply<QString> UsbBrowseBackend::goForward(const QUuid &identifier, int index)
{
    auto &state = m_state[identifier];

    const QIviStandardItem *i = qtivi_gadgetFromVariant<QIviStandardItem>(this, state.items.value(index, QVariant()));
    if (!i)
        return QIviPendingReply<QString>::createFailedReply();

    if (state.contentType != fileLiteral)
        return QIviPendingReply<QString>(state.contentType + "/" + i->id());
    else
        return QIviPendingReply<QString>(i->id());
}

QIviPendingReply<void> UsbBrowseBackend::insert(const QUuid &identifier, int index, const QVariant &item)
{
    Q_UNUSED(identifier)
    Q_UNUSED(index)
    Q_UNUSED(item)

    return QIviPendingReply<void>::createFailedReply();
}

QIviPendingReply<void> UsbBrowseBackend::remove(const QUuid &identifier, int index)
{
    Q_UNUSED(identifier)
    Q_UNUSED(index)

    return QIviPendingReply<void>::createFailedReply();
}

QIviPendingReply<void> UsbBrowseBackend::move(const QUuid &identifier, int currentIndex, int newIndex)
{
    Q_UNUSED(identifier)
    Q_UNUSED(currentIndex)
    Q_UNUSED(newIndex)

    return QIviPendingReply<void>::createFailedReply();
}

QIviPendingReply<int> UsbBrowseBackend::indexOf(const QUuid &identifier, const QVariant &item)
{
    Q_UNUSED(identifier)
    Q_UNUSED(item)

    return QIviPendingReply<int>::createFailedReply();
}
