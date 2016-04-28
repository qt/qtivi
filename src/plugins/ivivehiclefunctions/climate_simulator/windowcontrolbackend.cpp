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

#include "windowcontrolbackend.h"

WindowTimer::WindowTimer(const QString &zone, bool isBlind, WindowControlBackend *backend)
    : QObject(backend)
    , m_timer(new QTimer(this))
    , m_opening(true)
    , m_currentValue(0)
    , m_interval(4)
    , m_zone(zone)
    , m_blind(isBlind)
    , m_backend(backend)
{
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, &WindowTimer::checkValue);
}

void WindowTimer::setOpeningTime(int intervalInSeconds)
{
    m_interval = intervalInSeconds;
}

void WindowTimer::open()
{
    m_opening = true;
    m_timer->start();
}

void WindowTimer::close()
{
    m_opening = false;
    m_timer->start();
}

void WindowTimer::checkValue()
{
    if (m_opening)
        m_currentValue++;
    else
        m_currentValue--;

    QtIVIWindowControl::State state = QtIVIWindowControl::Closed;
    if (m_currentValue <= 0) {
        m_currentValue = 0;
        m_timer->stop();
        state = QtIVIWindowControl::Closed;
    } else if (m_currentValue >= m_interval) {
        m_currentValue = m_interval;
        m_timer->stop();
        state = QtIVIWindowControl::FullyOpen;
    } else {
        state = QtIVIWindowControl::Open;
    }

    if (m_blind) {
        if (m_backend->m_zoneMap[m_zone].blindState == state)
            return;
        qWarning() << "SIMULATION blind state for Zone" << m_zone << "changed to" << state;
        m_backend->m_zoneMap[m_zone].blindState = state;
        emit m_backend->blindStateChanged(state, m_zone);
    } else {
        if (m_backend->m_zoneMap[m_zone].state == state)
            return;
        qWarning() << "SIMULATION window state for Zone" << m_zone << "changed to" << state;
        m_backend->m_zoneMap[m_zone].state = state;
        emit m_backend->stateChanged(state, m_zone);
    }
}

