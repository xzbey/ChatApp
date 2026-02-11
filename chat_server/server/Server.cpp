#include "Server.h"

Server::Server(QObject* parent)
    : QTcpServer(parent)
{
    authValidator = new AuthValidator(this);
}

bool Server::start(const quint16& port) {
    if (port != 0 and this->listen(QHostAddress::Any, port)) {
        Utils::log("ChatServer started / port " + QString::number(port));
        return true;
    } else {
        Utils::log("Error in start ChatServer / port " + QString::number(port));
        return false;
    }
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

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    ClientSession* session = new ClientSession(socket, authValidator, this);
    activeSessions.append(session);

    connect(session, &ClientSession::clientAuthenticated, this, [this](const QString& login){
        ClientSession* session = qobject_cast<ClientSession*>(sender());
        if (session)
            loginToSession[login] = session;

        emit userListChanged();

        QSet<QString> unique;
        for (ClientSession* s: activeSessions)
            if (!s->getLogin().isEmpty())
                unique.insert(s->getLogin());

        Utils::log("User online: " + login + " / Active sessions: " + QString::number(activeSessions.size()) + "/ Online: " + QString::number(unique.size()));
    });

    connect(session, &ClientSession::clientDisconnected, this, [this, session](const QString& login){
        if (!login.isEmpty())
            loginToSession.remove(login);

        activeSessions.removeAll(session);
        emit userListChanged();
    });

    connect(session, &ClientSession::messageForUser, this, &Server::onMessageForUser);
    connect(session, &ClientSession::broadcastMessage, this, &Server::onBroadcastMessage);
    connect(session, &ClientSession::userListRequest, this, &Server::onUserListRequest);

    Utils::log("New connection");
}

void Server::onMessageForUser(const QString& receiver, const QJsonObject& msg) {
    Utils::log("Forwarding msg to " + receiver);

    if (loginToSession.contains(receiver)) {
        loginToSession[receiver]->sendMsg(msg);
        Utils::log("Msg from " + receiver + " sended");
    } else
        Utils::log("Receiver is offline: " + receiver);
}

void Server::onBroadcastMessage(const QString& exceptMyLogin, const QJsonObject& msg) {
    Utils::log("Broadcast msg from " + exceptMyLogin);

    int deliveredCount = 0;
    for (auto it = loginToSession.begin(); it != loginToSession.end(); it++)
        if (it.key() != exceptMyLogin) {
            it.value()->sendMsg(msg);
            deliveredCount++;
        }

    Utils::log("Broadcast msg from " + exceptMyLogin + " sended to " + QString::number(deliveredCount) + " users");
}

void Server::onUserListRequest(const QString& login) {
    Utils::log("UserList request from " + login);
    sendUserListTo(login);
}

void Server::sendUserListTo(const QString& login) {
    if (!loginToSession.contains(login)) {
        Utils::log("Requester not found: " + login);
        return;
    }

    QStringList users;
    users = getOnlineUsers();

    loginToSession[login]->sendMsg(ChatProtocol::makeUserListResponse(users));
    Utils::log("UserList with " + QString::number(users.size()) + " users sended");
}



