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

#include "qivisearchandbrowsemodelinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIviSearchAndBrowseModelInterface
    \inmodule QtIviCore
    \ingroup backends
    \inherits QObject
    \keyword org.qt-project.qtivi.SearchAndBrowseModel/1.0
    \brief The QIviSearchAndBrowseModelInterface defines the interface for backends to the
    QIviSearchAndBrowseModel feature class.

    The QIviSearchAndBrowseModelInterface is the interface used by \l QIviSearchAndBrowseModel

    The interface is discovered by a \l QIviSearchAndBrowseModel object, which connects to it and sets it up.

    Every QIviSearchAndBrowseModel generates its own QUuid which is passed to the backend interface and can
    be used to identify a model instance.

    \sa QIviSearchAndBrowseModel

    //TODO explain how the interface works on a example
    <example of a fully featured backend>
*/

QIviSearchAndBrowseModelInterface::QIviSearchAndBrowseModelInterface(QObject *parent)
    : QIviPagingModelInterface(parent)
{}

/*!
    \fn template <class T> QIviSearchAndBrowseModelInterface::identifiersFromItem()

    Returns all properties of type T.

    These can registered as identifiers for the \l {Qt IVI Query Language} using the
    queryIdentifiersChanged() signal.
*/
/*!
    \internal
*/
QSet<QString> QIviSearchAndBrowseModelInterface::identifiersFromItem(const QMetaObject &object)
{
    QSet<QString> identifiers;
    for (int i=0; i < object.propertyCount(); i++) {
        QLatin1String propName(object.property(i).name());
        if (propName != QLatin1String("objectName"))
            identifiers.insert(propName);
    }

    return identifiers;
}

/*!
    \fn void QIviSearchAndBrowseModelInterface::setContentType(const QUuid &identifier, const QString &contentType)

    Sets the \a contentType of the QIviSearchAndBrowseModel instance identified by \a identifier.
    The given contenType can contain additional path information. The encoding is defined by the
    goForward() method. In case the \a contentType is not valid the error() signal should be used.

    \note The QIviSearchAndBrowseModel doesn't check the validity of the contentType, this is the backend's
    responsibility.

    If the QIviSearchAndBrowseModel supports filtering (see QIviPagingModel::capabilitiesChanged),
    the backend needs to emit the queryIdentifiersChanged signal once the contentType is set.

    Finally, the contentTypeChanged signal needs to be emitted, when the backend has set the contentType
    and it's ready for use.

    Calls to this function are followed by calls to setupFilter() and fetchData().

    \sa identifiersFromItem queryIdentifiersChanged contentTypeChanged
*/

/*!
    \fn void QIviSearchAndBrowseModelInterface::setupFilter(const QUuid &identifier, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms)

    Setup the filter for the QIviSearchAndBrowseModel instance identified by \a identifier.

    The \a term and \a orderTerms arguments are representations of the query which is used for
    filtering and sorting. The \a term argument is a null-pointer when the backend doesn't support
    filtering and sorting or when no query was defined in the QIviSearchAndBrowseModel instance.
*/

/*!
    \fn QIviPendingReply<QString> QIviSearchAndBrowseModelInterface::goBack(const QUuid &identifier)

    Requests to go back to the previous displayed data set of the QIviSearchAndBrowseModel instance identified by \a identifier.

    The new content type is returned in the form of a QIviPendingReply. Once ready the new content type
    must be set using setSuccess(), or using setFailed() if there's an error.

    See \l Browsing for more information on how this is used.
    \sa canGoBackChanged()
*/

/*!
    \fn QIviPendingReply<QString> QIviSearchAndBrowseModelInterface::goForward(const QUuid &identifier, int index)

    Requests to go to the next data set of the QIviSearchAndBrowseModel instance identified by \a identifier at \a index.

    The new content type is returned in the form of a QIviPendingReply. Once ready the new content type
    must be set using setSuccess(), or using setFailed() if there's an error.

    See \l Browsing for more information on how this is used.
    \sa canGoForwardChanged()
*/

/*!
    \fn QIviSearchAndBrowseModelInterface::insert(const QUuid &identifier, int index, const QVariant &item)

    Adds the browsable \a item to the current dataset of the QIviSearchAndBrowseModel instance identified by \a identifier at \a index.

    The provided item could be owned by another model or QML, because of that it's expected that the backend stores its internal representation.

    \sa dataChanged()
*/

/*!
    \fn QIviSearchAndBrowseModelInterface::remove(const QUuid &identifier, int index)

    Removes the browsable item at position \a index from the current dataset of the QIviSearchAndBrowseModel instance identified by \a identifier.

    \sa dataChanged()
*/

/*!
    \fn QIviSearchAndBrowseModelInterface::move(const QUuid &identifier, int currentIndex, int newIndex)

    Moves the browsable item at position \a currentIndex of the current dataset of the QIviSearchAndBrowseModel instance identified by \a identifier to the new position \a newIndex.

    \sa dataChanged()
*/

/*!
    \fn QIviSearchAndBrowseModelInterface::indexOf(const QUuid &identifier, const QVariant &item)

    Determines the index of \a item in the model identified by \a identifier.
*/

/*!
    \fn QIviSearchAndBrowseModelInterface::canGoBackChanged(const QUuid &identifier, bool canGoBack)

    Emitted to inform the QIviSearchAndBrowseModel instance, identified by \a identifier, whether it \a canGoBack to the data set previously
    shown. If the instance can display the previous data set, \a canGoBack is set to \c true.

    See \l Browsing for more information on how this is used.
    \sa goBack
*/


/*!
    \fn QIviSearchAndBrowseModelInterface::canGoForwardChanged(const QUuid &identifier, const QVector<bool> &indexes, int start)

    Emitted to inform the QIviSearchAndBrowseModel instance identified by \a identifier that the following
    \a indexes can be used to show a new set of data.

    The \a start parameter can be used to inform only about a limited set of indexes. This signal
    can be emitted during a QIviPagingModelInterface::fetchData() call to inform about the state
    of the just fetched data.

    See \l Browsing for more information on how this is used.
    \sa goForward
*/

/*!
    \fn QIviSearchAndBrowseModelInterface::contentTypeChanged(const QUuid &identifier, const QString &contentType)

    Emitted as a result of a call to setContentType, to inform the QIviSearchAndBrowseModel instance identified by \a identifier
    about it's new \a contentType.

    \sa setContentType
*/

/*!
    \fn QIviSearchAndBrowseModelInterface:availableContentTypesChanged(const QStringList &availableContentTypes)

    Emitted during the initialization phase, to inform about all available content types(\a availableContentTypes).

    The value is provided to the user as indication of which content types can be used.

    \note The QIviSearchAndBrowseModel doesn't check the validity of the contentType, this is the responsibility
    of the backend.

    \sa setContentType
*/

/*!
    \fn QIviSearchAndBrowseModelInterface:queryIdentifiersChanged(const QUuid &identifier, const QSet<QString> &queryIdentifiers)

    Emitted as a result of a call to setContentType, to inform the QIviSearchAndBrowseModel instance identified by \a identifier
    about the currently supported \a queryIdentifiers.

    The \a queryIdentifiers are used to setup the \l {Qt IVI Query Language} to be able to show
    meaningful errors for invalid queries. Not emitting this signal, will cause the \l {Qt IVI Query Language} to not limit the
    possible identifiers.
*/

QT_END_NAMESPACE
