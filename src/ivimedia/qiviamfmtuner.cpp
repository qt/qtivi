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

#include "qiviamfmtuner.h"
#include "qiviamfmtuner_p.h"
#include <QtIviCore/QIviServiceObject>
#include <QtDebug>

QT_BEGIN_NAMESPACE

QIviAmFmTunerPrivate::QIviAmFmTunerPrivate(const QString &interface, QIviAmFmTuner *parent)
    : QIviAbstractFeaturePrivate(interface, parent)
    , q_ptr(parent)
    , m_frequency(-1)
    , m_minimumFrequency(-1)
    , m_maximumFrequency(-1)
    , m_stepSize(-1)
    , m_band(QIviAmFmTuner::FMBand)
    , m_scanRunning(false)
{
    qRegisterMetaType<QIviTunerStation>();
    qRegisterMetaType<QIviAmFmTunerStation>();
}

void QIviAmFmTunerPrivate::clearToDefaults()
{
    m_frequency = -1;
    m_minimumFrequency = -1;
    m_maximumFrequency = -1;
    m_stepSize = -1;
    m_band = QIviAmFmTuner::FMBand;
    m_station = QIviAmFmTunerStation();
    m_scanRunning = false;
}

void QIviAmFmTunerPrivate::onFrequencyChanged(int frequency)
{
    if (m_frequency == frequency)
        return;

    Q_Q(QIviAmFmTuner);
    m_frequency = frequency;
    emit q->frequencyChanged(frequency);
}

void QIviAmFmTunerPrivate::onMinimumFrequencyChanged(int frequency)
{
    if (m_minimumFrequency == frequency)
        return;

    Q_Q(QIviAmFmTuner);
    m_minimumFrequency = frequency;
    emit q->minimumFrequencyChanged(frequency);
}

void QIviAmFmTunerPrivate::onMaximumFrequencyChanged(int frequency)
{
    if (m_maximumFrequency == frequency)
        return;

    Q_Q(QIviAmFmTuner);
    m_maximumFrequency = frequency;
    emit q->maximumFrequencyChanged(frequency);
}

void QIviAmFmTunerPrivate::onStepSizeChanged(int stepSize)
{
    if (m_stepSize == stepSize)
        return;

    Q_Q(QIviAmFmTuner);
    m_stepSize = stepSize;
    emit q->stepSizeChanged(stepSize);
}

void QIviAmFmTunerPrivate::onBandChanged(QIviAmFmTuner::Band band)
{
    if (m_band == band)
        return;

    Q_Q(QIviAmFmTuner);
    m_band = band;
    emit q->bandChanged(band);
}

void QIviAmFmTunerPrivate::onStationChanged(const QIviAmFmTunerStation &station)
{
    if (m_station == station)
        return;

    Q_Q(QIviAmFmTuner);
    m_station = station;
    emit q->stationChanged(station);
}

void QIviAmFmTunerPrivate::onScanStatusChanged(bool scanRunning)
{
    if (m_scanRunning == scanRunning)
        return;

    Q_Q(QIviAmFmTuner);
    m_scanRunning = scanRunning;
    emit q->scanRunningChanged(scanRunning);
    if (scanRunning)
        emit q->scanStarted();
    else
        emit q->scanStopped();
}

QIviAmFmTunerBackendInterface *QIviAmFmTunerPrivate::tunerBackend() const
{
    Q_Q(const QIviAmFmTuner);
    QIviServiceObject *so = q->serviceObject();
    if (so)
        return qobject_cast<QIviAmFmTunerBackendInterface*>(so->interfaceInstance(QLatin1String(QIviAmFmTuner_iid)));

    return nullptr;
}

/*!
    \class QIviAmFmTuner
    \inmodule QtIviMedia
    \brief Provides a interface to control the am fm tuner

    The QIviAmFmTuner provides the methods to control a am/fm tuner.
*/

