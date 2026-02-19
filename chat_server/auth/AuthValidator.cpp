#include "AuthValidator.h"

AuthValidator::AuthValidator(QObject* parent)
    : QObject(parent)
{
    registerUsers.insert("admin");
}


bool AuthValidator::validateClient(const QString& login, const QString& token) {
    /*Utils::log("Validating client: " + login + " | Token = " + token);

    if (validateToken(token)) {
        bool exists = userExists(login);
        if (exists)
            Utils::log("Client is valid: " + login);
        else
            Utils::log("Client is not valid " + login);

        return exists;
    }

    Utils::log("Token is not valid: " + login + " | Token = " + token);
    return false;*/
    return true;
}

bool AuthValidator::validateToken(const QString& token) {
    //потом с токеном
    return true;
}

void AuthValidator::loadRegisterUsers(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Utils::log("File with users is not open: " + path);
        return;
    }

    QTextStream in(&file);
    registerUsers.clear();

    QString login;
    while (!in.atEnd()) {
        login = in.readLine().trimmed();
        if (!login.isEmpty())
            registerUsers.insert(login);
    }

    file.close();
    Utils::log("Load " + QString::number(registerUsers.size()) + " users");
}

bool AuthValidator::userExists(const QString& login) const {
    return registerUsers.contains(login);
}
