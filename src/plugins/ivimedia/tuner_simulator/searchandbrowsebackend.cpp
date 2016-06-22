/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#include "searchandbrowsebackend.h"
#include "amfmtunerbackend.h"

#include <QtDebug>

SearchAndBrowseBackend::SearchAndBrowseBackend(AmFmTunerBackend *tunerBackend, QObject *parent)
    : QIviSearchAndBrowseModelInterface(parent)
    , m_tunerBackend(tunerBackend)
{
    qRegisterMetaType<AmFmStation>();
    registerContentType<AmFmStation>("station");
}


QIviSearchAndBrowseModelInterface::Flags SearchAndBrowseBackend::supportedFlags() const
{
    return QIviSearchAndBrowseModelInterface::Flags(
           QIviSearchAndBrowseModelInterface::SupportsStatelessNavigation |
           QIviSearchAndBrowseModelInterface::SupportsGetSize);
}

void SearchAndBrowseBackend::fetchData(const QUuid &identifier, const QString &type, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms, int start, int count)
{
    Q_UNUSED(term)
    Q_UNUSED(orderTerms)

    if (type != "station")
        return;

    QVector<AmFmStation> stations = m_tunerBackend->m_bandHash[QIviAmFmTuner::AMBand].m_stations + m_tunerBackend->m_bandHash[QIviAmFmTuner::FMBand].m_stations;

    emit countChanged(identifier, stations.length());
    QVariantList requestedStations;

    int size = qMin(count, stations.length());
    for (int i = start; i < size; i++)
        requestedStations.append(QVariant::fromValue(stations.at(i)));

    emit dataFetched(identifier, requestedStations, start, start + count < stations.length());
}

bool SearchAndBrowseBackend::canGoBack(const QUuid &identifier, const QString &type)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    return false;
}

QString SearchAndBrowseBackend::goBack(const QUuid &identifier, const QString &type)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    return QString();
}

bool SearchAndBrowseBackend::canGoForward(const QUuid &identifier, const QString &type, const QString &itemId)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(itemId)
    return false;
}

QString SearchAndBrowseBackend::goForward(const QUuid &identifier, const QString &type, const QString &itemId)
{
    Q_UNUSED(identifier)
    Q_UNUSED(type)
    Q_UNUSED(itemId)
    return QString();
}
