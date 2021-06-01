/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
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
****************************************************************************/

#include "qivipagingmodelinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIviPagingModelInterface
    \inmodule QtIviCore
    \ingroup backends
    \inherits QObject
    \keyword org.qt-project.qtivi.PagingModel/1.0
    \brief The QIviPagingModelInterface defines the interface for backends to the
    QIviPagingModel feature class.

    The QIviPagingModelInterface is the interface used by \l QIviPagingModel

    The interface is discovered by a \l QIviPagingModel object, which connects to it and sets it up.

    Every QIviPagingModel generates its own QUuid which is passed to the backend interface and can
    be used to identify a model instance.

    \sa QIviPagingModel

    //TODO explain how the interface works on a example
    <example of a fully featured backend>
*/

/*!
    \fn QIviPagingModelInterface::QIviPagingModelInterface(QObject *parent = nullptr)

    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIviPagingModelInterface::QIviPagingModelInterface(QObject *parent)
    : QIviFeatureInterface(parent)
{}

QIviPagingModelInterface::QIviPagingModelInterface(QObjectPrivate &dd, QObject *parent)
    : QIviFeatureInterface(dd, parent)
{
}

/*!
    \fn void QIviPagingModelInterface::registerInstance(const QUuid &identifier)

    Registers the instance of QIviPagingModel identified by \a identifier with this backend. This
    function will be called by QIviPagingModel and all its derived classes after the initialize()
    function, but before any data will be requested e.g. via fetchData().

    \sa unregisterInstance
*/

/*!
    \fn void QIviPagingModelInterface::unregisterInstance(const QUuid &identifier)

    Unregisters the instance of QIviPagingModel identified by \a identifier with this backend. This
    function will be called by QIviPagingModel and all its derived classes before the
    ServiceObject gets disconnected and can be used to cleanup all cached data for this \a
    identifier.

    \sa registerInstance
*/

/*!
    \fn void QIviPagingModelInterface::fetchData(const QUuid &identifier, int start, int count)

    This function is called whenever new data needs to be retrieved by a QIviPagingModel identified by \a identifier.

    The parameters \a start and \a count define the range of data which should be fetched. This method is expected to emit the dataFetched() signal once
    the new data is ready.

    \sa dataFetched()
*/

/*!
    \fn void QIviPagingModelInterface::supportedCapabilitiesChanged(const QUuid &identifier, QtIviCoreModule::ModelCapabilities capabilities)

    Emitted when the \a capabilities of the model instance identified by \a identifier changed.

    \note If a null QQuuid is used as a identifier, all model instances will be informed.
*/

/*!
    \fn void QIviPagingModelInterface::countChanged(const QUuid &identifier, int count)

    This signal is emitted when the current number of items in the QIviPagingModel instance identified by \a identifier changed.
    The new number of items is returned as \a count.

    This signal is expected to be emitted after the model instance has requested new data for the first time by calling fetchData() and
    should be emitted before the data is returned by emitting the dataFetched() signal.

    \note If a null QQuuid is used as a identifier, all model instances will be informed.

    \sa fetchData() dataFetched()
*/

/*!
    \fn void QIviPagingModelInterface::dataFetched(const QUuid &identifier, const QList<QVariant> &data, int start, bool moreAvailable)

    This signal is emitted as a result of a call to fetchData() and returns the requested data in the argument \a data to the QIviPagingModel instance identified by \a identifier.
    The arguments \a start holds the index where the data starts and \a moreAvailable holds whether there is more data available and a new fetchData() call can be used to retrieve this data.

    \note If a null QQuuid is used as a identifier, all model instances will be informed.

    \sa fetchData() dataFetched()
*/

/*!
    \fn void QIviPagingModelInterface::dataChanged(const QUuid &identifier, const QList<QVariant> &data, int start, int count)

    This signal is emitted whenever the data in the QIviPagingModel instance identified by \a identifier changed and the model needs to be updated.
    The new data is passed as \a data. The arguments \a start and \a count can be used to define the set of items which should be replaced with the new data.

    For inserting a new item, the item is passed in \a data and \a start is used for where the item should be inserted, the \a count argument needs to be 0 as we don't want to replace existing data:

    \code
    QList<ExampleItem> list;
    ExampleItem item = ExampleItem();
    list.insert(index, item);
    QVariantList items = { QVariant::fromValue(item) };
    emit dataChanged(items, index, 0);
    \endcode

    Removing an item is very similar, \a start is used to indicate which item and \a count to indicate how much:

    \code
    list.removeAt(index);
    emit dataChanged(identifier, QVariantList(), index, 1);
    \endcode

    \note If a null QQuuid is used as a identifier, all model instances will be informed.
*/

QT_END_NAMESPACE
