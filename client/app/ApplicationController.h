#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>

#include "AuthService.h"
#include "ChatService.h"

class ApplicationController: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString statusMsg READ getStatusMsg NOTIFY statusMsgChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
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

signals:
    void statusMsgChanged();
    void isConnectedChanged();
    void authSuccess();
    void registerSuccess();

    void chatConnected();
    void messageReceived();
    void broadcastReceived();
    void userListReceived();

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

private:
    Client* client;
    AuthService* authService;
    ChatService* chatService;

    QString statusMsg;
    QString myLogin;

    QPair<QHostAddress, quint16> serverData = {QHostAddress::LocalHost, 50000};

    void setStatusMsg(const QString& msg);
};

#endif // APPLICATIONCONTROLLER_H
