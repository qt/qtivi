/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Layouts 1.1
import QtIvi 1.0
import QtIvi.Media 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Media Player")

    MediaPlayer {
        id: mediaPlayer
    }

    SplitView {
        id: splitView1
        anchors.fill: parent

        ColumnLayout {

            RowLayout {
                Label { text: "Artist:" }
                Label { text: mediaPlayer.currentTrack.artist }
            }

            RowLayout {
                Label { text: "Album:" }
                Label { text: mediaPlayer.currentTrack.album }
            }

            RowLayout {
                Label { text: "Title:" }
                Label { text: mediaPlayer.currentTrack.title }
            }

            RowLayout {
                Button {
                    text: "<-"
                    onClicked: mediaPlayer.previous()
                }

                Button {
                    text: "stop"
                    onClicked: mediaPlayer.stop()
                }

                Button {
                    text: ">"
                    onClicked: mediaPlayer.play()
                }

                Button {
                    text: "->"
                    onClicked: mediaPlayer.next()
                }
            }

            ListView {
                spacing: 8
                clip: true
                Layout.fillHeight: true
                Layout.fillWidth: true

                model: mediaPlayer.playQueue

                delegate: Rectangle {
                    width: ListView.view.width
                    height: column.height
                    color: "#efefef"

                    Column {
                        id: column
                        width: parent.width

                        Text { text: "Index: " + index }
                        Text { text: "Name: " + model.name }
                        Text { text: "Type: " + model.item.type }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {

                        }
                    }
                }
            }
        }

        ColumnLayout {

            RowLayout {
                Label { text: "filter:" }
                TextField { id: filterEdit }
            }

            ListView {
                spacing: 8
                clip: true
                Layout.fillHeight: true
                Layout.fillWidth: true

                header: Rectangle {
                    width: ListView.view.width
                    height: searchModel.canGoBack ? 50 : 0
                    visible: height > 0
                    color: "#efefef"

                    Text {
                        anchors.centerIn: parent
                        text: "back"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            searchModel.goBack()
                        }
                    }
                }

                model: SearchAndBrowseModel {
                    id: searchModel
                    serviceObject: mediaPlayer.serviceObject
                    contentType: "artist"
                    loadingType: SearchAndBrowseModel.FetchMore
                    query: filterEdit.text
                    onQueryChanged: filterEdit.text = query
                }

                delegate: Rectangle {
                    width: ListView.view.width
                    height: column.height
                    color: "#efefef"

                    Column {
                        id: column
                        width: parent.width

                        Text { text: "Index: " + index }
                        Text { text: "Name: " + model.name }
                        Text { text: "Type: " + model.item.type }
                        Text { text: "CanForward: " + model.canGoForward }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (item.type === "track")
                                mediaPlayer.playQueue.insert(0, item)
                            else
                                searchModel.goForward(index, SearchAndBrowseModel.InModelNavigation)
                        }
                    }
                }
            }
        }
    }
}
