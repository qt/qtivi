/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#include <QtCore/QObject>
#include <QtIVICore/QtIVIProperty>

#include "qtiviclimatecontrol.h"

QT_BEGIN_NAMESPACE

class QtIVIClimateControlPrivate : public QObject
{
public:
    QtIVIClimateControlPrivate(QtIVIClimateControl *parent);

    void clearToDefaults();

    QtIVIClimateControl::AirflowDirection m_airflowDirection;
    QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection> m_airflowDirectionAttribute;
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
};

QT_END_NAMESPACE

#endif // QTIVICLIMATECONTROL_P_H
