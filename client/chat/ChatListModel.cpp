#include "ChatListModel.h"

ChatListModel::ChatListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    addChat("broadcast", "Общий чат");
}

int ChatListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return chats.count();
}

QVariant ChatListModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= chats.count()) return QVariant();

    const Chat& chat = chats[index.row()];

    switch (role) {
    case ChatIdRole:
        return chat.chatId;
    case ChatNameRole:
        return chat.chatName;
    default:
        break;
    }
    return QVariant();

}

QHash<int, QByteArray> ChatListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ChatIdRole] = "chatId";
    roles[ChatNameRole] = "chatName";
    return roles;
}

void ChatListModel::addChat(const QString& chatId, const QString& chatName) {
    for (const Chat& chat: chats)
        if (chat.chatId == chatId)
            return;

    beginInsertRows(QModelIndex(), chats.count(), chats.count());
    chats.append({chatId, chatName});
    endInsertRows();

    if (!messageModels.contains(chatId))
        messageModels[chatId] = new MessageModel(this);
}

MessageModel* ChatListModel::getMessageModel(const QString& chatId) {
    if (!messageModels.contains(chatId))
        messageModels[chatId] = new MessageModel(this);
    return messageModels[chatId];
}


