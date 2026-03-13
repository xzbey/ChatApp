#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Utils::logWidget = ui->logList;

    ui->stopServerBtn->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startServerBtn_clicked()
{
    if (!server) {
        server = new Server(this);

        connect(server, &Server::userListChanged, this, &MainWindow::updateUserList);

        if (server->start(ui->port_spinbox->value())) {
            ui->port_spinbox->setEnabled(false);
            ui->startServerBtn->setEnabled(false);
            ui->stopServerBtn->setEnabled(true);

            updateUserList();
        }
    }
}


void MainWindow::on_stopServerBtn_clicked()
{
    if (server) {
        server->close();
        server->deleteLater();
        server = nullptr;

        ui->port_spinbox->setEnabled(true);
        ui->startServerBtn->setEnabled(true);
        ui->stopServerBtn->setEnabled(false);

        Utils::log("Server stopped");
    }
}

void MainWindow::updateUserList() {
    ui->userList->clear();

    QStringList allUsers = server->getUserStorage()->getAllUsernames(),
                onlineUsers = server->getOnlineUsers();

    for (const QString& user: allUsers) {
        QListWidgetItem* item = new QListWidgetItem(user);

        if (onlineUsers.contains(user))
            item->setBackground(QBrush(QColor(0, 200, 0)));

        else
            item->setBackground(QBrush(QColor(150, 150, 150)));

        ui->userList->addItem(item);
    }
}
