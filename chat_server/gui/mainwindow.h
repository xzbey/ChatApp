#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Server.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startServerBtn_clicked();

    void on_stopServerBtn_clicked();

private:
    Ui::MainWindow *ui;
    Server* server;

    void updateOnlineUsers();
};
#endif // MAINWINDOW_H
