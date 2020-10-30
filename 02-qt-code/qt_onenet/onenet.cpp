#include "onenet.h"

OneNet::OneNet(QObject *parent) : QObject(parent)
{
    this->masterKey_ = NULL;
    this->m_manager = NULL;
    this->datastream_id_ = "";
    this->device_id_ = "";
    timer = new QTimer(this);
    timer->start(1000);
}

void OneNet::getDeviceData()
{
    QString masterKey = QString("OY3T6ydU2xoXgkTd=OPqZzNYiE4=");
    QString device_id = QString("552655029");
    QString str = QString("temperature,humi,light");

    QString datastream_id = str;

    this->masterKey_ = masterKey.toUtf8();
    this->device_id_ = device_id;
    this->datastream_id_ = datastream_id;

    m_manager = new QNetworkAccessManager(this);
    QHostAddress host("api.heclouds.com");
    QNetworkRequest request;

    QString ur = "http://api.heclouds.com/devices/"+device_id_+"/datastreams?datastream_ids="+datastream_id_;

    request.setUrl(QUrl(ur));
    qDebug() << ur;
    request.setRawHeader("api-key",this->masterKey_);

    //每1s更新一次数据
    connect(timer,&QTimer::timeout,[=](){
        m_manager->get(request);
    });

    QObject::connect(m_manager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply){
         if(reply->error() != QNetworkReply::NoError)
         {
             qDebug() << "Error:" << reply->errorString();
             return;
         }
         QByteArray buf = reply->readAll();
         qDebug() << "OK:"<< QString(buf);

         QString QStrJson = QString(buf);
         QJsonParseError parseJsonErr;
         QJsonDocument document = QJsonDocument::fromJson(QStrJson.toUtf8(),&parseJsonErr);
         if(!(parseJsonErr.error == QJsonParseError::NoError))
         {
             qDebug()<<QString("解析json文件错误！");
             return;
         }

         QJsonObject jsonObject = document.object();

         if(jsonObject.contains(QStringLiteral("data")))
         {
             QJsonValue arrayValue = jsonObject.value(QStringLiteral("data"));
             if(arrayValue.isArray())
             {
                 QJsonArray array = arrayValue.toArray();

                 for(int i=0;i<array.size();i++)
                 {
                    QJsonValue iconArray = array.at(i);
                    QJsonObject icon = iconArray.toObject();
                    onenet_id_[i] = icon["id"].toString();
                    data[i] = icon["current_value"].toInt();

//                    qDebug()<<"id="<<onenet_id_[i]<<"data="<<data[i];

                    emit dataSignal(onenet_data_[0]);
                 }
             }
         }
     });
}

