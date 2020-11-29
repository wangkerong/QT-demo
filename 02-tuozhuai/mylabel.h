#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
class mylabel : public QLabel
{
    Q_OBJECT
public:
    explicit mylabel(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event);    //鼠标拖拽进入事件
    void dropEvent(QDropEvent *event);


private:
    QString filename;
signals:

};

#endif // MYLABEL_H
