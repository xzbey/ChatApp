#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QObject>

#include "Client.h"
#include "AuthProtocol.h"

class AuthService: public QObject
{
    Q_OBJECT
public:
    AuthService(Client* client, QObject* parent = nullptr);

    void login(const QString& login, const QString& password);
    void registration(const QString& login, const QString& password);

signals:
    void loginSuccess(const QString& msg);
    void loginFailed(const QString& error);

    void registrationSuccess(const QString& msg);
    void registrationFailed(const QString& error);

private slots:
    void onResponseReceived(const QJsonObject& response);

private:
    Client* client;
    AuthProtocol::RequestType viewedOperation = AuthProtocol::RequestType::None;
};

#endif // AUTHSERVICE_H