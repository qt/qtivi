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

#ifndef QIVISEARCHMODEL_P_H
#define QIVISEARCHMODEL_P_H

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

#include <QtIviCore/private/qivipagingmodel_p.h>
#include <private/qtiviglobal_p.h>

#include "qiviqueryterm.h"
#include "qivisearchandbrowsemodel.h"
#include "qivisearchandbrowsemodelinterface.h"
#include "qivistandarditem.h"

#include <QBitArray>
#include <QUuid>

QT_BEGIN_NAMESPACE

class Q_QTIVICORE_EXPORT QIviSearchAndBrowseModelPrivate : public QIviPagingModelPrivate
{
public:
    QIviSearchAndBrowseModelPrivate(const QString &interface, QIviSearchAndBrowseModel *model);
    ~QIviSearchAndBrowseModelPrivate() override;

    void resetModel() override;
    void parseQuery();
    void setupFilter(QIviAbstractQueryTerm* queryTerm, const QList<QIviOrderTerm> &orderTerms);
    void clearToDefaults() override;
    void onCanGoForwardChanged(const QUuid &identifier, const QVector<bool> &indexes, int start);
    void onCanGoBackChanged(const QUuid &identifier, bool canGoBack);
    void onContentTypeChanged(const QUuid &identifier, const QString &contentType);
    void onAvailableContentTypesChanged(const QStringList &contentTypes);
    void onQueryIdentifiersChanged(const QUuid &identifier, const QSet<QString> &queryIdentifiers);

    QIviSearchAndBrowseModelInterface *searchBackend() const;
    void updateContentType(const QString &contentType);

    QIviSearchAndBrowseModel * const q_ptr;
    Q_DECLARE_PUBLIC(QIviSearchAndBrowseModel)

    QString m_query;

    QIviAbstractQueryTerm *m_queryTerm;
    QList<QIviOrderTerm> m_orderTerms;

    QString m_contentTypeRequested;
    QString m_contentType;
    QStringList m_availableContentTypes;
    QSet<QString> m_queryIdentifiers;
    QVector<bool> m_canGoForward;
    bool m_canGoBack;
};

QT_END_NAMESPACE

#endif // QIVISEARCHMODEL_P_H
