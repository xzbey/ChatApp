#ifndef CHATPROTOCOL_H
#define CHATPROTOCOL_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

namespace ChatProtocol {

    enum class MessageType {
        Error = 0,
        Hello = 1,
        TextMessage = 2,
        BroadcastMessage = 3,
        UserListRequest = 4,
        UserListResponse = 5,
        SystemMessage = 6,
        None = 300
    };

    struct HelloMessage {
        QString login,
                token;
    };

    struct BroadcastMessage {
        QString from,
                msg;
    };

    struct TextMessage: BroadcastMessage {
        QString to;
    };

    struct SystemMessage {
        bool success;
        QString msg;
    };


    inline QJsonObject makeHelloMessage(const HelloMessage& msg) {
        QJsonObject obj;
        obj["type"] = static_cast<unsigned short>(MessageType::Hello);
        obj["login"] = msg.login;
        obj["token"] = msg.token;

        return obj;
    }

    inline QJsonObject makeTextMessage(const TextMessage& request) {
        QJsonObject obj;
        obj["type"] = static_cast<unsigned short>(MessageType::TextMessage);
        obj["from"] = request.from;
        obj["to"] = request.to;
        obj["msg"] = request.msg;

        return obj;
    }

    inline QJsonObject makeBroadcastMessage(const BroadcastMessage& request) {
        QJsonObject obj;
        obj["type"] = static_cast<unsigned short>(MessageType::BroadcastMessage);
        obj["from"] = request.from;
        obj["msg"] = request.msg;

        return obj;
    }

    inline QJsonObject makeUserListRequest() {
        QJsonObject obj;
        obj["type"] = static_cast<unsigned short>(MessageType::UserListRequest);

        return obj;
    }

    inline QJsonObject makeUserListResponse(const QStringList& users) {
        QJsonObject obj;
        obj["type"] = static_cast<unsigned short>(MessageType::UserListResponse);

        QJsonArray usersArray;
        for (const QString& user: users)
            usersArray.append(user);

        obj["users"] = usersArray;

        return obj;
    }

    inline QJsonObject makeSystemMessage(const SystemMessage& msg) {
        QJsonObject obj;
        obj["type"] = static_cast<unsigned short>(MessageType::SystemMessage);
        obj["success"] = msg.success;
        obj["msg"] = msg.msg;

        return obj;
    }


    inline MessageType getMessageType(const QJsonObject& obj) {
        if (obj.contains("type"))
            return static_cast<MessageType>(obj["type"].toInt());
        return MessageType::Error;
    }

    inline HelloMessage parseHelloMessage(const QJsonObject& obj) {
        if (obj.contains("login") and obj.contains("token"))
            return {obj["login"].toString(), obj["token"].toString()};
        return {"-", "-"};
    }

    inline TextMessage parseTextMessage(const QJsonObject& obj) {
        if (obj.contains("from") and obj.contains("to") and obj.contains("msg"))
            return {obj["from"].toString(), obj["to"].toString(), obj["msg"].toString()};
        return {"-", "-", "-"};
    }

    inline BroadcastMessage parseBroadcastMessage(const QJsonObject& obj) {
        if (obj.contains("from") and obj.contains("msg"))
            return {obj["from"].toString(), obj["msg"].toString()};
        return {"-", "-"};
    }

    inline QStringList parseUserList(const QJsonObject& obj) {
        QStringList users;
        if (obj.contains("users") and obj["users"].isArray()) {
            QJsonArray usersArray = obj["users"].toArray();

            for (const QJsonValue& value: usersArray)
                users.append(value.toString());
        }
        return users;
    }

    inline SystemMessage parseSystemMessage(const QJsonObject& obj) {
        if (obj.contains("success") and obj.contains("msg"))
            return {obj["success"].toBool(), obj["msg"].toString()};
        return {false, "-"};
    }

}

#endif // CHATPROTOCOL_H
