#ifndef AUTHVALIDATOR_H
#define AUTHVALIDATOR_H

#include <QObject>
#include <QSet>
#include <QFile>

#include "Utils.h"

class AuthValidator: public QObject
{
    Q_OBJECT
public:
    AuthValidator(QObject* parent = nullptr);

    bool validateClient(const QString& login, const QString& token);

    bool validateToken(const QString& token);

    void loadRegisterUsers(const QString& path);

private:
    QSet<QString> registerUsers; //потом чет типа QMap<QString, QString> - логин и токен

    bool userExists(const QString& login) const;
};

#endif // AUTHVALIDATOR_H
