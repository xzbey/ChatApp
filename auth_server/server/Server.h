#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

#include "ClientSession.h"
#include "ValidationServer.h"

class Server: public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject* parent = nullptr);

    bool start(const quint16& port);
    void close();

    QStringList getOnlineUsers() const;
    UserStorage* getUserStorage() const;

signals:
    void userListChanged();

public slots:
    void incomingConnection(qintptr socketDescriptor);

private:
    UserStorage* userStorage;
    QList<ClientSession*> activeSessions;
    ValidationServer* validationServer;
};

#endif // SERVER_H
