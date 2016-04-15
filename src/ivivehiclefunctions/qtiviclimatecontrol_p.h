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

#ifndef QTIVICLIMATECONTROL_P_H
#define QTIVICLIMATECONTROL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtIVICore/QtIVIProperty>
#include "private/qtiviabstractzonedfeature_p.h"

#include "qtiviclimatecontrol.h"

QT_BEGIN_NAMESPACE

class QtIVIClimateControlPrivate : public QtIVIAbstractZonedFeaturePrivate
{
public:
    QtIVIClimateControlPrivate(const QString &interface, const QString &zone, QtIVIClimateControl *parent);

    void init();
    void clearToDefaults();

    void onAirflowDirectionsChanged(QtIVIClimateControl::AirflowDirections value, const QString &zone);
    void onAirflowDirectionsAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> &airflowDirectionsAttribute, const QString &zone);
    void onAirConditioningEnabledChanged(bool enabled, const QString &zone);
    void onAirConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> &airConditioningEnabledAttribute, const QString &zone);
    void onHeaterEnabledChanged(bool enabled, const QString &zone);
    void onHeaterAttributeChanged(const QtIVIPropertyAttribute<bool> &heaterEnabledAttribute, const QString &zone);
    void onAirRecirculationEnabledChanged(bool enabled, const QString &zone);
    void onAirRecirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &airRecirculationEnabledAttribute, const QString &zone);
    void onSteeringWheelHeaterChanged(int value, const QString &zone);
    void onSteeringWheelHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &steeringWheelHeaterAttribute, const QString &zone);
    void onFanSpeedLevelChanged(int value, const QString &zone);
    void onFanSpeedLevelAttributeChanged(const QtIVIPropertyAttribute<int> &fanSpeedAttribute, const QString &zone);
    void onTargetTemperatureChanged(int temperature, const QString &zone);
    void onTargetTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &temperatureAttribute, const QString &zone);
    void onSeatCoolerChanged(int value, const QString &zone);
    void onSeatCoolerAttributeChanged(const QtIVIPropertyAttribute<int> &seatCoolerAttribute, const QString &zone);
    void onSeatHeaterChanged(int value, const QString &zone);
    void onSeatHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &seatHeaterAttribute, const QString &zone);

    QtIVIClimateControlBackendInterface* climateControlBackend();

    QtIVIClimateControl::AirflowDirections m_airflowDirections;
    QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> m_airflowDirectionsAttribute;
    QtIVIProperty *m_airFlowDirectionProperty;
    bool m_airConditioning;
    QtIVIPropertyAttribute<bool> m_airConditioningAttribute;
    QtIVIProperty *m_airConditioningProperty;
    bool m_heater;
    QtIVIPropertyAttribute<bool> m_heaterAttribute;
    QtIVIProperty *m_heaterProperty;
    bool m_airRecirculation;
    QtIVIPropertyAttribute<bool> m_airRecirculationAttribute;
    QtIVIProperty *m_airRecirculationProperty;
    int m_targetTemperature;
    QtIVIPropertyAttribute<int> m_targetTemperatureAttribute;
    QtIVIProperty *m_targetTemperatureProperty;
    int m_seatCooler;
    QtIVIPropertyAttribute<int> m_seatCoolerAttribute;
    QtIVIProperty *m_seatCoolerProperty;
    int m_seatHeater;
    QtIVIPropertyAttribute<int> m_seatHeaterAttribute;
    QtIVIProperty *m_seatHeaterProperty;
    int m_steeringWheelHeater;
    QtIVIPropertyAttribute<int> m_steeringWheelHeaterAttribute;
    QtIVIProperty *m_steeringWheelHeaterProperty;
    int m_fanSpeedLevel;
    QtIVIPropertyAttribute<int> m_fanSpeedLevelAttribute;
    QtIVIProperty *m_fanSpeedLevelProperty;

    QtIVIClimateControl * const q_ptr;
    Q_DECLARE_PUBLIC(QtIVIClimateControl)
};

QT_END_NAMESPACE

#endif // QTIVICLIMATECONTROL_P_H
