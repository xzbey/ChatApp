#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Utils::logWidget = ui->logList;

    server = new Server(this);
    connect(server, &Server::userListChanged, this, &MainWindow::updateOnlineUsers);

    ui->stopServerBtn->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startServerBtn_clicked() {
    if (server->start(ui->port_spinbox->value())) {
        ui->startServerBtn->setEnabled(false);
        ui->stopServerBtn->setEnabled(true);
        ui->port_spinbox->setEnabled(false);

        updateOnlineUsers();
    }
}

void MainWindow::on_stopServerBtn_clicked() {
    server->close();

    ui->startServerBtn->setEnabled(true);
    ui->stopServerBtn->setEnabled(false);
    ui->port_spinbox->setEnabled(true);

    updateOnlineUsers();

    Utils::log("Server stopped");
}


void MainWindow::updateOnlineUsers() {
    ui->userList->clear();

    QStringList users = server->getOnlineUsers();

    ui->userList->addItem("Online: " + QString::number(users.size()));
    for (const QString& user: users)
        ui->userList->addItem(user);
}



