#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void read_info();

private:
    Ui::MainWindow *ui;
    //QThread *thread;
    QTimer *timer;
    SYSTEM_POWER_STATUS power;

};

#endif // MAINWINDOW_H
