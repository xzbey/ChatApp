#include "ApplicationController.h"

ApplicationController::ApplicationController(QObject* parent)
    : QObject(parent)
{
    client = new Client(this);
    authService = new AuthService(client, this);

    connect(client, &Client::connected, this, &ApplicationController::onConnected);
    connect(client, &Client::disconnected, this, &ApplicationController::onDisconnect);
    connect(client, &Client::error0ccurred, this, &ApplicationController::onError);

    connect(authService, &AuthService::loginSuccess, this, &ApplicationController::onLoginSuccess);
    connect(authService, &AuthService::loginFailed, this, &ApplicationController::onLoginFailed);
    connect(authService, &AuthService::registrationSuccess, this, &ApplicationController::onRegistrationSuccess);
    connect(authService, &AuthService::registrationFailed, this, &ApplicationController::onRegistrationFailed);

    //connectToServer(QHostAddress::LocalHost, 50000);
}

Q_INVOKABLE void ApplicationController::connectToServer(const QString& hostAddress, const quint16& port) {
    setStatusMsg("Connecting to server...");
    client->connectToServer(QHostAddress(hostAddress), port);
}

Q_INVOKABLE void ApplicationController::login(const QString& login, const QString& password) {
    if (!client->isConnected()) {
        connectToServer(serverData.first.toString(), serverData.second);

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
        connectToServer(serverData.first.toString(), serverData.second);

        connect(client, &Client::connected, this, [this, login, password]() {
            setStatusMsg("Registration...");
            authService->registration(login, password);
        }, Qt::SingleShotConnection);

        return;
    }

    setStatusMsg("Registration...");
    authService->registration(login, password);
}

QString ApplicationController::getStatusMsg() const {
    return statusMsg;
}

bool ApplicationController::isConnected() const {
    return client->isConnected();
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
    emit authSuccess();
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


void ApplicationController::setStatusMsg(const QString& msg) {
    if (statusMsg != msg) {
        statusMsg = msg;
        emit statusMsgChanged();
        Utils::log("Status: " + msg);
    }
    Utils::log("Status are the same");
}
