#include "ApplicationController.h"

ApplicationController::ApplicationController(QObject* parent)
    : QObject(parent)
{
    client = new Client(this);
    authService = new AuthService(client, this);
    chatService = new ChatService(client, this);
    chatListModel = new ChatListModel(this);

    connect(client, &Client::connected, this, &ApplicationController::onConnected);
    connect(client, &Client::disconnected, this, &ApplicationController::onDisconnect);
    connect(client, &Client::error0ccurred, this, &ApplicationController::onError);

    connect(authService, &AuthService::loginSuccess, this, &ApplicationController::onLoginSuccess);
    connect(authService, &AuthService::loginFailed, this, &ApplicationController::onLoginFailed);
    connect(authService, &AuthService::registrationSuccess, this, &ApplicationController::onRegistrationSuccess);
    connect(authService, &AuthService::registrationFailed, this, &ApplicationController::onRegistrationFailed);

    connect(chatService, &ChatService::chatConnected, this, &ApplicationController::onChatConnected);
    connect(chatService, &ChatService::chatConnectionFailed, this, &ApplicationController::onChatConnectionFailed);

    connect(chatService, &ChatService::messageReceived, this, &ApplicationController::onMessageReceived);
    connect(chatService, &ChatService::broadcastReceived, this, &ApplicationController::onBroadcastReceived);
    connect(chatService, &ChatService::userListReceived, this, &ApplicationController::userListReceived);

    //connectToServer(QHostAddress::LocalHost, 50000);
}

Q_INVOKABLE void ApplicationController::connectToServer(const QString& hostAddress, const quint16& port) {
    setStatusMsg("Connecting to server...");
    client->connectToServer(QHostAddress(hostAddress), port);
}

Q_INVOKABLE void ApplicationController::login(const QString& login, const QString& password) {
    myLogin = login;

    if (!client->isConnected()) {
        connectToServer(authServerData.first.toString(), authServerData.second);

        connect(client, &Client::connected, this, [this, login, password]() {
            setStatusMsg("Authorization...");
            authService->login(login, password);
        }, Qt::SingleShotConnection);

        return;
    }
    setStatusMsg("Authorization...");
    authService->login(login, password);

}

Q_INVOKABLE void ApplicationController::registration(const QString& login, const QString& password) {
    if (!client->isConnected()) {
        connectToServer(authServerData.first.toString(), authServerData.second);

        connect(client, &Client::connected, this, [this, login, password]() {
            setStatusMsg("Registration...");
            authService->registration(login, password);
        }, Qt::SingleShotConnection);

        return;
    }

    setStatusMsg("Registration...");
    authService->registration(login, password);
}

Q_INVOKABLE void ApplicationController::sendMessage(const QString& to, const QString& msg) {
    chatListModel->getMessageModel("private_" + to)->addMessage({myLogin, msg});
    chatService->sendMessage(to, msg);
}

Q_INVOKABLE void ApplicationController::sendBroadcast(const QString& msg) {
    chatListModel->getMessageModel("broadcast")->addMessage({myLogin, msg});
    chatService->sendBroadcast(msg);
}

Q_INVOKABLE void ApplicationController::userListRequest() {
    chatService->userListRequest();
}


QString ApplicationController::getStatusMsg() const {
    return statusMsg;
}

bool ApplicationController::isConnected() const {
    return client->isConnected();
}

ChatListModel* ApplicationController::getChatListModel() const {
    return chatListModel;
}

void ApplicationController::onConnected() {
    setStatusMsg("Connected to server");
    emit isConnectedChanged();
}

void ApplicationController::onDisconnect() {
    setStatusMsg("Disconnected from server");
    emit isConnectedChanged();
}

void ApplicationController::onLoginSuccess(const QString& msg) {
    setStatusMsg(msg);
    Utils::log("Successful login. Emit authorization");
    chatService->connectToChatServer(myLogin, chatServerData.first.toString(), chatServerData.second);
}

void ApplicationController::onLoginFailed(const QString& error) {
    setStatusMsg("Login error: " + error);
}

void ApplicationController::onRegistrationSuccess(const QString& msg) {
    setStatusMsg(msg);
    emit registerSuccess();
}

void ApplicationController::onRegistrationFailed(const QString& error) {
    setStatusMsg("Registration error: " + error);
}

void ApplicationController::onError(const QString& error) {
    setStatusMsg("Error: " + error);
    emit isConnectedChanged();
}

void ApplicationController::onChatConnected() {
    setStatusMsg("Connected to chat");
    Utils::log("Successful connect to chat server");
    emit authSuccess();
}

void ApplicationController::onChatConnectionFailed(const QString& error) {
    setStatusMsg("Chat connection failed: " + error);
}

void ApplicationController::onMessageReceived(const QString& from, const QString& msg) {
    chatListModel->addChat("private_" + from, from);
    chatListModel->getMessageModel("private_" + from)->addMessage({from, msg});

    emit messageReceived(from, msg);
}

void ApplicationController::onBroadcastReceived(const QString& from, const QString& msg) {
    chatListModel->getMessageModel("broadcast")->addMessage({from, msg});
    emit broadcastReceived(from, msg);
}

void ApplicationController::setStatusMsg(const QString& msg) {
    if (statusMsg != msg) {
        statusMsg = msg;
        emit statusMsgChanged();
        Utils::log("Status: " + msg);
    }
    else Utils::log("Status are the same");
}
