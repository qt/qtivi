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
import QtGraphicalEffects 1.0

Item {
    id: root

    width: 0.8 * 720
    height: 720

    property real value: 0.0
    property bool metricSystem: false

    Image {
        id: overlay

        width: 0.91 * root.width
        height: 0.98 * width
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.verticalCenter: parent.verticalCenter
        source: Qt.resolvedUrl("images/left_dial.png")
    }

    Text {
        id: speedText

        anchors.verticalCenter: overlay.verticalCenter
        anchors.horizontalCenter: overlay.horizontalCenter
        anchors.verticalCenterOffset: -7
        anchors.horizontalCenterOffset: 5
        font.family: "Source Sans Pro"
        font.pixelSize: 60
        font.letterSpacing: 4
        color: "white"
        text: root.value
    }

    Rectangle {
        width: 60
        height: 1
        opacity: 0.4
        anchors.top: speedText.bottom
        anchors.topMargin: -8
        anchors.horizontalCenter: overlay.horizontalCenter
        anchors.horizontalCenterOffset: 2
    }

    Text {
        id: mph
        anchors.top: speedText.bottom
        anchors.topMargin: -5
        anchors.horizontalCenter: overlay.horizontalCenter
        anchors.horizontalCenterOffset: 2
        font.family: "Source Sans Pro"
        font.pixelSize: 24
        color: "white"
        text: root.metricSystem ? "km/h" : "mph"
    }

    Dial {
        width: 0.66 * root.height
        height: width
        anchors.centerIn: overlay
        anchors.verticalCenterOffset: 2
        fillImage: "images/dial_fill_color_left.png"
        value: root.value / 240
    }
}
