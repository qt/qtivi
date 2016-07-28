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

#ifndef QIVISEARCHANDBROWSEMODEL_H
#define QIVISEARCHANDBROWSEMODEL_H

#include <QtIviCore/QIviAbstractFeatureListModel>
#include <QtIviCore/QIviServiceObject>
#include <QtQml/QJSValue>

QT_BEGIN_NAMESPACE

class QIviSearchAndBrowseModelPrivate;

class Q_QTIVICORE_EXPORT QIviSearchAndBrowseModel : public QIviAbstractFeatureListModel
{
    Q_OBJECT

    Q_PROPERTY(Capabilities capabilities READ capabilities NOTIFY capabilitiesChanged)
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QString contentType READ contentType WRITE setContentType NOTIFY contentTypeChanged)
    Q_PROPERTY(QStringList availableContentTypes READ availableContentTypes NOTIFY availableContentTypesChanged)
    Q_PROPERTY(int chunkSize READ chunkSize WRITE setChunkSize NOTIFY chunkSizeChanged)
    Q_PROPERTY(int fetchMoreThreshold READ fetchMoreThreshold WRITE setFetchMoreThreshold NOTIFY fetchMoreThresholdChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY canGoBackChanged)

    //TODO fix naming
    Q_PROPERTY(QIviSearchAndBrowseModel::LoadingType loadingType READ loadingType WRITE setLoadingType NOTIFY loadingTypeChanged)

public:

    enum Roles {
        NameRole = Qt::DisplayRole,
        TypeRole = Qt::UserRole,
        ItemRole = Qt::UserRole + 1,
        CanGoForwardRole = Qt::UserRole +2
    };

    //TODO fix naming
    enum NavigationType {
        InModelNavigation,
        OutOfModelNavigation
    };
    Q_ENUM(NavigationType)

    //TODO fix naming
    enum LoadingType {
        FetchMore,
        DataChanged
    };
    Q_ENUM(LoadingType)

    //TODO Do we need to split this further into backend dependent and contentType dependent caps ?
    enum Capability {
        NoExtras = 0x0,
        SupportsFiltering = 0x1,
        SupportsSorting = 0x2,
        SupportsAndConjunction = 0x4,
        SupportsOrConjunction = 0x8,
        SupportsStatelessNavigation = 0x10, // (the backend supports to have multiple models showing different contentTypes and filters at the same time)
        SupportsGetSize = 0x20, // (the backend knows the size of the model when the query is done and the user can select a different way for loading the model content)
        SupportsInsert = 0x40,
        SupportsMove = 0x80,
        SupportsRemove = 0x100
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)
    Q_FLAG(Capabilities)

    QIviSearchAndBrowseModel(QObject *parent = Q_NULLPTR);
    virtual ~QIviSearchAndBrowseModel();

    Capabilities capabilities() const;

    QString query() const;
    void setQuery(const QString &query);

    int chunkSize() const;
    void setChunkSize(int chunkSize);

    int fetchMoreThreshold() const;
    void setFetchMoreThreshold(int fetchMoreThreshold);

    QString contentType() const;
    void setContentType(const QString &contentType);

    QStringList availableContentTypes() const;

    bool canGoBack() const;

    QIviSearchAndBrowseModel::LoadingType loadingType() const;
    void setLoadingType(QIviSearchAndBrowseModel::LoadingType loadingType);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    bool canFetchMore(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    void fetchMore(const QModelIndex &parent) Q_DECL_OVERRIDE;

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    Q_INVOKABLE QVariant get(int index) const;
    Q_INVOKABLE void goBack();
    Q_INVOKABLE bool canGoForward(int index) const;
    Q_INVOKABLE QIviSearchAndBrowseModel *goForward(int index, NavigationType navigationType);
    Q_INVOKABLE void insert(int index, const QVariant &variant);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void move(int cur_index, int new_index);
    //TODO add a C++ version for this, similar to QTimer::singleShot()
    Q_INVOKABLE void indexOf(const QVariant &variant, const QJSValue &functor);

    template <typename T> T at(int i) const {
        return data(index(i,0), ItemRole).value<T>();
    }

Q_SIGNALS:
    void capabilitiesChanged(Capabilities capabilities);
    void queryChanged(const QString &query);
    void chunkSizeChanged(int chunkSize);
    void countChanged();
    void fetchMoreThresholdChanged(int fetchMoreThreshold);
    void fetchMoreThresholdReached() const;
    void contentTypeChanged(const QString &contentType);
    void availableContentTypesChanged(const QStringList &availableContentTypes);
    void canGoBackChanged(bool canGoBack);
    void loadingTypeChanged(QIviSearchAndBrowseModel::LoadingType loadingType);

protected:
    QIviSearchAndBrowseModel(QIviServiceObject *serviceObject, const QString &contentType, QObject *parent = Q_NULLPTR);
    QIviSearchAndBrowseModel(QIviSearchAndBrowseModelPrivate &dd, QObject *parent);
    virtual bool acceptServiceObject(QIviServiceObject *serviceObject) Q_DECL_OVERRIDE;
    virtual void connectToServiceObject(QIviServiceObject *serviceObject) Q_DECL_OVERRIDE;
    virtual void disconnectFromServiceObject(QIviServiceObject *serviceObject) Q_DECL_OVERRIDE;
    virtual void clearServiceObject() Q_DECL_OVERRIDE;

private:
    Q_DECLARE_PRIVATE(QIviSearchAndBrowseModel)
    Q_PRIVATE_SLOT(d_func(), void onCapabilitiesChanged(const QUuid &identifier, QIviSearchAndBrowseModel::Capabilities capabilities))
    Q_PRIVATE_SLOT(d_func(), void onDataFetched(const QUuid &identifer, const QList<QVariant> &items, int start, bool moreAvailable))
    Q_PRIVATE_SLOT(d_func(), void onCountChanged(const QUuid &identifier, int new_length))
    Q_PRIVATE_SLOT(d_func(), void onDataChanged(const QUuid &identifier, const QList<QVariant> &data, int start, int count))
    Q_PRIVATE_SLOT(d_func(), void onIndexOfCallResult(const QUuid &identifier, int callID, int index))
    Q_PRIVATE_SLOT(d_func(), void onFetchMoreThresholdReached())
};

QT_END_NAMESPACE

#endif // QIVISEARCHANDBROWSEMODEL_H
