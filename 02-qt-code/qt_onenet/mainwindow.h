#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mygauge2.h"

#include <QThread>
#include "onenet.h"

#define SMOG_PIN46_R	1000			//烟雾传感器管脚4、6接出到地的电阻值

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

    void setValue(QString data);   //QString id[],QString data[]

private:
    Ui::MainWindow *ui;

    QThread * thread_;
    OneNet * onenet_;

    int tempreture_;
    int humidity_;
    float smoke_;
    int light_;
    double pm25_;
    QString air_quality_;

    int tempreture_percent_;
    int humidity_percent_;
    float smoke_percent_;
    int light_percent_;

private:
    void dealData(QString data);


};

#endif // MAINWINDOW_H
