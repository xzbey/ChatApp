#ifndef USERSTORAGE_H
#define USERSTORAGE_H

#include <QObject>
#include <QMap>

#include "Utils.h"

class UserStorage: public QObject
{
    Q_OBJECT
public:
    UserStorage(QObject* parent = nullptr);

    bool userExists(QString login) const;

    bool registerUser(const QString& login, const QString& password);

    bool checkPassword(const QString& login, const QString& password) const;

    QStringList getAllUsernames() const;
private:
    QMap<QString, QString> users;

};

#endif // USERSTORAGE_H