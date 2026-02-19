#include "Client.h"

Client::Client(QObject* parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &Client::onError);
}


void Client::connectToServer(const QHostAddress& hostAddress, const quint16& port) {
    if (isConnected()) {
        Utils::log("Already connected");
        return;
    }

    Utils::log("Connecting to " + hostAddress.toString() + ":" + QString::number(port));
    socket->connectToHost(hostAddress, port);
}

void Client::disconnectFromServer() {
    if (!isConnected()) {
        Utils::log("Already disconnected");
        return;
    }

    Utils::log("Disconnecting from server");
    socket->disconnectFromHost();
}

void Client::sendRequest(const QJsonObject& request) {
    if (!isConnected()) {
        emit error0ccurred("Not connected to server");
        return;
    }

    QJsonDocument doc(request);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    Utils::log("Sending request " + data);
    socket->write(data);
    socket->flush();
}

bool Client::isConnected() const {
    return socket->state() == QAbstractSocket::ConnectedState;
}

void Client::onConnected() {
    Utils::log("Connected to server");
    emit connected();
}

void Client::onDisconnected() {
    Utils::log("Disconnected from server");
    emit disconnected();
}

void Client::onReadyRead() {
    QByteArray data = socket->readAll();
    Utils::log("Response received: " + data);

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        emit error0ccurred("Json response parse error: " + parseError.errorString());
        return;
    }

    if (!doc.isObject()) {
        emit error0ccurred("Json response is not obj");
        return;
    }

    emit responseReceived(doc.object());
}

void Client::onError() {
    QString error = socket->errorString();
    Utils::log("Socket error: " + error);
    emit error0ccurred(error);
}

