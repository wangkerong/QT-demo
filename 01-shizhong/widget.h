#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QRect>
#include <QDateTime>
#include <QLCDNumber>
#include <QFrame>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QTimer *updateTimer;
    //当前界面的长宽
    int _width;
    int _height;
    //外圈圆的半径
    int _Ellipse_r;
    //定义时间间隔线的长度
    int _timeLine = 5;
    QLCDNumber lcd ;
    int sec = 0;
    int min = 0;
    int hour= 0;
    int year;
    int month;
    int day;
    void DrawEllipse1(QPainter &pointer);
    void DrawEllipse2(QPainter &pointer);
    void DrawShortLine(QPainter &pointer);
    void DrawNumber(QPainter &pointer);
    void DrawHourLine(QPainter &pointer);
    void DrawMinateLine(QPainter &pointer);
    void DrawSecLine(QPainter &pointer);
    void DrawDateLCD(QPainter &paint);
    QRectF textRectF(double radius, int pointSize, double angle);
    void paintEvent(QPaintEvent *event);
};
#endif // WIDGET_H
