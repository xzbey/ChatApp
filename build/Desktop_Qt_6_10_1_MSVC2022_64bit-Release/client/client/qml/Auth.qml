import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Controls.Basic
import "../common/Utils.js" as Utils

Window {
    id: authWindow
    width: 800
    height: 600
    visible: true
    property bool isLoginMode: true
    title: qsTr(isLoginMode ? "Вход" : "Регистрация")

    Connections {
        target: appController

        function onAuthSuccess() {
            let component = Qt.createComponent("Chat.qml");

            if (component.status === Component.Ready) {
                let chat = component.createObject(null);
                chat.show();
                authWindow.close();

            } else if (component.status === Component.Error)
                console.log("Ошибка загрузки Chat.qml: " + component.errorString());

            else
                component.statusChanged.connect(function() {
                    if (component.status === Component.Ready) {
                        let chat = component.createObject(null);
                        chat.show();
                        authWindow.close();

                    } else if (component.status === Component.Error)
                        console.log("Ошибка: " + component.errorString());
                });
        }

        function onRegisterSuccess() {
            isLoginMode = true;
            passwordField1.clear();
            passwordField2.clear();
            errorLabel.text = "Успешная регистрация";
        }
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#e8f5e9" }
            GradientStop { position: 1.0; color: "#f1f8e9" }
        }
    }

    Rectangle {
        anchors.centerIn: parent
        width: 360
        height: isLoginMode ? 420 : 500
        radius: 30

        Column {
            anchors {
                fill: parent
                verticalCenter: parent.verticalCenter
            }
            spacing: 25
            topPadding: 20

            Label {
                id: topLabel
                anchors {
                    left: parent.left
                    right: parent.right
                }
                horizontalAlignment: Text.AlignHCenter
                topPadding: 10
                text: isLoginMode ? "Вход" : "Регистрация"
                color: "#2d3e2f"
                font {
                    pixelSize: 30
                    bold: true
                }
            }

            TextField {
                id: loginField
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: 50
                }
                height: 40
                background: Rectangle {
                    radius: 15
                    border.color: "#e0e0e0"
                    border.width: 2
                }
                verticalAlignment: Text.AlignVCenter
                leftPadding: 15
                rightPadding: 15
                placeholderText: isLoginMode ? "Логин" : "Придумайте логин"
                color: "black"
                font.pixelSize: 14
            }

            TextField {
                id: passwordField1
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: 50
                }
                height: 40
                background: Rectangle {
                    radius: 15
                    border.color: "#e0e0e0"
                    border.width: 2
                }
                verticalAlignment: Text.AlignVCenter
                leftPadding: 15
                rightPadding: 15
                placeholderText: isLoginMode ? "Пароль" : "Придумайте пароль"
                color: "black"
                font.pixelSize: 14
                echoMode: TextInput.PasswordEchoOnEdit
            }

            TextField {
                id: passwordField2
                visible: !isLoginMode
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: 50
                }
                height: 40
                background: Rectangle {
                    radius: 15
                    border.color: "#e0e0e0"
                    border.width: 2
                }
                verticalAlignment: Text.AlignVCenter
                leftPadding: 15
                rightPadding: 15
                placeholderText: "Подтвердите пароль"
                color: "black"
                font.pixelSize: 14
                echoMode: TextInput.PasswordEchoOnEdit
            }

            Button {
                id: actionBtn
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: 50
                }
                height: 40
                hoverEnabled: true

                background: Rectangle {
                    radius: 15
                    color: actionBtn.hovered ? "#27ae60" : "#2ecc71"
                }

                contentItem: Text {
                    text: isLoginMode ? "Войти" : "Зарегистрироваться"
                    font {
                        pixelSize: 20
                        bold: true
                    }
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (Utils.isCorrected(isLoginMode, loginField.text, passwordField1.text, passwordField2.text)) {
                        if (isLoginMode) {
                            appController.login(loginField.text, passwordField1.text);
                        } else {
                            appController.registration(loginField.text, passwordField1.text);
                        }
                    }
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10

                Rectangle {
                    width: 100
                    height: 1
                    color: "#b0b0b0"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Label {
                    text: "или"
                    color: "#b0b0b0"
                    font.pixelSize: 12
                }

                Rectangle {
                    width: 100
                    height: 1
                    color: "#b0b0b0"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Button {
                id: switchBtn
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: 50
                }
                height: 40
                hoverEnabled: true

                background: Rectangle {
                    radius: 15
                    color: switchBtn.hovered ? "#27ae60" : "#2ecc71"
                }

                contentItem: Text {
                    text: isLoginMode ? "Зарегистрироваться" : "Уже есть аккаунт? Войти"
                    font {
                        pixelSize: 20
                        bold: true
                    }
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    isLoginMode = !isLoginMode
                    loginField.clear()
                    passwordField1.clear()
                    passwordField2.clear()
                }
            }

            Label {
                id: errorLabel
                anchors.horizontalCenter: parent.horizontalCenter
                text: appController.statusMsg
                color: "#b0b0b0"
                font.pixelSize: 14
            }
        }
    }
}
