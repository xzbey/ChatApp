#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

#include "ClientSession.h"
#include "AuthValidator.h"

class Server: public QTcpServer {
    Q_OBJECT
public:
    Server(QObject* parent = nullptr);

    bool start(const quint16& port);
    void close();

    QStringList getOnlineUsers() const;

signals:
    void userListChanged();

public slots:
    void incomingConnection(qintptr socketDescriptor);

private slots:
    void onMessageForUser(const QString& receiver, const QJsonObject& msg);
    void onBroadcastMessage(const QString& exceptMyLogin, const QJsonObject& msg);
    void onUserListRequest(const QString& login);

private:
    AuthValidator* authValidator;

    QList<ClientSession*> activeSessions;
    QMap<QString, ClientSession*> loginToSession;

    void sendUserListTo(const QString& login);
};

#endif // SERVER_H
