#ifndef MESSAGEROUTER_H
#define MESSAGEROUTER_H

#include <QObject>

class MessageRouter: public QObject
{
    Q_OBJECT
public:
    MessageRouter(QObject* parent = nullptr);
};

#endif // MESSAGEROUTER_H
