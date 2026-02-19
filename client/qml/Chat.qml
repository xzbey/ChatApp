import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: chatWindow
    width: 800
    height: 600
    visible: false
    title: qsTr("Чат")

    //Текущая модель сообщений (по умолчанию broadcast)
    property var currentMessageModel: appController.chatListModel.getMessageModel("broadcast")
    property string currentChatId: "broadcast"
    property bool chatStarted: false

    Connections {
        target: appController
        function onChatDisconnected() {
            console.log("onChatDisconnected called");
            let component = Qt.createComponent("Auth.qml");
            if (component.status === Component.Ready) {
                let auth = component.createObject(null);
                auth.show();
                Qt.callLater(function() { chatWindow.destroy(); });
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 1

        //Левая панель со списком чатов
        Rectangle {
            color: "gray"
            Layout.fillHeight: true
            Layout.preferredWidth: chatWindow.width * 0.25

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                ListView {
                    id: chatList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    model: appController.chatListModel

                    delegate: Rectangle {
                        width: chatList.width
                        height: 55
                        color: chatList.currentIndex === index ? "#5a5a5a" : "transparent"

                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.margins: 12
                            text: model.chatName
                            color: "white"
                            font.pixelSize: 14
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                chatList.currentIndex = index
                                chatWindow.currentChatId = model.chatId
                                chatWindow.currentMessageModel = appController.chatListModel.getMessageModel(model.chatId)
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }
                }

                Button {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 45
                    text: "+ Новый чат"
                    onClicked: {
                        //appController.chatListModel.addChat("private_Челик", "Челик")
                    }
                }
            }
        }

        //Правая панель с чатом
        Rectangle {
            color: "green"
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                //Заголовок текущего чата
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 45
                    color: "#4a4a4a"

                    Text {
                        anchors.centerIn: parent
                        text: chatList.currentIndex >= 0
                              ? appController.chatListModel.data(
                                    appController.chatListModel.index(chatList.currentIndex, 0),
                                    257)  // 257 = Qt.UserRole + 1 + 1 = ChatNameRole
                              : "Общий чат"
                        color: "white"
                        font.pixelSize: 15
                        font.bold: true
                    }
                }

                //Список сообщений
                ListView {
                    id: messageListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    model: chatWindow.currentMessageModel

                    delegate: Rectangle {
                        width: messageListView.width
                        height: msgCol.height + 16
                        color: "transparent"

                        Column {
                            id: msgCol
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.margins: 10
                            anchors.verticalCenter: parent.verticalCenter

                            Text {
                                text: model.author
                                font.bold: true
                                font.pixelSize: 12
                                color: "#003399"
                            }
                            Text {
                                text: model.text
                                font.pixelSize: 14
                                wrapMode: Text.WordWrap
                                width: parent.width
                                color: "white"
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }

                    onCountChanged: positionViewAtEnd()
                }

                //Поле ввода
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 55
                    color: "#4a4a4a"

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 8

                        TextField {
                            id: messageInput
                            Layout.fillWidth: true
                            placeholderText: "Введите сообщение..."
                            onAccepted: sendButton.clicked()
                        }

                        Button {
                            id: sendButton
                            text: "Отправить"
                            Layout.preferredWidth: 100
                            onClicked: {
                                let text = messageInput.text.trim()
                                if (text === "") return

                                if (chatWindow.currentChatId === "broadcast") {
                                    appController.sendBroadcast(text)
                                } else {
                                    let to = chatWindow.currentChatId.replace("private_", "")
                                    appController.sendMessage(to, text)
                                }

                                messageInput.text = ""
                            }
                        }
                    }
                }
            }
        }
    }
}
