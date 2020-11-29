#include "widget.h"
#include <QDebug>
#include <QtMath>
#include <QDate>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    _Ellipse_r = 200;
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout,[=](){
    this->update();});

    lcd.setFrameShape(QFrame::NoFrame);
    lcd.setParent(this);
    lcd.setDigitCount(10);
    lcd.setParent(this);
    lcd.setDigitCount(10);
    lcd.resize(200,100);
    this->resize(600,600);

    updateTimer->start(1000);
}
void Widget::paintEvent(QPaintEvent *event)
{
    //    获取系统时间
    QDateTime time = QDateTime::currentDateTime();
    hour = time.time().hour();
    sec = time.time().second()+1;
    min = time.time().minute();
    QDate data = QDate::currentDate();
    year = data.year();
    month = data.month();
    day = data.day();

    QPainter paint(this);
    _width = this->width();
    _height = this->height();
    //反走样,即抗锯齿处理
    paint.setRenderHint(QPainter::Antialiasing);
    //使用平滑的pixmap变换算法(双线性插值算法),而不是近邻插值算法
    paint.setRenderHint(QPainter::SmoothPixmapTransform);

    //移动画家到当前界面的中心点
    paint.translate(_width/2,_height/2);

    DrawDateLCD(paint);     //绘制日期显示LCD
    DrawEllipse1(paint);    //画外测圆
    DrawEllipse2(paint);    //画内测圆

    DrawShortLine(paint);   //画刻度线
    DrawNumber(paint);      //显示日期

    //画中心的黑点
    QBrush brush(Qt::black);
    paint.setBrush(brush);
    paint.drawEllipse(QPoint(0,0),5,5);

    //画秒针
    DrawSecLine(paint);
    //画分针
    DrawMinateLine(paint);
    //画时针
    DrawHourLine(paint);

    QWidget::paintEvent(event);

}
Widget::~Widget()
{
}
void Widget::DrawDateLCD(QPainter &paint)
{
    lcd.display(QString("%1-%2-%3").arg(year).arg(month).arg(day));
    lcd.move(_width/2-(lcd.width())/2,_height/2-(lcd.height())/2+50);
    lcd.show();

}
void Widget::DrawShortLine(QPainter &pointer)
{
    pointer.save();
    QPen pen;
    pen.setWidth(2);
    pointer.setPen(pen);
    for(int i=0;i<60;i++)
    {
        if(i%5==0)
        {
            pointer.drawLine(QPoint(0,_Ellipse_r-_timeLine-10),QPoint(0,_Ellipse_r));
        }
        else
            pointer.drawLine(QPoint(0,_Ellipse_r-_timeLine),QPoint(0,_Ellipse_r));
        pointer.rotate(6);
    }
    pointer.restore();

}
void Widget::DrawHourLine(QPainter &pointer)
{
    const QPointF points[4] = {
          QPointF(-5, 0),
          QPointF(5, 0),
          QPointF(5, _Ellipse_r-100),
        QPointF(-5, _Ellipse_r-100),
      };
    pointer.save();
    pointer.rotate(180+hour*30+(min/60.0)*30);
    pointer.drawConvexPolygon(points,4);
    pointer.restore();

}
void Widget::DrawMinateLine(QPainter &pointer)
{
    const QPointF points[3] = {
          QPointF(-5, 0),
          QPointF(5, 0),
          QPointF(0, _Ellipse_r-70),
      };
    pointer.save();
    pointer.rotate(180+min*6+(sec/60.0)*5);
    pointer.drawConvexPolygon(points,3);
    pointer.restore();

}
void Widget::DrawSecLine(QPainter &pointer)
{
    const QPointF points[3] = {
          QPointF(-5, 0),
          QPointF(5, 0),
          QPointF(0, _Ellipse_r-20),
      };
    pointer.save();
    pointer.rotate(180+sec*6);
    pointer.drawConvexPolygon(points,3);
    pointer.restore();

}

QRectF Widget::textRectF(double radius, int pointSize, double angle)
{
    QRectF rectF;
    const double pi = 3.1415926;
    rectF.setX(radius*qCos(angle*pi/180.0) - pointSize*2.0);
    rectF.setY(radius*qSin(angle*pi/180.0) - pointSize/2.0);
    rectF.setWidth(pointSize*4);
    rectF.setHeight(pointSize);
    return rectF;
}
void Widget::DrawNumber(QPainter &pointer)
{
    pointer.save();
       QFont font = pointer.font();
       font.setBold(true);//字体加粗
       pointer.setFont(font);
       int pointSize = font.pointSize();

    // 绘制小时文本
       int nHour = 0;
       for (int i = 0; i < 12; ++i) {
           nHour = i + 3;
           if (nHour > 12)
               nHour -= 12;
           pointer.drawText(textRectF(_Ellipse_r-_timeLine-20, pointSize+2, i * 30), Qt::AlignCenter, QString::number(nHour));
       }

    pointer.restore();

}
void Widget::DrawEllipse2(QPainter &pointer)
{
    pointer.save();
    QBrush brush(Qt::SolidPattern);
    brush.setColor(Qt::white);
    pointer.setBrush(brush);
    pointer.drawEllipse(QPoint(0,0),_Ellipse_r,_Ellipse_r);
    pointer.restore();
}
void Widget::DrawEllipse1(QPainter &pointer)
{
    pointer.save();
    QBrush brush(Qt::SolidPattern);
    brush.setColor(Qt::black);
    pointer.setBrush(brush);
    pointer.drawEllipse(QPoint(0,0),_Ellipse_r+10,_Ellipse_r+10);
    pointer.restore();
}
