#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QObject>

class ClientSession: public QObject
{
    Q_OBJECT
public:
    ClientSession(QObject* parent = nullptr);
};

#endif // CLIENTSESSION_H
