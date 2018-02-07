/****************************************************************************
**
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

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

import QtIvi.VehicleFunctions 1.0

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
                CheckBox {
                    text: "Windshield"
                    checked: climateControl.airflowDirections & ClimateControl.Windshield
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirections |= ClimateControl.Windshield
                        else
                            climateControl.airflowDirections &= ~ClimateControl.Windshield
                    }
                }

                CheckBox {
                    text: "Dashboard"
                    checked: climateControl.airflowDirections & ClimateControl.Dashboard
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirections |= ClimateControl.Dashboard
                        else
                            climateControl.airflowDirections &= ~ClimateControl.Dashboard
                    }
                }

                CheckBox {
                    text: "Floor"
                    checked: climateControl.airflowDirections & ClimateControl.Floor
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirections |= ClimateControl.Floor
                        else
                            climateControl.airflowDirections &= ~ClimateControl.Floor
                    }
                }
            }
        }

        CheckBox {
            text: "Air Condition"
            checked: climateControl.airConditioningEnabled
            onClicked: {
                    climateControl.airConditioningEnabled = checked
            }
        }

        CheckBox {
            text: "Heater"
            checked: climateControl.heaterEnabled
            onClicked: {
                    climateControl.heaterEnabled = checked
            }
        }

        CheckBox {
            text: "Air Recirculation"
            checked: climateControl.recirculationMode === ClimateControl.RecirculationOn
            onClicked: {
                if (checked)
                    climateControl.recirculationMode = ClimateControl.RecirculationOn
                else
                    climateControl.recirculationMode = ClimateControl.RecirculationOff
            }
        }

        ColumnLayout {
            RowLayout {

                Label {
                    text: "Fan Speed"
                }

                SpinBox {
                    value: climateControl.fanSpeedLevel
                    onValueChanged: {
                        climateControl.fanSpeedLevel = value
                    }
                }
            }


            RowLayout {

                Label {
                    text: "Steering Wheel Heater"
                }

                SpinBox {
                    value: climateControl.steeringWheelHeater
                    onValueChanged: {
                        climateControl.steeringWheelHeater = value
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
                            value: climateControl.zoneAt.FrontLeft.targetTemperature
                            onValueChanged: {
                                climateControl.zoneAt.FrontLeft.targetTemperature = value
                            }
                        }
                    }


                    RowLayout {

                        Label {
                            text: "Seat Heater"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.FrontLeft.seatHeater
                            onValueChanged: {
                                climateControl.zoneAt.FrontLeft.seatHeater = value
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
                            value: climateControl.zoneAt.FrontRight.targetTemperature
                            onValueChanged: {
                                climateControl.zoneAt.FrontRight.targetTemperature = value
                            }
                        }
                    }


                    RowLayout {

                        Label {
                            text: "Seat Heater"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.FrontRight.seatHeater
                            onValueChanged: {
                                climateControl.zoneAt.FrontRight.seatHeater = value
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
                            value: climateControl.zoneAt.Rear.targetTemperature
                            onValueChanged: {
                                climateControl.zoneAt.Rear.targetTemperature = value
                            }
                        }
                    }


                    RowLayout {

                        Label {
                            text: "Seat Heater"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.Rear.seatHeater
                            onValueChanged: {
                                climateControl.zoneAt.Rear.seatHeater = value
                            }
                        }
                    }
                }
            }
            //![3]
        }
    }

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
