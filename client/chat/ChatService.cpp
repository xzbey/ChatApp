#include "ChatService.h"

ChatService::ChatService(Client* client, QObject* parent)
    : QObject(parent), client(client)
{
    awaitHelloResponse = false;

    connect(client, &Client::connected, this, &ChatService::onConnected);
    connect(client, &Client::responseReceived, this, &ChatService::onResponseReceived);

}


void ChatService::connectToChatServer(const QString& login, const QString& hostAddress, const quint16& port) {
    myLogin = login;
    awaitHelloResponse = true;

    Utils::log("Connecting to chat server + " + hostAddress + ":" + QString::number(port));

    if (client->isConnected()) {
        connect(client, &Client::disconnected, this, [this, hostAddress, port](){
            client->connectToServer(QHostAddress(hostAddress), port);
        }, Qt::SingleShotConnection);

        Utils::log("Disconnecting from auth server");
        client->disconnectFromServer();
        return;
    }

    client->connectToServer(QHostAddress(hostAddress), port);
}

void ChatService::sendMessage(const QString& to, const QString& msg) {
    if (!client->isConnected()) {
        emit chatConnectionFailed("Not connected to chat server");
        return;
    }

    ChatProtocol::TextMessage textMsg;
    textMsg.to = to;
    textMsg.from = myLogin;
    textMsg.msg = msg;

    QJsonObject request = ChatProtocol::makeTextMessage(textMsg);

    Utils::log("Sending message to " + to + " from " + myLogin + " : " + msg);
    client->sendRequest(request);
}

void ChatService::sendBroadcast(const QString& msg) {
    if (!client->isConnected()) {
        emit chatConnectionFailed("Not connected to chat server");
        return;
    }

    ChatProtocol::BroadcastMessage broadcastMsg {myLogin, msg};

    QJsonObject request = ChatProtocol::makeBroadcastMessage(broadcastMsg);

    Utils::log("Sending broadcast message from " + myLogin + " : " + msg);
    client->sendRequest(request);
}

void ChatService::userListRequest() {
    if (!client->isConnected()) {
        emit chatConnectionFailed("Not connected to chat server");
        return;
    }

    QJsonObject request = ChatProtocol::makeUserListRequest();

    Utils::log("Requesting user list");
    client->sendRequest(request);
}

void ChatService::onConnected() {
    if (!awaitHelloResponse)
        return;
    Utils::log("Connected to chat server");

    ChatProtocol::HelloMessage helloMsg {myLogin, ""};
    QJsonObject request = ChatProtocol::makeHelloMessage(helloMsg);
    client->sendRequest(request);
}

void ChatService::onResponseReceived(const QJsonObject& response) {
    if (!awaitHelloResponse and myLogin.trimmed().isEmpty())
        return;

    ChatProtocol::MessageType msgType = ChatProtocol::getMessageType(response);
    switch (msgType) {
        case ChatProtocol::MessageType::TextMessage:
            handleTextMessage(response);
            break;

        case ChatProtocol::MessageType::BroadcastMessage:
            handleBroadcastMessage(response);
            break;

        case ChatProtocol::MessageType::UserListResponse:
            handleUserListResponse(response);
            break;

        case ChatProtocol::MessageType::SystemMessage:
            handleSystemMessage(response);
            break;

        default:
            Utils::log("Unknown message type from chat server");
            break;
    }
}

void ChatService::handleTextMessage(const QJsonObject& response) {
    ChatProtocol::TextMessage msg = ChatProtocol::parseTextMessage(response);

    Utils::log("Message received from " + msg.from + " to " + msg.to + " : " + msg.msg);

    emit messageReceived(msg.from, msg.msg);
}

void ChatService::handleBroadcastMessage(const QJsonObject& response) {
    ChatProtocol::BroadcastMessage msg = ChatProtocol::parseBroadcastMessage(response);

    Utils::log("Broadcast message received from " + msg.from + " : " + msg.msg);

    emit broadcastReceived(msg.from, msg.msg);
}

void ChatService::handleUserListResponse(const QJsonObject& response) {
    QStringList users = ChatProtocol::parseUserList(response);

    Utils::log("User list received with " + QString::number(users.size()) + " users");

    emit userListReceived(users);
}

void ChatService::handleSystemMessage(const QJsonObject& response) {
    ChatProtocol::SystemMessage msg = ChatProtocol::parseSystemMessage(response);

    Utils::log("System message received: " + msg.msg + " | Success: " + (msg.success ? "True" : "False"));

    if (awaitHelloResponse) {
        awaitHelloResponse = false;

        if (msg.success) {
            Utils::log("Success connect to chat server");
            emit chatConnected();
        } else {
            Utils::log("Failed connect to chat server");
            emit chatConnectionFailed(msg.msg);
        }
    } else
        emit systemMessageReceived(msg.success, msg.msg);
}




