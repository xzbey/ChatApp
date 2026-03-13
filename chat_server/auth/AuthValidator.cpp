#include "AuthValidator.h"

AuthValidator::AuthValidator(QObject* parent)
    : QObject(parent)
{}


void AuthValidator::validateClient(const QString& login, const QString& token) {
    QTcpSocket* socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, [socket, login, token](){
        QJsonDocument doc(ValidationProtocol::makeRequest(login, token));
        socket->write(doc.toJson(QJsonDocument::Compact));
        socket->flush();
    });

    connect(socket, &QTcpSocket::readyRead, this, [this, socket, login, token](){
        onReadyRead(socket, login, token);
    });

    connect(socket, &QTcpSocket::errorOccurred, this, [this, socket, login, token](){
        onError(socket, login, token);
    });

    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    socket->connectToHost(QHostAddress::LocalHost, port);
}

void AuthValidator::onReadyRead(QTcpSocket* socket, const QString& login, const QString& token) {
    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    bool valid = false;
    if (doc.isObject())
        valid = ValidationProtocol::parseResponse(doc.object());

    Utils::log("Validation result for " + login + ": valid = " + QString(valid ? "true" : "false"));

    emit validationResult(login, token, valid);
    socket->disconnectFromHost();
}

void AuthValidator::onError(QTcpSocket* socket, const QString& login, const QString& token) {
    Utils::log("Validation error for " + login + ": " + socket->errorString());
    emit validationResult(login, token, false);
    socket->deleteLater();
}
