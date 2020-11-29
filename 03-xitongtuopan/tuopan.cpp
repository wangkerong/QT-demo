#include "tuopan.h"
#include "ui_tuopan.h"

tuopan::tuopan(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::tuopan)
{
    ui->setupUi(this);
    flag = false;
    timer = new QTimer();
    //新建QSystemTrayIcon对象
    mSysTrayIcon = new QSystemTrayIcon(this);

    //隐藏托盘后一秒钟发送消息
    connect(timer,&QTimer::timeout,[=](){
       timer->stop();
       QIcon icon2(":/new/prefix1/git.png");
      mSysTrayIcon->showMessage("title","This is Test Message",icon2,10);
    });
    //用户点击消息直接打开主界面
    connect(mSysTrayIcon,&QSystemTrayIcon::messageClicked,[=](){
       this->show();
    });
    //鼠标点击托盘后的操作
    void (QSystemTrayIcon::*ptr)(QSystemTrayIcon::ActivationReason val) = NULL;
    ptr = &QSystemTrayIcon::activated;
    connect(mSysTrayIcon,ptr,[=](QSystemTrayIcon::ActivationReason val){
        switch (val) {
        case QSystemTrayIcon::Context : /*something*/;break;
        case QSystemTrayIcon::DoubleClick : QWidget::show();mSysTrayIcon->hide();break;  //双击托盘图标显示主界面
        case QSystemTrayIcon::Trigger :/*do something */; break;
        case QSystemTrayIcon::MiddleClick: /* do something */break;
        default:
            /*do something*/;
        }
    });

}


void tuopan::CreatAction()
{

    open = new QAction(QIcon(":/new/prefix1/1.png"),QString("OPEN MAIN WINDOW").toUtf8(),this);
    close = new QAction(QIcon(":/new/prefix1/git.png"),QString("QUIT").toUtf8(),this);
    connect(close,&QAction::triggered,[=](){
        flag = true;
        QWidget::close();
    });

    //对OPEN选项添加二级子菜单
    CreatMeum2();
    open->setMenu(menu2);

}

void tuopan::CreatMeum2()
{
    menu2 = new QMenu(this);
    menu2->addAction(QIcon(":/new/prefix1/1.png"),QString("test1").toUtf8());
    menu2->addAction(QIcon(":/new/prefix1/1.png"),QString("test2").toUtf8());
    menu2->addAction(QIcon(":/new/prefix1/1.png"),QString("test3").toUtf8());
    menu2->addAction(QIcon(":/new/prefix1/1.png"),QString("test4").toUtf8());
}
void tuopan::CreatMeum()
{
    CreatAction();
    menu = new QMenu(this);
    menu->addAction(open);
//    menu->addSeparator();//添加分隔符
    menu->addAction(close);
    mSysTrayIcon->setContextMenu(menu);
}
tuopan::~tuopan()
{
    delete ui;
    delete timer;
    delete mSysTrayIcon;

}

void tuopan::closeEvent(QCloseEvent *event)
{
    if(flag)
    {

    }
    else {
        timer->start(2000);
        //隐藏程序主窗口
        this->hide();
        //新建托盘要显示的icon
        QIcon icon(":/new/prefix1/1.png");
        //将icon设到QSystemTrayIcon对象中
        mSysTrayIcon->setIcon(icon);
        //当鼠标移动到托盘上的图标时，会显示此处设置的内容
        mSysTrayIcon->setToolTip(QString("Test Syseam tuopan ICON").toUtf8());
        //创建系统托盘图标的菜单
        CreatMeum();
        //在系统托盘显示此对象
        mSysTrayIcon->show();
        event->ignore();
    }

}
