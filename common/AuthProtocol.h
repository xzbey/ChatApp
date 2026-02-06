#ifndef AUTHPROTOCOL_H
#define AUTHPROTOCOL_H

#include <QString>
#include <QJsonObject>

namespace AuthProtocol {

    enum class RequestType {
        Error = 0,
        Login = 1,
        Registration = 2,
        None = 300
    };

    struct Request {
        RequestType type;
        QString login,
                password;
    };

    struct Response {
        bool success;
        QString msg;
    };

    inline QJsonObject makeRequest(const Request& request) {
        QJsonObject obj;
        obj["type"] = static_cast<unsigned short>(request.type);
        obj["login"] = request.login;
        obj["password"] = request.password;

        return obj;
    }

    inline Request parseRequest(const QJsonObject& obj) {
        Request request;
        if (obj.contains("type") and obj.contains("login") and obj.contains("password")) {
            request.type = static_cast<RequestType>(obj["type"].toInt());
            request.login = obj["login"].toString();
            request.password = obj["password"].toString();
        } else
            request.type = RequestType::Error;

        return request;
    }

    inline QJsonObject makeResponse(const Response& response) {
        QJsonObject obj;
        obj["success"] = response.success;
        obj["msg"] = response.msg;
        return obj;
    }

    inline Response parseResponse(const QJsonObject& obj) {
        Response response;
        if (obj.contains("success") and obj.contains("msg")) {
            response.success = obj["success"].toBool();
            response.msg = obj["msg"].toString();
        } else {
            response.success = false;
            response.msg = "Unknown struct";
        }

        return response;
    }
}

#endif // AUTHPROTOCOL_H
