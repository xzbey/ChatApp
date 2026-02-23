#ifndef VALIDATIONPROTOCOL_H
#define VALIDATIONPROTOCOL_H

#include <QString>
#include <QJsonObject>

namespace ValidationProtocol {
    inline QJsonObject makeRequest(const QString& login, const QString& token) {
        QJsonObject obj;
        obj["login"] = login;
        obj["token"] = token;
        return obj;
    }

    inline QPair<QString, QString> parseRequest(const QJsonObject& obj) {
        return {obj["login"].toString(), obj["token"].toString()};
    }

    inline QJsonObject makeResponse(bool valid) {
        QJsonObject obj;
        obj["valid"] = valid;
        return obj;
    }

    inline bool parseResponse(const QJsonObject& obj) {
        return obj["valid"].toBool();
    }
}

#endif // VALIDATIONPROTOCOL_H
