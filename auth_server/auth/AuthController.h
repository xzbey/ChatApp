#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>

#include "Utils.h"
#include "AuthProtocol.h"
#include "UserStorage.h"

class AuthController: public QObject
{
    Q_OBJECT
public:
    AuthController(UserStorage* storage, QObject* parent = nullptr);

    AuthProtocol::Response handleRequest(const AuthProtocol::Request& request);

private:
    UserStorage* userStorage;

    AuthProtocol::Response handleLogin(const QString& login, const QString& password);

    AuthProtocol::Response handleRegister(const QString& login, const QString& password);

    bool isCorrectInput(const QString& login, const QString& password, QString& error);
};

#endif // AUTHCONTROLLER_H