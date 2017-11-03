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

#include "qiviamfmtunerbackendinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIviAmFmTunerBackendInterface
    \inmodule QtIviMedia
    \ingroup backends
    \inherits QObject
    \keyword org.qt-project.qtivi.AmFmTuner/1.0
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
    : QIviFeatureInterface(parent)
{
}

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
    \fn QIviAmFmTunerBackendInterface::startScan()

    Starts a scan through all available stations.
    The scan will seek to the next available station and will stay there for some seconds until it seeks to the next station.
    It is expected that this function will inform about the current state of the backend by emitting the scanStatusChanged() signal.

    \sa stopScan() scanStatusChanged()
*/

/*!
    \fn QIviAmFmTunerBackendInterface::stopScan()

    Stops the currently active scan. If no scan is active, this method does nothing.
    It is expected that this function will inform about the current state of the backend by emitting the scanStatusChanged() signal.

    \sa startScan() scanStatusChanged()
*/

/*!
    \fn QIviAmFmTunerBackendInterface::frequencyChanged(int frequency)

    Emitted when the current frequency changed. The new value is passed as \a frequency.
*/

/*!
    \fn QIviAmFmTunerBackendInterface::minimumFrequencyChanged(int minimumFrequency)

    Emitted when the minimum frequency of the current band changed. The new value is passed as \a minimumFrequency.
    This is usually caused by a switching the band.

    \sa bandChanged() maximumFrequencyChanged() stepSizeChanged()
*/

/*!
    \fn QIviAmFmTunerBackendInterface::maximumFrequencyChanged(int maximumFrequency)

    Emitted when the maximum frequency of the current band changed. The new value is passed as \a maximumFrequency.
    This is usually caused by a switching the band.

    \sa bandChanged() minimumFrequencyChanged() stepSizeChanged()
*/

/*!
    \fn QIviAmFmTunerBackendInterface::stepSizeChanged(int stepSize)

    Emitted when the frequency step size of the current band changed. The new value is passed as \a stepSize.
    This is usually caused by a switching the band.

    \sa bandChanged() minimumFrequencyChanged() maximumFrequencyChanged()
*/

/*!
    \fn QIviAmFmTunerBackendInterface::bandChanged(QIviAmFmTuner::Band band)

    Emitted when the current band changed. The new value is passed as \a band.

    \sa minimumFrequencyChanged() maximumFrequencyChanged() stepSizeChanged()
*/

/*!
    \fn QIviAmFmTunerBackendInterface::stationChanged(const QIviAmFmTunerStation &station)

    Emitted when the current station changed. The new station is passed as \a station.
*/

/*!
    \fn QIviAmFmTunerBackendInterface::scanStatusChanged(bool scanRunning)

    Emitted when the status of the station scan has changed. This signal should be emitted
    as a result of a call to startScan() or stopScan() and informs about the current status using the \a scanRunning argument.

    \sa startScan() stopScan()
*/

QT_END_NAMESPACE
