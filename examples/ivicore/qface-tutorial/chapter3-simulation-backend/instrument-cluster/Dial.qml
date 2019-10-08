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

import QtQuick 2.12

Item {
    id: root

    width: 480
    height: 480

    property real value: 0
    property int upDuration: 2000
    property int downDuration: 1000
    property string fillImage: "images/dial_fill_color_left.png"
    property string circleRadius: "0.193"
    property string dialCursor: "images/dial_cursor.png"

    Image {
        id: meter
        property real min: -83.5
        property real max: 157
        width: root.width
        height: width - 1
        rotation: min + (max - min) * root.value
        source: Qt.resolvedUrl(root.dialCursor)
    }

    ShaderEffect {
        anchors.fill: meter
        property var pattern: Image {
            source: Qt.resolvedUrl("images/dial_pattern.png")
        }
        property var fill: Image {
            source: Qt.resolvedUrl(root.fillImage)
        }
        property real value: root.value

        fragmentShader: "
#define M_PI 3.141592653589793
#define INNER " + root.circleRadius + "

varying highp vec2 qt_TexCoord0;
uniform lowp float qt_Opacity;
uniform sampler2D pattern;
uniform sampler2D fill;
uniform lowp float value;

void main() {
    lowp vec4 pattern = texture2D(pattern, qt_TexCoord0);
    lowp vec4 fill = texture2D(fill, qt_TexCoord0);

    lowp vec2 pos = vec2(qt_TexCoord0.x - 0.5, 0.501 - qt_TexCoord0.y);
    lowp float d = length(pos);
    lowp float angle = atan(pos.x, pos.y) / (2.0 * M_PI);
    lowp float v = 0.66 * value - 0.33;

    // Flare pattern
    lowp vec4 color = mix(pattern, vec4(0.0), smoothstep(v, v + 0.1, angle));
    // Gradient fill color
    color += mix(fill, vec4(0.0), step(v, angle));
    // Punch out the center hole
    color = mix(vec4(0.0), color, smoothstep(INNER - 0.001, INNER + 0.001, d));
    // Fade out below 0
    gl_FragColor = mix(color, vec4(0.0), smoothstep(-0.35, -0.5, angle));
}
        "
    }
}
