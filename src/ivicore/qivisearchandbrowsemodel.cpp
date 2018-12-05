/****************************************************************************
**
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

#include "qivisearchandbrowsemodel.h"
#include "qivisearchandbrowsemodel_p.h"

#include "qiviqmlconversion_helper.h"
#include "qivisearchandbrowsemodelinterface.h"
#include "queryparser/qiviqueryparser_p.h"

#include <QDebug>
#include <QMetaObject>

QT_BEGIN_NAMESPACE

QIviSearchAndBrowseModelPrivate::QIviSearchAndBrowseModelPrivate(const QString &interface, QIviSearchAndBrowseModel *model)
    : QIviPagingModelPrivate(interface, model)
    , q_ptr(model)
    , m_queryTerm(nullptr)
    , m_canGoBack(false)
{
}

QIviSearchAndBrowseModelPrivate::~QIviSearchAndBrowseModelPrivate()
{
    delete m_queryTerm;
}

void QIviSearchAndBrowseModelPrivate::resetModel()
{
    QIviSearchAndBrowseModelInterface* backend = searchBackend();
    if (backend)
        setAvailableContenTypes(backend->availableContentTypes().toList());

    checkType();

    if (backend)
        backend->setContentType(m_identifier, m_contentType);

    parseQuery();

    QIviPagingModelPrivate::resetModel();
}

void QIviSearchAndBrowseModelPrivate::parseQuery()
{
    if (!searchBackend())
        return;

    if (m_query.isEmpty()) {
        //The new query is empty, tell it to the backend and delete the old term
        setupFilter(nullptr, {});
        return;
    }

    if (!m_capabilities.testFlag(QtIviCoreModule::SupportsFiltering) && !m_capabilities.testFlag(QtIviCoreModule::SupportsSorting)) {
        qtivi_qmlOrCppWarning(q_ptr, QStringLiteral("The backend doesn't support filtering or sorting. Changing the query will have no effect"));
        return;
    }

    QIviQueryParser parser;
    parser.setQuery(m_query);
    parser.setAllowedIdentifiers(searchBackend()->supportedIdentifiers(m_contentType));

    QIviAbstractQueryTerm* queryTerm = parser.parse();

    if (!queryTerm) {
        qtivi_qmlOrCppWarning(q_ptr, parser.lastError());
        return;
    }
    QList<QIviOrderTerm> orderTerms = parser.orderTerms();

    setupFilter(queryTerm, orderTerms);
}

void QIviSearchAndBrowseModelPrivate::setupFilter(QIviAbstractQueryTerm* queryTerm, const QList<QIviOrderTerm> &orderTerms)
{
    //1. Tell the backend about the new filter (or none)
    QIviSearchAndBrowseModelInterface* backend = searchBackend();
    if (backend)
        backend->setupFilter(m_identifier, queryTerm, orderTerms);

    //2. Now it's safe to delete the old filter
    delete m_queryTerm;
    m_queryTerm = nullptr;

    //3. Save the new filter
    m_queryTerm = queryTerm;
    m_orderTerms = orderTerms;
}

void QIviSearchAndBrowseModelPrivate::checkType()
{
    if (!searchBackend() || m_contentType.isEmpty())
        return;

    if (!m_availableContentTypes.contains(m_contentType)) {
        QString error = QString(QStringLiteral("Unsupported type: \"%1\" \n Supported types are: \n")).arg(m_contentType);
        for (const QString &type : qAsConst(m_availableContentTypes))
            error.append(type + QLatin1String("\n"));
        qtivi_qmlOrCppWarning(q_ptr, error);
    }
}

void QIviSearchAndBrowseModelPrivate::clearToDefaults()
{
    QIviPagingModelPrivate::clearToDefaults();

    delete m_queryTerm;
    m_queryTerm = nullptr;
    m_contentType = QString();
    m_canGoBack = false;
    m_availableContentTypes.clear();
}

void QIviSearchAndBrowseModelPrivate::setCanGoBack(bool canGoBack)
{
    Q_Q(QIviSearchAndBrowseModel);
    if (m_canGoBack == canGoBack)
        return;

    m_canGoBack = canGoBack;
    emit q->canGoBackChanged(m_canGoBack);
}

void QIviSearchAndBrowseModelPrivate::setAvailableContenTypes(const QStringList &contentTypes)
{
    Q_Q(QIviSearchAndBrowseModel);
    if (m_availableContentTypes == contentTypes)
        return;

    m_availableContentTypes = contentTypes;
    emit q->availableContentTypesChanged(contentTypes);
}

QIviSearchAndBrowseModelInterface *QIviSearchAndBrowseModelPrivate::searchBackend() const
{
    return qobject_cast<QIviSearchAndBrowseModelInterface*>(QIviPagingModelPrivate::backend());
}

void QIviSearchAndBrowseModelPrivate::updateContentType(const QString &contentType)
{
    Q_Q(QIviSearchAndBrowseModel);
    m_query = QString();
    emit q->queryChanged(m_query);
    m_contentType = contentType;
    emit q->contentTypeChanged(m_contentType);

    if (searchBackend())
        setCanGoBack(searchBackend()->canGoBack(m_identifier, m_contentType));

    resetModel();
}

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

    \section1 Setting it up
    The QIviSearchAndBrowseModel is using QtIviCore's \l {Dynamic Backend System} and is derived from QIviAbstractFeatureListModel.
    Other than most "QtIvi Feature classes", the QIviSearchAndBrowseModel doesn't automatically connect to available backends.

    The easiest approach to set it up, is to connect to the same backend used by another feature. E.g. for connecting to the
    media backend, use the instance from the mediaplayer feature:
    \code
        QIviMediaPlayer *player = new QIviMediaPlayer();
        player->startAutoDiscovery();
        QIviSearchAndBrowseModel *model = new QIviSearchAndBrowseModel();
        model->setServiceObject(player->serviceObject());
    \endcode

    \section2 Content Types

    Once the model is connected to a backend, the contentType needs to be selected. All possible content types can be queried
    from the availableContentTypes property. As the name already suggests, this property selects what type of content
    should be shown in the model. For the mediaplayer example, the available content types could be "track", "album" and "artist".

    \section1 Filtering and Sorting
    \target FilteringAndSorting

    One of the main use case of the QIviSearchAndBrowseModel is to provide a powerful way of filtering and sorting the content
    of the underlying data model. As explained above, the filtering and sorting is supposed to happen where the data is produced.
    To make this work across multiple backends the \l {Qt IVI Query Language} was invented.

    The \l {QIviSearchAndBrowseModel::}{query} property is used to sort the content of the model: e.g. by setting the string "[/name]", the content
    will be sorted by name in ascending order.

    For filtering, the same property is used but without the brackets e.g. "name='Example Item'" for only showing items which
    have the 'name' property set to 'Example Item'.

    Filtering and sorting can also be combined in one string and the filter part can also be more complex. More on that
    can be found in the detailed \l {Qt IVI Query Language} Documentation.

    \section1 Browsing
    \target Browsing

    In addition to filtering and sorting, the QIviSearchAndBrowseModel also supports browsing through a hierarchy of different
    content types. The easiest way to explain this is to look at the existing media example.

    When implementing a library view of all available media files, you might want to provide a way for the user to browse
    through the media database and select a song. You might also want to provide several staring points and from there
    limit the results. E.g.

    \list
        \li Artist -> Album -> Track
        \li Album -> Track
        \li Track
    \endlist

    This can be achieved by defining a complex filter query which takes the previously selected item into account.
    That is the most powerful way of doing it, as the developer/designer can define the browsing order and it can easily
    be changed. The downside of this is that the backend needs to support this way of filtering and sorting as well, which
    is not always be the case. A good example here is a DLNA backend, where the server already defines a fixed  browsing order.

    The QIviSearchAndBrowseModel provides the following methods for browsing:
    \list
        \li canGoForward()
        \li goForward()
        \li canGoBack()
        \li goBack()
    \endlist

    \section2 Navigation Types

    The QIviSearchAndBrowseModel supports two navigation types when browsing through the available data: for most use cases
    the simple InModelNavigation type is sufficient. By using this, the content type of the current model instance changes
    when navigating and the model is reset to show the new data.
    The other navigation type is OutOfModelNavigation and leaves the current model instance as it is. Instead the goForward()
    method returns a new model instance which contains the new data. This is especially useful when several views need to
    be open at the same time. E.g. when used inside a QML StackView.

    \code
        QIviSearchAndBrowseModel *artistModel = new QIviSearchAndBrowseModel();
        model->setContentType("artist");
        //Returns a new instance of QIviSearchAndBrowseModel which contains all albums from the artist at index '0'
        QIviSearchAndBrowseModel *albumModel = artistModel->goForward(0, QIviSearchAndBrowseModel::OutOfModelNavigation);
    \endcode

    \note Please also see the \l{QIviPagingModel}{QIviPagingModel documentation} for how the data loading works and
          the \l{Models} section for more information about all models in QtIvi.
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
    \enum QIviSearchAndBrowseModel::Roles
    \value CanGoForwardRole
          True if this item can be used to go one level forward and display the next set of items. See also goForward()
    \omitvalue LastRole

    \sa QIviPagingModel::Roles
*/

