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

#include "amfmtunerbackend.h"

#include <QtDebug>

AmFmTunerBackend::AmFmTunerBackend(QObject *parent)
    : QIviAmFmTunerBackendInterface(parent)
    , m_band(QIviAmFmTuner::FMBand)
    , m_timerId(-1)
{
    qRegisterMetaType<QIviAmFmTunerStation>();

    QVector<QIviAmFmTunerStation> fm_stations;
    QIviAmFmTunerStation radioQt;
    radioQt.setId("0");
    radioQt.setStationName("Radio Qt");
    radioQt.setFrequency(87500000);
    radioQt.setBand(QIviAmFmTuner::FMBand);
    fm_stations.append(radioQt);

    QIviAmFmTunerStation qtRocksNonStop;
    qtRocksNonStop.setId("1");
    qtRocksNonStop.setStationName("Qt Rocks non-stop");
    qtRocksNonStop.setFrequency(102500000);
    qtRocksNonStop.setBand(QIviAmFmTuner::FMBand);
    fm_stations.append(qtRocksNonStop);
    BandData fmdata;
    fmdata.m_frequency = 87500000;
    fmdata.m_minimumFrequency = 87500000;
    fmdata.m_maximumFrequency = 108000000;
    fmdata.m_stepSize = 100000;
    fmdata.m_stations = fm_stations;
    m_bandHash.insert(QIviAmFmTuner::FMBand, fmdata);

    BandData amdata;
    amdata.m_frequency = 535000;
    amdata.m_minimumFrequency = 535000;
    amdata.m_maximumFrequency = 1700000;
    amdata.m_stepSize = 10000;
    m_bandHash.insert(QIviAmFmTuner::AMBand, amdata);
}

void AmFmTunerBackend::initialize()
{
    emit bandChanged(m_band);
    emit minimumFrequencyChanged(m_bandHash[m_band].m_minimumFrequency);
    emit maximumFrequencyChanged(m_bandHash[m_band].m_maximumFrequency);
    emit stepSizeChanged(m_bandHash[m_band].m_stepSize);
    emit frequencyChanged(m_bandHash[m_band].m_frequency);
    emit stationChanged(m_bandHash[m_band].m_stations.at(0));
}

void AmFmTunerBackend::setFrequency(int frequency)
{
    if (m_bandHash[m_band].m_frequency == frequency)
        return;

    if (frequency < m_bandHash[m_band].m_minimumFrequency || frequency > m_bandHash[m_band].m_maximumFrequency) {
        qWarning() << "SIMULATION Frequency out of range";
        return;
    }

    qWarning() << "SIMULATION Frequency changed to" << frequency;

    m_bandHash[m_band].m_frequency = frequency;
    emit frequencyChanged(frequency);
    emit stationChanged(stationAt(m_bandHash[m_band].m_frequency));
}

void AmFmTunerBackend::setBand(QIviAmFmTuner::Band band)
{
    if (m_band == band)
        return;

    qWarning() << "SIMULATION Band changed to" << band;

    m_band = band;
    emit bandChanged(band);
    emit minimumFrequencyChanged(m_bandHash[m_band].m_minimumFrequency);
    emit maximumFrequencyChanged(m_bandHash[m_band].m_maximumFrequency);
    emit stepSizeChanged(m_bandHash[m_band].m_stepSize);
    emit frequencyChanged(m_bandHash[m_band].m_frequency);
    emit stationChanged(stationAt(m_bandHash[m_band].m_frequency));
}

void AmFmTunerBackend::stepUp()
{
    qWarning() << "SIMULATION Step Up";

    int newFreq = m_bandHash[m_band].m_frequency + m_bandHash[m_band].m_stepSize;
    if (newFreq > m_bandHash[m_band].m_maximumFrequency)
        newFreq = m_bandHash[m_band].m_minimumFrequency;

    setFrequency(newFreq);
}

void AmFmTunerBackend::stepDown()
{
    qWarning() << "SIMULATION Step Down";

    int newFreq = m_bandHash[m_band].m_frequency + m_bandHash[m_band].m_stepSize;
    if (newFreq < m_bandHash[m_band].m_minimumFrequency)
        newFreq = m_bandHash[m_band].m_maximumFrequency;

    setFrequency(m_bandHash[m_band].m_frequency - m_bandHash[m_band].m_stepSize);
}

void AmFmTunerBackend::seekUp()
{
    qWarning() << "SIMULATION Seek Up";

    QVector<QIviAmFmTunerStation> stations = m_bandHash[m_band].m_stations;
    if (stations.count() == 0) {
        return;
    } else if (stations.count() == 1) {
        setCurrentStation(stations.at(0));
    } else {
        int index = stationIndexFromFrequency(m_bandHash[m_band].m_frequency);
        index++;
        if (index >= stations.count())
            index = 0;

        setCurrentStation(stations.at(index));
    }
}

void AmFmTunerBackend::seekDown()
{
    qWarning() << "SIMULATION Seek Down";

    QVector<QIviAmFmTunerStation> stations = m_bandHash[m_band].m_stations;
    if (stations.count() == 0) {
        return;
    } else if (stations.count() == 1) {
        setCurrentStation(stations.at(0));
    } else {
        int index = stationIndexFromFrequency(m_bandHash[m_band].m_frequency);
        index--;
        if (index < 0)
            index = stations.count() - 1;

        setCurrentStation(stations.at(index));
    }
}

void AmFmTunerBackend::startScan()
{
    if (m_timerId != -1) {
        qWarning() << "SIMULATION a Scan is already in progress";
        return;
    }

    qWarning() << "SIMULATION Scan started";

    emit scanStatusChanged(true);
    seekUp();
    m_timerId = startTimer(5000);
}

void AmFmTunerBackend::stopScan()
{
    if (m_timerId == -1) {
        qWarning() << "SIMULATION no Scan is currently in progress, which can be stopped";
        return;
    }

    qWarning() << "SIMULATION Scan stopped";

    killTimer(m_timerId);
    m_timerId = -1;
    emit scanStatusChanged(false);
}

void AmFmTunerBackend::setCurrentStation(const QIviAmFmTunerStation &station)
{
    m_bandHash[m_band].m_frequency = station.frequency();

    qWarning() << "SIMULATION Station changed to" << station.stationName() << station.frequency();

    emit frequencyChanged(station.frequency());
    emit stationChanged(station);
}

int AmFmTunerBackend::stationIndexFromFrequency(int frequency) const
{
    QVector<QIviAmFmTunerStation> stations = m_bandHash[m_band].m_stations;
    for (int i=0; i < stations.count(); i++) {
        const QIviAmFmTunerStation& station = stations.at(i);
        if (station.frequency() == frequency)
            return i;
    }

    return -1;
}

QIviAmFmTunerStation AmFmTunerBackend::stationAt(int frequency) const
{
    int index = stationIndexFromFrequency(frequency);
    if (index != -1)
        return m_bandHash[m_band].m_stations.at(index);

    return QIviAmFmTunerStation();
}

void AmFmTunerBackend::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    seekUp();
}
