#include "mycombobox.h"

MyComboBox::MyComboBox(QWidget *parent):QComboBox(parent)
{

}

void MyComboBox::mousePressEvent(QMouseEvent *event)  //添加鼠标点击事件
{

    if(event->button() ==Qt::LeftButton)
        emit clicked();

    QComboBox::mousePressEvent(event);
}
