/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#ifndef QTIVISERVICEMANAGER_H
#define QTIVISERVICEMANAGER_H

#include <QtCore/QAbstractListModel>
#include <QtIVICore/qtiviglobal.h>

QT_BEGIN_NAMESPACE

class QtIVIServiceObject;
class QtIVIServiceManagerPrivate;

class Q_QTIVICORE_EXPORT QtIVIServiceManager : public QAbstractListModel
{
    Q_OBJECT
public:
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

    static QtIVIServiceManager *instance();
    virtual ~QtIVIServiceManager();

    QList<QtIVIServiceObject*> findServiceByInterface(const QString &interface, SearchFlags searchFlags = IncludeAll);
    bool hasInterface(const QString &interface) const;

    bool registerService(QObject *serviceBackendInterface, const QStringList &interfaces, BackendType backendType = ProductionBackend);
    void unloadAllBackends();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
Q_SIGNALS:

private:
    explicit QtIVIServiceManager();
    QtIVIServiceManagerPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QtIVIServiceManager)
};

QT_END_NAMESPACE

#endif // QTIVISERVICEMANAGER_H
