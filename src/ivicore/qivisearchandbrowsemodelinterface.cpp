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

#include "qivisearchandbrowsemodelinterface.h"
#include "qivisearchandbrowsemodelinterface_p.h"

QIviSearchAndBrowseModelInterfacePrivate::QIviSearchAndBrowseModelInterfacePrivate()
{
}

/*!
    \class QIviSearchAndBrowseModelInterface
    \inmodule QtIviCore
    \ingroup backends
    \inherits QObject
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

/*!
    \enum QIviSearchAndBrowseModelInterface::Flag
    \value NoExtras
           The backend does only support the minimum feature set and is stateful.
    \value SupportsFiltering
           The backend supports filtering of the content. availableContentTypes() and supportedIdentifiers() will be used as input for the
           \l {Qt IVI Query Language}. \sa registerContentType
    \value SupportsSorting
           The backend supports sorting of the content. availableContentTypes() and supportedIdentifiers() will be used as input for the
           \l {Qt IVI Query Language}. \sa registerContentType
    \value SupportsAndConjunction
           The backend supports handling multiple filters at the same time and these filters can be combined by using the AND conjunction.
    \value SupportsOrConjunction
           The backend supports handling multiple filters at the same time and these filters can be combined by using the OR conjunction.
    \value SupportsStatelessNavigation
           The backend is stateless and supports handling multiple instances of a QIviSearchAndBrowseModel requesting different data at the same time.
           E.g. One request for artists, sorted by name and another request for tracks. The backend has to consider that both request come from models which are
           currently visible at the same time.
    \value SupportsGetSize
           The backend can return the final number of items for a specific request. This makes it possible to support the QIviSearchAndBrowseModel::DataChanged loading
           type.
*/

/*!
    \fn QIviSearchAndBrowseModelInterface::QIviSearchAndBrowseModelInterface(QObject *parent=0)

    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIviSearchAndBrowseModelInterface::QIviSearchAndBrowseModelInterface(QObject *parent)
    : QObject(*new QIviSearchAndBrowseModelInterfacePrivate(), parent)
{}

QIviSearchAndBrowseModelInterface::~QIviSearchAndBrowseModelInterface()
{}

/*!
    Returns all the available content types for this backend instance.

    It's recommended to use the registerContentType() function instead, which takes care of this function for you.
*/
QSet<QString> QIviSearchAndBrowseModelInterface::availableContentTypes() const
{
    Q_D(const QIviSearchAndBrowseModelInterface);
    return d->m_types;
}

/*!
    Returns the available identifiers for the given \a contentType.

    Every identifier can be used in the \l {Qt IVI Query Language} for filtering or sorting.

    It's recommended to use the registerContentType() function instead, which takes care of this function for you.
*/
QSet<QString> QIviSearchAndBrowseModelInterface::supportedIdentifiers(const QString &contentType) const
{
    Q_D(const QIviSearchAndBrowseModelInterface);
    return d->m_identifiers.values(contentType).toSet();
}


/*!
    \fn registerContentType(const QString &contentType)

    Registers the type T with the name \a contentType.
    In addition all properties of type T will be registered as identifiers for the \l {Qt IVI Query Language}
*/
/*!
    \internal
*/
void QIviSearchAndBrowseModelInterface::registerContentType(const QMetaObject &object, const QString &contentType)
{
    Q_D(QIviSearchAndBrowseModelInterface);
    for (int i=0; i < object.propertyCount(); i++) {
        QLatin1String propName(object.property(i).name());
        if (propName != QLatin1String("objectName"))
            d->m_identifiers.insert(contentType, propName);
    }

    d->m_types.insert(contentType);
}

/*!
    \fn Flags QIviSearchAndBrowseModelInterface::supportedFlags() const

    Returns all the flags this backend instance supports.
*/

/*!
    \fn void QIviSearchAndBrowseModelInterface::fetchData(const QUuid &identifier, const QString &type, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms, int start, int count)

    This function is called whenever new data of the given type \a type needs to be retrieved by a QIviSearchAndBrowseModel identified by \a identifier.
    The \a term and \a orderTerms properties are representations of the query which is used for filtering and sorting. The \a term pointer is null when the backend
    doesn't support filtering and sorting or when no query was defined in the QIviSearchAndBrowseModel instance.

    The parameters \a start and \a count define the range of data which should be fetched. This method is expected to emit the dataFetched() signal once
    the new data is ready.

    \sa dataFetched()
*/

/*!
    \fn bool QIviSearchAndBrowseModelInterface::canGoBack(const QUuid &identifier, const QString &type)

    Returns true when the QIviSearchAndBrowseModel instance identified by \a identifier can go back and return to the previously shown data set.
    The \a type parameter holds the currently displayed content type.

    See \l Browsing for more information on how this is used.
    \sa goBack()
*/

/*!
    \fn QString QIviSearchAndBrowseModelInterface::goBack(const QUuid &identifier, const QString &type)

    Requests to go back to the previous displayed data set of the QIviSearchAndBrowseModel instance identified by \a identifier.
    The current content type is passed as \a type and the new content type should be returned.

    See \l Browsing for more information on how this is used.
    \sa canGoBack()
*/

/*!
    \fn bool QIviSearchAndBrowseModelInterface::canGoForward(const QUuid &identifier, const QString &type, const QString &itemId)

    Returns true when the QIviSearchAndBrowseModel instance identified by \a identifier and the current content type \a type has an
    item identified by \a itemId and this item can be used to show a new set of data.

    See \l Browsing for more information on how this is used.
    \sa goForward
*/

/*!
    \fn QString QIviSearchAndBrowseModelInterface::goForward(const QUuid &identifier, const QString &type, const QString &itemId)

    Requests to go to the next data set of the QIviSearchAndBrowseModel instance identified by \a identifier and the current content type \a type.
    The item which is used for the new set of data is passed as \a itemId and the new content type should be returned.

    See \l Browsing for more information on how this is used.
    \sa canGoForward()
*/

/*!
    \fn void QIviSearchAndBrowseModelInterface::countChanged(const QUuid &identifier, int newLength)

    This signal is emitted when the current number of items in the QIviSearchAndBrowseModel instance identified by \a identifier changed.
    The new number of items is returned as \a newLength.

    This signal is expected to be emitted after the model instance has requested new data for the first time by calling fetchData() and
    should be emitted before the data is returned by emitting the dataFetched() signal

    \sa fetchData() dataFetched()
*/

/*!
    \fn void QIviSearchAndBrowseModelInterface::dataFetched(const QUuid &identifier, const QList<QVariant> &data, int start, bool moreAvailable)

    This signal is emitted as a result of a call to fetchData() and returns the requested data in the argument \a data to the QIviSearchAndBrowseModel instance identified by \a identifier.
    The arguments \a start holds the index where the data starts and \a moreAvailable holds whether there is more data available and a new fetchData() call can be used to retrieve this data.

    \sa fetchData() dataFetched()
*/

/*!
    \fn void QIviSearchAndBrowseModelInterface::dataChanged(const QUuid &identifier, const QList<QVariant> &data, int start, int count)

    This signal is emitted whenever the data in the QIviSearchAndBrowseModel instance identified by \a identifier changed and the model needs to be updated.
    The new data is passed as \a data. The arguments \a start and \a count can be used to define the set of items which should be replaced with the new data.

    <example>
*/
