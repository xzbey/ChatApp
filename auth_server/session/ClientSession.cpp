#include "ClientSession.h"

ClientSession::ClientSession(QTcpSocket* socket, UserStorage* userStorage, QObject *parent)
    :QObject(parent), socket(socket)
{
    connect(socket, &QTcpSocket::readyRead, this, &ClientSession::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientSession::onDisconnected);

    authController = new AuthController(userStorage, this);
}

void ClientSession::onReadyRead() {
    QByteArray data = socket->readAll();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        Utils::log("Json parse error: " + parseError.errorString());
        return;
    }

    if (!doc.isObject()) {
        Utils::log("Json is not obj");
        return;
    }

    QJsonObject obj = doc.object();

    AuthProtocol::Request request = AuthProtocol::parseRequest(obj);

    if (request.type == AuthProtocol::RequestType::Error) {
        Utils::log("Error in request");
        sendResponse({false, "Error in request"});
        return;
    }

    AuthProtocol::Response response = authController->handleRequest(request);
    sendResponse(response);
    socket->flush();

    if (response.success) {
        if (request.type == AuthProtocol::RequestType::Login) {
            login = request.login;
            emit userAuthenticated(request.login);
        } else if (request.type == AuthProtocol::RequestType::Registration) {
            emit userRegistered(request.login);
        }
    }

}

void ClientSession::onDisconnected() {
    Utils::log("Client disconnected");
    socket->deleteLater();
    deleteLater();
}

void ClientSession::sendResponse(const AuthProtocol::Response& response) {
    QJsonObject obj = AuthProtocol::makeResponse(response);
    QJsonDocument doc(obj);

    socket->write(doc.toJson(QJsonDocument::Compact));
}

QString ClientSession::getLogin() const {
    return login;
}
