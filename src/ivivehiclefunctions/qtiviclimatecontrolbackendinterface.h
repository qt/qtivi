/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.pelagicore.com/
**
** This file is part of Qt IVI.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions see http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QTIVICLIMATECONTROLBACKENDINTERFACE_H
#define QTIVICLIMATECONTROLBACKENDINTERFACE_H

#include <QObject>

#include "qtiviclimatecontrol.h"

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIClimateControlBackendInterface : public QObject
{
    Q_OBJECT

public:
    QtIVIClimateControlBackendInterface(QObject *parent=0) : QObject(parent) {}

    static QString interfaceName;

    virtual QStringList zones() const = 0;

    virtual QtIVIClimateControl::OptionalFeatures climateFeatures() const = 0;
    virtual QtIVIClimateZone::OptionalFeatures zoneFeatures(const QString &zone) const = 0;

    virtual void setTargetTemperature(const QString &zone, int) = 0;
    virtual void setSeatCooler(const QString &zone, int) = 0;
    virtual void setSeatHeater(const QString &zone, int) = 0;
    virtual void setSteeringWheelHeater(const QString &zone, int) = 0;
    virtual void setFanSpeedLevel(const QString &zone, int) = 0;

    virtual void setAirflowDirection(QtIVIClimateControl::AirflowDirection) = 0;
    virtual void setAirConditioningEnabled(bool) = 0;
    virtual void setHeaterEnabled(bool) = 0;
    virtual void setAirRecirculationEnabled(bool) = 0;

    virtual int targetTemperature(const QString &zone) const = 0;
    virtual int seatCooler(const QString &zone) const = 0;
    virtual int seatHeater(const QString &zone) const = 0;
    virtual int steeringWheelHeater(const QString &zone) const = 0;
    virtual int fanSpeedLevel(const QString &zone) const = 0;

    virtual QtIVIClimateControl::AirflowDirection airflowDirection() const = 0;
    virtual bool airConditioningEnabled() const = 0;
    virtual bool heaterEnabled() const = 0;
    virtual bool airRecirculationEnabled() const = 0;

signals:
    void targetTemperatureChanged(const QString &zone, int);
    void seatCoolerChanged(const QString &zone, int);
    void seatHeaterChanged(const QString &zone, int);
    void steeringWheelHeaterChanged(const QString &zone, int);
    void fanSpeedLevelChanged(const QString &zone, int);

    void airflowDirectionChanged(QtIVIClimateControl::AirflowDirection);
    void airConditioningEnabledChanged(bool);
    void heaterEnabledChanged(bool);
    void airRecirculationEnabledChanged(bool);
};

#endif // QTIVICLIMATECONTROLBACKENDINTERFACE_H

