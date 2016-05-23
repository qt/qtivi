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

#include "qivisearchandbrowsemodel.h"
#include "qivisearchandbrowsemodel_p.h"

#include "qivisearchandbrowsemodelinterface.h"
#include "queryparser/qiviqueryparser_p.h"

#include <QMetaObject>
#include <QDebug>

QIviSearchAndBrowseModelPrivate::QIviSearchAndBrowseModelPrivate(const QString &interface, QIviSearchAndBrowseModel *model)
    : QIviAbstractFeatureListModelPrivate(interface, model)
    , q_ptr(model)
    , m_chunkSize(30)
    , m_queryTerm(nullptr)
    , m_moreAvailable(false)
    , m_identifier(QUuid::createUuid())
    , m_fetchMoreThreshold(10)
    , m_fetchedDataCount(0)
    , m_canGoBack(false)
    , m_loadingType(QIviSearchAndBrowseModel::FetchMore)
{
    qRegisterMetaType<QIviSearchAndBrowseListItem>();
}

QIviSearchAndBrowseModelPrivate::~QIviSearchAndBrowseModelPrivate()
{
    delete m_queryTerm;
}

void QIviSearchAndBrowseModelPrivate::init()
{
    Q_Q(QIviSearchAndBrowseModel);
    q->setDiscoveryMode(QIviAbstractFeature::NoAutoDiscovery);

    QObject::connect(q, &QAbstractListModel::rowsInserted,
                     q, &QIviSearchAndBrowseModel::countChanged);
    QObject::connect(q, &QAbstractListModel::rowsRemoved,
                     q, &QIviSearchAndBrowseModel::countChanged);
    QObject::connect(q, &QAbstractListModel::modelReset,
                     q, &QIviSearchAndBrowseModel::countChanged);
    QObjectPrivate::connect(q, &QIviSearchAndBrowseModel::fetchMoreThresholdReached,
                            this, &QIviSearchAndBrowseModelPrivate::onFetchMoreThresholdReached);
}

void QIviSearchAndBrowseModelPrivate::onDataFetched(const QUuid &identifer, const QList<QVariant> &items, int start, bool moreAvailable)
{
    if (!items.count() || identifer != m_identifier)
        return;

    Q_Q(QIviSearchAndBrowseModel);
    m_moreAvailable = moreAvailable;

    if (m_loadingType == QIviSearchAndBrowseModel::FetchMore) {
        q->beginInsertRows(QModelIndex(), m_itemList.count(), m_itemList.count() + items.count() -1);
        m_itemList += items;
        m_fetchedDataCount = m_itemList.count();
        q->endInsertRows();
    } else {
        if (m_itemList.count() < start + items.count()) {
            qWarning() << "countChanged signal needs to be emitted before the dataFetched signal";
            return;
        }

        m_fetchedDataCount = start + items.count();

        for (int i = 0; i < items.count(); i++)
            m_itemList.replace(start + i, items.at(i));
        q->dataChanged(q->index(start), q->index(start + items.count() -1));
    }
}

void QIviSearchAndBrowseModelPrivate::onCountChanged(const QUuid &identifier, int new_length)
{
    if (identifier != m_identifier || m_loadingType != QIviSearchAndBrowseModel::DataChanged || m_itemList.count() == new_length)
        return;

    Q_Q(QIviSearchAndBrowseModel);
    q->beginInsertRows(QModelIndex(), m_itemList.count(), m_itemList.count() + new_length -1);
    for (int i = 0; i < new_length; i++)
        m_itemList.append(QVariant());
    q->endInsertRows();
}

void QIviSearchAndBrowseModelPrivate::onDataChanged(const QUuid &identifier, const QList<QVariant> &data, int start, int count)
{
    if (identifier != m_identifier)
        return;
    Q_UNUSED(data)
    Q_UNUSED(start)
    Q_UNUSED(count)

    //TODO Handle add/update/remove here.
}

void QIviSearchAndBrowseModelPrivate::onFetchMoreThresholdReached()
{
    Q_Q(QIviSearchAndBrowseModel);
    q->fetchMore(QModelIndex());
}

void QIviSearchAndBrowseModelPrivate::resetModel()
{
    Q_Q(QIviSearchAndBrowseModel);
    q->beginResetModel();
    m_itemList.clear();
    q->endResetModel();
    m_fetchedDataCount = 0;

    checkType();
    parseQuery();

    m_moreAvailable = false;
    q->fetchMore(QModelIndex());
}

