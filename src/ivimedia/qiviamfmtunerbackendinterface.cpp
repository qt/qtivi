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

#include "qiviamfmtunerbackendinterface.h"

/*!
    \class QIviAmFmTunerBackendInterface
    \inmodule QtIviMedia
    \ingroup backends
    \inherits QObject
    \brief The QIviAmFmTunerBackendInterface defines the interface for backends to the
    QIviAmFmTuner feature class.

    The QIviAmFmTunerBackendInterface is the interface used by \l QIviAmFmTuner

    The interface is discovered by a \l QIviAmFmTuner object, which connects to it and sets it up.

    <example of a fully featured backend>
*/

/*!
    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIviAmFmTunerBackendInterface::QIviAmFmTunerBackendInterface(QObject *parent)
    : QObject(parent)
{
}

/*!
    \fn QIviAmFmTunerBackendInterface::initialize()

    Initializes the backend. This function is called, after a feature connected to the backend.
    It is expected that this function will inform about the current state of the backend by emitting signals with the current status.
*/

/*!
    \fn QIviAmFmTunerBackendInterface::setFrequency(int frequency)

    Changes the frequency to the new value passed as \a frequency.

    This method is expected to emit a \l frequencyChanged() signal when the internal state changes due to this function call
*/

/*!
    \fn QIviAmFmTunerBackendInterface::setBand(QIviAmFmTuner::Band band)

    Changes the band to the new value passed as \a band.

    This method is expected to emit a \l bandChanged() signal when the internal state changes due to this function call
*/

/*!
    \fn QIviAmFmTunerBackendInterface::stepUp()

    Increases the frequency by the current bands step size.

    \sa frequencyChanged stationChanged
*/

/*!
    \fn QIviAmFmTunerBackendInterface::stepDown()

    Decreases the frequency by the current bands step size.

    \sa frequencyChanged stationChanged
*/

/*!
    \fn QIviAmFmTunerBackendInterface::seekUp()

    Seeks the next available Station in upwards direction and tunes it.

    If the end of the band where hit without finding a station, the search will be continued from the beginning of the band.

    \sa frequencyChanged stationChanged
*/

/*!
    \fn QIviAmFmTunerBackendInterface::seekDown()

    Seeks the next available Station in downwards direction and tunes it.

    If the beginning of the band where hit without finding a station, the search will be continued from the end of the band.

    \sa frequencyChanged stationChanged
*/

/*!
    \fn QIviAmFmTunerBackendInterface::frequencyChanged(int frequency)

    Emitted when the current frequency changed. The new value is passed as \a frequency.
*/

/*!
    \fn QIviAmFmTunerBackendInterface::bandChanged(QIviAmFmTuner::Band band)

    Emitted when the current band changed. The new value is passed as \a band.
*/

/*!
    \fn QIviAmFmTunerBackendInterface::stationChanged(QVariant station)

    Emitted when the current station changed. The new station is passed as \a station.
*/
