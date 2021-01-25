#include "warningbox.h"

warningbox::warningbox(QWidget *parent) : QLabel(parent)
{
    this->close();
    //UI
    this->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    this->resize(110,20);
    this->setStyleSheet(R"(
    background-color:#00BFFF;
    border-radius:10px;
    font-size:15px;
    font-family:"SimSun";
    border-style:solid;
    border-width:1px;
    border-color:#FF1493;
                        )");

    connect(&timer,&QTimer::timeout,[=](){
        this->close();
    });
}



warningbox::~warningbox(void)
{

}

//最好控制在6个汉字以内
void warningbox::showMessage(int sec, const char *text,int x,int y)
{
    timer.stop();
    this->setText(QString::fromLocal8Bit(text));
    this->setGeometry(x,y,110,20);
    this->show();
    timer.start(sec*1000);

}