void QIviSearchAndBrowseModelPrivate::parseQuery()
{
    if (!searchBackend())
        return;

    delete m_queryTerm;
    m_queryTerm = nullptr;
    m_orderTerms.clear();

    if (m_query.isEmpty())
        return;

    if (!searchBackend()->supportedFlags().testFlag(QIviSearchAndBrowseModelInterface::SupportsFiltering) && !searchBackend()->supportedFlags().testFlag(QIviSearchAndBrowseModelInterface::SupportsSorting)) {
        qWarning("The backend doesn't support filtering or sorting. Changing the query will have no effect");
        return;
    }

    QIviQueryParser parser;
    parser.setQuery(m_query);
    parser.setAllowedIdentifiers(searchBackend()->supportedIdentifiers(m_contentType));

    m_queryTerm = parser.parse();

    if (!m_queryTerm) {
        qWarning("%s", qPrintable(parser.lastError()));
        return;
    }
    m_orderTerms = parser.orderTerms();
}

void QIviSearchAndBrowseModelPrivate::checkType()
{
    if (!searchBackend() || m_contentType.isEmpty())
        return;

    QSet<QString> types = searchBackend()->availableContentTypes();
    if (!types.contains(m_contentType)) {
        QString error = QString(QLatin1String("Unsupported type: \"%1\" \n Supported types are: \n")).arg(m_contentType);
        for (const QString &type : types)
            error.append(type + QLatin1String("\n"));
        qWarning("%s", qPrintable(error));
    }
}

void QIviSearchAndBrowseModelPrivate::clearToDefaults()
{
    m_chunkSize = 30;
    delete m_queryTerm;
    m_queryTerm = nullptr;
    m_moreAvailable = false;
    m_identifier = QUuid::createUuid();
    m_fetchMoreThreshold = 10;
    m_contentType = QString();
    m_fetchedDataCount = 0;
    m_canGoBack = false;
    m_loadingType = QIviSearchAndBrowseModel::FetchMore;
}

void QIviSearchAndBrowseModelPrivate::setCanGoBack(bool canGoBack)
{
    Q_Q(QIviSearchAndBrowseModel);
    if (m_canGoBack == canGoBack)
        return;

    m_canGoBack = canGoBack;
    emit q->canGoBackChanged(m_canGoBack);
}

const QIviSearchAndBrowseListItem *QIviSearchAndBrowseModelPrivate::itemAt(int i) const
{
    QVariant var = m_itemList.at(i);
    if (!var.isValid())
        return nullptr;
    const void *data = var.constData();

    QMetaType type(var.userType());
    if (!type.flags().testFlag(QMetaType::IsGadget)) {
        qCritical() << "QVariant at" << i << "needs to use the Q_GADGET macro";
        return nullptr;
    }

    const QMetaObject *mo = type.metaObject();
    while (mo) {
        if (mo->className() == QIviSearchAndBrowseListItem::staticMetaObject.className())
            return reinterpret_cast<const QIviSearchAndBrowseListItem*>(data);
        mo = mo->superClass();
    }

    qCritical() << "QVariant at" << i << "is not derived from QIviSearchAndBrowseListItem";

    return nullptr;
}

QIviSearchAndBrowseModelInterface *QIviSearchAndBrowseModelPrivate::searchBackend() const
{
    Q_Q(const QIviSearchAndBrowseModel);
    QIviServiceObject* so = q->serviceObject();
    if (so)
        return qobject_cast<QIviSearchAndBrowseModelInterface*>(so->interfaceInstance(QIviStringSearchAndBrowseModelInterfaceName));

    return nullptr;
}

void QIviSearchAndBrowseModelPrivate::updateContentType(const QString &contentType)
{
    Q_Q(QIviSearchAndBrowseModel);
    m_query = QString();
    emit q->queryChanged(m_query);
    m_contentType = contentType;
    emit q->contentTypeChanged(m_contentType);
    setCanGoBack(searchBackend()->canGoBack(m_identifier, m_contentType));

    resetModel();
}

QIviSearchAndBrowseModel::QIviSearchAndBrowseModel(QObject *parent)
    : QIviAbstractFeatureListModel(*new QIviSearchAndBrowseModelPrivate(QIviStringSearchAndBrowseModelInterfaceName, this), parent)
{
    Q_D(QIviSearchAndBrowseModel);
    d->init();
}

QIviSearchAndBrowseModel::~QIviSearchAndBrowseModel()
{
}

QString QIviSearchAndBrowseModel::query() const
{
    Q_D(const QIviSearchAndBrowseModel);
    return d->m_query;
}

void QIviSearchAndBrowseModel::setQuery(const QString &query)
{
    Q_D(QIviSearchAndBrowseModel);
    if (d->m_query == query)
        return;

    //TODO If we use the stateless navigation this needs to be prevented on the second+ model

    d->m_query = query;
    emit queryChanged(d->m_query);

    //The query is checked in resetModel
    d->resetModel();
}

int QIviSearchAndBrowseModel::chunkSize() const
{
    Q_D(const QIviSearchAndBrowseModel);
    return d->m_chunkSize;
}

