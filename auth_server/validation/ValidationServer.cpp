#include "ValidationServer.h"

ValidationServer::ValidationServer(UserStorage* userStorage, QObject* parent)
    : QTcpServer(parent), userStorage(userStorage)
{}

bool ValidationServer::start(const quint16& port) {
    if (port != 0 and this->listen(QHostAddress::Any, port)) {
        Utils::log("ValidationServer started / port " + QString::number(port));
        return true;
    } else {
        Utils::log("Error in start ValidationServer / port " + QString::number(port));
        return false;
    }
}

void ValidationServer::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, [this, socket](){
        handleRequest(socket);
    });

    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Utils::log("ValidationServer have new connection");

}

void ValidationServer::handleRequest(QTcpSocket* socket) {
    QByteArray data = socket->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError or !doc.isObject()) {
        Utils::log("ValidationServer parse error: " + parseError.errorString());
        socket->disconnectFromHost();
        return;
    }

    auto [login, token] = ValidationProtocol::parseRequest(doc.object());
    bool validLogin = userStorage->userExists(login),
         validToken = true,
         valid = validLogin and validToken;

    Utils::log("ValidationServer: validating " + login + " / valid: " + QString(valid ? "true" : "false"));

    QJsonDocument response(ValidationProtocol::makeResponse(valid));
    socket->write(response.toJson(QJsonDocument::Compact));
    socket->flush();
    socket->disconnectFromHost();


}



