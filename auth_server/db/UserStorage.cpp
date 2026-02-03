#include "UserStorage.h"

UserStorage::UserStorage(QObject* parent)
    : QObject(parent)
{
    users["admin"] = "12345";
}

bool UserStorage::userExists(QString login) const {
    return users.contains(login);
}

bool UserStorage::registerUser(const QString& login, const QString& password){
    if (userExists(login))
        return false;

    users[login] = password;
    return true;
}

bool UserStorage::checkPassword(const QString& login, const QString& password) const {
    if (!userExists(login))
        return false;

    return users[login] == password;
}

QStringList UserStorage::getAllUsernames() const {
    return users.keys();
}