void QIviSearchAndBrowseModel::setChunkSize(int chunkSize)
{
    Q_D(QIviSearchAndBrowseModel);
    if (d->m_chunkSize == chunkSize)
        return;

    d->m_chunkSize = chunkSize;
    emit chunkSizeChanged(chunkSize);
}

int QIviSearchAndBrowseModel::fetchMoreThreshold() const
{
    Q_D(const QIviSearchAndBrowseModel);
    return d->m_fetchMoreThreshold;
}

void QIviSearchAndBrowseModel::setFetchMoreThreshold(int fetchMoreThreshold)
{
    Q_D(QIviSearchAndBrowseModel);
    if (d->m_fetchMoreThreshold == fetchMoreThreshold)
        return;

    d->m_fetchMoreThreshold = fetchMoreThreshold;
    emit fetchMoreThresholdChanged(fetchMoreThreshold);
}

QString QIviSearchAndBrowseModel::contentType() const
{
    Q_D(const QIviSearchAndBrowseModel);
    return d->m_contentType;
}

void QIviSearchAndBrowseModel::setContentType(const QString &contentType)
{
    Q_D(QIviSearchAndBrowseModel);
    if (d->m_contentType == contentType)
        return;

    d->m_contentType = contentType;
    d->resetModel();

    emit contentTypeChanged(contentType);
}

QStringList QIviSearchAndBrowseModel::availableContentTypes() const
{
    Q_D(const QIviSearchAndBrowseModel);
    return d->m_availableContentTypes;
}

bool QIviSearchAndBrowseModel::canGoBack() const
{
    Q_D(const QIviSearchAndBrowseModel);
    return d->m_canGoBack;
}

QIviSearchAndBrowseModel::LoadingType QIviSearchAndBrowseModel::loadingType() const
{
    Q_D(const QIviSearchAndBrowseModel);
    return d->m_loadingType;
}

void QIviSearchAndBrowseModel::setLoadingType(QIviSearchAndBrowseModel::LoadingType loadingType)
{
    Q_D(QIviSearchAndBrowseModel);
    if (d->m_loadingType == loadingType)
        return;

    if (loadingType == QIviSearchAndBrowseModel::DataChanged && !d->searchBackend()->supportedFlags().testFlag(QIviSearchAndBrowseModelInterface::SupportsGetSize)) {
        qWarning("The backend doesn't support the DataChanged loading type. This call will have no effect");
        return;
    }

    d->m_loadingType = loadingType;
    emit loadingTypeChanged(loadingType);

    d->resetModel();
}

int QIviSearchAndBrowseModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIviSearchAndBrowseModel);
    if (parent.isValid())
        return 0;

    return d->m_itemList.count();
}

QVariant QIviSearchAndBrowseModel::data(const QModelIndex &index, int role) const
{
    Q_D(const QIviSearchAndBrowseModel);
    Q_UNUSED(role)
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= d->m_itemList.count() || row < 0)
        return QVariant();

    if (row >= d->m_fetchedDataCount - d->m_fetchMoreThreshold && canFetchMore(QModelIndex()))
        emit fetchMoreThresholdReached();

    const QIviSearchAndBrowseListItem *item = d->itemAt(row);
    if (!item)
        return QVariant();

    switch (role) {
        case NameRole: return item->name();
        case TypeRole: return item->type();
        case CanGoForwardRole: return canGoForward(row);
        case ItemRole: return d->m_itemList.at(row);
    }

    return QVariant();
}

QVariantMap QIviSearchAndBrowseModel::get(int i) const
{
    QVariantMap map;
    map[QLatin1String("name")] = data(index(i,0), NameRole);
    map[QLatin1String("type")] = data(index(i,0), TypeRole);
    map[QLatin1String("canGoForward")] = data(index(i,0), CanGoForwardRole);
    map[QLatin1String("item")] = data(index(i,0), ItemRole);

    return map;
}

void QIviSearchAndBrowseModel::goBack()
{
    Q_D(QIviSearchAndBrowseModel);
    QIviSearchAndBrowseModelInterface *backend = d->searchBackend();

    if (!backend) {
        qWarning("No backend connected");
        return;
    }

    if (!backend->canGoBack(d->m_identifier, d->m_contentType)) {
        qWarning("Can't go backward anymore");
        return;
    }

    QString newContentType = backend->goBack(d->m_identifier, d->m_contentType);
    if (!newContentType.isEmpty())
        d->updateContentType(newContentType);
}

bool QIviSearchAndBrowseModel::canGoForward(int i) const
{
    Q_D(const QIviSearchAndBrowseModel);
    QIviSearchAndBrowseModelInterface *backend = d->searchBackend();

    if (i >= d->m_itemList.count() || i < 0)
        return false;

    if (!backend) {
        qWarning("No backend connected");
        return false;
    }

    const QIviSearchAndBrowseListItem *item = d->itemAt(i);
    if (!item)
        return false;

    return backend->canGoForward(d->m_identifier, d->m_contentType, item->id());
}

