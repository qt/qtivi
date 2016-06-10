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

/*!
    \class QIviSearchAndBrowseListItem
    \inmodule QtIviCore
    \brief The QIviSearchAndBrowseListItem is the base class of a row in the QIviSearchAndBrowseModel model.
*/

/*!
    \qmltype SearchAndBrowseListItem
    \qmlabstract
    \instantiates QIviSearchAndBrowseListItem
    \inqmlmodule QtIvi
    \brief The SearchAndBrowseListItem is the base class of a row in the SearchAndBrowseModel model.

    \note This item is not creatable from QML.
*/

/*!
    \qmlproperty string SearchAndBrowseListItem::id
    A unique identifier, which can be used to identify this item.

    This is mainly used by the backend to implement filtering or browsing.
*/

/*!
    \property QIviSearchAndBrowseListItem::id
    A unique identifier, which can be used to identify this item.

    This is mainly used by the backend to implement filtering or browsing.
*/

/*!
    \qmlproperty string SearchAndBrowseListItem::name
    The name of the item. E.g. The name of a contact in a addressbook, or the artist-name in a list of artists.
*/

/*!
    \property QIviSearchAndBrowseListItem::name
    The name of the item. E.g. The name of a contact in a addressbook, or the artist-name in a list of artists.
*/

/*!
    \qmlproperty string SearchAndBrowseListItem::type
    The type of the item. E.g. "artist", "track", "contact".
*/

/*!
    \property QIviSearchAndBrowseListItem::type
    The type of the item. E.g. "artist", "track", "contact".
*/

/*!
    \class QIviSearchAndBrowseModel
    \inmodule QtIviCore
    \brief The QIviSearchAndBrowseModel is a generic model which can be used to search, browse, filter and sort data.

    The QIviSearchAndBrowseModel should be used directly or as a base class whenever a lot of data needs to be
    presented in a ListView.

    The model is built upon the basic principle of filtering and sorting the data already where
    they are created instead of retrieving everything and sort or filter it locally. In addition the QIviSearchAndBrowseModel
    only fetches the data it really needs and can it can be configured how this can be done.

    The backend filling the model with data needs to implement the QIviSearchAndBrowseModelInterface class.

    \section1 Filtering and Sorting
    \target FilteringAndSorting

    \l {Qt IVI Query Language}

    \section1 Browsing
    \target Browsing

    \section1 Fetch Modes
*/

/*!
    \enum QIviSearchAndBrowseModel::NavigationType
    \value InModelNavigation
           The new content will be loaded into this model and the existing model data will be reset
    \value OutOfModelNavigation
           A new model will be returned which loads the new content. The model data of this model will
           not be changed and can still be used.
*/

/*!
    \enum QIviSearchAndBrowseModel::LoadingType
    \value FetchMore
           This is the default and can be used if you don't know the final size of the list (e.g. a infinite list).
           The list will detect that it is near the end (fetchMoreThreshold) and then fetch the next chunk of data using canFetchMore and fetchMore.
           The drawback of this method is that, because the final size of the data is not known, you can't display a dynamic scroll-bar indicator which is resized depending on the content of the list.
           The other problem could be fast scrolling, as the data might not come in-time and scrolling stops. This can be tweaked by the fetchMoreThreshold property.

    \value DataChanged
           For this loading type you need to know how many items are in the list, as dummy items are created and the user can already start scrolling even though the data is not yet ready to be displayed.
           Similar to FetchMore the data is also loaded in chunks. You can safely use a scroll indicator here.
           The delegate needs to support this approach, as it doesn't have a content when it's first created.
*/

/*!
   \enum QIviSearchAndBrowseModel::Roles
   \value NameRole
          The name of the item. E.g. The name of a contact in a addressbook, or the artist-name in a list of artists.
   \value TypeRole
          The type of the item. E.g. "artist", "track", "contact".
   \value ItemRole
          The item itself. This provides access to the properties which are type specific. E.g. the address of a contact.
   \value CanGoForwardRole
          True if this item can be used to go one level forward and display the next set of items. \sa goForward()
*/

/*!
    \qmltype SearchAndBrowseModel
    \instantiates QIviSearchAndBrowseModel
    \inqmlmodule QtIvi
    \inherits AbstractFeatureListModel
    \brief The SearchAndBrowseModel is a generic model which can be used to search, browse, filter and sort data.

    The SearchAndBrowseModel should be used directly or as a base class whenever a lot of data needs to be
    presented in a ListView.

    The model is built upon the basic principle of filtering and sorting the data already where
    they are created instead of retrieving everything and sort or filter it locally. In addition the SearchAndBrowseModel
    only fetches the data it really needs and can it can be configured how this can be done.

    All rows in the model need to be subclassed from SearchAndBrowseListItem.

    The following roles are available in this model:

    \table
    \header
        \li Role name
        \li Type
        \li Description
    \row
        \li \c name
        \li string
        \li The name of the item. E.g. The name of a contact in a addressbook, or the artist-name in a list of artists.
    \row
        \li \c type
        \li string
        \li The type of the item. E.g. "artist", "track", "contact".
    \row
        \li \c item
        \li object
        \li The item itself. This provides access to the properties which are type specific. E.g. the address of a contact.
    \row
        \li \c canGoForward
        \li bool
        \li True if this item can be used to go one level forward and display the next set of items. \sa goForward()
    \endtable
*/

