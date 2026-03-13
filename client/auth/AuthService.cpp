#include "AuthService.h"

AuthService::AuthService(Client* client, QObject* parent)
    : QObject(parent), client(client)
{
    connect(client, &Client::responseReceived, this, &AuthService::onResponseReceived);
}


void AuthService::login(const QString& login, const QString& password) {
    if (!client->isConnected()) {
        emit loginFailed("Not connected to auth server");
        return;
    }

    Utils::log("Login attempt: " + login);
    AuthProtocol::Request request = {AuthProtocol::RequestType::Login, login, password};
    viewedOperation = request.type;

    client->sendRequest(AuthProtocol::makeRequest(request));
}

void AuthService::registration(const QString& login, const QString& password) {
    if (!client->isConnected()) {
        emit loginFailed("Not connected to auth server");
        return;
    }

    Utils::log("Registration attempt: " + login);
    AuthProtocol::Request request = {AuthProtocol::RequestType::Registration, login, password};
    viewedOperation = request.type;

    client->sendRequest(AuthProtocol::makeRequest(request));
}


void AuthService::onResponseReceived(const QJsonObject& response) {
    AuthProtocol::Response authResponse = AuthProtocol::parseResponse(response);

    Utils::log("Auth response / Success: " + QString(authResponse.success == true ? "True" : "False") + " / Msg: " + authResponse.msg);

    switch (viewedOperation) {
    case AuthProtocol::RequestType::Login:
        if (authResponse.success)
            emit loginSuccess(authResponse.msg);

        else
            emit loginFailed(authResponse.msg);

        break;

    case AuthProtocol::RequestType::Registration:
        if (authResponse.success)
            emit registrationSuccess(authResponse.msg);

        else
            emit registrationFailed(authResponse.msg);

        break;

    default:
        Utils::log("Unknown response");
        break;
    }

    viewedOperation = AuthProtocol::RequestType::None;
}