QIviSearchAndBrowseModel *QIviSearchAndBrowseModel::goForward(int i, NavigationType navigationType)
{
    Q_D(QIviSearchAndBrowseModel);
    QIviSearchAndBrowseModelInterface *backend = d->searchBackend();

    if (i >= d->m_itemList.count() || i < 0)
        return nullptr;

    if (!backend) {
        qWarning("No backend connected");
        return nullptr;
    }

    const QIviSearchAndBrowseListItem *item = d->itemAt(i);
    if (!item)
        return nullptr;

    if (!backend->canGoForward(d->m_identifier, d->m_contentType, item->id())) {
        qWarning("Can't go forward anymore");
        return nullptr;
    }

    if (navigationType == OutOfModelNavigation) {
        if (backend->supportedFlags() & QIviSearchAndBrowseModelInterface::SupportsStatelessNavigation) {
            QString newContentType = backend->goForward(d->m_identifier, d->m_contentType, item->id());
            QIviSearchAndBrowseModel* newModel = new QIviSearchAndBrowseModel(serviceObject(), newContentType, this);
            return newModel;
        } else {
            qWarning("The backend doesn't support the OutOfModelNavigation");
            return nullptr;
        }
    } else {
        QString newContentType = backend->goForward(d->m_identifier, d->m_contentType, item->id());
        d->updateContentType(newContentType);
    }

    return nullptr;
}

bool QIviSearchAndBrowseModel::canFetchMore(const QModelIndex &parent) const
{
    Q_D(const QIviSearchAndBrowseModel);
    if (parent.isValid())
        return false;

    return d->m_moreAvailable;
}

void QIviSearchAndBrowseModel::fetchMore(const QModelIndex &parent)
{
    Q_D(QIviSearchAndBrowseModel);
    if (parent.isValid())
        return;

    if (!d->searchBackend() || d->m_contentType.isEmpty())
        return;

    d->m_moreAvailable = false;
    d->searchBackend()->fetchData(d->m_identifier, d->m_contentType, d->m_queryTerm, d->m_orderTerms, d->m_fetchedDataCount, d->m_chunkSize);
}

QHash<int, QByteArray> QIviSearchAndBrowseModel::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[ItemRole] = "item";
        roles[CanGoForwardRole] = "canGoForward";
    }
    return roles;
}

QIviSearchAndBrowseModel::QIviSearchAndBrowseModel(QIviServiceObject *serviceObject, const QString &contentType, QObject *parent)
    : QIviSearchAndBrowseModel(parent)
{
    Q_D(QIviSearchAndBrowseModel);
    d->init();
    setServiceObject(serviceObject);
    setContentType(contentType);
}

QIviSearchAndBrowseModel::QIviSearchAndBrowseModel(QIviSearchAndBrowseModelPrivate &dd, QObject *parent)
    : QIviAbstractFeatureListModel(dd, parent)
{
    Q_D(QIviSearchAndBrowseModel);
    d->init();
}

bool QIviSearchAndBrowseModel::acceptServiceObject(QIviServiceObject *serviceObject)
{
    if (serviceObject)
        return serviceObject->interfaces().contains(interfaceName());
    return false;
}

void QIviSearchAndBrowseModel::connectToServiceObject(QIviServiceObject *serviceObject)
{
    Q_UNUSED(serviceObject);

    Q_D(QIviSearchAndBrowseModel);

    QIviSearchAndBrowseModelInterface* backend = d->searchBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIviSearchAndBrowseModelInterface::dataFetched,
                            d, &QIviSearchAndBrowseModelPrivate::onDataFetched);
    QObjectPrivate::connect(backend, &QIviSearchAndBrowseModelInterface::countChanged,
                            d, &QIviSearchAndBrowseModelPrivate::onCountChanged);
    QObjectPrivate::connect(backend, &QIviSearchAndBrowseModelInterface::dataChanged,
                            d, &QIviSearchAndBrowseModelPrivate::onDataChanged);

    d->setCanGoBack(backend->canGoBack(d->m_identifier, d->m_contentType));

    d->resetModel();
}

void QIviSearchAndBrowseModel::disconnectFromServiceObject(QIviServiceObject *serviceObject)
{
    QIviSearchAndBrowseModelInterface *backend = qobject_cast<QIviSearchAndBrowseModelInterface*>(serviceObject->interfaceInstance(QIviStringSearchAndBrowseModelInterfaceName));

    if (backend)
        disconnect(backend, 0, this, 0);
}

void QIviSearchAndBrowseModel::clearServiceObject()
{
    Q_D(QIviSearchAndBrowseModel);
    d->clearToDefaults();
}

 #include "moc_qivisearchandbrowsemodel.cpp"
