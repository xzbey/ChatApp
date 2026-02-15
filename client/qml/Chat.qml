import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: chatWindow
    width: 800
    height: 600
    visible: false
    title: qsTr("Чат")

    Connections {
        target: appController

        function onMessageReceived(from, msg) {

        }
    }

    Row {
        anchors.fill: parent

        Rectangle {
            color: "#808080"

            Column {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Label {
                    text: "Чат"
                    font {
                        pixelSize: 24
                        bold: true
                    }
                }

                Rectangle {

                }
            }
        }
    }
}




/* с теста

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: chatWindow
    width: 800
    height: 600
    visible: true
    title: qsTr("Чат")

    Rectangle {
        color: "red"
        anchors.fill: parent


        RowLayout {
            anchors.fill: parent
            spacing: 1

            Rectangle {
                color: "gray"
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.width * 0.2

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 4

                    ListView {
                        id: chatList
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        model: chatListModel

                        delegate: Rectangle {
                            width: parent.width
                            height: 50
                            color: parent.currentIndex === index ? "blue" : "lightblue"

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    chatList.currentIndex = index
                                }
                            }
                        }

                    }


                }
            }

            Rectangle {
                color: "green"
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.width * 0.8
            }
        }
    }




}



  */
