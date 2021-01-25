#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    uiInit();

    updateWeather();

    connect(manager,&QNetworkAccessManager::finished,this,&Widget::replyFinished);


    connect(ui->location,&MyLable::click,[=](){
    //弹出城市选择框
        wBox->showMessage(MESSAGETIME,"这是测试字串",(int)((this->width())/2)-55,20);
    });
    connect(ui->label_7,&MyLable::click,[=](){
        wBox->showMessage(MESSAGETIME,"正在更新",(int)((this->width())/2)-55,20);
        updateWeather();
    });



}
void Widget::updateWeather(void)
{
    memset(&WeatherINFO,0,sizeof (WeatherINFO));
    //获取现在的天气数据
    QUrl url = GetNowWeatherUrl();
    requestWeatherInfo(manager,url);
    //获取三天内的天气数据
    url = GetFutureWeatherUrl();
    requestWeatherInfo(manager,url);
}

void Widget::requestWeatherInfo(QNetworkAccessManager *Manager,QUrl &url)
{
    /*构建请求对象*/
    QNetworkRequest request;
    request.setUrl(url);
    Manager->get(request);
    //get请求失败/成功，都会通过QNetworkAccessManager对象的finished信号告知
}

Widget::~Widget()
{
    delete ui;
    if(manager!=nullptr)
        delete manager;
    if(wBox!=nullptr)
        delete wBox;

    manager = nullptr;
    wBox = nullptr;
}

void Widget::replyFinished(QNetworkReply *reply)
{
    if(reply->error()!=QNetworkReply::NoError)
    {
        //处理中的错误信息
        qDebug()<<"reply error:"<<reply->errorString();
        QByteArray errorinfo =  reply->readAll();
        //处理服务器返回的错误信息,不是网络参数错误(例如：未联网等等)
        if(!errorinfo.isNull())
        {
            ServerRecErrorInfo(errorinfo);
            return;
        }
        wBox->showMessage(MESSAGETIME,"网络参数错误",(int)((this->width())/2)-55,20);
    }
    else
    {
        //        //请求方式
        //        qDebug()<<"operation:"<<reply->operation();
        //        //状态码
        //        qDebug()<<"status code:"<<reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        //        //qDebug()<<"raw header:"<<reply->rawHeaderList();

        //获取响应信息
        JsonArray = reply->readAll();
        Deserialization(JsonArray);
    }

}

void Widget::Deserialization(const QByteArray &arry)
{
    //按照服务器返回的Json格式解析数据
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(arry,&error);
    if(error.error!= QJsonParseError::NoError)
    {
        wBox->showMessage(MESSAGETIME,"Json解析错误",(int)((this->width())/2)-55,20);
        qDebug()<<"error number:"<<error.offset;
        return;
    }
    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue resultVal = jsonObj.value("results");
    if(!resultVal.isArray()){
        wBox->showMessage(MESSAGETIME,"Json解析错误",(int)((this->width())/2)-55,20);
        qDebug()<<"results is not arry";
        return ;
    }
    QJsonObject resJsonObj = resultVal.toArray().at(0).toObject();
    if(!resJsonObj.value("now").isUndefined())//key = "now" 存在则说明是实时天气的数据
    {
        //处理实时天气数据
        saveNowWeatherInfoToStruct(resJsonObj);
    }
    else if(!resJsonObj.value("daily").isUndefined())//key = "daily"存在则说明是未来三天的数据
    {
        saveFutureWeatherInfoToStruct(resJsonObj);
        //处理未来三天的数据
    }
    else
    {
        wBox->showMessage(MESSAGETIME,"未知数据",(int)((this->width())/2)-55,20);
        //数据出错
    }



}