/*!
    \qmltype SearchAndBrowseModel
    \instantiates QIviSearchAndBrowseModel
    \inqmlmodule QtIvi
    \inherits PagingModel
    \brief The SearchAndBrowseModel is a generic model which can be used to search, browse, filter and sort data.

    The SearchAndBrowseModel should be used directly or as a base class whenever a lot of data needs to be
    presented in a ListView.

    The model is built upon the basic principle of filtering and sorting the data already where
    they are created instead of retrieving everything and sort or filter it locally. In addition the SearchAndBrowseModel
    only fetches the data it really needs and can it can be configured how this can be done.

    All rows in the model need to be subclassed from StandardItem.

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

    \section1 Setting it up
    The SearchAndBrowseModel is using QtIviCore's \l {Dynamic Backend System} and is derived from QIviAbstractFeatureListModel.
    Other than most "QtIvi Feature classes", the SearchAndBrowseModel doesn't automatically connect to available backends.

    The easiest approach to set it up, is to connect to the same backend used by another feature. E.g. for connecting to the
    media backend, use the instance from the mediaplayer feature:
    \qml
        Item {
            MediaPlayer {
                id: player
            }

            SearchAndBrowseModel {
                serviceObject: player.serviceObject
            }
        }
    \endqml

    \section2 Content Types

    Once the model is connected to a backend, the contentType needs to be selected. All possible content types can be queried
    from the availableContentTypes property. As the name already suggests, this property selects what type of content
    should be shown in the model. For the mediaplayer example, the available content types could be "track", "album" and "artist".

    \section1 Filtering and Sorting
    \target FilteringAndSorting

    One of the main use case of the SearchAndBrowseModel is to provide a powerful way of filtering and sorting the content
    of the underlying data model. As explained above, the filtering and sorting is supposed to happen where the data is produced.
    To make this work across multiple backends the \l {Qt IVI Query Language} was invented.

    The \l {SearchAndBrowseModel::}{query} property is used to sort the content of the model: e.g. by setting the string "[/name]", the content
    will be sorted by name in ascending order.

    For filtering, the same property is used but without the brackets e.g. "name='Example Item'" for only showing items which
    have the 'name' property set to 'Example Item'.

    Filtering and sorting can also be combined in one string and the filter part can also be more complex. More on that
    can be found in the detailed \l {Qt IVI Query Language} Documentation.

    \section1 Browsing
    \target Browsing

    In addition to filtering and sorting, the SearchAndBrowseModel also supports browsing through a hierarchy of different
    content types. The easiest way to explain this is to look at the existing media example.

    When implementing a library view of all available media files, you might want to provide a way for the user to browse
    through the media database and select a song. You might also want to provide several staring points and from there
    limit the results. E.g.

    \list
        \li Artist -> Album -> Track
        \li Album -> Track
        \li Track
    \endlist

    This can be achieved by defining a complex filter query which takes the previously selected item into account.
    That is the most powerful way of doing it, as the developer/designer can define the browsing order and it can easily
    be changed. The downside of this is that the backend needs to support this way of filtering and sorting as well, which
    is not always be the case. A good example here is a DLNA backend, where the server already defines a fixed  browsing order.

    The SearchAndBrowseModel provides the following methods/properties for browsing:
    \list
        \li canGoForward()
        \li goForward()
        \li canGoBack
        \li goBack()
    \endlist

    \section2 Navigation Types

    The SearchAndBrowseModel supports two navigation types when browsing through the available data: for most use cases
    the simple InModelNavigation type is sufficient. By using this, the content type of the current model instance changes
    when navigating and the model is reset to show the new data.
    The other navigation type is OutOfModelNavigation and leaves the current model instance as it is. Instead the goForward()
    method returns a new model instance which contains the new data. This is especially useful when several views need to
    be open at the same time. E.g. when used inside a QML StackView.

    \qml
        StackView {
            id: stack
            initialItem: view

            Component {
                id: view
                ListView {
                    model: SearchAndBrowseModel {
                        contentType: "artist"
                    }
                    delegate: MouseArea {
                        onClicked: {
                            stack.push({ "item" : view,
                                        "properties:" {
                                            "model" : model->goForward(index, SearchAndBrowseModel.OutOfModelNavigation)
                                        }});
                        }
                    }
                }
            }
        }
    \endqml

    \note Please also see the \l{PagingModel}{PagingModel documentation} for how the data loading works and
          the \l{Models} section for more information about all models in QtIvi.
*/

