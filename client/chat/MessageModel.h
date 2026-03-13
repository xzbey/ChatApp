#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>

#include "ChatProtocol.h"


class MessageModel: public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        AuthorRole = Qt::UserRole + 1,
        TextRole
    };

    MessageModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addMessage(const ChatProtocol::BroadcastMessage& msg);

private:
    QList<ChatProtocol::BroadcastMessage> messages;
};

#endif // MESSAGEMODEL_H
