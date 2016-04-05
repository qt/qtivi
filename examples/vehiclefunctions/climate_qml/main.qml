/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
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
**   * Neither the name of Pelagicore AG nor the names of its
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


import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

import QtIVIVehicleFunctions 1.0

ApplicationWindow {
    title: "Climate Control"
    width: 384
    height: 324
    visible: true

//![1]
    ClimateControl {
        id: climateControl
        discoveryMode: ClimateControl.AutoDiscovery
    }
//![1]

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

//![2]
        GroupBox {
            title: "Air Flow Direction"

            ColumnLayout {
                anchors.fill: parent
                ExclusiveGroup { id: group }
                RadioButton {
                    text: "Floor Panel"
                    exclusiveGroup: group
                    checked: climateControl.airflowDirection.value === ClimateControl.FloorPanel
                    enabled: climateControl.airflowDirection.availableValues.indexOf(ClimateControl.FloorPanel) !== -1
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirection.value = ClimateControl.FloorPanel
                    }
                }

                RadioButton {
                    text: "Floor Duct"
                    exclusiveGroup: group
                    checked: climateControl.airflowDirection.value === ClimateControl.FloorDuct
                    enabled: climateControl.airflowDirection.availableValues.indexOf(ClimateControl.FloorDuct) !== -1
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirection.value = ClimateControl.FloorDuct
                    }
                }

                RadioButton {
                    text: "Bi Level"
                    exclusiveGroup: group
                    checked: climateControl.airflowDirection.value === ClimateControl.BiLevel
                    enabled: climateControl.airflowDirection.availableValues.indexOf(ClimateControl.BiLevel) !== -1
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirection.value = ClimateControl.BiLevel
                    }
                }

                RadioButton {
                    text: "Defrost Floor"
                    exclusiveGroup: group
                    checked: climateControl.airflowDirection.value === ClimateControl.DefrostFloor
                    enabled: climateControl.airflowDirection.availableValues.indexOf(ClimateControl.DefrostFloor) !== -1
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirection.value = ClimateControl.DefrostFloor
                    }
                }
            }
        }

        CheckBox {
            text: "Air Condition"
            checked: climateControl.airConditioning.value
            enabled: climateControl.airConditioning.available
            onClicked: {
                    climateControl.airConditioning.value = checked
            }
        }

        CheckBox {
            text: "Heater"
            checked: climateControl.heater.value
            enabled: climateControl.heater.available
            onClicked: {
                    climateControl.heater.value = checked
            }
        }

        CheckBox {
            text: "Air Recirculation"
            checked: climateControl.airRecirculation.value
            enabled: climateControl.airRecirculation.available
            onClicked: {
                    climateControl.airRecirculation.value = checked
            }
        }

        ColumnLayout {
            RowLayout {

                Label {
                    text: "Fan Speed"
                }

                SpinBox {
                    value: climateControl.fanSpeedLevel.value
                    minimumValue: climateControl.fanSpeedLevel.minimumValue
                    maximumValue: climateControl.fanSpeedLevel.maximumValue
                    enabled: climateControl.fanSpeedLevel.available
                    onValueChanged: {
                        climateControl.fanSpeedLevel.value = value
                    }
                }
            }


            RowLayout {

                Label {
                    text: "Steering Wheel Heater"
                }

                SpinBox {
                    value: climateControl.steeringWheelHeater.value
                    minimumValue: climateControl.steeringWheelHeater.minimumValue
                    maximumValue: climateControl.steeringWheelHeater.maximumValue
                    enabled: climateControl.steeringWheelHeater.available
                    onValueChanged: {
                        climateControl.steeringWheelHeater.value = value
                    }
                }
            }
        }
//![2]
        Row {
//![3]
            GroupBox {
                title: "Front Left Zone"

                ColumnLayout {
                    RowLayout {

                        Label {
                            text: "Temperature"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.FrontLeft.targetTemperature.value
                            minimumValue: climateControl.zoneAt.FrontLeft.targetTemperature.minimumValue
                            maximumValue: climateControl.zoneAt.FrontLeft.targetTemperature.maximumValue
                            enabled: climateControl.zoneAt.FrontLeft.targetTemperature.available
                            onValueChanged: {
                                climateControl.zoneAt.FrontLeft.targetTemperature.value = value
                            }
                        }
                    }


                    RowLayout {

                        Label {
                            text: "Seat Heater"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.FrontLeft.seatHeater.value
                            minimumValue: climateControl.zoneAt.FrontLeft.seatHeater.minimumValue
                            maximumValue: climateControl.zoneAt.FrontLeft.seatHeater.maximumValue
                            enabled: climateControl.zoneAt.FrontLeft.seatHeater.available
                            onValueChanged: {
                                climateControl.zoneAt.FrontLeft.seatHeater.value = value
                            }
                        }
                    }
                }
            }

            GroupBox {
                title: "Front Right Zone"

                ColumnLayout {
                    RowLayout {

                        Label {
                            text: "Temperature"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.FrontRight.targetTemperature.value
                            minimumValue: climateControl.zoneAt.FrontRight.targetTemperature.minimumValue
                            maximumValue: climateControl.zoneAt.FrontRight.targetTemperature.maximumValue
                            enabled: climateControl.zoneAt.FrontRight.targetTemperature.available
                            onValueChanged: {
                                climateControl.zoneAt.FrontRight.targetTemperature.value = value
                            }
                        }
                    }


                    RowLayout {

                        Label {
                            text: "Seat Heater"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.FrontRight.seatHeater.value
                            minimumValue: climateControl.zoneAt.FrontRight.seatHeater.minimumValue
                            maximumValue: climateControl.zoneAt.FrontRight.seatHeater.maximumValue
                            enabled: climateControl.zoneAt.FrontRight.seatHeater.available
                            onValueChanged: {
                                climateControl.zoneAt.FrontRight.seatHeater.value = value
                            }
                        }
                    }
                }
            }

            GroupBox {
                title: "Rear Zone"

                ColumnLayout {
                    RowLayout {

                        Label {
                            text: "Temperature"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.Rear.targetTemperature.value
                            minimumValue: climateControl.zoneAt.Rear.targetTemperature.minimumValue
                            maximumValue: climateControl.zoneAt.Rear.targetTemperature.maximumValue
                            enabled: climateControl.zoneAt.Rear.targetTemperature.available
                            onValueChanged: {
                                climateControl.zoneAt.Rear.targetTemperature.value = value
                            }
                        }
                    }


                    RowLayout {

                        Label {
                            text: "Seat Heater"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.Rear.seatHeater.value
                            minimumValue: climateControl.zoneAt.Rear.seatHeater.minimumValue
                            maximumValue: climateControl.zoneAt.Rear.seatHeater.maximumValue
                            enabled: climateControl.zoneAt.Rear.seatHeater.available
                            onValueChanged: {
                                climateControl.zoneAt.Rear.seatHeater.value = value
                            }
                        }
                    }
                }
            }
        }
    }
//![3]

    MessageDialog {
        id: messageDialog
        title: "Auto Discovery Failed !"
        text: "No Climate Backend available"
        icon: StandardIcon.Critical
    }

//![4]
    Component.onCompleted: {
        if (!climateControl.isValid)
            messageDialog.open()
    }
//![4]
}
