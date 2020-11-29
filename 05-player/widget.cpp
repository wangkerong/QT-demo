#include "widget.h"
#include "ui_widget.h"

/*
 * 已知问题：当视频从全屏退出时，视频窗口会脱离Widget主窗口
*/


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    Init();
}
void Widget::Init()
{
    ispress = false;
    timer = new QTimer(this);

    connect(timer,&QTimer::timeout,[=](){
    int value = MediaPlay->position();
    ui->label_3->setText(QString("%1:%2").arg((value/1000)/60,2,10,QChar('0')).arg((value/1000)%60,2,10,QChar('0')));
    ui->horizontalSlider->setValue(value/1000);
    });

    VideoWidget = new MyVideoWidget(ui->label);
    connect(VideoWidget,&MyVideoWidget::KeyESCPress,this,&Widget::QuitFullScreen);
    connect(VideoWidget,&MyVideoWidget::LeftDbClick,this,&Widget::SetVideoFullScreen);
    MediaPlay = new QMediaPlayer(this);
    MediaPlay->setVideoOutput(VideoWidget);
    VideoWidget->setAutoFillBackground(true);
    VideoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    VideoWidget->resize(ui->label->width(),ui->label->height());

    VideoWidget->show();
    ui->comboBox->setCurrentIndex(3);
    connect(MediaPlay,&QMediaPlayer::durationChanged,this,&Widget::VideoTime);      //获取总播放时间
    connect(MediaPlay,&QMediaPlayer::mediaStatusChanged,this,&Widget::videoend);    //播放结束
//    HideCortil();
}
void Widget::resizeEvent(QResizeEvent *event)
{
    VideoWidget->resize(ui->label->width(),ui->label->height());
    size.setWidth(this->width());
    size.setHeight(this->height());

}
void Widget::SetVideoFullScreen()
{
    if(MediaPlay->state() == QMediaPlayer::PlayingState || MediaPlay->state() == QMediaPlayer::PausedState)
    {
        VideoWidget->setFullScreen(true);
    }
}
void Widget::QuitFullScreen()
{

    VideoWidget->setFullScreen(false);
    //在全屏之前的尺寸上加1时为了触发resizeEvent（）事件函数
    this->resize(size.width()+1,size.height()+1);

}
Widget::~Widget()
{
    delete ui;
}
void Widget::VideoTime(qint64 val)  //获取总时长
{
    if(val!=0)
    {
        ui->horizontalSlider->setMaximum(val/1000);
        ui->label_2->setText(QString("%1:%2").arg((val/1000)/60,2,10,QChar('0')).arg((val/1000)%60,2,10,QChar('0')));
    }
}
void Widget::videoend(QMediaPlayer::MediaStatus status) //播放结束
{
    if(status == QMediaPlayer::EndOfMedia)
    {
        timer->stop();
    }
}
void Widget::on_pushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this,"chose vide","C:/Users/Mr.wang/Desktop","video(*.mp4)");
    if(filename.isEmpty())
        return;
    MediaPlay->setMedia(QUrl::fromLocalFile(filename));
    ui->horizontalSlider->setValue(0);
    timer->start(1000);
    MediaPlay->play();
    MediaPlay->setVolume(20);
}

void Widget::on_pushButton_2_clicked()
{
    if(MediaPlay->state() == QMediaPlayer::PlayingState)
    {

        MediaPlay->pause();
        timer->stop();
    }
    else if(MediaPlay->state() == QMediaPlayer::PausedState)
    {
        MediaPlay->play();
        timer->start(1000);
    }
    else
        return;

}

void Widget::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(MediaPlay->state() == QMediaPlayer::PlayingState)
    {

        MediaPlay->setPlaybackRate(QString(arg1).replace("x","").toDouble());
    }
}



void Widget::on_dial_valueChanged(int value)    //设置音量大小
{
    if(MediaPlay->state() == QMediaPlayer::PlayingState || MediaPlay->state() == QMediaPlayer::PausedState)
    {

        MediaPlay->setVolume(value);
    }
}

void Widget::on_horizontalSlider_valueChanged(int value)    //设置当前播放进度
{
    if(ispress)
    {
        if(MediaPlay->state() == QMediaPlayer::PlayingState || MediaPlay->state() == QMediaPlayer::PausedState)
        {
            MediaPlay->setPosition(value*1000);
        }
    }
}

void Widget::on_horizontalSlider_sliderPressed()
{
    ispress = true;
}

void Widget::on_horizontalSlider_sliderReleased()
{
    ispress = false;
}

void Widget::HideCortil()
{
    ui->dial->hide();
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->comboBox->hide();
    ui->horizontalSlider->hide();
}
