#ifndef CHATPROTOCOL_H
#define CHATPROTOCOL_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

namespace ChatProtocol {

    enum class MessageType {
        Error = 0,
        TextMessage = 1,
        BroadcastMessage = 2,
        UserListRequest = 3,
        UserListResponse = 4,
        None = 300
    };

    struct Message {
        QString from,
                to,
                msg;
    };

    inline QJsonObject makeTextMessage(const Message& request) {
        QJsonObject obj;
        obj["type"] = static_cast<unsigned short>(MessageType::TextMessage);
        obj["from"] = request.from;
        obj["to"] = request.to;
        obj["msg"] = request.msg;

        return obj;
    }

    inline QJsonObject makeBroadcastMessage(const Message& request) {
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


    inline MessageType getMessageType(const QJsonObject& obj) {
        if (obj.contains("type"))
            return static_cast<MessageType>(obj["type"].toInt());
        return MessageType::Error;
    }

    inline Message parseTextMessage(const QJsonObject& obj) {
        if (obj.contains("from") and obj.contains("to") and obj.contains("msg"))
            return {obj["from"].toString(), obj["to"].toString(), obj["msg"].toString()};
        return {"-", "-", "-"};
    }

    inline Message parseBroadcastMessage(const QJsonObject& obj) {
        if (obj.contains("from") and obj.contains("msg"))
            return {obj["from"].toString(), "-", obj["msg"].toString()};
        return {"-", "-", "-"};
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

}

#endif // CHATPROTOCOL_H
