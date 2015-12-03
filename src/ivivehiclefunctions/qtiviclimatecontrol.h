/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
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
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#ifndef CLIMATECONTROL_H
#define CLIMATECONTROL_H

#include <QtIVIVehicleFunctions/qtiviabstractzonedfeature.h>

class QtIVIClimateControlBackendInterface;

static const QLatin1String QtIVIStringClimateControlInterfaceName("com.qt-project.ClimateControl");

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIClimateControl : public QtIVIAbstractZonedFeature
{
    Q_OBJECT
    Q_PROPERTY(bool airConditioning READ isAirConditioningEnabled  WRITE setAirConditioningEnabled  NOTIFY airConditioningEnabledChanged)
    Q_PROPERTY(AirflowDirection airflowDirection READ airflowDirection WRITE setAirflowDirection NOTIFY airflowDirectionChanged)
    Q_PROPERTY(bool heater READ isHeaterEnabled WRITE setHeaterEnabled NOTIFY heaterEnabledChanged)
    Q_PROPERTY(bool airRecirculation READ isAirRecirculationEnabled WRITE setAirRecirculationEnabled NOTIFY airRecirculationEnabledChanged)
    Q_PROPERTY(int fanSpeedLevel READ fanSpeedLevel WRITE setFanSpeedLevel NOTIFY fanSpeedLevelChanged)
    Q_PROPERTY(int steeringWheelHeater READ steeringWheelHeater WRITE setSteeringWheelHeater NOTIFY steeringWheelHeaterChanged)
    Q_PROPERTY(int targetTemperature READ targetTemperature WRITE setTargetTemperature NOTIFY targetTemperatureChanged)
    Q_PROPERTY(int seatCooler READ seatCooler WRITE setSeatCooler NOTIFY seatCoolerChanged)
    Q_PROPERTY(int seatHeater READ seatHeater WRITE setSeatHeater NOTIFY seatHeaterChanged)

    Q_ENUMS(AirflowDirection)

public:
    enum AirflowDirection {
        None,
        FloorPanel,
        FloorDuct,
        BiLevel,
        DefrostFloor
    };

    QtIVIClimateControl(QObject *parent=0);

    QtIVIClimateControl(const QString &zone, bool autoDiscovery = true, QObject* parent=0);

    virtual void connectToServiceObject(QtIVIServiceObject *serviceObject) Q_DECL_OVERRIDE;
    virtual void clearServiceObject() Q_DECL_OVERRIDE;

    bool isAirConditioningEnabled() const;
    QtIVIClimateControl::AirflowDirection airflowDirection() const;
    bool isHeaterEnabled() const;
    bool isAirRecirculationEnabled() const;
    int steeringWheelHeater() const;
    int fanSpeedLevel() const;
    int targetTemperature() const;
    int seatCooler() const;
    int seatHeater() const;

public Q_SLOTS:
    void setAirConditioningEnabled(bool enabled);
    void setAirflowDirection(QtIVIClimateControl::AirflowDirection value);
    void setHeaterEnabled(bool enabled);
    void setAirRecirculationEnabled(bool enabled);
    void setSteeringWheelHeater(int value);
    void setFanSpeedLevel(int value);
    void setTargetTemperature(int targetTemperature);
    void setSeatCooler(int seatCooler);
    void setSeatHeater(int seatHeater);

Q_SIGNALS:
    void airflowDirectionChanged(QtIVIClimateControl::AirflowDirection value);
    void airConditioningEnabledChanged(bool enabled);
    void heaterEnabledChanged(bool enabled);
    void airRecirculationEnabledChanged(bool enabled);
    void steeringWheelHeaterChanged(int value);
    void fanSpeedLevelChanged(int value);
    void targetTemperatureChanged(int temperature);
    void seatCoolerChanged(int value);
    void seatHeaterChanged(int value);

private Q_SLOTS:
    void onAirflowDirectionChanged(QtIVIClimateControl::AirflowDirection value, const QString &zone);
    void onAirConditioningEnabledChanged(bool enabled, const QString &zone);
    void onHeaterEnabledChanged(bool enabled, const QString &zone);
    void onAirRecirculationEnabledChanged(bool enabled, const QString &zone);
    void onSteeringWheelHeaterChanged(int value, const QString &zone);
    void onFanSpeedLevelChanged(int value, const QString &zone);
    void onTargetTemperatureChanged(int temperature, const QString &zone);
    void onSeatCoolerChanged(int value, const QString &zone);
    void onSeatHeaterChanged(int value, const QString &zone);

protected:
    virtual QtIVIAbstractZonedFeature* createZoneFeature(const QString &zone) Q_DECL_OVERRIDE;

private:
    QtIVIClimateControlBackendInterface* climateControlBackend();

private:
    AirflowDirection m_airflowDirection;
    bool m_airConditioning;
    bool m_heater;
    bool m_airRecirculation;
    int m_targetTemperature;
    int m_seatCooler;
    int m_seatHeater;
    int m_steeringWheelHeater;
    int m_fanSpeedLevel;
};

#endif // CLIMATECONTROL_H
