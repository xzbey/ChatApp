#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>

#include "AuthController.h"

class ClientSession: public QObject
{
    Q_OBJECT
signals:
    void userAuthenticated(const QString& login);
    void userRegistered(const QString& login);

public:
    ClientSession(QTcpSocket* socket, UserStorage* userStorage, QObject *parent = nullptr);

    QString getLogin() const;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket* socket;
    AuthController* authController;

    QString login;

    void sendResponse(const AuthProtocol::Response& response);
};


#endif // CLIENTSESSION_H
