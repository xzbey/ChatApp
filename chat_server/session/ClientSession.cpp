#include "ClientSession.h"

ClientSession::ClientSession(QTcpSocket* socket, AuthValidator* authValidator, QObject* parent)
    : QObject(parent), socket(socket), authValidator(authValidator), authenticated(false)
{
    msgRouter = new MessageRouter(this);

    connect(socket, &QTcpSocket::readyRead, this, &ClientSession::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientSession::onDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &ClientSession::onSocketError);

    connect(msgRouter, &MessageRouter::forwardMessage, this, &ClientSession::messageForUser);
    connect(msgRouter, &MessageRouter::broadcastMessage, this, &ClientSession::broadcastMessage);
    connect(msgRouter, &MessageRouter::userListMessage, this, &ClientSession::userListRequest);

    Utils::log("New connected client: " + getClientAddress());
}

ClientSession::~ClientSession() {
    if (socket) {
        if (!pendingDelete) {
            pendingDelete = true;
            Utils::log("Socket deleting. Penging delete: False->True");
            socket->deleteLater();
        }
        else
            Utils::log("Object already in delete queue. Penging delete = True");
    } else
        Utils::log("Failed socket");
}

QString ClientSession::getLogin() const {
    return login;
}

bool ClientSession::isAuthenticated() const {
    return authenticated;
}

QString ClientSession::getClientAddress() const {
    if (!socket)
        return "";
    return socket->peerAddress().toString() + ":" + QString::number(socket->peerPort());
}

void ClientSession::sendMsg(const QJsonObject& msg) const {
    if (!socket or socket->state() != QAbstractSocket::ConnectedState)
        return;

    QJsonDocument doc(msg);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    socket->write(data);
    socket->flush();
}

void ClientSession::onReadyRead() {
    QByteArray data = socket->readAll();

    Utils::log("Get data from " + getClientAddress() + " | " + data);

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        Utils::log("Json parse error: " + parseError.errorString());
        sendSystemMessage(false, "Json parse error: " + parseError.errorString());
        return;
    }

    if (!doc.isObject()) {
        Utils::log("Json is not obj");
        sendSystemMessage(false, "Json is not obj");
        return;
    }

    QJsonObject obj = doc.object();

    if (authenticated)
        handleChatMessage(obj);
    else
        handleHelloMessage(obj);

}

void ClientSession::onDisconnected() {
    Utils::log("Client disconnected: " + login + " | " + getClientAddress());

    emit clientDisconnected(login);

    Utils::log("Client disconnected");
    if (!pendingDelete) {
        pendingDelete = true;
        Utils::log("Socket deleting. Penging delete: False->True");
        socket->deleteLater();
        deleteLater();
    } else
        Utils::log("Object already in delete queue. Penging delete = True");
}

void ClientSession::onSocketError() {
    Utils::log("Socket error: " + getClientAddress() + " | Error: " + socket->errorString());
}

void ClientSession::handleHelloMessage(const QJsonObject& msg) {
    ChatProtocol::MessageType typeMsg = ChatProtocol::getMessageType(msg);

    if (typeMsg != ChatProtocol::MessageType::Hello) {
        Utils::log("Unknown handle: Not hello message");
        sendSystemMessage(false, "Unknown handle: Not hello message");
        socket->disconnectFromHost();
        return;
    }

    ChatProtocol::HelloMessage helloMsg = ChatProtocol::parseHelloMessage(msg);

    Utils::log("Hello msg from client: " + helloMsg.login + " | Token = " + helloMsg.token);

    if (!authValidator->validateClient(helloMsg.login, helloMsg.token)) {
        Utils::log("Client is invalid: " + helloMsg.login + " | Token = " + helloMsg.token);
        sendSystemMessage(false, "Client is invalid: " + helloMsg.login + " | Token = " + helloMsg.token);
        socket->disconnectFromHost();
        return;
    }

    login = helloMsg.login;
    authenticated = true;

    Utils::log("Successful client auth: " + login);
    sendSystemMessage(true, "Successful auth");

    emit clientAuthenticated(login);
}

void ClientSession::handleChatMessage(const QJsonObject& msg) {
    msgRouter->handleMessage(login, msg);
}

void ClientSession::sendSystemMessage(bool success, const QString& msg) {
    ChatProtocol::SystemMessage sysMsg {success, msg};

    QJsonObject obj = ChatProtocol::makeSystemMessage(sysMsg);
    sendMsg(obj);
}

void ClientSession::disconnectClient() {
    if (socket and socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();
}
