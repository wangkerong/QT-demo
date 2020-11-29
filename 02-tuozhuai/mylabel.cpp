#include "mylabel.h"

mylabel::mylabel(QWidget *parent) : QLabel(parent)
{
    filename.clear();
    this->setAcceptDrops(true);//是否启用拖放事件
}

void mylabel::dragEnterEvent(QDragEnterEvent *event)
{
    QString path = event->mimeData()->text();
    if(filename.isEmpty())
    {
        filename = path;
    }
    else {
        if(filename != path)
        {
            filename = path;
        }
    }
    event->accept();//事件已处理，不需要再传递

}
void mylabel::dropEvent(QDropEvent *event)
{
    if(filename.isEmpty())
        return;
    filename.replace("file:///","");    //替换路径字符串中的固定头为""空，即去掉这些字符串
    QPixmap pixmap(filename);
    pixmap = pixmap.scaled(this->size(),Qt::IgnoreAspectRatio);
    this->setPixmap(pixmap);
//    this->setScaledContents(false);  //缩放图片以填充整个区域


}