/*!
    Constructs a QIviSearchAndBrowseModel.

    The \a parent argument is passed on to the \l QIviAbstractFeatureListModel base class.
*/
QIviSearchAndBrowseModel::QIviSearchAndBrowseModel(QObject *parent)
    : QIviPagingModel(*new QIviSearchAndBrowseModelPrivate(QStringLiteral(QIviSearchAndBrowseModel_iid), this), parent)
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

    d->updateContentType(contentType);
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

    if (role == CanGoForwardRole)
        return canGoForward(row);
    else
        return QIviPagingModel::data(index, role);
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
        qtivi_qmlOrCppWarning(this, "No backend connected");
        return;
    }

    if (!backend->canGoBack(d->m_identifier, d->m_contentType)) {
        qtivi_qmlOrCppWarning(this, "Can't go backward anymore");
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
        qtivi_qmlOrCppWarning(this, "No backend connected");
        return false;
    }

    const QIviStandardItem *item = d->itemAt(i);
    if (!item)
        return false;

    return backend->canGoForward(d->m_identifier, d->m_contentType, item->id());
}

/*!
    \qmlmethod SearchAndBrowseModel SearchAndBrowseModel::goForward(i, navigationType)
    Uses the item at index \a i and shows the next set of items.

    \a navigationType can be one of the following values:
    \target InModelNavigation
    \value InModelNavigation
           The new content will be loaded into this model and the existing model data will be reset
    \target OutOfModelNavigation
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
    should be created and returned. If a instance is returned, this instance is owned
    by the caller.

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
        qtivi_qmlOrCppWarning(this, "No backend connected");
        return nullptr;
    }

    const QIviStandardItem *item = d->itemAt(i);
    if (!item)
        return nullptr;

    if (!backend->canGoForward(d->m_identifier, d->m_contentType, item->id())) {
        qtivi_qmlOrCppWarning(this, "Can't go forward anymore");
        return nullptr;
    }

    if (navigationType == OutOfModelNavigation) {
        if (d->m_capabilities.testFlag(QtIviCoreModule::SupportsStatelessNavigation)) {
            QString newContentType = backend->goForward(d->m_identifier, d->m_contentType, item->id());
            auto newModel = new QIviSearchAndBrowseModel(serviceObject(), newContentType);
            return newModel;
        } else {
            qtivi_qmlOrCppWarning(this, "The backend doesn't support the OutOfModelNavigation");
            return nullptr;
        }
    } else {
        QString newContentType = backend->goForward(d->m_identifier, d->m_contentType, item->id());
        d->updateContentType(newContentType);
    }

    return nullptr;
}

/*!
    \qmlmethod SearchAndBrowseModel::insert(int index, StandardItem item)

    Insert the \a item at the position \a index.

    If the backend doesn't accept the provided item, this operation will end in a no op.

    The returned PendingReply notifies about when the action has been done or whether it failed.
*/

