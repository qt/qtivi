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

#include "instrumentcluster.h"
#include <QThread>

InstrumentCluster::InstrumentCluster(QObject *parent)
    : QObject(parent)
    , m_speed(0)
    , m_rpm(0)
    , m_fuel(0)
    , m_temperature(15)
    , m_systemType(InstrumentClusterModule::Metric)
{
    startTimer(100);
}

int InstrumentCluster::speed() const
{
    //QThread::sleep(5);
    return m_speed;
}

int InstrumentCluster::rpm() const
{
    return m_rpm;
}

qreal InstrumentCluster::fuel() const
{
    return m_fuel;
}

qreal InstrumentCluster::temperature() const
{
    return m_temperature;
}

InstrumentClusterModule::SystemType InstrumentCluster::systemType() const
{
    return m_systemType;
}

Warning InstrumentCluster::currentWarning() const
{
    return m_currentWarning;
}

void InstrumentCluster::setSpeed(int speed)
{
    if (m_speed == speed)
        return;

    m_speed = speed;
    emit speedChanged(m_speed);
}

void InstrumentCluster::setRpm(int rpm)
{
    if (m_rpm == rpm)
        return;

    m_rpm = rpm;
    emit rpmChanged(m_rpm);
}

void InstrumentCluster::setFuel(qreal fuel)
{
    if (qFuzzyCompare(m_fuel, fuel))
        return;

    m_fuel = fuel;
    emit fuelChanged(m_fuel);
}

void InstrumentCluster::setTemperature(qreal temperature)
{
    if (qFuzzyCompare(m_temperature, temperature))
        return;

    m_temperature = temperature;
    emit temperatureChanged(m_temperature);
}

void InstrumentCluster::setSystemType(InstrumentClusterModule::SystemType systemType)
{
    if (m_systemType == systemType)
        return;

    m_systemType = systemType;
    emit systemTypeChanged(m_systemType);
}

void InstrumentCluster::setCurrentWarning(const Warning &currentWarning)
{
    if (m_currentWarning == currentWarning)
        return;

    m_currentWarning = currentWarning;
    emit currentWarningChanged(m_currentWarning);
}

void InstrumentCluster::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (speed() >= 250)
        setSpeed(0);
    else
        setSpeed(speed() + 1);

    if (rpm() >= 5000)
        setRpm(0);
    else
        setRpm(rpm() + 100);
}
