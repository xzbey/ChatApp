#ifndef AUTHVALIDATOR_H
#define AUTHVALIDATOR_H

#include <QTcpSocket>
#include <QJsonDocument>

#include "Utils.h"
#include "ValidationProtocol.h"

class AuthValidator: public QObject
{
    Q_OBJECT
public:
    AuthValidator(QObject* parent = nullptr);

    void validateClient(const QString& login, const QString& token);

signals:
    void validationResult(const QString& login, const QString& token, bool valid);

private slots:
    void onReadyRead(QTcpSocket* socket, const QString& login, const QString& token);
    void onError(QTcpSocket* socket, const QString& login, const QString& token);

private:
    quint16 port = 50002;
};

#endif // AUTHVALIDATOR_H