/*!
    \fn void QIviSearchAndBrowseModel::insert(int index, const QVariant &variant)

    Insert the \a variant at the position \a index.

    If the backend doesn't accept the provided item, this operation will end in a no op.

    The returned QIviPendingReply notifies about when the action has been done or whether it failed.
*/
QIviPendingReply<void> QIviSearchAndBrowseModel::insert(int index, const QVariant &variant)
{
    Q_D(QIviSearchAndBrowseModel);
    const auto item = qtivi_gadgetFromVariant<QIviStandardItem>(this, variant);
    if (!item)
        return QIviPendingReply<void>::createFailedReply();

    QIviSearchAndBrowseModelInterface *backend = d->searchBackend();
    if (!backend) {
        qtivi_qmlOrCppWarning(this, "Can't insert itmes without a connected backend");
        return QIviPendingReply<void>::createFailedReply();
    }

    if (!d->m_capabilities.testFlag(QtIviCoreModule::SupportsInsert)) {
        qtivi_qmlOrCppWarning(this, "The backend doesn't support inserting items");
        return QIviPendingReply<void>::createFailedReply();
    }

    return backend->insert(d->m_identifier, d->m_contentType, index, item);
}

/*!
    \qmlmethod SearchAndBrowseModel::remove(int index)

    Removes the item at position \a index.

    The returned PendingReply notifies about when the action has been done or whether it failed.
*/

