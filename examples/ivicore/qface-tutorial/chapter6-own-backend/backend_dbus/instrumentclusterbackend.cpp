/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "instrumentclusterbackend.h"

#include <QDBusConnection>

InstrumentClusterBackend::InstrumentClusterBackend(QObject *parent)
    : InstrumentClusterBackendInterface(parent)
    , m_client(nullptr)
{
    qDBusRegisterMetaType<Warning>();
    qDBusRegisterMetaType<InstrumentClusterModule::SystemType>();
}

void InstrumentClusterBackend::initialize()
{
    if (!m_client)
        setupConnection();
    if (m_fetchList.isEmpty()) {
        emit speedChanged(m_speed);
        emit rpmChanged(m_rpm);
        emit fuelChanged(m_fuel);
        emit temperatureChanged(m_temperature);
        emit systemTypeChanged(m_systemType);
        emit currentWarningChanged(m_warning);
        emit initializationDone();
    }
}

void InstrumentClusterBackend::setupConnection()
{
    qInfo() << "Connecting to the Server";
    m_client = new ExampleIVIInstrumentClusterInterface("Example.IVI.InstrumentCluster", "/", QDBusConnection::sessionBus());
    connect(m_client, &ExampleIVIInstrumentClusterInterface::speedChanged,
            this, &InstrumentClusterBackend::onSpeedChanged);
    connect(m_client, &ExampleIVIInstrumentClusterInterface::rpmChanged,
            this, &InstrumentClusterBackend::onRpmChanged);
    connect(m_client, &ExampleIVIInstrumentClusterInterface::fuelChanged,
            this, &InstrumentClusterBackend::onFuelChanged);
    connect(m_client, &ExampleIVIInstrumentClusterInterface::temperatureChanged,
            this, &InstrumentClusterBackend::onTemperaturChanged);
    connect(m_client, &ExampleIVIInstrumentClusterInterface::systemTypeChanged,
            this, &InstrumentClusterBackend::onSystemTypeChanged);
    connect(m_client, &ExampleIVIInstrumentClusterInterface::currentWarningChanged,
            this, &InstrumentClusterBackend::onCurrentWarningChanged);
    fetchSpeed();
    fetchRpm();
    fetchFuel();
    fetchTemperature();
    fetchSystemType();
    fetchCurrentWarning();
}

void InstrumentClusterBackend::fetchSpeed()
{
    m_fetchList.append("speed");
    auto reply = m_client->asyncCall("speed");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<int> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("speed");
            this->onSpeedChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::fetchRpm()
{
    m_fetchList.append("rpm");
    auto reply = m_client->asyncCall("rpm");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<int> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("rpm");
            this->onRpmChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::fetchFuel()
{
    m_fetchList.append("fuel");
    auto reply = m_client->asyncCall("fuel");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<qreal> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("fuel");
            this->onFuelChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::fetchTemperature()
{
    m_fetchList.append("temperature");
    auto reply = m_client->asyncCall("temperature");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<qreal> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("temperature");
            this->temperatureChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::fetchSystemType()
{
    m_fetchList.append("systemType");
    auto reply = m_client->asyncCall("systemType");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<InstrumentClusterModule::SystemType> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("systemType");
            this->onSystemTypeChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::fetchCurrentWarning()
{
    m_fetchList.append("currentWarning");
    auto reply = m_client->asyncCall("currentWarning");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<Warning> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("currentWarning");
            this->onCurrentWarningChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::checkInitDone()
{
    if (m_fetchList.isEmpty()) {
        qInfo() << "All properties initialized";
        emit initializationDone();
    }
}

void InstrumentClusterBackend::onSpeedChanged(int speed)
{
    m_speed = speed;
    emit speedChanged(speed);
}

void InstrumentClusterBackend::onRpmChanged(int rpm)
{
    m_rpm = rpm;
    emit rpmChanged(rpm);
}

void InstrumentClusterBackend::onFuelChanged(qreal fuel)
{
    m_fuel = fuel;
    emit fuelChanged(fuel);
}

void InstrumentClusterBackend::onTemperaturChanged(qreal temperature)
{
    m_temperature = temperature;
    emit temperatureChanged(temperature);
}

void InstrumentClusterBackend::onSystemTypeChanged(InstrumentClusterModule::SystemType systemType)
{
    m_systemType = systemType;
    emit systemTypeChanged(systemType);
}

void InstrumentClusterBackend::onCurrentWarningChanged(const Warning &warning)
{
    m_warning = warning;
    emit currentWarningChanged(warning);
}
