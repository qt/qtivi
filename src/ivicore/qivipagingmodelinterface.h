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

#ifndef QIVIPAGINGMODELINTERFACE_H
#define QIVIPAGINGMODELINTERFACE_H

#include <QUuid>
#include <QtIviCore/QIviFeatureInterface>
#include <QtIviCore/QIviPagingModel>
#include <QtIviCore/QtIviCoreModule>

QT_BEGIN_NAMESPACE

class QIviPagingModelInterfacePrivate;

class Q_QTIVICORE_EXPORT QIviPagingModelInterface : public QIviFeatureInterface
{
    Q_OBJECT

public:
    explicit QIviPagingModelInterface(QObject *parent = nullptr);

    virtual void registerInstance(const QUuid &identifier) = 0;
    virtual void unregisterInstance(const QUuid &identifier) = 0;

    virtual void fetchData(const QUuid &identifier, int start, int count) = 0;

protected:
    QIviPagingModelInterface(QObjectPrivate &dd, QObject *parent = nullptr);

Q_SIGNALS:
    void supportedCapabilitiesChanged(const QUuid &identifier, QtIviCoreModule::ModelCapabilities capabilities);
    void countChanged(const QUuid &identifier = QUuid(), int count = -1);
    void dataFetched(const QUuid &identifier, const QList<QVariant> &data, int start, bool moreAvailable);
    void dataChanged(const QUuid &identifier, const QList<QVariant> &data, int start, int count);
};

#define QIviPagingModel_iid "org.qt-project.qtivi.PagingModel/1.0"

QT_END_NAMESPACE

#endif // QIVIPAGINGMODELINTERFACE_H
