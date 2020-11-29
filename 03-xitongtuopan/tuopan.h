#ifndef TUOPAN_H
#define TUOPAN_H

#include <QWidget>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QDebug>
#include <QMenu>
#include <QAction>
QT_BEGIN_NAMESPACE
namespace Ui { class tuopan; }
QT_END_NAMESPACE

class tuopan : public QWidget
{
    Q_OBJECT

public:
    tuopan(QWidget *parent = nullptr);
    ~tuopan();
    void closeEvent(QCloseEvent *event);
    void CreatMeum();
    void CreatMeum2();
    void CreatAction();

private:
    Ui::tuopan *ui;
    QSystemTrayIcon *mSysTrayIcon;
    QTimer *timer;
    QMenu *menu;
    QMenu *menu2;

    QAction *open;
    QAction *close;

    bool flag;


};
#endif // TUOPAN_H
