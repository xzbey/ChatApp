#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startServerBtn_clicked() {
    ui->logList->append("text");
}

void MainWindow::on_stopServerBtn_clicked() {
    ui->logList->append("text2");
}
