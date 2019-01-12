#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),timer(new QTimer)
{
    ui->setupUi(this);

    connect(this,SIGNAL(destroyed()),this,SLOT(close()));
    connect(timer,SIGNAL(timeout()),SLOT(read_info()));
    timer->start(1000);
}

void MainWindow::read_info()
{
    if(!GetSystemPowerStatus(&power))
    {
        QMessageBox::warning(this,"Error.","Couldn't read info of battery.");
        emit destroyed();
    }

    ui->power_life->setText("Уровень заряда: " + QString::number(power.BatteryLifePercent) + '%');
    ui->power_status->setText("Тип питания: " + (power.ACLineStatus ? QString("Заряжается"): QString("Разряжается")));
    ui->saving_power->setText("Режим энергосбережения: " + (power.Reserved1 ? QString("Включен"): QString("Отключен")));
    ui->time_power->setText("Время до разрядки: " +QString::number(power.BatteryLifeTime/(60*60)) + "ч." +
                            QString::number(power.BatteryLifeTime/60%60)+ "м.");
}

MainWindow::~MainWindow()
{
    timer->stop();
    delete timer;
    delete ui;
}
