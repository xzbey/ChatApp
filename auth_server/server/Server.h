#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

#include "ClientSession.h"

class Server: public QTcpServer
{
    Q_OBJECT
signals:
    void userListChanged();

public:
    Server(QObject* parent = nullptr);

    bool start(const quint16& port);

    QStringList getOnlineUsers() const;
    UserStorage* getUserStorage() const;

public slots:
    void incomingConnection(qintptr socketDescriptor);

private:
    UserStorage* userStorage;
    QList<ClientSession*> activeSessions;
};

#endif // SERVER_H
