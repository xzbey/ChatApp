#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTcpSocket>

#include "Utils.h"

class Client: public QObject
{
    Q_OBJECT
public:
    Client(QObject* parent = nullptr);

    void connectToServer(const QHostAddress& hostAddress, const quint16& port);
    void disconnectFromServer();

    void sendRequest(const QJsonObject& request);

    bool isConnected() const;

signals:
    void connected();
    void disconnected();
    void responseReceived(const QJsonObject& response);
    void error0ccurred(const QString& error);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError();

private:
    QTcpSocket* socket;
};

#endif // CLIENT_H
