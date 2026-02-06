#ifndef AUTHVALIDATOR_H
#define AUTHVALIDATOR_H

#include <QObject>

class AuthValidator: public QObject
{
    Q_OBJECT
public:
    AuthValidator(QObject* parent = nullptr);
};

#endif // AUTHVALIDATOR_H
