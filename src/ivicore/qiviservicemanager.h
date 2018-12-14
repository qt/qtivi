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

#ifndef QIVISERVICEMANAGER_H
#define QIVISERVICEMANAGER_H

#include <QtCore/QAbstractListModel>
#include <QtIviCore/qtiviglobal.h>

QT_BEGIN_NAMESPACE

class QIviServiceObject;
class QIviServiceManagerPrivate;

class Q_QTIVICORE_EXPORT QIviServiceManager : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::DisplayRole,
        ServiceObjectRole = Qt::UserRole,
        InterfacesRole = Qt::UserRole +1
    };

    enum SearchFlag {
        IncludeProductionBackends = 0x01,
        IncludeSimulationBackends = 0x02,
        IncludeAll = IncludeProductionBackends | IncludeSimulationBackends,
    };
    Q_DECLARE_FLAGS(SearchFlags, SearchFlag)
    Q_FLAG(SearchFlags)

    enum BackendType {
        ProductionBackend,
        SimulationBackend
    };
    Q_ENUM(BackendType)

    static QIviServiceManager *instance();

    Q_INVOKABLE QList<QIviServiceObject*> findServiceByInterface(const QString &interface, SearchFlags searchFlags = IncludeAll);
    Q_INVOKABLE bool hasInterface(const QString &interface) const;

    bool registerService(QObject *serviceBackendInterface, const QStringList &interfaces, BackendType backendType = ProductionBackend);
    void unloadAllBackends();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    explicit QIviServiceManager();
    QIviServiceManagerPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QIviServiceManager)
};

QT_END_NAMESPACE

#endif // QIVISERVICEMANAGER_H
