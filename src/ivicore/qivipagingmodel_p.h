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

#ifndef QIVIPAGINGMODEL_P_H
#define QIVIPAGINGMODEL_P_H

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

#include <QtIviCore/private/qiviabstractfeaturelistmodel_p.h>
#include <private/qtiviglobal_p.h>

#include "qivipagingmodel.h"
#include "qivipagingmodelinterface.h"
#include "qivistandarditem.h"

#include <QBitArray>
#include <QUuid>

QT_BEGIN_NAMESPACE

class Q_QTIVICORE_EXPORT QIviPagingModelPrivate : public QIviAbstractFeatureListModelPrivate
{
public:
    QIviPagingModelPrivate(const QString &interface, QIviPagingModel *model);
    ~QIviPagingModelPrivate() override;

    void initialize() override;
    void onInitializationDone();
    void onCapabilitiesChanged(const QUuid &identifier, QtIviCoreModule::ModelCapabilities capabilities);
    void onDataFetched(const QUuid &identifier, const QList<QVariant> &items, int start, bool moreAvailable);
    void onCountChanged(const QUuid &identifier, int new_length);
    void onDataChanged(const QUuid &identifier, const QList<QVariant> &data, int start, int count);
    void onFetchMoreThresholdReached();
    virtual void resetModel();
    virtual void clearToDefaults();
    const QIviStandardItem *itemAt(int i) const;
    void fetchData(int startIndex);

    QIviPagingModelInterface *backend() const;

    QIviPagingModel * const q_ptr;
    Q_DECLARE_PUBLIC(QIviPagingModel)

    QtIviCoreModule::ModelCapabilities m_capabilities;
    int m_chunkSize;

    QList<QVariant> m_itemList;
    QBitArray m_availableChunks;
    bool m_moreAvailable;

    QUuid m_identifier;
    int m_fetchMoreThreshold;
    int m_fetchedDataCount;
    QIviPagingModel::LoadingType m_loadingType;
};

QT_END_NAMESPACE

#endif // QIVIPAGINGMODEL_P_H
