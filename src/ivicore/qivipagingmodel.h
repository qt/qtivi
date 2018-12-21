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

#ifndef QIVIPAGINGMODEL_H
#define QIVIPAGINGMODEL_H

#include <QtIviCore/QIviAbstractFeatureListModel>
#include <QtIviCore/QtIviCoreModule>
#include <QtIviCore/QIviServiceObject>

QT_BEGIN_NAMESPACE

class QIviPagingModelPrivate;

class Q_QTIVICORE_EXPORT QIviPagingModel : public QIviAbstractFeatureListModel
{
    Q_OBJECT

    Q_PROPERTY(QtIviCoreModule::ModelCapabilities capabilities READ capabilities NOTIFY capabilitiesChanged)
    Q_PROPERTY(int chunkSize READ chunkSize WRITE setChunkSize NOTIFY chunkSizeChanged)
    Q_PROPERTY(int fetchMoreThreshold READ fetchMoreThreshold WRITE setFetchMoreThreshold NOTIFY fetchMoreThresholdChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

    //TODO fix naming
    Q_PROPERTY(QIviPagingModel::LoadingType loadingType READ loadingType WRITE setLoadingType NOTIFY loadingTypeChanged)

public:

    enum Roles {
        NameRole = Qt::DisplayRole,
        TypeRole = Qt::UserRole,
        ItemRole = Qt::UserRole + 1,
        LastRole = ItemRole
    };

    //TODO fix naming
    enum LoadingType {
        FetchMore,
        DataChanged
    };
    Q_ENUM(LoadingType)

    explicit QIviPagingModel(QObject *parent = nullptr);

    QtIviCoreModule::ModelCapabilities capabilities() const;

    int chunkSize() const;
    void setChunkSize(int chunkSize);

    int fetchMoreThreshold() const;
    void setFetchMoreThreshold(int fetchMoreThreshold);

    QIviPagingModel::LoadingType loadingType() const;
    void setLoadingType(QIviPagingModel::LoadingType loadingType);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QVariant get(int index) const;
    Q_INVOKABLE void reload();

    template <typename T> T at(int i) const {
        return data(index(i,0), ItemRole).value<T>();
    }

Q_SIGNALS:
    void capabilitiesChanged(QtIviCoreModule::ModelCapabilities capabilities);
    void chunkSizeChanged(int chunkSize);
    void countChanged();
    void fetchMoreThresholdChanged(int fetchMoreThreshold);
    void fetchMoreThresholdReached() const;
    void loadingTypeChanged(QIviPagingModel::LoadingType loadingType);

protected:
    QIviPagingModel(QIviServiceObject *serviceObject, QObject *parent = nullptr);
    QIviPagingModel(QIviPagingModelPrivate &dd, QObject *parent);
    void connectToServiceObject(QIviServiceObject *serviceObject) override;
    void disconnectFromServiceObject(QIviServiceObject *serviceObject) override;
    void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIviPagingModel)
    Q_PRIVATE_SLOT(d_func(), void onCapabilitiesChanged(const QUuid &identifier, QtIviCoreModule::ModelCapabilities capabilities))
    Q_PRIVATE_SLOT(d_func(), void onDataFetched(const QUuid &identifer, const QList<QVariant> &items, int start, bool moreAvailable))
    Q_PRIVATE_SLOT(d_func(), void onCountChanged(const QUuid &identifier, int new_length))
    Q_PRIVATE_SLOT(d_func(), void onDataChanged(const QUuid &identifier, const QList<QVariant> &data, int start, int count))
    Q_PRIVATE_SLOT(d_func(), void onFetchMoreThresholdReached())
};

QT_END_NAMESPACE

#endif // QIVIPAGINGMODEL_H
