/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

#include "qivimediadevicediscoverymodelbackendinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIviMediaDeviceDiscoveryModelBackendInterface
    \inmodule QtIviMedia
    \ingroup backends
    \inherits QObject
    \keyword org.qt-project.qtivi.MediaDiscovery/1.0
    \brief The QIviMediaDeviceDiscoveryModelBackendInterface defines the interface for backends to the
    QIviMediaDeviceDiscoveryModel feature class.

    The QIviMediaDeviceDiscoveryModelBackendInterface is the interface used by \l QIviMediaDeviceDiscoveryModel

    The interface is discovered by a \l QIviMediaDeviceDiscoveryModel object, which connects to it and sets it up.

    <example of a fully featured backend>
*/

/*!
    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIviMediaDeviceDiscoveryModelBackendInterface::QIviMediaDeviceDiscoveryModelBackendInterface(QObject *parent)
    : QIviFeatureInterface(parent)
{

}

/*!
    \fn QIviMediaDeviceDiscoveryModelBackendInterface::initialize()

    Initializes the backend. This function is called after a feature connected to the backend.
    It is expected that this function will use the availableDevices signal to inform the feature about the
    currently available devices.

    \note It may happen that a second feature instance is connecting to the same backend and because of this,
    this function can be called multiple times. The implementation should inform about the current state after every call.

    \sa availableDevices()
*/

//TODO add a unit test for testing multiple features instances connecting to the same instance.

/*!
    \fn void QIviMediaDeviceDiscoveryModelBackendInterface::availableDevices(const QList<QIviServiceObject*> devices)

    This signal is emitted after a call to initialize() and indicates all the already connected \a devices, which are ready
    to be used.

    \sa initialize()
*/

/*!
    \fn void QIviMediaDeviceDiscoveryModelBackendInterface::deviceAdded(QIviServiceObject *device)

    This signal is emitted when a new \a device is added to the system and is ready to be used.
*/

/*!
    \fn void QIviMediaDeviceDiscoveryModelBackendInterface::deviceRemoved(QIviServiceObject *device)

    This signal is emitted when a \a device has been removed from the system and cannot be used anymore.
*/

QT_END_NAMESPACE
