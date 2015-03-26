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

        CheckBox {
            text: "Steering Wheel Heater"
            checked: climateControl.steeringWheelHeater
            onClicked: {
                    climateControl.steeringWheelHeater = checked
            }
        }

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
        if (climateControl.serviceObject === null)
            messageDialog.open()
    }
}
