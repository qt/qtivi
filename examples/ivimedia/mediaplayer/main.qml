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

import QtQuick 2.6
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.1
import QtIvi 1.0
import QtIvi.Media 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("Media Player")

    MediaPlayer {
        id: mediaPlayer
        discoveryMode: AbstractFeature.LoadOnlySimulationBackends
    }

    SplitView {
        id: splitView1
        anchors.fill: parent

        ColumnLayout {

            RowLayout {
                Label { text: "Artist:" }
                Label { text: mediaPlayer.currentTrack ? mediaPlayer.currentTrack.artist : "" }
            }

            RowLayout {
                Label { text: "Album:" }
                Label { text: mediaPlayer.currentTrack ? mediaPlayer.currentTrack.album : "" }
            }

            RowLayout {
                Label { text: "Title:" }
                Label { text: mediaPlayer.currentTrack ? mediaPlayer.currentTrack.title : "" }
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
                    color: mediaPlayer.playQueue.currentIndex == index ? "lightblue" : "#efefef"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            mediaPlayer.playQueue.currentIndex = index
                            mediaPlayer.play()
                        }
                    }

                    Row {
                        anchors.fill: parent
                        Column {
                            id: column
                            width: parent.width * 7 / 10

                            Text { text: "Index: " + index }
                            Text { text: "Name: " + model.name }
                            Text { text: "Type: " + model.item.type }
                        }

                        Button {
                            text: "\u2227"
                            width: parent.width / 10
                            height: parent.height

                            enabled: index > 0

                            onClicked: {
                                mediaPlayer.playQueue.move(index, index - 1)
                            }
                        }

                        Button {
                            text: "\u2228"
                            width: parent.width / 10
                            height: parent.height

                            enabled: index < mediaPlayer.playQueue.count -1

                            onClicked: {
                                mediaPlayer.playQueue.move(index, index + 1)
                            }
                        }

                        Button {
                            text: "X"
                            width: parent.width / 10
                            height: parent.height

                            onClicked: {
                                mediaPlayer.playQueue.remove(index)
                            }
                        }
                    }

                }
            }
        }

        ColumnLayout {

            RowLayout {
                Label { text: "filter:" }
                TextField { id: filterEdit }
                Button { text: "Refresh"; onClicked: searchModel.reload(); }
            }

            ListView {
                spacing: 8
                clip: true
                Layout.fillHeight: true
                Layout.minimumWidth: 300

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

                        Text { text: "Name: " + model.name }
                        Text { text: "Type: " + model.item.type }
                        Text { text: "CanForward: " + model.canGoForward }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (item.type === "audiotrack")
                                mediaPlayer.playQueue.insert(0, item)
                            else
                                searchModel.goForward(index, SearchAndBrowseModel.InModelNavigation)
                        }
                    }
                }
            }
        }

        ColumnLayout {
            ListView {
                id: browseView
                spacing: 8
                clip: true
                Layout.fillHeight: true
                Layout.minimumWidth: 300

                header: Rectangle {
                    width: ListView.view.width
                    height: browseView.model !== discoveryModel ? 50 : 0
                    visible: height > 0
                    color: "#efefef"

                    Text {
                        anchors.centerIn: parent
                        text: "back"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (filterModel.canGoBack)
                                filterModel.goBack()
                            else
                                browseView.model = discoveryModel
                        }
                    }
                }

                model: MediaDeviceDiscoveryModel {
                    id: discoveryModel

                    onDeviceRemoved: {
                        if (device === filterModel.serviceObject)
                            browseView.model = discoveryModel
                    }
                }

                SearchAndBrowseModel {
                    id: filterModel
                    contentType: "file"
                }

                delegate: Rectangle {
                    width: ListView.view.width
                    height: column.height
                    color: "#efefef"

                    Column {
                        id: column
                        width: parent.width

                        Text { text: "Name: " + model.name }
                        Text { text: "Type: " + model.type }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (browseView.model === discoveryModel) {
                                filterModel.serviceObject = model.serviceObject
                                browseView.model = filterModel
                                return;
                            }

                            filterModel.goForward(index, SearchAndBrowseModel.InModelNavigation)
                        }
                    }
                }
            }
        }
    }
}