void Widget::saveNowWeatherInfoToStruct(QJsonObject &JsonObj)
{
    int rec = 0;
    QJsonObject locationObj = JsonObj.value("location").toObject();
    //将读取到的数据存至WeatherINFO对象
    /*location info*/
    rec |= strcpy_s(WeatherINFO.location.city_id,sizeof(WeatherINFO.location.city_id),locationObj.value("id").toString().toUtf8());
    rec |= strcpy_s(WeatherINFO.location.city_name,sizeof(WeatherINFO.location.city_name),locationObj.value("name").toString().toUtf8());
    rec |= strcpy_s(WeatherINFO.location.country,sizeof(WeatherINFO.location.country),locationObj.value("country").toString().toUtf8());
    rec |= strcpy_s(WeatherINFO.location.path,sizeof(WeatherINFO.location.path),locationObj.value("path").toString().toUtf8());
    rec |= strcpy_s(WeatherINFO.location.timezone,sizeof(WeatherINFO.location.timezone),locationObj.value("timezone").toString().toUtf8());
    rec |= strcpy_s(WeatherINFO.location.timezone_offset,sizeof(WeatherINFO.location.timezone_offset),locationObj.value("timezone_offset").toString().toUtf8());
    if(rec!=0){
        wBox->showMessage(MESSAGETIME,"strcpy_s错误",(int)((this->width())/2)-55,20);
        qDebug()<<"strcpy_s error:[location info]";
        return;
    }
    /*now info*/
    QJsonObject nowObj = JsonObj.value("now").toObject();
    rec |= strcpy_s(WeatherINFO.now.text,sizeof(WeatherINFO.now.text),nowObj.value("text").toString().toUtf8());
    WeatherINFO.now.code = nowObj.value("code").toString().toInt();
    WeatherINFO.now.temperature = nowObj.value("temperature").toString().toFloat();
    rec |= strcpy_s(WeatherINFO.last_update ,sizeof(WeatherINFO.last_update),JsonObj.value("last_update").toString().toUtf8());
    if(rec!=0){
        wBox->showMessage(MESSAGETIME,"strcpy_s错误",(int)((this->width())/2)-55,20);
        qDebug()<<"strcpy_s error:[now info]";
    }
    UpdateNow(WeatherINFO);
}
void Widget::saveFutureWeatherInfoToStruct(QJsonObject &JsonObj)
{
    int rec = 0;
    QJsonArray futureThreeArry  = JsonObj.value("daily").toArray();
    QJsonObject oneDayObj ;
    for (int i = 0; i <3; i++) {
        oneDayObj = futureThreeArry[i].toObject();
        rec |= strcpy_s(WeatherINFO.date[i].date,sizeof(WeatherINFO.date[i].date),oneDayObj.value("date").toString().toUtf8());
        rec |= strcpy_s(WeatherINFO.date[i].text_day,sizeof(WeatherINFO.date[i].text_day),oneDayObj.value("text_day").toString().toUtf8());
        WeatherINFO.date[i].code_day = oneDayObj.value("code_day").toString().toInt();
        rec |= strcpy_s(WeatherINFO.date[i].text_night,sizeof(WeatherINFO.date[i].text_night),oneDayObj.value("text_night").toString().toUtf8());
        WeatherINFO.date[i].code_night = oneDayObj.value("code_night").toString().toInt();
        WeatherINFO.date[i].high = oneDayObj.value("high").toString().toFloat();
        WeatherINFO.date[i].low = oneDayObj.value("low").toString().toFloat();
        WeatherINFO.date[i].rainfall = oneDayObj.value("rainfall").toString().toFloat();
        rec |= strcpy_s(WeatherINFO.date[i].wind_direction,sizeof(WeatherINFO.date[i].wind_direction),oneDayObj.value("wind_direction").toString().toUtf8());
        WeatherINFO.date[i].wind_direction_degree = oneDayObj.value("wind_direction_degree").toString().toFloat();
        WeatherINFO.date[i].wind_speed = oneDayObj.value("wind_speed").toString().toFloat();
        WeatherINFO.date[i].wind_scale = oneDayObj.value("wind_scale").toString().toInt();
        WeatherINFO.date[i].humidity = oneDayObj.value("humidity").toString().toFloat();
        if(rec!=0)
        {
            wBox->showMessage(MESSAGETIME,"strcpy_s错误",(int)((this->width())/2)-55,20);
            qDebug()<<"第"<<i<<"天数据解析异常";
        }

    }
    rec |= strcpy_s(WeatherINFO.last_update ,sizeof(WeatherINFO.last_update),JsonObj.value("last_update").toString().toUtf8());

    UpdateThreeDay(WeatherINFO);

}
void Widget::ServerRecErrorInfo(QByteArray &arry)
{
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(arry,&error);
    if(error.error!= QJsonParseError::NoError)
    {
        wBox->showMessage(MESSAGETIME,"Json解析错误",(int)((this->width())/2)-55,20);
        return;
    }
    QString errorcode = jsonDoc.object().value("status_code").toString();
    QString str = QString("code:%1").arg(errorcode);
    wBox->showMessage(MESSAGETIME,str.toLocal8Bit().data(),(int)((this->width())/2)-55,20);


}

QUrl Widget::GetNowWeatherUrl(void)
{
    QString baseurl = "https://api.seniverse.com/v3/weather/now.json";
    QUrl url(baseurl);
    QUrlQuery query;
    query.addQueryItem("key", "SjR8xoyd7d59bn4sq");
    query.addQueryItem("location", "ningbo");
    query.addQueryItem("language", "zh-Hans");
    query.addQueryItem("unit", "c");
    url.setQuery(query);
    return url;
//"https://api.seniverse.com/v3/weather/now.json?key=SjR8xoyd7d59bn4sq&location=beijing&language=zh-Hans&unit=c"
}

