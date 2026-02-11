#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QObject>
#include <QTcpSocket>

#include "AuthValidator.h"
#include "MessageRouter.h"
#include "ChatProtocol.h"

class ClientSession: public QObject
{
    Q_OBJECT
public:
    ClientSession(QTcpSocket* socket, AuthValidator* authValidator, QObject* parent = nullptr);
    ~ClientSession();

    QString getLogin() const;
    bool isAuthenticated() const;
    QString getClientAddress() const;

    void sendMsg(const QJsonObject& msg) const;
signals:
    void clientAuthenticated(const QString& login);
    void clientDisconnected(const QString& login);

    void messageForUser(const QString& receiver, const QJsonObject& msg);
    void broadcastMessage(const QString& exceptMyLogin, const QJsonObject& msg);
    void userListRequest(const QString& login);

private slots:
    void onReadyRead();
    void onDisconnected();
    void onSocketError();

private:
    QTcpSocket* socket;
    AuthValidator* authValidator;
    MessageRouter* msgRouter;

    QString login;
    bool authenticated;

    void handleHelloMessage(const QJsonObject& msg);
    void handleChatMessage(const QJsonObject& msg);
    void sendSystemMessage(bool success, const QString& msg);
};

#endif // CLIENTSESSION_H
