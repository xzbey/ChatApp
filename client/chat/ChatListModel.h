#ifndef CHATLISTMODEL_H
#define CHATLISTMODEL_H

#include "MessageModel.h"

struct Chat {
    QString chatId,
            chatName;
};

class ChatListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        ChatIdRole = Qt::UserRole + 1,
        ChatNameRole
    };

    ChatListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addChat(const QString& chatId, const QString& chatName);

    Q_INVOKABLE MessageModel* getMessageModel(const QString& chatId);

private:
    QList<Chat> chats;
    QMap<QString, MessageModel*> messageModels;
};

#endif // CHATLISTMODEL_H
