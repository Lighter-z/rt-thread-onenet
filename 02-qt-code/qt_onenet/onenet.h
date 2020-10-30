#ifndef ONENET_H
#define ONENET_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QNetworkReply>

#include <QHostAddress>
#include <QString>
#include <QTimer>

class OneNet : public QObject
{
    Q_OBJECT
public:
    explicit OneNet(QObject *parent = nullptr);

signals:
    void dataSignal(QString data);  //QString id[],QString data[]

public:
//    void getDeviceData( QObject *object, QByteArray masterKey, QString device_id, QString datastream_id_);

    void getDeviceData();

    QString onenet_id_[10];
    QString onenet_data_[10];
    int data[10];

private:
    QByteArray masterKey_;
    QString device_id_;
    QString datastream_id_;
    QNetworkAccessManager *m_manager;
    QTimer * timer;

};

#endif // ONENET_H
