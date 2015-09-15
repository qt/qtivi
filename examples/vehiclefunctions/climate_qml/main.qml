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

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

import QtIVICore 1.0
import QtIVIVehicleFunctions 1.0

ApplicationWindow {
    title: "Climate Control"
    width: 384
    height: 324
    visible: true


    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        GroupBox {
            title: "Air Flow Direction"

            ColumnLayout {
                anchors.fill: parent
                ExclusiveGroup { id: group }
                RadioButton {
                    text: "Floor Panel"
                    exclusiveGroup: group
                    checked: climateControl.airflowDirection === ClimateControl.FloorPanel
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirection = ClimateControl.FloorPanel
                    }
                }

                RadioButton {
                    text: "Floor Duct"
                    exclusiveGroup: group
                    checked: climateControl.airflowDirection === ClimateControl.FloorDuct
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirection = ClimateControl.FloorDuct
                    }
                }

                RadioButton {
                    text: "Bi Level"
                    exclusiveGroup: group
                    checked: climateControl.airflowDirection === ClimateControl.BiLevel
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirection = ClimateControl.BiLevel
                    }
                }

                RadioButton {
                    text: "Defrost Floor"
                    exclusiveGroup: group
                    checked: climateControl.airflowDirection === ClimateControl.DefrostFloor
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirection = ClimateControl.DefrostFloor
                    }
                }
            }
        }

        CheckBox {
            text: "Air Condition"
            checked: climateControl.airConditioning
            onClicked: {
                    climateControl.airConditioning = checked
            }
        }

        CheckBox {
            text: "Heater"
            checked: climateControl.heater
            onClicked: {
                    climateControl.heater = checked
            }
        }

        CheckBox {
            text: "Air Recirculation"
            checked: climateControl.airRecirculation
            onClicked: {
                    climateControl.airRecirculation = checked
            }
        }

        GroupBox {
            title: "Front Left Zone"

            ColumnLayout {
                RowLayout {

                    Label {
                        text: "Fan Speed"
                    }

                    SpinBox {
                        value: climateControl.Zones.frontLeft.fanSpeedLevel
                        onValueChanged: {
                            climateControl.Zones.frontLeft.fanSpeedLevel = value
                        }
                    }
                }


                RowLayout {

                    Label {
                        text: "Steering Wheel Heater"
                    }

                    SpinBox {
                        value: climateControl.Zones.frontLeft.steeringWheelHeater
                        onValueChanged: {
                            climateControl.Zones.frontLeft.steeringWheelHeater = value
                        }
                    }
                }
            }
        }
    }

    ClimateControl {
        id: climateControl
        autoDiscovery: true
    }

    MessageDialog {
        id: messageDialog
        title: "Auto Discovery Failed !"
        text: "No Climate Backend available"
        icon: StandardIcon.Critical
    }

    Component.onCompleted: {
        if (!climateControl.isValid)
            messageDialog.open()
    }
}
