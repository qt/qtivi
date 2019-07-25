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

import QtQuick 2.1

Item {
    id: root
    width: 0.37 * 1920
    height: 0.12 * 720

    property real temperature: 0

    Image {
        id: background
        width: parent.width
        height: parent.height
        source: Qt.resolvedUrl("images/top_bar.png")
    }

    Label {
        id: timeText
        anchors.verticalCenter: background.verticalCenter
        anchors.left: background.left
        anchors.leftMargin: 0.2 * background.width

        font.pixelSize: 0.42 * background.height
        font.bold: true

        text: Qt.formatTime(currentDate, "hh:mm")

        property var currentDate: new Date();

        Timer {
            interval: 1000
            repeat: true
            running: true
            onTriggered: {
                timeText.currentDate = new Date();
            }
        }
    }

    Item {
        id: navigator

        width: 0.25 * background.width
        height: background.height
        anchors.verticalCenter: background.verticalCenter
        anchors.horizontalCenter: background.horizontalCenter

        Row {
            id: row
            property int radius: 7
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -16
            spacing: 8

            Repeater {
                model: 3
                delegate: Rectangle {
                    height: row.radius * 2
                    width: row.radius * 2
                    radius: row.radius
                    color: 1 === index ? "white" : "#4d4d4d"
                }
            }
        }
    }

    Label {
        id: temperatureText
        anchors.verticalCenter: background.verticalCenter
        anchors.left: navigator.right
        anchors.leftMargin: 15

        font.pixelSize: 0.42 * background.height
        font.bold: true

        text: root.temperature + "°C"
    }
}