QUrl Widget::GetFutureWeatherUrl(void)
{
    QString baseurl = "https://api.seniverse.com/v3/weather/daily.json";
    QUrl url(baseurl);
    QUrlQuery query;
    query.addQueryItem("key", "SjR8xoyd7d59bn4sq");
    query.addQueryItem("location", "ningbo");
    query.addQueryItem("language", "zh-Hans");
    query.addQueryItem("unit", "c");
    query.addQueryItem("start", "0");
    query.addQueryItem("days", "3");
    url.setQuery(query);
    return url;
//https://api.seniverse.com/v3/weather/daily.json?key=SjR8xoyd7d59bn4sq&location=beijing&language=zh-Hans&unit=c&start=0&days=3
}

void Widget::TEST()
{

   \
//    /*以下为测试*/
//    server返回后的字符为UTF-8编码，转换成Json对象后再转换成QString,在转换成char *
//    显示时用fromutf8(char *)就可以
//    char bufbuf[20]= {0};
//    strcpy(bufbuf,locationObj.value("name").toString().toUtf8());
//    ui->label->setText(QString::fromUtf8(bufbuf));
//    /*以上为测试*/



}
void Widget::UpdateThreeDay(struct weatherInfo &weatherInfoObj)
{
    updataTime = QTime::currentTime();
    /****************更新当天数据************************/
    QFont font("楷体",15,5);
    ui->label_10->setFont(font);
    ui->label_8->setFont(font);
    ui->label_11->setFont(font);

    //风向
    ui->label_10->setText(QString::fromUtf8(weatherInfoObj.date[0].wind_direction));
    //温度
    QString tempture = QString::fromLocal8Bit(" %1℃~%2℃").arg(weatherInfoObj.date[0].low).arg(weatherInfoObj.date[0].high);
    ui->label_8->setText(tempture);
    //湿度
    ui->label_11->setText(QString::fromLocal8Bit(" %1％").arg(weatherInfoObj.date[0].humidity));
    /****************更新第一天数据**********************/
    //font.setBold(true);
    font.setFamily("宋体");
    font.setPointSize(11);

    ui->label_26->setFont(font);
    ui->label_27->setFont(font);
    ui->label_28->setFont(font);
    ui->label_30->setFont(font);
    ui->label_31->setFont(font);
    //日期
    ui->label_26->setText(QString::fromUtf8(weatherInfoObj.date[0].date));
    //气象文字
    ui->label_27->setText(QString::fromUtf8(weatherInfoObj.date[0].text_day));
    //温度
    ui->label_28->setText(tempture);
    //风向
    ui->label_30->setText(QString::fromUtf8(weatherInfoObj.date[0].wind_direction));
    //湿度
    ui->label_31->setText(QString::fromLocal8Bit("湿度：%1％").arg(weatherInfoObj.date[0].humidity));
    //气象图标
    setWeatherIcon(weatherInfoObj.date[0].code_day, ui->label_32);

    /****************更新第二天数据**********************/
    ui->label_19->setFont(font);
    ui->label_20->setFont(font);
    ui->label_21->setFont(font);
    ui->label_23->setFont(font);
    ui->label_24->setFont(font);
    //日期
    ui->label_19->setText(QString::fromUtf8(weatherInfoObj.date[1].date));
    //气象文字
    ui->label_20->setText(QString::fromUtf8(weatherInfoObj.date[1].text_day));
    //温度
    tempture = QString::fromLocal8Bit("%1℃~%2℃").arg(weatherInfoObj.date[1].low).arg(weatherInfoObj.date[1].high);
    ui->label_21->setText(tempture);
    //风向
    ui->label_23->setText(QString::fromUtf8(weatherInfoObj.date[1].wind_direction));
    //湿度
    ui->label_24->setText(QString::fromLocal8Bit("湿度：%1％").arg(weatherInfoObj.date[1].humidity));
    //气象图标
    setWeatherIcon(weatherInfoObj.date[1].code_day, ui->label_25);

    /****************更新第三天数据**********************/
    ui->label_12->setFont(font);
    ui->label_13->setFont(font);
    ui->label_14->setFont(font);
    ui->label_16->setFont(font);
    ui->label_17->setFont(font);
    //日期
    ui->label_12->setText(QString::fromUtf8(weatherInfoObj.date[2].date));
    //气象文字
    ui->label_13->setText(QString::fromUtf8(weatherInfoObj.date[2].text_day));
    //温度
    tempture = QString::fromLocal8Bit("%1℃~%2℃").arg(weatherInfoObj.date[2].low).arg(weatherInfoObj.date[2].high);
    ui->label_14->setText(tempture);
    //风向
    ui->label_16->setText(QString::fromUtf8(weatherInfoObj.date[2].wind_direction));
    //湿度
    ui->label_17->setText(QString::fromLocal8Bit("湿度：%1％").arg(weatherInfoObj.date[2].humidity));
    //气象图标
    setWeatherIcon(weatherInfoObj.date[2].code_day, ui->label_18);
    //更新最近更新时间
    ui->label->setFont(QFont("华文仿宋",7));
    ui->label->setText(QString::fromLocal8Bit("最近更新时间%1:%2").arg(updataTime.hour()).arg(updataTime.minute()));



}
void Widget::UpdateNow(const struct weatherInfo &weatherInfoObj)
{
    updataTime = QTime::currentTime();
    //城市
    ui->location->setFont(QFont("楷体",15,0));
    ui->location->setText(QString::fromUtf8(weatherInfoObj.location.city_name));
    //天气图片
    setWeatherIcon(weatherInfoObj.now.code,ui->label_3);
    //天气文字
    ui->text->setFont(QFont("楷体",35,0));
    ui->text->setText(QString::fromUtf8(weatherInfoObj.now.text));
    //现在的温度
    ui->temperature->setFont(QFont("楷体",35,0));
    ui->temperature->setText(QString("%1%2").arg(weatherInfoObj.now.temperature).arg(QString::fromLocal8Bit("℃")));
    QFont font("楷体",10,10);
    //位置信息
    ui->label_33->setFont(font);
    ui->label_33->setText(QString::fromUtf8(weatherInfoObj.location.path));
    //数据更新时间
    ui->label_34->setFont(font);
    ui->label_34->setText(QString::fromUtf8(weatherInfoObj.last_update));
    //更新最近更新时间
    ui->label->setFont(QFont("华文仿宋",7));
    ui->label->setText(QString::fromLocal8Bit("最近更新时间%1:%2").arg(updataTime.hour()).arg(updataTime.minute()));
    //更新天气背景
    setBackGroundPicture(weatherInfoObj.now.code);
}
void Widget::uiInit(void)
{
    setBackGroundPicture(0);
    setWeatherIcon(-1,ui->label_3);
    //弹窗对象
    wBox = new warningbox(this);
    //QNetworkAccessManager类：提供了网络请求和回复的功能
    manager = new QNetworkAccessManager(this);
    //风向、温湿度、刷新图标
    ui->label_4->setPixmap(QPixmap::fromImage(QImage(":/background/image/sucai/fc.png").scaled(ui->label_4->size(),Qt::IgnoreAspectRatio)));
    ui->label_2->setPixmap(QPixmap::fromImage(QImage(":/background/image/sucai/tep.png").scaled(ui->label_2->size(),Qt::IgnoreAspectRatio)));
    ui->label_5->setPixmap(QPixmap::fromImage(QImage(":/background/image/sucai/shidu.png").scaled(ui->label_5->size(),Qt::IgnoreAspectRatio)));
    ui->label_7->setPixmap(QPixmap::fromImage(QImage(":/background/image/sucai/rfush.png").scaled(ui->label_7->size(),Qt::KeepAspectRatio)));
    //关于
    ui->label_9->setText("<a style='color: green;' href=\"https://docs.seniverse.com/api/start/error.html\">About");
    ui->label_9->setOpenExternalLinks(true);

}
void Widget::setBackGroundPicture(const int code)
{
    QString path;
    if(code==7||code==8||code==9)
        path = ":/background/image/sucai/yintain.jpg";
    else if(code==10||code==11||code==12||code==13||code==14||code==15||code==16||code==17||code==18||code==19)
        path = ":/background/image/sucai/rain.jpg";
    else if(code==20||code==21||code==22||code==23||code==24||code==25)
        path = ":/background/image/sucai/xue.jpg";
    else
        path = ":/background/image/sucai/sun.jpg";
    QPixmap pixmap = QPixmap(path).scaled(this->size(),Qt::IgnoreAspectRatio) ;
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(pixmap));
    this->setPalette(palette);

}
void Widget::setWeatherIcon(const int code,QLabel *lab)
{
    int weatherCode = code;
    if(weatherCode <0 ||weatherCode >38)
        weatherCode = 99;
    QString path = QString(":/weatherIcon/image/white/%1@2x.png").arg(weatherCode);
    QImage image;
    image.load(path);
    image.scaled(lab->size(),Qt::IgnoreAspectRatio);
    lab->setPixmap(QPixmap::fromImage(image));
}
