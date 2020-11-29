#include "widget.h"
#include "ui_widget.h"
#include <QImage>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    camera=new QCamera(this);
    viewfinder=new QCameraViewfinder(ui->label); //相机取景器类
    camera->setViewfinder(viewfinder);      //设置相机取景框
    imageCapture = new QCameraImageCapture(camera);
    camera->start();

    addViewfinderToCombox(ui->comboBox);
    void (QComboBox::*p)(int) = &QComboBox::currentIndexChanged;
    void (Widget::*p2)(int) =&Widget::switchViewfinderSize;
    connect(ui->comboBox,p,this,p2);


    void (QCameraImageCapture::*p1)(int ,const QImage &) = &QCameraImageCapture::imageCaptured;
    void (Widget::*p3)(int ,const QImage &) = &Widget::SHOUIMAGE;
    connect(imageCapture,p1,this,p3);
    connect(imageCapture,&QCameraImageCapture::imageSaved,this,&Widget::save);

}
void Widget::SHOUIMAGE(int val,const QImage &image)
{
    QPixmap pixmap = QPixmap::fromImage(image);
    QFile file("yourFile.txt");
    file.open(QIODevice::WriteOnly);
    qDebug()<<pixmap.save(&file,"jpg");
    ui->label_2->setPixmap(pixmap.scaled(QSize(ui->label_2->height(),ui->label_2->width()),
                                         Qt::KeepAspectRatio));

}
void Widget::save(int id, const QString &fileName)
{
    qDebug()<<id;
}
void Widget::paintEvent(QPaintEvent *event)
{
    viewfinder->resize(ui->label->width(),ui->label->height());

    QWidget::paintEvent(event);
}
Widget::~Widget()
{
    delete ui;
}
void Widget::addViewfinderToCombox(QComboBox *p)
{
    QStringList sList;
    QList<QSize> list = camera->supportedViewfinderResolutions();
    for(auto it = list.begin();it!=list.end();it++)
    {
        sList.append(QString("%1x%2").arg(it->width()).arg(it->height()));
//        qDebug()<<it->width()<<"x"<<it->height();
    }
    p->addItems(sList);

}

void Widget::switchViewfinderSize(int val)
{
    QCameraViewfinderSettings set;
    //QSize(160, 120), QSize(320, 240), QSize(640, 360), QSize(640, 480), QSize(800, 600), QSize(1280, 720))
    switch (val) {
    case 0: set.setResolution(QSize(160,120)); break;
    case 1: set.setResolution(QSize(320,240)); break;
    case 2: set.setResolution(QSize(640,360)); break;
    case 3: set.setResolution(QSize(640,480)); break;
    case 4: set.setResolution(QSize(800,600));break;
    case 5: set.setResolution(QSize(1280,720));break;
    }
    camera->setViewfinderSettings(set);

}



void Widget::on_pushButton_clicked()
{
   imageCapture->capture("C:");
}
