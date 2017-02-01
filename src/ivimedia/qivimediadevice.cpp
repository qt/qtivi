/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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

#include "qivimediadevice.h"

/*!
    \class QIviMediaDevice
    \inmodule QtIviMedia
    \brief QIviMediaDevice is the base class for all Media Devices

    A Media Device is an externally or internally connected device on which media is stored.
    It can be a physical medium like a USB thumbdrive or a CD/DVD, but it could also be a DLNA Media Server running
    on a different machine e.g. on the rear seat entertainment system.

    Media Devices can be retrieved by using the QIviMediaDeviceDiscoveryModel, which asks the backend
    which devices it knows of that are currently connected to the IVI system.

    \section1 Supported Media Devices
    \target SupportedMediaDevices

    The following media devices are supported by the QIviMedia module:
    \annotatedlist qtivi_mediadevices
*/

/*!
    \qmltype MediaDevice
    \instantiates QIviMediaDevice
    \inqmlmodule QtIvi.Media
    \inherits ServiceObject
    \brief MediaDevice is the base item for all Media Devices

    A Media Device is an externally or internally connected device on which media is stored.
    It can be a physical medium like a USB thumbdrive or a CD/DVD, but it could also be a DLNA Media Server running
    on a different machine e.g. on the rear seat entertainment system.

    Media Devices can be retrieved by using the MediaDeviceDiscoveryModel, which asks the backend
    which devices it knows of that are currently connected to the IVI system.

    \section1 Supported Media Devices
    \target QmlSupportedMediaDevices

    The following media devices are supported by the QIviMedia module:
    \annotatedlist qtivi_qmlmediadevices
*/

/*!
    \qmlproperty string MediaDevice::name
    The name of the media device. E.g. the CD-name or the name of the thumbdrive.
*/

/*!
    \property QIviMediaDevice::name
    The name of the media device. E.g. the CD-name or the name of the thumbdrive.
*/

/*!
    \qmlproperty string MediaDevice::type
    The type of the media device. E.g. "usb", "CD". See \l QmlSupportedMediaDevices for the supported Devices.
*/

/*!
    \property QIviMediaDevice::type
    The type of the media device. E.g. "usb", "CD". See \l SupportedMediaDevices for the supported Devices.
*/

QIviMediaDevice::QIviMediaDevice(QObject *parent)
    : QIviServiceObject(parent)
{

}

/*!
    \class QIviMediaUsbDevice
    \inmodule QtIviMedia
    \ingroup qtivi_mediadevices
    \brief QIviMediaUsbDevice represents a USB device connected to the system.

    This device instance can be used as a serviceObject for other Features e.g. the
    QIviSearchAndBrowseModel to browse through the content. Whether a feature is supported
    by this device is decided by the backend implementing it.

    \note The type of this device is \e "usb"
*/

/*!
    \qmltype MediaUsbDevice
    \instantiates QIviMediaUsbDevice
    \inqmlmodule QtIvi.Media
    \ingroup qtivi_qmlmediadevices
    \inherits MediaDevice
    \brief MediaUsbDevice represents a USB device connected to the system.

    This device instance can be used as a serviceObject for other Features e.g. the
    SearchAndBrowseModel to browse through the content. Whether a feature is supported
    by this device is decided by the backend implementing it.

    \note The type of this device is \e "usb"
*/

QIviMediaUsbDevice::QIviMediaUsbDevice(QObject *parent)
    : QIviMediaDevice(parent)
{

}

/*!
    \reimp

    Returns \e "usb".
*/
QString QIviMediaUsbDevice::type() const
{
    return QLatin1String("usb");
}

/*!
    \qmlmethod MediaUsbDevice::eject()

    Ejects the USB media device and makes sure all data is written to the thumb-drive so it can safely be removed.
*/

/*!
    \fn void QIviMediaUsbDevice::eject()

    Ejects the USB media device and makes sure all data is written to the thumb-drive so it can safely be removed.
*/
