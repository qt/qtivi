/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
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

#include "qivimediaindexercontrolbackendinterface.h"

/*!
    \class QIviMediaIndexerControlBackendInterface
    \inmodule QtIviMedia
    \ingroup backends
    \inherits QObject
    \brief The QIviMediaIndexerControlBackendInterface defines the interface for backends to the
    QIviMediaIndexerControl feature class.

    The QIviMediaIndexerControlBackendInterface is the interface used by \l QIviMediaIndexerControl

    The interface is discovered by a \l QIviMediaIndexerControl object, which connects to it and sets it up.

    <example of a fully featured backend>
*/

/*!
    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIviMediaIndexerControlBackendInterface::QIviMediaIndexerControlBackendInterface(QObject *parent)
    : QObject(parent)
{

}

/*!
    \fn QIviMediaIndexerControlBackendInterface::initialize()

    Initializes the backend. This function is called after a feature connected to the backend.
    It is expected that this function will use the stateChanged and the progressChanged signals to inform the feature about the
    current state of the indexing.

    \sa stateChanged() progressChanged()
*/

/*!
    \fn QIviMediaIndexerControlBackendInterface::pause()

    Pauses the currently ongoing indexing. If currently no indexing is ongoing this call should not perform any actions.

    \sa resume()
*/

/*!
    \fn QIviMediaIndexerControlBackendInterface::resume()

    Resumes the previously paused indexing. If there is nothing to resume this call should not perform any actions.

    \sa pause()
*/

/*!
    \fn void QIviMediaIndexerControlBackendInterface::progressChanged(qreal progress)

    This signal is emitted when indexing is currently ongoing and the progress of the operation changed.
    The \a progress argument passes the current progress between \e 0 and \e 1.
*/

/*!
    \fn void QIviMediaIndexerControlBackendInterface::stateChanged(QIviMediaIndexerControl::State state)

    This signal is emitted whenever the indexing state changed. The \a state argument holds the new state
    of the indexer.
*/
