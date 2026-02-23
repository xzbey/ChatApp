#ifndef VALIDATIONSERVER_H
#define VALIDATIONSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonParseError>
#include <QJsonDocument>

#include "UserStorage.h"
#include "Utils.h"
#include "ValidationProtocol.h"

class ValidationServer: public QTcpServer
{
    Q_OBJECT
public:
    ValidationServer(UserStorage* userStorage, QObject* parent = nullptr);

    bool start(const quint16& port);

public slots:
    void incomingConnection(qintptr socketDescriptor);

private:
    UserStorage* userStorage;

    void handleRequest(QTcpSocket* socket);
};

#endif // VALIDATIONSERVER_H
