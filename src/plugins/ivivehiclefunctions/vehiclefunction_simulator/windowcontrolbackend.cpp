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

    QIviWindowControl::State state = QIviWindowControl::Closed;
    if (m_currentValue <= 0) {
        m_currentValue = 0;
        m_timer->stop();
        state = QIviWindowControl::Closed;
    } else if (m_currentValue >= m_interval) {
        m_currentValue = m_interval;
        m_timer->stop();
        state = QIviWindowControl::FullyOpen;
    } else {
        state = QIviWindowControl::Open;
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
    : QIviWindowControlBackendInterface(parent)
{
    // No Blind in the front
    ZoneBackend frontLeft;
    frontLeft.heaterMode = QIviWindowControl::HeaterOff;
    frontLeft.heaterModeAttribute = QIviPropertyAttribute<QIviWindowControl::HeaterMode>(QVector<QIviWindowControl::HeaterMode>({QIviWindowControl::HeaterOff, QIviWindowControl::HeaterOn}));
    frontLeft.heaterEnabled = false;
    frontLeft.heaterAttribute = QIviPropertyAttribute<bool>(true);
    frontLeft.blindMode = QIviWindowControl::BlindClosed;
    frontLeft.blindModeAttribute = QIviPropertyAttribute<QIviWindowControl::BlindMode>(false);
    frontLeft.blindState = QIviWindowControl::Closed;
    frontLeft.blindStateAttribute = QIviPropertyAttribute<QIviWindowControl::State>(false);
    frontLeft.blindTimer = new WindowTimer("FrontLeft", true, this);
    frontLeft.state = QIviWindowControl::Closed;
    frontLeft.stateAttribute = QIviPropertyAttribute<QIviWindowControl::State>({QIviWindowControl::Closed, QIviWindowControl::Open, QIviWindowControl::FullyOpen});
    frontLeft.stateTimer = new WindowTimer("FrontLeft", false, this);
    m_zoneMap.insert("FrontLeft", frontLeft);

    ZoneBackend frontRight;
    frontRight.heaterMode = QIviWindowControl::HeaterOff;
    frontRight.heaterModeAttribute = QIviPropertyAttribute<QIviWindowControl::HeaterMode>(QVector<QIviWindowControl::HeaterMode>({QIviWindowControl::HeaterOff, QIviWindowControl::HeaterOn}));
    frontRight.heaterEnabled = false;
    frontRight.heaterAttribute = QIviPropertyAttribute<bool>(true);
    frontRight.blindMode = QIviWindowControl::BlindClosed;
    frontRight.blindModeAttribute = QIviPropertyAttribute<QIviWindowControl::BlindMode>(false);
    frontRight.blindState = QIviWindowControl::Closed;
    frontRight.blindStateAttribute = QIviPropertyAttribute<QIviWindowControl::State>(false);
    frontRight.blindTimer = new WindowTimer("FrontRight", true, this);
    frontRight.state = QIviWindowControl::Closed;
    frontRight.stateAttribute = QIviPropertyAttribute<QIviWindowControl::State>({QIviWindowControl::Closed, QIviWindowControl::Open, QIviWindowControl::FullyOpen});
    frontRight.stateTimer = new WindowTimer("FrontRight", false, this);
    m_zoneMap.insert("FrontRight", frontRight);

    // Everything in the rear
    ZoneBackend rearLeft;
    rearLeft.heaterMode = QIviWindowControl::HeaterOff;
    rearLeft.heaterModeAttribute = QIviPropertyAttribute<QIviWindowControl::HeaterMode>(QVector<QIviWindowControl::HeaterMode>({QIviWindowControl::HeaterOff, QIviWindowControl::HeaterOn}));
    rearLeft.heaterEnabled = false;
    rearLeft.heaterAttribute = QIviPropertyAttribute<bool>(true);
    rearLeft.blindMode = QIviWindowControl::BlindClosed;
    rearLeft.blindModeAttribute = QIviPropertyAttribute<QIviWindowControl::BlindMode>(QVector<QIviWindowControl::BlindMode>({QIviWindowControl::BlindClosed, QIviWindowControl::BlindOpen}));
    rearLeft.blindState = QIviWindowControl::Closed;
    rearLeft.blindStateAttribute = QIviPropertyAttribute<QIviWindowControl::State>(false);
    rearLeft.blindTimer = new WindowTimer("RearLeft", true, this);
    rearLeft.state = QIviWindowControl::Closed;
    rearLeft.stateAttribute = QIviPropertyAttribute<QIviWindowControl::State>({QIviWindowControl::Closed, QIviWindowControl::Open, QIviWindowControl::FullyOpen});
    rearLeft.stateTimer = new WindowTimer("RearLeft", false, this);
    m_zoneMap.insert("RearLeft", rearLeft);

    ZoneBackend rearRight;
    rearRight.heaterMode = QIviWindowControl::HeaterOff;
    rearRight.heaterModeAttribute = QIviPropertyAttribute<QIviWindowControl::HeaterMode>(QVector<QIviWindowControl::HeaterMode>({QIviWindowControl::HeaterOff, QIviWindowControl::HeaterOn}));
    rearRight.heaterEnabled = false;
    rearRight.heaterAttribute = QIviPropertyAttribute<bool>(true);
    rearRight.blindMode = QIviWindowControl::BlindClosed;
    rearRight.blindModeAttribute = QIviPropertyAttribute<QIviWindowControl::BlindMode>(QVector<QIviWindowControl::BlindMode>({QIviWindowControl::BlindClosed, QIviWindowControl::BlindOpen}));
    rearRight.blindState = QIviWindowControl::Closed;
    rearRight.blindStateAttribute = QIviPropertyAttribute<QIviWindowControl::State>({QIviWindowControl::Closed, QIviWindowControl::Open, QIviWindowControl::FullyOpen});
    rearRight.blindTimer = new WindowTimer("RearRight", true, this);
    rearRight.state = QIviWindowControl::Closed;
    rearRight.stateAttribute = QIviPropertyAttribute<QIviWindowControl::State>({QIviWindowControl::Closed, QIviWindowControl::Open, QIviWindowControl::FullyOpen});
    rearRight.stateTimer = new WindowTimer("RearRight", false, this);
    m_zoneMap.insert("RearRight", rearRight);

    // Just heating in the back window
    ZoneBackend rear;
    rear.heaterMode = QIviWindowControl::HeaterOff;
    rear.heaterModeAttribute = QIviPropertyAttribute<QIviWindowControl::HeaterMode>(QVector<QIviWindowControl::HeaterMode>({QIviWindowControl::HeaterOff, QIviWindowControl::HeaterOn}));
    rear.heaterEnabled = false;
    rear.heaterAttribute = QIviPropertyAttribute<bool>(true);
    rear.blindMode = QIviWindowControl::BlindClosed;
    rear.blindModeAttribute = QIviPropertyAttribute<QIviWindowControl::BlindMode>(false);
    rear.blindState = QIviWindowControl::Closed;
    rear.blindStateAttribute = QIviPropertyAttribute<QIviWindowControl::State>(false);
    rear.blindTimer = nullptr;
    rear.state = QIviWindowControl::Closed;
    rear.stateAttribute = QIviPropertyAttribute<QIviWindowControl::State>(false);
    rear.stateTimer = nullptr;
    m_zoneMap.insert("Rear", rear);

    // Just no heater in the roof
    ZoneBackend roof;
    roof.heaterMode = QIviWindowControl::HeaterOff;
    roof.heaterModeAttribute = QIviPropertyAttribute<QIviWindowControl::HeaterMode>(false);
    roof.heaterEnabled = false;
    roof.heaterAttribute = QIviPropertyAttribute<bool>(true);
    roof.blindMode = QIviWindowControl::BlindClosed;
    roof.blindModeAttribute = QIviPropertyAttribute<QIviWindowControl::BlindMode>(QVector<QIviWindowControl::BlindMode>({QIviWindowControl::BlindClosed, QIviWindowControl::BlindOpen}));
    roof.blindState = QIviWindowControl::Closed;
    roof.blindStateAttribute = QIviPropertyAttribute<QIviWindowControl::State>({QIviWindowControl::Closed, QIviWindowControl::Open, QIviWindowControl::FullyOpen});
    roof.blindTimer = new WindowTimer("Roof", true, this);
    roof.state = QIviWindowControl::Closed;
    roof.stateAttribute = QIviPropertyAttribute<QIviWindowControl::State>({QIviWindowControl::Closed, QIviWindowControl::Open, QIviWindowControl::FullyOpen});
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
    const auto zones = availableZones();
    for (const QString &zone : zones) {
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

void WindowControlBackend::setHeaterMode(QIviWindowControl::HeaterMode value, const QString &zone)
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

    m_zoneMap[zone].heaterEnabled = (value == QIviWindowControl::HeaterOn);
    emit heaterEnabledChanged(m_zoneMap[zone].heaterEnabled, zone);
}

void WindowControlBackend::setBlindMode(QIviWindowControl::BlindMode value, const QString &zone)
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

    if (value == QIviWindowControl::BlindOpen)
        m_zoneMap[zone].blindTimer->open();
    else if (value == QIviWindowControl::BlindClosed)
        m_zoneMap[zone].blindTimer->close();
    m_zoneMap[zone].blindMode = value;
    emit blindModeChanged(value, zone);
}

void WindowControlBackend::open(const QString &zone)
{
    if (!m_zoneMap.contains(zone))
        return;

    if (m_zoneMap[zone].state == QIviWindowControl::Open)
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

    if (m_zoneMap[zone].state == QIviWindowControl::Closed)
        return;

    if (!m_zoneMap[zone].stateAttribute.isAvailable()) {
        qWarning() << "SIMULATION closing Window" << zone << "is not possible.";
        return;
    }

    qWarning() << "SIMULATION close Window:" << zone;
    m_zoneMap[zone].stateTimer->close();
}
