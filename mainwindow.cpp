#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    local_timer = new QTimer;
    connect(local_timer,SIGNAL(timeout()),this,SLOT(show_local_information()));
    local_timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_video(unsigned char *rgb, int width, int height)
{
    QImage view(rgb,width,height,QImage::Format_RGB888);
    view.scaled(480,360);
    ui->label_video->setPixmap(QPixmap::fromImage(view, Qt::AutoColor));
}

void MainWindow::show_sensor(int sensor_num)
{
    int i;
    QByteArray info;
    for(i = 0; i < sensor_num; i++)
    {
        info.append(sensors_data[i].sensor_name);
        info.append(":");
        info.append(sensors_data[i].sensor_info);
        info.append("\n");
    }
    ui->textEdit_videoinfo->setText(info);
}

void MainWindow::show_local_information()
{
    ui->lineEdit_systemip->setText(device_setting_read.settings_localip);
    if(device_setting_read.settings_state == 0)
        ui->lineEdit_systemstate->setText("Idle");
    else
        ui->lineEdit_systemstate->setText("Busy");
    ui->lineEdit_time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::show_blank()
{
    ui->label_video->clear();
    ui->textEdit_videoinfo->clear();
    ui->textEdit_systeminfo->clear();
}

void MainWindow::show_system_data(QString remoteip)
{
    ui->textEdit_systeminfo->setText("Decoding" + remoteip + "video!");
}
