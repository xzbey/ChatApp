#include "MessageRouter.h"

MessageRouter::MessageRouter(QObject* parent)
    : QObject(parent)
{}


void MessageRouter::handleMessage(const QString& senderLogin, const QJsonObject& msg) {
    ChatProtocol::MessageType msgType = ChatProtocol::getMessageType(msg);

    switch (msgType) {
        case (ChatProtocol::MessageType::TextMessage):
        {
            ChatProtocol::TextMessage textMsg = ChatProtocol::parseTextMessage(msg);

            if (textMsg.from != senderLogin)
                textMsg.from = senderLogin;

            QJsonObject forwardMsg = ChatProtocol::makeTextMessage(textMsg);

            emit forwardMessage(textMsg.to, forwardMsg);
            break;
        }

        case (ChatProtocol::MessageType::BroadcastMessage):
        {
            ChatProtocol::BroadcastMessage broadcastMsg = ChatProtocol::parseBroadcastMessage(msg);

            if (broadcastMsg.from != senderLogin)
                broadcastMsg.from = senderLogin;

            QJsonObject broadcastMsg_json = ChatProtocol::makeBroadcastMessage(broadcastMsg);

            emit broadcastMessage(broadcastMsg.from, broadcastMsg_json);
            break;
        }

        case (ChatProtocol::MessageType::UserListRequest):
        {
            Utils::log("User list request from " + senderLogin);

            emit userListMessage(senderLogin);
            break;
        }
        default:
        {
            Utils::log("Unknown request from " + senderLogin);
            break;
        }
    }
}

