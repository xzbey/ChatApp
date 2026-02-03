#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>

#include "AuthService.h"

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

    QString getStatusMsg() const;
    bool isConnected() const;

signals:
    void statusMsgChanged();
    void isConnectedChanged();
    void authSuccess();
    void registerSuccess();

private slots:
    void onConnected();
    void onDisconnect();
    void onLoginSuccess(const QString& msg);
    void onLoginFailed(const QString& error);
    void onRegistrationSuccess(const QString& msg);
    void onRegistrationFailed(const QString& error);
    void onError(const QString& error);

private:
    Client* client;
    AuthService* authService;
    QString statusMsg;

    QPair<QHostAddress, quint16> serverData = {QHostAddress::LocalHost, 50000};

    void setStatusMsg(const QString& msg);
};

#endif // APPLICATIONCONTROLLER_H
