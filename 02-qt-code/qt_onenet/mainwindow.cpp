#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "onenet.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    onenet_ = new OneNet();
    thread_ = new QThread(this);

    onenet_->moveToThread(thread_);
    thread_->start();

    ui->light_widget->scale_value_ = 80;//设置刻度值

    connect(onenet_,&OneNet::dataSignal,this,&MainWindow::setValue);
    connect(ui->conBtn,&QPushButton::clicked,onenet_,&OneNet::getDeviceData);

    connect(this,&MainWindow::destroyed,this,[=](){
        if(thread_->isRunning() == false)
        {
            return;
        }
        thread_->quit();
        thread_->wait();
        delete onenet_;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dealData(QString data)
{
    tempreture_ = onenet_->data[0];
    humidity_ = onenet_->data[1];
    light_ = (int)onenet_->data[2];
}

void MainWindow::setValue(QString data)
{
    dealData(data);

    ui->conBtn->setText("已连接");
    ui->tempreture_widget->setValue(tempreture_);
    ui->humidity_widget->setValue(humidity_);
    ui->light_widget->setValue(light_);
}
