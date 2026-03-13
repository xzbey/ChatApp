#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>

#include "AuthService.h"
#include "ChatService.h"
#include "ChatListModel.h"

class ApplicationController: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString statusMsg READ getStatusMsg NOTIFY statusMsgChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(ChatListModel* chatListModel READ getChatListModel CONSTANT)
    Q_PROPERTY(QString myLogin READ getMyLogin CONSTANT)

public:
    ApplicationController(QObject* parent = nullptr);

    Q_INVOKABLE void connectToServer(const QString& hostAddress, const quint16& port);
    Q_INVOKABLE void login(const QString& login, const QString& password);
    Q_INVOKABLE void registration(const QString& login, const QString& password);

    Q_INVOKABLE void sendMessage(const QString& to, const QString& msg);
    Q_INVOKABLE void sendBroadcast(const QString& msg);
    Q_INVOKABLE void userListRequest();

    QString getStatusMsg() const;
    bool isConnected() const;
    ChatListModel* getChatListModel() const;
    QString getMyLogin() const;

signals:
    void statusMsgChanged();
    void isConnectedChanged();
    void authSuccess();
    void registerSuccess();

    void chatConnected();
    void chatDisconnected();

    void messageReceived(const QString& from, const QString& msg);
    void broadcastReceived(const QString& from, const QString& msg);
    void userListReceived(const QStringList& users);

private slots:
    void onConnected();
    void onDisconnect();
    void onLoginSuccess(const QString& msg);
    void onLoginFailed(const QString& error);
    void onRegistrationSuccess(const QString& msg);
    void onRegistrationFailed(const QString& error);
    void onError(const QString& error);

    void onChatConnected();
    void onChatConnectionFailed(const QString& error);

    void onMessageReceived(const QString& from, const QString& msg);
    void onBroadcastReceived(const QString& from, const QString& msg);

private:
    Client* client;
    AuthService* authService;
    ChatService* chatService;
    ChatListModel* chatListModel;

    QString statusMsg;
    QString myLogin;

    bool inChat = false;

    QPair<QHostAddress, quint16> authServerData = {QHostAddress::LocalHost, 50000};
    QPair<QHostAddress, quint16> chatServerData = {QHostAddress::LocalHost, 50001};

    void setStatusMsg(const QString& msg);
};

#endif // APPLICATIONCONTROLLER_H