/*!
    \qmltype AmFmTuner
    \instantiates QIviAmFmTuner
    \inqmlmodule QtIvi.Media
    \inherits AbstractFeature
    \brief Provides a interface to control the am fm tuner

    The AmFmTuner provides the methods to control a am/fm tuner.
*/

/*!
    \enum QIviAmFmTuner::Band
    \value AMBand
          The AM Band is based on the Amplitude Modulation technique and can range from 520 to 1610 kHz (1710 kHz).
          The step size is usually between 9 or 10 kHz.
    \value FMBand
          The FM Band is based on the Frequency Modulation technique and can range from 87.5 to 108.0 MHz.
          The step size is usually 100 kHz.
*/

/*!
    Constructs a QIviAmFmTuner.

    The \a parent argument is passed on to the \l QIviAbstractFeature base class.
*/
QIviAmFmTuner::QIviAmFmTuner(QObject *parent)
    : QIviAbstractFeature(*new QIviAmFmTunerPrivate(QLatin1String(QIviAmFmTuner_iid), this), parent)
{
}

/*!
    \qmlproperty int AmFmTuner::frequency
    \brief The current frequency of the tuner.
*/
/*!
    \property QIviAmFmTuner::frequency
    \brief The current frequency of the tuner.
*/
int QIviAmFmTuner::frequency() const
{
    Q_D(const QIviAmFmTuner);
    return d->m_frequency;
}

/*!
    \qmlproperty int AmFmTuner::minimumFrequency
    \brief The minimum frequency of the current band.
*/
/*!
    \property QIviAmFmTuner::minimumFrequency
    \brief The minimum frequency of the current band.
*/
int QIviAmFmTuner::minimumFrequency() const
{
    Q_D(const QIviAmFmTuner);
    return d->m_minimumFrequency;
}

/*!
    \qmlproperty int AmFmTuner::maximumFrequency
    \brief The maximum frequency of the current band.
*/
/*!
    \property QIviAmFmTuner::maximumFrequency
    \brief The maximum frequency of the current band.
*/
int QIviAmFmTuner::maximumFrequency() const
{
    Q_D(const QIviAmFmTuner);
    return d->m_maximumFrequency;
}

/*!
    \qmlproperty int AmFmTuner::stepSize
    \brief The frequency step size of the current band.

    \sa stepUp() stepDown()
*/
/*!
    \property QIviAmFmTuner::stepSize
    \brief The frequency step size of the current band.

    \sa stepUp() stepDown()
*/
int QIviAmFmTuner::stepSize() const
{
    Q_D(const QIviAmFmTuner);
    return d->m_stepSize;
}

/*!
    \qmlproperty enumeration AmFmTuner::band
    \brief The current band of the tuner.

    Available values are:
    \value AMBand
           The AM Band is based on the Amplitude Modulation technique and can range from 520 to 1610 kHz (1710 kHz).
           The step size is usually between 9 or 10 kHz.
    \value FMBand
           The FM Band is based on the Frequency Modulation technique and can range from 87.5 to 108.0 MHz.
           The step size is usually 100 kHz.
*/
/*!
    \property QIviAmFmTuner::band
    \brief The current band of the tuner.
*/
QIviAmFmTuner::Band QIviAmFmTuner::band() const
{
    Q_D(const QIviAmFmTuner);
    return d->m_band;
}

/*!
    \qmlproperty AmFmTunerStation AmFmTuner::station
    \brief The currently tuned station.
*/
/*!
    \property QIviAmFmTuner::station
    \brief The currently tuned station.
*/
QIviAmFmTunerStation QIviAmFmTuner::station() const
{
    Q_D(const QIviAmFmTuner);
    return d->m_station;
}

/*!
    \qmlproperty bool AmFmTuner::scanRunning
    \c true while a scan is in progress, \c false otherwise.

    \sa startScan() stopScan() scanStarted() scanStopped()
*/
/*!
    \property QIviAmFmTuner::scanRunning
    \c true while a scan is in progress, \c false otherwise.

    \sa startScan() stopScan() scanStarted() scanStopped()
*/
bool QIviAmFmTuner::isScanRunning() const
{
    Q_D(const QIviAmFmTuner);
    return d->m_scanRunning;
}

