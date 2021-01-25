#include "mylable.h"

MyLable::MyLable(QWidget *parent) : QLabel(parent)
{

}


void MyLable::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit click();
    }
}



