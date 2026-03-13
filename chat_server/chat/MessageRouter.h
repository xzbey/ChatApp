#ifndef MESSAGEROUTER_H
#define MESSAGEROUTER_H

#include <QObject>

#include "ChatProtocol.h"
#include "Utils.h"

class MessageRouter: public QObject
{
    Q_OBJECT
public:
    MessageRouter(QObject* parent = nullptr);

    void handleMessage(const QString& senderLogin, const QJsonObject& msg);

signals:
    void forwardMessage(const QString& receiver, const QJsonObject& msg);

    void broadcastMessage(const QString& exceptMyLogin, const QJsonObject& msg);

    void userListMessage(const QString& myLogin);
};

#endif // MESSAGEROUTER_H