/*!
    \fn void QIviSearchAndBrowseModel::remove(int index)

    Removes the item at position \a index.

    The returned QIviPendingReply notifies about when the action has been done or whether it failed.
*/
QIviPendingReply<void> QIviSearchAndBrowseModel::remove(int index)
{
    Q_D(QIviSearchAndBrowseModel);
    QIviSearchAndBrowseModelInterface *backend = d->searchBackend();
    if (!backend) {
        qtivi_qmlOrCppWarning(this, "Can't remove items without a connected backend");
        return QIviPendingReply<void>::createFailedReply();
    }

    if (!d->m_capabilities.testFlag(QtIviCoreModule::SupportsRemove)) {
        qtivi_qmlOrCppWarning(this, "The backend doesn't support removing of items");
        return QIviPendingReply<void>::createFailedReply();
    }

    return backend->remove(d->m_identifier, d->m_contentType, index);
}

/*!
    \qmlmethod SearchAndBrowseModel::move(int cur_index, int new_index)

    Moves the item at position \a cur_index to the new position \a new_index.

    The returned PendingReply notifies about when the action has been done or whether it failed.
*/

/*!
    \fn void QIviSearchAndBrowseModel::move(int cur_index, int new_index)

    Moves the item at position \a cur_index to the new position \a new_index.

    The returned QIviPendingReply notifies about when the action has been done or whether it failed.
*/
QIviPendingReply<void> QIviSearchAndBrowseModel::move(int cur_index, int new_index)
{
    Q_D(QIviSearchAndBrowseModel);
    QIviSearchAndBrowseModelInterface *backend = d->searchBackend();
    if (!backend) {
        qtivi_qmlOrCppWarning(this, "Can't move items without a connected backend");
        return QIviPendingReply<void>::createFailedReply();
    }

    if (!d->m_capabilities.testFlag(QtIviCoreModule::SupportsMove)) {
        qtivi_qmlOrCppWarning(this, "The backend doesn't support moving of items");
        return QIviPendingReply<void>::createFailedReply();
    }

    return backend->move(d->m_identifier, d->m_contentType, cur_index, new_index);
}

/*!
    \qmlmethod SearchAndBrowseModel::indexOf(StandardItem item)

    Determines the index of \a item in this model.

    The result is returned as a PendingReply.
*/

/*!
    \fn void QIviSearchAndBrowseModel::indexOf(const QVariant &variant)

    Determines the index of \a variant in this model.

    The result is returned as a QIviPendingReply.
*/
QIviPendingReply<int> QIviSearchAndBrowseModel::indexOf(const QVariant &variant)
{
    Q_D(QIviSearchAndBrowseModel);
    const auto *item = qtivi_gadgetFromVariant<QIviStandardItem>(this, variant);
    if (!item)
        return QIviPendingReply<int>::createFailedReply();

    QIviSearchAndBrowseModelInterface *backend = d->searchBackend();
    if (!backend) {
        qtivi_qmlOrCppWarning(this, "Can't get the index without a connected backend");
        return QIviPendingReply<int>::createFailedReply();
    }

    return backend->indexOf(d->m_identifier, d->m_contentType, item);
}

/*!
    \reimp
*/
QHash<int, QByteArray> QIviSearchAndBrowseModel::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles =  QIviPagingModel::roleNames();
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
    setServiceObject(serviceObject);
    setContentType(contentType);
}

/*!
    \internal
*/
QIviSearchAndBrowseModel::QIviSearchAndBrowseModel(QIviSearchAndBrowseModelPrivate &dd, QObject *parent)
    : QIviPagingModel(dd, parent)
{
}

/*!
    \reimp
*/
void QIviSearchAndBrowseModel::connectToServiceObject(QIviServiceObject *serviceObject)
{
    Q_D(QIviSearchAndBrowseModel);

    QIviSearchAndBrowseModelInterface *backend = d->searchBackend();
    if (!backend)
        return;

    QIviPagingModel::connectToServiceObject(serviceObject);

    d->setCanGoBack(backend->canGoBack(d->m_identifier, d->m_contentType));

    d->resetModel();
}

/*!
    \reimp
*/
void QIviSearchAndBrowseModel::clearServiceObject()
{
    Q_D(QIviSearchAndBrowseModel);
    d->clearToDefaults();
}

QT_END_NAMESPACE

#include "moc_qivisearchandbrowsemodel.cpp"
