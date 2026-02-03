#include "AuthController.h"

AuthController::AuthController(UserStorage* storage, QObject* parent)
    : QObject(parent), userStorage(storage) {}


AuthProtocol::Response AuthController::handleRequest(const AuthProtocol::Request& request) {
    QString error;
    if (!isCorrectInput(request.login, request.password, error))
        return {false, error};

    switch (request.type) {
        case AuthProtocol::RequestType::Login:
            return handleLogin(request.login, request.password);

        case AuthProtocol::RequestType::Registration:
            return handleRegister(request.login, request.password);

        default:
            return {false, "Unknown request type"};
    }
}

AuthProtocol::Response AuthController::handleLogin(const QString& login, const QString& password) {
    Utils::log("Login attempt: " + login);

    if (!userStorage->userExists(login)) {
        Utils::log("Login failed: user " + login + " not found");
        return {false, "User not found"};
    }

    if (!userStorage->checkPassword(login, password)) {
        Utils::log("Login failed: wrong password for user " + login);
        return {false, "Wrong password"};
    }

    Utils::log("Successful login: " + login);
    return {true, "Successful login"};
}

AuthProtocol::Response AuthController::handleRegister(const QString& login, const QString& password) {
    Utils::log("Registration attempt: " + login);

    if (userStorage->userExists(login)) {
        Utils::log("Registration failed: user " + login + " is exists");
        return {false, "User is exists"};
    }

    if (userStorage->registerUser(login, password)) {
        Utils::log("Successful registration: " + login);
        return {true, "Successful registration"};
    }

    return {false, "Registration failed"};
}

bool AuthController::isCorrectInput(const QString& login, const QString& password, QString& error) {
    if (login.trimmed().isEmpty()) {
        error = "Empty login";
        return false;
    }
    if (password.trimmed().isEmpty()) {
        error = "Empty password";
        return false;
    }
    return true;
}