/*!
    \qmlmethod AmFmTuner::tune(AmFmTunerStation station)

    Tunes to the provided \a station.
*/

/*!
    \fn void QIviAmFmTuner::tune(const QIviAmFmTunerStation &station)

    Tunes to the provided \a station.
*/
void QIviAmFmTuner::tune(const QIviAmFmTunerStation &station)
{
    Q_D(QIviAmFmTuner);

    if (station.band() != d->m_band)
        setBand(station.band());
    if (station.frequency() != d->m_frequency)
        setFrequency(station.frequency());
}

void QIviAmFmTuner::setFrequency(int frequency)
{
    Q_IVI_BACKEND(QIviAmFmTuner, d->tunerBackend(), "Can't set the frequency without a connected backend");

    //TODO Check the minimum/maximum Frequency here ? Add a convention how things like this are done this is also used in the vehicle functions module.

    backend->setFrequency(frequency);
}

void QIviAmFmTuner::setBand(QIviAmFmTuner::Band band)
{
    Q_IVI_BACKEND(QIviAmFmTuner, d->tunerBackend(), "Can't set the band without a connected backend");

    //TODO Check the minimum/maximum Frequency here ? Add a convention how things like this are done this is also used in the vehicle functions module.

    backend->setBand(band);
}

/*!
    \qmlmethod AmFmTuner::stepUp()

    Increases the frequency by the current bands step size.
*/

/*!
    \fn void QIviAmFmTuner::stepUp()

    Increases the frequency by the current bands step size.
*/
void QIviAmFmTuner::stepUp()
{
    Q_IVI_BACKEND(QIviAmFmTuner, d->tunerBackend(), "Can't step up without a connected backend");

    //TODO Should we pass this down or use setFrequency instead ?
    backend->stepUp();
}

/*!
    \qmlmethod AmFmTuner::stepDown()

    Decreases the frequency by the current bands step size.
*/

/*!
    \fn void QIviAmFmTuner::stepDown()

    Decreases the frequency by the current bands step size.
*/
void QIviAmFmTuner::stepDown()
{
    Q_IVI_BACKEND(QIviAmFmTuner, d->tunerBackend(), "Can't step down without a connected backend");

    //TODO Should we pass this down or use setFrequency instead ?
    backend->stepDown();
}

/*!
    \qmlmethod AmFmTuner::seekUp()

    Seeks the next available Station in upwards direction and tunes it.

    If the end of the band where hit without finding a station, the search will be continued from the beginning of the band.
*/

/*!
    \fn void QIviAmFmTuner::seekUp()

    Seeks the next available Station in upwards direction and tunes it.

    If the end of the band where hit without finding a station, the search will be continued from the beginning of the band.
*/
void QIviAmFmTuner::seekUp()
{
    Q_IVI_BACKEND(QIviAmFmTuner, d->tunerBackend(), "Can't seek up without a connected backend");

    backend->seekUp();
}

/*!
    \qmlmethod AmFmTuner::seekDown()

    Seeks the next available Station in downwards direction and tunes it.

    If the beginning of the band where hit without finding a station, the search will be continued from the end of the band.
*/

/*!
    \fn void QIviAmFmTuner::seekDown()

    Seeks the next available Station in downwards direction and tunes it.

    If the beginning of the band where hit without finding a station, the search will be continued from the end of the band.
*/
void QIviAmFmTuner::seekDown()
{
    Q_IVI_BACKEND(QIviAmFmTuner, d->tunerBackend(), "Can't seek down without a connected backend");

    backend->seekDown();
}

/*!
    \qmlmethod AmFmTuner::startScan()

    Starts a scan through all available stations.

    The scan will seek to the next available station and will stay there for some seconds until it seeks to the next station.

    \sa stopScan scanRunning scanStarted scanStopped
*/

