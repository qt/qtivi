/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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

#include "qivifeatureinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIviFeatureInterface
    \inmodule QtIviCore
    \ingroup backends

    \brief QIviFeatureInterface defines the base class for all backends.

    To implement a backend for a specific feature you need to derive from this class.
    There's a one-to-one mapping between a spezialized feature API and its corresponding feature interface
    class.

    The feature interface class specifies which functions and signals need to be implemented by a backend
    to work with a specific feature.

    This base class contains the generic error handling, which is common between all interfaces.

    See the full example backend implementation from \c {src/plugins/ivivehiclefunctions/climate_simulator}.
    \sa QIviAbstractFeature, QIviServiceInterface
*/

QIviFeatureInterface::QIviFeatureInterface(QObject *parent)
    : QObject(parent)
{
}

QIviFeatureInterface::~QIviFeatureInterface()
{
}

QIviFeatureInterface::QIviFeatureInterface(QObjectPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
}

/*!
    \fn void QIviFeatureInterface::initialize()

    Initializes the backend. This function is called after a feature connected to the backend.
    It is expected that this function will inform about the current state of the backend by
    emitting signals with the current status.

    The last signal which needs to be sent is the initializationDone() signal.

    \sa initializationDone()
*/

/*!
    \fn void QIviFeatureInterface::errorChanged(QIviAbstractFeature::Error error, const QString &message = QString())

    The signal is emitted when \a error occurs in the backend.
    Error \a message is optional.
*/

/*!
    \fn void QIviFeatureInterface::initializationDone()

    The signal is emitted  once the current backend state has been sent to the feature after
    a call to initialize()

    \sa initialize
*/

QT_END_NAMESPACE