/*!
   Constructs a QIviSearchAndBrowseModel.

   The \a parent argument is passed on to the \l QIviAbstractFeatureListModel base class.
 */
QIviSearchAndBrowseModel::QIviSearchAndBrowseModel(QObject *parent)
    : QIviAbstractFeatureListModel(*new QIviSearchAndBrowseModelPrivate(QIviStringSearchAndBrowseModelInterfaceName, this), parent)
{
    Q_D(QIviSearchAndBrowseModel);
    d->init();
}

QIviSearchAndBrowseModel::~QIviSearchAndBrowseModel()
{
}

/*!
    \qmlproperty string SearchAndBrowseModel::query
    \brief Holds the current query used for filtering and sorting the current content of the model.

    \note When changing this property the content will be reset.

    See \l {Qt IVI Query Language} for more information.
    \sa FilteringAndSorting
 */

/*!
    \property QIviSearchAndBrowseModel::query
    \brief Holds the current query used for filtering and sorting the current content of the model.

    \note When changing this property the content will be reset.

    See \l {Qt IVI Query Language} for more information.
    \sa FilteringAndSorting
 */
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

/*!
    \qmlproperty int SearchAndBrowseModel::chunkSize
    \brief Holds the number of rows which are requested from the backend interface.

    This property can be used to fine tune the loading performance.

    Bigger chunks means less calls to the backend and to a potential IPC underneath, but more data
    to be transferred and probably longer waiting time until the request was finished.
 */

/*!
    \property QIviSearchAndBrowseModel::chunkSize
    \brief Holds the number of rows which are requested from the backend interface.

    This property can be used to fine tune the loading performance.

    Bigger chunks means less calls to the backend and to a potential IPC underneath, but more data
    to be transferred and probably longer waiting time until the request was finished.
 */
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

/*!
    \qmlproperty int SearchAndBrowseModel::fetchMoreThreshold
    \brief Holds the row delta to the end before the next chunk is loaded

    This property can be used to fine tune the loading performance. When the
    threshold is reached the next chunk of rows are requested from the backend. How many rows are fetched
    can be defined by using the chunkSize property.

    The threshold defines the number of rows before the cached rows ends.

    \note This property is only used when loadingType is set to FetchMore.
 */

/*!
    \property QIviSearchAndBrowseModel::fetchMoreThreshold
    \brief Holds the row delta to the end before the next chunk is loaded

    This property can be used to fine tune the loading performance. When the
    threshold is reached the next chunk of rows are requested from the backend. How many rows are fetched
    can be defined by using the chunkSize property.

    The threshold defines the number of rows before the cached rows ends.

    \note This property is only used when loadingType is set to FetchMore.
 */
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

/*!
    \qmlproperty string SearchAndBrowseModel::contentType
    \brief Holds the current type of content displayed in this model.

    \note When changing this property the content will be reset.

    \sa SearchAndBrowseModel::availableContentTypes
 */

/*!
    \property QIviSearchAndBrowseModel::contentType
    \brief Holds the current type of content displayed in this model.

    \note When changing this property the content will be reset.

    \sa availableContentTypes
 */
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

/*!
    \qmlproperty list<string> SearchAndBrowseModel::availableContentTypes
    \brief Holds all the available content types

    \sa contentType
 */

/*!
    \property QIviSearchAndBrowseModel::availableContentTypes
    \brief Holds all the available content types

    \sa contentType
 */
QStringList QIviSearchAndBrowseModel::availableContentTypes() const
{
    Q_D(const QIviSearchAndBrowseModel);
    return d->m_availableContentTypes;
}

/*!
    \qmlproperty bool SearchAndBrowseModel::canGoBack
    \brief Holds whether the goBack() function can be used to return to the previous content.

    See \l Browsing for more information.
 */

/*!
    \property QIviSearchAndBrowseModel::canGoBack
    \brief Holds whether the goBack() function can be used to return to the previous content.

    See \l Browsing for more information.
 */
bool QIviSearchAndBrowseModel::canGoBack() const
{
    Q_D(const QIviSearchAndBrowseModel);
    return d->m_canGoBack;
}

/*!
    \qmlproperty enumeration SearchAndBrowseModel::loadingType
    \brief Holds the currently used loading type used for loading the data.

    \note When changing this property the content will be reset.
 */

/*!
    \property QIviSearchAndBrowseModel::loadingType
    \brief Holds the currently used loading type used for loading the data.

    \note When changing this property the content will be reset.
 */
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

/*!
    \qmlproperty int SearchAndBrowseModel::count
    \brief Holds the current number of rows in this model.
 */
