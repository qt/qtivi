
/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
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
** SPDX-License-Identifier: BSD-3-Clause
**
****************************************************************************/

#ifndef INSTRUMENTCLUSTER_H
#define INSTRUMENTCLUSTER_H

#include <QObject>
#include "warning.h"
#include "instrumentclustermodule.h"

class InstrumentCluster : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int rpm READ rpm WRITE setRpm NOTIFY rpmChanged)
    Q_PROPERTY(qreal fuel READ fuel WRITE setFuel NOTIFY fuelChanged)
    Q_PROPERTY(qreal temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(InstrumentClusterModule::SystemType systemType READ systemType WRITE setSystemType NOTIFY systemTypeChanged)
    Q_PROPERTY(Warning currentWarning READ currentWarning WRITE setCurrentWarning NOTIFY currentWarningChanged)

public:
    explicit InstrumentCluster(QObject *parent = nullptr);

    Q_INVOKABLE int speed() const;
    Q_INVOKABLE int rpm() const;
    Q_INVOKABLE qreal fuel() const;
    Q_INVOKABLE qreal temperature() const;
    Q_INVOKABLE InstrumentClusterModule::SystemType systemType() const;
    Q_INVOKABLE Warning currentWarning() const;

signals:
    void speedChanged(int speed);
    void rpmChanged(int rpm);
    void fuelChanged(qreal fuel);
    void temperatureChanged(qreal temperature);
    void systemTypeChanged(InstrumentClusterModule::SystemType systemType);
    void currentWarningChanged(Warning currentWarning);

public slots:
    void setSpeed(int speed);
    void setRpm(int rpm);
    void setFuel(qreal fuel);
    void setTemperature(qreal temperature);
    void setSystemType(InstrumentClusterModule::SystemType systemType);
    void setCurrentWarning(const Warning &currentWarning);

protected:
    virtual void timerEvent(QTimerEvent *event) override;

private:
    int m_speed;
    int m_rpm;
    qreal m_fuel;
    qreal m_temperature;
    InstrumentClusterModule::SystemType m_systemType;
    Warning m_currentWarning;
};

#endif // INSTRUMENTCLUSTER_H
