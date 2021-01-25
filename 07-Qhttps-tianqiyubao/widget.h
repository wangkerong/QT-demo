#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <string>
#include <string.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QUrl>
#include <QPixmap>
#include <QUrlQuery>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <memory>
#include "WeatherInfo.h"
#include "warningbox.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE



#define MESSAGETIME 1

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void replyFinished(QNetworkReply *reply);
    void Deserialization(const QByteArray &arry);
    void ServerRecErrorInfo(QByteArray &arry);
    void updateWeather(void);
    void requestWeatherInfo(QNetworkAccessManager *Manager,QUrl &url);
    void setBackGroundPicture(const int code);
    void setWeatherIcon(const int code,QLabel *lab);
    void uiInit(void);
    void TEST();

private:
    Ui::Widget *ui;
    //天气信息存储结构体
    struct weatherInfo WeatherINFO;
    //服务器返回Json对象
    QByteArray JsonArray;
    //最近更新时间
    QTime updataTime;
    //
    QNetworkAccessManager *manager;
    warningbox *wBox;




    QUrl GetNowWeatherUrl(void);
    QUrl GetFutureWeatherUrl(void);
    void saveNowWeatherInfoToStruct(QJsonObject &JsonObj);
    void saveFutureWeatherInfoToStruct(QJsonObject &JsonObj);
    void UpdateNow(const struct weatherInfo &weatherInfoObj);
    void UpdateThreeDay(struct weatherInfo &weatherInfoObj);

};
#endif // WIDGET_H
