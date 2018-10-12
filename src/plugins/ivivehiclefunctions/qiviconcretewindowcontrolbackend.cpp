/****************************************************************************
**
** Copyright (C) 2018 Pelagicore AG
** Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
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

#include "qiviconcretewindowcontrolbackend.h"

#include <QDebug>

QT_BEGIN_NAMESPACE

WindowTimer::WindowTimer(const QString &zone, bool isBlind, QIviConcreteWindowControlBackend *backend)
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

void WindowTimer::open(QIviPendingReply<void> reply)
{
    if (!m_pendingReply.isResultAvailable())
        m_pendingReply.setFailed();

    m_pendingReply = reply;

    m_opening = true;
    m_timer->start();
}

void WindowTimer::close(QIviPendingReply<void> reply)
{
    if (!m_pendingReply.isResultAvailable())
        m_pendingReply.setFailed();

    m_pendingReply = reply;

    m_opening = false;
    m_timer->start();
}

void WindowTimer::checkValue()
{
    if (m_opening)
        m_currentValue++;
    else
        m_currentValue--;

    QtIviVehicleFunctionsModule::WindowState state = QtIviVehicleFunctionsModule::Closed;
    if (m_currentValue <= 0) {
        m_currentValue = 0;
        m_timer->stop();
        state = QtIviVehicleFunctionsModule::Closed;
        m_pendingReply.setSuccess();
    } else if (m_currentValue >= m_interval) {
        m_currentValue = m_interval;
        m_timer->stop();
        state = QtIviVehicleFunctionsModule::FullyOpen;
        m_pendingReply.setSuccess();
    } else {
        state = QtIviVehicleFunctionsModule::Open;
    }

    if (m_blind) {
        m_backend->setBlindState(state, m_zone);
    } else {
        m_backend->setWindowState(state, m_zone);
    }
}


QIviConcreteWindowControlBackend::QIviConcreteWindowControlBackend(QObject *parent)
    : QIviWindowControlBackend(parent)
{
    const auto zones = availableZones();
    for (const QString &zone : zones) {
        m_zoneWindowTimers[zone] = new WindowTimer(zone, false, this);
        m_zoneBlindTimers[zone] = new WindowTimer(zone, true, this);
    }
}

QIviConcreteWindowControlBackend::~QIviConcreteWindowControlBackend()
{
}

void QIviConcreteWindowControlBackend::setBlindMode(QtIviVehicleFunctionsModule::BlindMode blindMode, const QString &zone)
{
    if (!availableZones().contains(zone))
        return;
    if (zoneAt(zone)->blindMode() == blindMode)
        return;

    if (blindMode == QtIviVehicleFunctionsModule::BlindOpen)
        m_zoneBlindTimers[zone]->open(QIviPendingReply<void>());
    else if (blindMode == QtIviVehicleFunctionsModule::BlindClosed)
        m_zoneBlindTimers[zone]->close(QIviPendingReply<void>());

    QIviWindowControlBackend::setBlindMode(blindMode, zone);
}

QIviPendingReply<void> QIviConcreteWindowControlBackend::open(const QString &zone)
{
    if (!availableZones().contains(zone))
        return QIviPendingReply<void>::createFailedReply();

    if (zoneAt(zone)->state() == QtIviVehicleFunctionsModule::Open)
        return QIviPendingReply<void>::createFailedReply();

    qWarning() << "SIMULATION open Window:" << zone;
    QIviPendingReply<void> reply;
    m_zoneWindowTimers[zone]->open(reply);

    return reply;
}

QIviPendingReply<void> QIviConcreteWindowControlBackend::close(const QString &zone)
{
    if (!availableZones().contains(zone))
        return QIviPendingReply<void>::createFailedReply();

    if (zoneAt(zone)->state() == QtIviVehicleFunctionsModule::Closed)
        return QIviPendingReply<void>::createFailedReply();

    qWarning() << "SIMULATION close Window:" << zone;
    QIviPendingReply<void> reply;
    m_zoneWindowTimers[zone]->close(reply);

    return reply;
}

QtIviVehicleFunctionsModule::WindowState QIviConcreteWindowControlBackend::windowState(QString zone)
{
    Q_ASSERT(availableZones().contains(zone));
    return zoneAt(zone)->state();
}

void QIviConcreteWindowControlBackend::setWindowState(QtIviVehicleFunctionsModule::WindowState state, const QString &zone)
{
    if (zoneAt(zone)->state() == state)
        return;
    qWarning() << "SIMULATION window state for Zone" << zone << "changed to" << state;
    zoneAt(zone)->setState(state);
    emit stateChanged(state, zone);
}

QtIviVehicleFunctionsModule::WindowState QIviConcreteWindowControlBackend::blindState(QString zone)
{
    Q_ASSERT(availableZones().contains(zone));
    return zoneAt(zone)->blindState();
}

void QIviConcreteWindowControlBackend::setBlindState(QtIviVehicleFunctionsModule::WindowState state, const QString &zone)
{
    if (zoneAt(zone)->blindState() == state)
        return;
    qWarning() << "SIMULATION blind state for Zone" << zone << "changed to" << state;
    zoneAt(zone)->setBlindState(state);
    emit blindStateChanged(state, zone);
}


QT_END_NAMESPACE
