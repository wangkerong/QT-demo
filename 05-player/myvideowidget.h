#ifndef MYVIDEOWIDGET_H
#define MYVIDEOWIDGET_H

#include <QWidget>
#include <QVideoWidget>
#include <QKeyEvent>
#include <QMouseEvent>
class MyVideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    explicit MyVideoWidget(QWidget *parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *e); //鼠标双击事件


signals:
    void KeyESCPress();
    void LeftDbClick();

};

#endif // MYVIDEOWIDGET_H