/*!
    \fn void QIviAmFmTuner::startScan()

    Starts a scan through all available stations.

    The scan will seek to the next available station and will stay there for some seconds until it seeks to the next station.

    \sa stopScan scanRunning scanStarted scanStopped
*/
void QIviAmFmTuner::startScan()
{
    Q_IVI_BACKEND(QIviAmFmTuner, d->tunerBackend(), "Can't start scanning without a connected backend");

    if (d->m_scanRunning) {
        qWarning("A scan is already in progress");
        return;
    }

    backend->startScan();
}

/*!
    \qmlmethod AmFmTuner::stopScan()

    Stops the currently active scan. If no scan is active, this method does nothing.

    \sa startScan scanRunning scanStarted scanStopped
*/

/*!
    \fn void QIviAmFmTuner::stopScan()

    Stops the currently active scan. If no scan is active, this method does nothing.

    \sa startScan scanRunning scanStarted scanStopped
*/
void QIviAmFmTuner::stopScan()
{
    Q_IVI_BACKEND(QIviAmFmTuner, d->tunerBackend(), "Can't stop scanning without a connected backend");

    if (!d->m_scanRunning) {
        qWarning("Currently no scan is active which can be stopped");
        return;
    }

    backend->stopScan();
}

/*!
    \internal
*/
QIviAmFmTuner::QIviAmFmTuner(QIviAmFmTunerPrivate &dd, QObject *parent)
    : QIviAbstractFeature(dd, parent)
{
}

/*!
    \reimp
*/
void QIviAmFmTuner::connectToServiceObject(QIviServiceObject *serviceObject)
{
    Q_UNUSED(serviceObject);

    Q_D(QIviAmFmTuner);

    QIviAmFmTunerBackendInterface *backend = d->tunerBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIviAmFmTunerBackendInterface::frequencyChanged,
                            d, &QIviAmFmTunerPrivate::onFrequencyChanged);
    QObjectPrivate::connect(backend, &QIviAmFmTunerBackendInterface::minimumFrequencyChanged,
                            d, &QIviAmFmTunerPrivate::onMinimumFrequencyChanged);
    QObjectPrivate::connect(backend, &QIviAmFmTunerBackendInterface::maximumFrequencyChanged,
                            d, &QIviAmFmTunerPrivate::onMaximumFrequencyChanged);
    QObjectPrivate::connect(backend, &QIviAmFmTunerBackendInterface::stepSizeChanged,
                            d, &QIviAmFmTunerPrivate::onStepSizeChanged);
    QObjectPrivate::connect(backend, &QIviAmFmTunerBackendInterface::bandChanged,
                            d, &QIviAmFmTunerPrivate::onBandChanged);
    QObjectPrivate::connect(backend, &QIviAmFmTunerBackendInterface::stationChanged,
                            d, &QIviAmFmTunerPrivate::onStationChanged);
    QObjectPrivate::connect(backend, &QIviAmFmTunerBackendInterface::scanStatusChanged,
                            d, &QIviAmFmTunerPrivate::onScanStatusChanged);

    QIviAbstractFeature::connectToServiceObject(serviceObject);
}

/*!
    \reimp
*/
void QIviAmFmTuner::clearServiceObject()
{
    Q_D(QIviAmFmTuner);
    d->clearToDefaults();
}

/*!
    \qmlsignal AmFmTuner::scanStarted()

    A new scan has started and is now running.

    \sa startScan stopScan scanRunning scanStopped
*/

/*!
    \fn void QIviAmFmTuner::scanStarted()

    A new scan has started and is now running.

    \sa startScan stopScan scanRunning scanStopped
*/

/*!
    \qmlsignal AmFmTuner::scanStopped()

    The currently active scan has stopped.

    \sa startScan stopScan scanRunning scanStarted
*/

/*!
    \fn void QIviAmFmTuner::scanStopped()

    The currently active scan has stopped.

    \sa startScan stopScan scanRunning scanStarted
*/

QT_END_NAMESPACE

#include "moc_qiviamfmtuner.cpp"
