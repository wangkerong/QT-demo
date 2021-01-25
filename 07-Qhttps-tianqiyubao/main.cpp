#include "widget.h"
#include <QApplication>
#include <QIcon>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowIcon(QIcon(":/title/image/title.png"));
    w.setWindowTitle(QString::fromLocal8Bit("天气"));
    //w.setBackgroundRole(QPalette::Dark);
    w.show();

    return a.exec();
}
/*
https访问需要用到SSL认证，而QT默认是不支持SSL认证，所以在使用之前必须先做一些准备工作：需要安装OpenSSL库
参考：https://blog.csdn.net/itjobtxq/article/details/8244509
*/
