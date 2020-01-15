/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
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

import QtQuick 2.0
import QtIvi.VehicleFunctions.simulation 1.0

QtObject {
    property var settings : IviSimulator.findData(IviSimulator.simulationData, "QIviWindowControl")
    property bool defaultInitialized: false
    property var windowTimers : []
    property var blindTimers : []
    property Component comp : Component {
        id: timerComp
        Timer {
            id: timer

            interval: 1000
            repeat: true
            property var currentReply
            property string zone
            property bool isBlind: false
            property int currentValue: 0
            property int goal: 4
            property bool opening: false

            function open(reply) {
                if (currentReply && !currentReply.resultAvailable)
                    currentReply.setFailed();

                currentReply = reply
                opening = true
                start();
            }

            function close(reply) {
                if (currentReply && !currentReply.resultAvailable)
                    currentReply.setFailed();

                currentReply = reply
                opening = false
                start();
            }

            onTriggered: {
                if (opening)
                    currentValue++
                else
                    currentValue--

                var state = VehicleFunctions.Closed
                if (currentValue <= 0) {
                    timer.stop();
                    state = VehicleFunctions.Closed
                    if (currentReply)
                        currentReply.setSuccess(true)
                } else if (currentValue >= goal) {
                    timer.stop();
                    state = VehicleFunctions.FullyOpen
                    if (currentReply)
                        currentReply.setSuccess(true)
                } else {
                    state = VehicleFunctions.Open
                }

                if (isBlind)
                    backend.zones[zone].blindState = state
                else
                    backend.zones[zone].state = state
            }
        }
    }

    property var backend : QIviWindowControlBackend {

        function initialize() {
            print("QIviConcreteWindowControlSimulation INITIALIZE")
            if (!defaultInitialized) {
                IviSimulator.initializeDefault(settings, backend)
                defaultInitialized = true
            }
            Base.initialize()
        }

        function availableZones() {
            var zones = settings.zones;
            for (var zone in zones) {
                windowTimers[zones[zone]] = timerComp.createObject(backend, { zone: zones[zone] });
                blindTimers[zones[zone]] = timerComp.createObject(backend, { zone: zones[zone], isBlind: true });
            }

            return zones;
        }

        function open(reply, zone) {
            windowTimers[zone].open(reply)
        }

        function close(reply, zone) {
            windowTimers[zone].close(reply)
        }


        function setHeaterMode(heaterMode, zone) {
            if (IviSimulator.checkSettings(settings["heaterMode"], heaterMode, zone)) {
                if (zone) {
                    console.log("SIMULATION heaterMode for zone: " + zone + " changed to: " + heaterMode);
                    backend.zones[zone].heaterMode = heaterMode
                } else {
                    console.log("SIMULATION heaterMode changed to: " + heaterMode);
                    backend.heaterMode = heaterMode
                }
            } else {
                console.error("SIMULATION changing heaterMode is not possible: provided: " + heaterMode + "constraint: " + IviSimulator.constraint(settings["heaterMode"]));
            }
        }

        function setHeater(heater, zone) {
            if (IviSimulator.checkSettings(settings["heater"], heater, zone)) {
                console.log("SIMULATION heater changed to: " + heater);
                if (zone) {
                    console.log("SIMULATION heater for zone: " + zone + " changed to: " + heater);
                    backend.zones[zone].heater = heater
                } else {
                    console.log("SIMULATION heater changed to: " + heater);
                    backend.heater = heater
                }
            } else {
                console.error("SIMULATION changing heater is not possible: provided: " + heater + "constraint: " + IviSimulator.constraint(settings["heater"]));
            }
        }

        function setState(state, zone) {
            if (IviSimulator.checkSettings(settings["state"], state, zone)) {
                if (zone) {
                    console.log("SIMULATION state for zone: " + zone + " changed to: " + state);
                    backend.zones[zone].state = state
                } else {
                    console.log("SIMULATION state changed to: " + state);
                    backend.state = state
                }
            } else {
                console.error("SIMULATION changing state is not possible: provided: " + state + "constraint: " + IviSimulator.constraint(settings["state"]));
            }
        }

        function setBlindMode(blindMode, zone) {
            if (blindMode == VehicleFunctions.BlindOpen)
                blindTimers[zone].open(undefined)
            else if (blindMode == VehicleFunctions.BlindClosed)
                blindTimers[zone].close(undefined)
        }

        function setBlindState(blindState, zone) {
            if (IviSimulator.checkSettings(settings["blindState"], blindState, zone)) {
                if (zone) {
                    console.log("SIMULATION blindState for zone: " + zone + " changed to: " + blindState);
                    backend.zones[zone].blindState = blindState
                } else {
                    console.log("SIMULATION blindState changed to: " + blindState);
                    backend.blindState = blindState
                }
            } else {
                console.error("SIMULATION changing blindState is not possible: provided: " + blindState + "constraint: " + IviSimulator.constraint(settings["blindState"]));
            }
        }
    }
}
