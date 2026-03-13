#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <QObject>

#include "Client.h"
#include "ChatProtocol.h"

class ChatService: public QObject
{
    Q_OBJECT
public:
    ChatService(Client* client, QObject* parent = nullptr);

    void connectToChatServer(const QString& login, const QString& hostAddress, const quint16& port);

    void sendMessage(const QString& to, const QString& msg);
    void sendBroadcast(const QString& msg);
    void userListRequest();

signals:
    void chatConnected();
    void chatConnectionFailed(const QString& error);

    void messageReceived(const QString& from, const QString& msg);
    void broadcastReceived(const QString& from, const QString& msg);
    void userListReceived(const QStringList& users);

    void systemMessageReceived(bool success, const QString& msg);

private slots:
    void onConnected();
    void onResponseReceived(const QJsonObject& response);

private:
    Client* client;
    QString myLogin;
    bool awaitHelloResponse;

    void handleTextMessage(const QJsonObject& response);
    void handleBroadcastMessage(const QJsonObject& response);
    void handleUserListResponse(const QJsonObject& response);
    void handleSystemMessage(const QJsonObject& response);

};

#endif // CHATSERVICE_H
