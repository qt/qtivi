/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
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
****************************************************************************/

import QtQuick 2.1

Item {
    id: root

    width: 0.8 * 720
    height: 720

    property color warningColor: "transparent"
    property string warningIcon
    property string warningText
    property int value
    property alias fuelLevel: fuelMeter.value

    Item {
        width: root.width/3
        height: 0.1 * root.width
        anchors.bottom: overlay.top
        anchors.left: parent.left
        anchors.leftMargin: 0.2 * root.width

        Image {
            id: gears

            width: 0.2 * root.width
            height: 0.25 * width
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            source: Qt.resolvedUrl("images/P-R-N-D.png")
        }

        Image {
            id: plusMinus
            anchors.bottom: parent.bottom
            anchors.left: gears.right
            anchors.leftMargin: 10
            source: Qt.resolvedUrl("images/+--.png")
        }
    }

    Image {
        id: overlay

        width: 0.91 * root.width
        height: 0.99 * width

        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.verticalCenter: parent.verticalCenter
        source: Qt.resolvedUrl("images/right_dial.png")

        Rectangle {
            id: rect
            width: circle.width + 5
            height: width
            radius: width
            color: "transparent"
            border.color: root.warningColor
            border.width: rect.borderWidth
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -3

            property int borderWidth: 3

            SequentialAnimation {
                running: root.warningColor != "transparent"
                loops: Animation.Infinite
                NumberAnimation {

                    target: rect
                    properties: "borderWidth"
                    from: 3
                    to: 7
                    duration: 500
                }

                NumberAnimation {

                    target: rect
                    properties: "borderWidth"
                    from: 7
                    to: 3
                    duration: 500
                }

                onStopped: rect.borderWidth = 3

            }
        }

        Image {
            id: circle

            width: root.width/2
            height: width
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -6
            anchors.verticalCenterOffset: 0
            source: Qt.resolvedUrl("images/middle-bkg.png")

            Image {
                id: circle_overlay

                width: parent.width
                height: parent.height
                anchors.centerIn: parent
                source: Qt.resolvedUrl("images/middle-circle.png")
            }

            Image {
                id: fuelSymbol
                width: 70
                height: 70
                anchors.top: parent.top
                anchors.topMargin: 55
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: 0
                source: root.warningIcon
                fillMode: Image.PreserveAspectFit

            }

            Item {
                width: parent.width
                height: parent.height/4
                anchors.centerIn: parent
                anchors.verticalCenterOffset: 40

                Rectangle {
                    id: speedText
                    width: parent.width - 80
                    height: 40
                    radius: 20
                    anchors.horizontalCenter: parent.horizontalCenter
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: Qt.darker("grey", 1.5) }
                        GradientStop { position: 0.4; color: "#0c0c0c" }
                    }

                    Label {
                        width: parent.width
                        anchors.centerIn: parent
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: text.length > 10 ? 18 : 24
                        font.bold: true
                        text: root.warningText
                        elide: Text.ElideMiddle
                    }
                }
            }
        }
    }

    Fuel {
        id: fuelMeter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0.05 * root.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -20
    }

    Dial {
        id: dial

        width: 0.69 * root.height
        height: width
        anchors.centerIn: overlay
        anchors.verticalCenterOffset: 0
        anchors.horizontalCenterOffset: -5
        fillImage: "images/dial_fill_color.png"
        circleRadius: "0.29"
        dialCursor: "images/dial_cursor_right.png"
        value: root.value / 7000
    }
}
