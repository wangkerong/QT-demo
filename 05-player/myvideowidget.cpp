#include "myvideowidget.h"

MyVideoWidget::MyVideoWidget(QWidget *parent) : QVideoWidget(parent)
{

}

void MyVideoWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        emit KeyESCPress();
    }

}

void MyVideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit LeftDbClick();
    }

}
