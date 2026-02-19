#include "Server.h"

Server::Server(QObject* parent)
    :QTcpServer(parent)
{
    userStorage = new UserStorage(this);
}

bool Server::start(const quint16& port) {
    if (port != 0 and this->listen(QHostAddress::Any, port)) {
        Utils::log("AuthServer started / port " + QString::number(port));
        return true;
    } else {
        Utils::log("Error in start AuthServer / port " + QString::number(port));
        return false;
    }
}

void Server::close() {
    for (ClientSession* session: activeSessions)
        session->disconnectClient();
    QTcpServer::close();
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    ClientSession* session = new ClientSession(socket, userStorage, this);

    activeSessions.append(session);

    connect(session, &ClientSession::userAuthenticated, this, [this](const QString& login){
        emit userListChanged();

        QSet<QString> unique;
        for (ClientSession* s: activeSessions)
            if (!s->getLogin().isEmpty())
                unique.insert(s->getLogin());

        Utils::log("User online: " + login + " / Active sessions: " + QString::number(activeSessions.size()) + "/ Online: " + QString::number(unique.size()));
    });

    connect(session, &ClientSession::userRegistered, this, [this](const QString& login){
        emit userListChanged();
        Utils::log("Registered: " + login);
    });

    connect(session, &QObject::destroyed, this, [this, session](){
        activeSessions.removeAll(session);

        emit userListChanged();
        /*
        QString login = session->getLogin();
        if (!login.isEmpty()) {
            emit userListChanged();

            QSet<QString> unique;
            for (ClientSession* s: activeSessions)
                if (!s->getLogin().isEmpty())
                    unique.insert(s->getLogin());

            Utils::log("User offline: " + login + " / Active sessions: " + QString::number(activeSessions.size()) + "/ Online: " + QString::number(unique.size()));
        }
        */
    });

    Utils::log("New connection");
}

QStringList Server::getOnlineUsers() const {
    QSet<QString> uniqueUsers;

    for (const auto* session: activeSessions) {
        QString login = session->getLogin();
        if (!login.isEmpty()) {
            uniqueUsers.insert(login);
        }
    }

    return uniqueUsers.values();
}

UserStorage* Server::getUserStorage() const {
    return userStorage;
}
