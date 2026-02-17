#include "MessageModel.h"

MessageModel::MessageModel(QObject* parent)
    : QAbstractListModel(parent)
{}

int MessageModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return messages.count();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= messages.count()) return QVariant();

    const ChatProtocol::BroadcastMessage& msg = messages[index.row()];

    switch (role) {
    case AuthorRole:
        return msg.from;
    case TextRole:
        return msg.msg;
    default:
        break;
    }
    return QVariant();

}

QHash<int, QByteArray> MessageModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[AuthorRole] = "author";
    roles[TextRole] = "text";
    return roles;
}


void MessageModel::addMessage(const ChatProtocol::BroadcastMessage& msg) {
    beginInsertRows(QModelIndex(), messages.count(), messages.count());
    messages.append(msg);
    endInsertRows();
}