WindowControlBackend::WindowControlBackend(QObject *parent)
    : QtIVIWindowControlBackendInterface(parent)
{
    // No Blind in the front
    ZoneBackend frontLeft;
    frontLeft.heaterMode = QtIVIWindowControl::HeaterOff;
    frontLeft.heaterModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>(QVector<QtIVIWindowControl::HeaterMode>({QtIVIWindowControl::HeaterOff, QtIVIWindowControl::HeaterOn}));
    frontLeft.heaterEnabled = false;
    frontLeft.heaterAttribute = QtIVIPropertyAttribute<bool>(true);
    frontLeft.blindMode = QtIVIWindowControl::BlindClosed;
    frontLeft.blindModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>(false);
    frontLeft.blindState = QtIVIWindowControl::Closed;
    frontLeft.blindStateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>(false);
    frontLeft.blindTimer = new WindowTimer("FrontLeft", true, this);
    frontLeft.state = QtIVIWindowControl::Closed;
    frontLeft.stateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>({QtIVIWindowControl::Closed, QtIVIWindowControl::Open, QtIVIWindowControl::FullyOpen});
    frontLeft.stateTimer = new WindowTimer("FrontLeft", false, this);
    m_zoneMap.insert("FrontLeft", frontLeft);

    ZoneBackend frontRight;
    frontRight.heaterMode = QtIVIWindowControl::HeaterOff;
    frontRight.heaterModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>(QVector<QtIVIWindowControl::HeaterMode>({QtIVIWindowControl::HeaterOff, QtIVIWindowControl::HeaterOn}));
    frontRight.heaterEnabled = false;
    frontRight.heaterAttribute = QtIVIPropertyAttribute<bool>(true);
    frontRight.blindMode = QtIVIWindowControl::BlindClosed;
    frontRight.blindModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>(false);
    frontRight.blindState = QtIVIWindowControl::Closed;
    frontRight.blindStateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>(false);
    frontRight.blindTimer = new WindowTimer("FrontRight", true, this);
    frontRight.state = QtIVIWindowControl::Closed;
    frontRight.stateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>({QtIVIWindowControl::Closed, QtIVIWindowControl::Open, QtIVIWindowControl::FullyOpen});
    frontRight.stateTimer = new WindowTimer("FrontRight", false, this);
    m_zoneMap.insert("FrontRight", frontRight);

    // Everything in the rear
    ZoneBackend rearLeft;
    rearLeft.heaterMode = QtIVIWindowControl::HeaterOff;
    rearLeft.heaterModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>(QVector<QtIVIWindowControl::HeaterMode>({QtIVIWindowControl::HeaterOff, QtIVIWindowControl::HeaterOn}));
    rearLeft.heaterEnabled = false;
    rearLeft.heaterAttribute = QtIVIPropertyAttribute<bool>(true);
    rearLeft.blindMode = QtIVIWindowControl::BlindClosed;
    rearLeft.blindModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>(QVector<QtIVIWindowControl::BlindMode>({QtIVIWindowControl::BlindClosed, QtIVIWindowControl::BlindOpen}));
    rearLeft.blindState = QtIVIWindowControl::Closed;
    rearLeft.blindStateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>(false);
    rearLeft.blindTimer = new WindowTimer("RearLeft", true, this);
    rearLeft.state = QtIVIWindowControl::Closed;
    rearLeft.stateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>({QtIVIWindowControl::Closed, QtIVIWindowControl::Open, QtIVIWindowControl::FullyOpen});
    rearLeft.stateTimer = new WindowTimer("RearLeft", false, this);
    m_zoneMap.insert("RearLeft", rearLeft);

    ZoneBackend rearRight;
    rearRight.heaterMode = QtIVIWindowControl::HeaterOff;
    rearRight.heaterModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>(QVector<QtIVIWindowControl::HeaterMode>({QtIVIWindowControl::HeaterOff, QtIVIWindowControl::HeaterOn}));
    rearRight.heaterEnabled = false;
    rearRight.heaterAttribute = QtIVIPropertyAttribute<bool>(true);
    rearRight.blindMode = QtIVIWindowControl::BlindClosed;
    rearRight.blindModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>(QVector<QtIVIWindowControl::BlindMode>({QtIVIWindowControl::BlindClosed, QtIVIWindowControl::BlindOpen}));
    rearRight.blindState = QtIVIWindowControl::Closed;
    rearRight.blindStateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>({QtIVIWindowControl::Closed, QtIVIWindowControl::Open, QtIVIWindowControl::FullyOpen});
    rearRight.blindTimer = new WindowTimer("RearRight", true, this);
    rearRight.state = QtIVIWindowControl::Closed;
    rearRight.stateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>({QtIVIWindowControl::Closed, QtIVIWindowControl::Open, QtIVIWindowControl::FullyOpen});
    rearRight.stateTimer = new WindowTimer("RearRight", false, this);
    m_zoneMap.insert("RearRight", rearRight);

    // Just heating in the back window
    ZoneBackend rear;
    rear.heaterMode = QtIVIWindowControl::HeaterOff;
    rear.heaterModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>(QVector<QtIVIWindowControl::HeaterMode>({QtIVIWindowControl::HeaterOff, QtIVIWindowControl::HeaterOn}));
    rear.heaterEnabled = false;
    rear.heaterAttribute = QtIVIPropertyAttribute<bool>(true);
    rear.blindMode = QtIVIWindowControl::BlindClosed;
    rear.blindModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>(false);
    rear.blindState = QtIVIWindowControl::Closed;
    rear.blindStateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>(false);
    rear.blindTimer = nullptr;
    rear.state = QtIVIWindowControl::Closed;
    rear.stateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>(false);
    rear.stateTimer = nullptr;
    m_zoneMap.insert("Rear", rear);

    // Just no heater in the roof
    ZoneBackend roof;
    roof.heaterMode = QtIVIWindowControl::HeaterOff;
    roof.heaterModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>(false);
    roof.heaterEnabled = false;
    roof.heaterAttribute = QtIVIPropertyAttribute<bool>(true);
    roof.blindMode = QtIVIWindowControl::BlindClosed;
    roof.blindModeAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>(QVector<QtIVIWindowControl::BlindMode>({QtIVIWindowControl::BlindClosed, QtIVIWindowControl::BlindOpen}));
    roof.blindState = QtIVIWindowControl::Closed;
    roof.blindStateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>({QtIVIWindowControl::Closed, QtIVIWindowControl::Open, QtIVIWindowControl::FullyOpen});
    roof.blindTimer = new WindowTimer("Roof", true, this);
    roof.state = QtIVIWindowControl::Closed;
    roof.stateAttribute = QtIVIPropertyAttribute<QtIVIWindowControl::State>({QtIVIWindowControl::Closed, QtIVIWindowControl::Open, QtIVIWindowControl::FullyOpen});
    roof.stateTimer = new WindowTimer("Roof", false, this);
    m_zoneMap.insert("Roof", roof);
}