/*!
    \property QIviSearchAndBrowseModel::count
    \brief Holds the current number of rows in this model.
 */
int QIviSearchAndBrowseModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIviSearchAndBrowseModel);
    if (parent.isValid())
        return 0;

    return d->m_itemList.count();
}

/*!
    \reimp
 */
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

/*!
    \fn T QIviSearchAndBrowseModel::at(int i) const

    Returns the item at index \a i converted to the template type T.
*/
/*!
    \qmlmethod object SearchAndBrowseModel::get(i)

    Returns the item at index \a i.
 */
/*!
    Returns the item at index \a i represented as QVariantMap.
*/
QVariantMap QIviSearchAndBrowseModel::get(int i) const
{
    QVariantMap map;
    map[QLatin1String("name")] = data(index(i,0), NameRole);
    map[QLatin1String("type")] = data(index(i,0), TypeRole);
    map[QLatin1String("canGoForward")] = data(index(i,0), CanGoForwardRole);
    map[QLatin1String("item")] = data(index(i,0), ItemRole);

    return map;
}

/*!
    \qmlmethod void SearchAndBrowseModel::goBack()
    Goes one level back in the navigation history.

    See also \l Browsing for more information.
 */
/*!
    Goes one level back in the navigation history.

    See also \l Browsing for more information.
 */
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

/*!
    \qmlmethod bool SearchAndBrowseModel::canGoForward(i)
    Returns true when the item at index \a i can be used to show the next set of elements.

    See also \l Browsing for more information.
 */
/*!
    Returns true when the item at index \a i can be used to show the next set of elements.

    See also \l Browsing for more information.
 */
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

/*!
    \qmlmethod SearchAndBrowseModel SearchAndBrowseModel::goForward(i, navigationType)
    Uses the item at index \a i and shows the next set of items.

    \a navigationType can be one of the following values:
    \value InModelNavigation
           The new content will be loaded into this model and the existing model data will be reset
    \value OutOfModelNavigation
           A new model will be returned which loads the new content. The model data of this model will
           not be changed and can still be used.

    \note Whether the OutOfModelNavigation navigation type is supported is decided by the backend.

    See also \l Browsing for more information.
 */
/*!
    Returns true when the item at index \a i can be used to show the next set of elements.

    Uses the item at index \a i and shows the next set of items. The \a navigationType can be used
    to control whether the new data should be shown in this model instance or whether a new instance
    should be created and returned.

    \note Whether the OutOfModelNavigation navigation type is supported is decided by the backend.

    See also \l Browsing for more information.
 */
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

/*!
    \reimp
*/
bool QIviSearchAndBrowseModel::canFetchMore(const QModelIndex &parent) const
{
    Q_D(const QIviSearchAndBrowseModel);
    if (parent.isValid())
        return false;

    return d->m_moreAvailable;
}

/*!
    \reimp
*/
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

/*!
    \reimp
*/
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

/*!
    \internal
*/
QIviSearchAndBrowseModel::QIviSearchAndBrowseModel(QIviServiceObject *serviceObject, const QString &contentType, QObject *parent)
    : QIviSearchAndBrowseModel(parent)
{
    Q_D(QIviSearchAndBrowseModel);
    d->init();
    setServiceObject(serviceObject);
    setContentType(contentType);
}

/*!
    \internal
*/
QIviSearchAndBrowseModel::QIviSearchAndBrowseModel(QIviSearchAndBrowseModelPrivate &dd, QObject *parent)
    : QIviAbstractFeatureListModel(dd, parent)
{
    Q_D(QIviSearchAndBrowseModel);
    d->init();
}

/*!
    \reimp
*/
bool QIviSearchAndBrowseModel::acceptServiceObject(QIviServiceObject *serviceObject)
{
    if (serviceObject)
        return serviceObject->interfaces().contains(interfaceName());
    return false;
}

/*!
    \reimp
*/
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

/*!
    \reimp
*/
void QIviSearchAndBrowseModel::disconnectFromServiceObject(QIviServiceObject *serviceObject)
{
    QIviSearchAndBrowseModelInterface *backend = qobject_cast<QIviSearchAndBrowseModelInterface*>(serviceObject->interfaceInstance(QIviStringSearchAndBrowseModelInterfaceName));

    if (backend)
        disconnect(backend, 0, this, 0);
}

/*!
    \reimp
*/
void QIviSearchAndBrowseModel::clearServiceObject()
{
    Q_D(QIviSearchAndBrowseModel);
    d->clearToDefaults();
}

/*!
    \fn void QIviSearchAndBrowseModel::fetchMoreThresholdReached() const

    This signal is emitted whenever the fetchMoreThreshold is reached and new data is requested from the backend.
*/

/*!
    \qmlsignal SearchAndBrowseModel::fetchMoreThresholdReached()

    This signal is emitted whenever the fetchMoreThreshold is reached and new data is requested from the backend.
*/

#include "moc_qivisearchandbrowsemodel.cpp"