WindowControlBackend::~WindowControlBackend()
{
}

QStringList WindowControlBackend::availableZones() const
{
    return m_zoneMap.keys();
}

void WindowControlBackend::initializeAttributes()
{
    foreach (QString zone, availableZones()) {
        emit heaterModeChanged(m_zoneMap[zone].heaterMode, zone);
        emit heaterModeAttributeChanged(m_zoneMap[zone].heaterModeAttribute, zone);
        emit heaterEnabledChanged(m_zoneMap[zone].heaterEnabled, zone);
        emit heaterAttributeChanged(m_zoneMap[zone].heaterAttribute, zone);
        emit blindModeChanged(m_zoneMap[zone].blindMode, zone);
        emit blindModeAttributeChanged(m_zoneMap[zone].blindModeAttribute, zone);
        emit blindStateChanged(m_zoneMap[zone].blindState, zone);
        emit blindStateAttributeChanged(m_zoneMap[zone].blindStateAttribute, zone);
        emit stateChanged(m_zoneMap[zone].state, zone);
        emit stateAttributeChanged(m_zoneMap[zone].stateAttribute, zone);
    }
}

void WindowControlBackend::setHeaterMode(QtIVIWindowControl::HeaterMode value, const QString &zone)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].heaterMode == value)
        return;

    if (!m_zoneMap[zone].heaterModeAttribute.availableValues().contains(value)) {
        qWarning() << "SIMULATION HeaterMode" << zone << "is not supported.";
        return;
    }

    qWarning() << "SIMULATION HeaterMode for Zone" << zone << "changed to" << value;

    m_zoneMap[zone].heaterMode = value;
    emit heaterModeChanged(value, zone);

    m_zoneMap[zone].heaterEnabled = (value == QtIVIWindowControl::HeaterOn);
    emit heaterEnabledChanged(m_zoneMap[zone].heaterEnabled, zone);
}

void WindowControlBackend::setBlindMode(QtIVIWindowControl::BlindMode value, const QString &zone)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].blindMode == value)
        return;

    if (!m_zoneMap[zone].blindModeAttribute.availableValues().contains(value)) {
        qWarning() << "SIMULATION HeaterMode" << zone << "is not supported.";
        return;
    }

    qWarning() << "SIMULATION BlindMode for Zone" << zone << "changed to" << value;

    if (value == QtIVIWindowControl::BlindOpen)
        m_zoneMap[zone].blindTimer->open();
    else if (value == QtIVIWindowControl::BlindClosed)
        m_zoneMap[zone].blindTimer->close();
    m_zoneMap[zone].blindMode = value;
    emit blindModeChanged(value, zone);
}

void WindowControlBackend::open(const QString &zone)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].state == QtIVIWindowControl::Open)
        return;

    if (!m_zoneMap[zone].stateAttribute.isAvailable()) {
        qWarning() << "SIMULATION opening Window" << zone << "is not possible.";
        return;
    }

    qWarning() << "SIMULATION open Window:" << zone;
    m_zoneMap[zone].stateTimer->open();
}

void WindowControlBackend::close(const QString &zone)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].state == QtIVIWindowControl::Closed)
        return;

    if (!m_zoneMap[zone].stateAttribute.isAvailable()) {
        qWarning() << "SIMULATION closing Window" << zone << "is not possible.";
        return;
    }

    qWarning() << "SIMULATION close Window:" << zone;
    m_zoneMap[zone].stateTimer->close();
}
