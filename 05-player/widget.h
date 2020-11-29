#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMediaPlayer>
#include "myvideowidget.h"
#include <QString>
#include <QChar>
#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>
#include <QSize>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void Init();
    void HideCortil();
    void QuitFullScreen();
    void SetVideoFullScreen();


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_dial_valueChanged(int value);
    void VideoTime(qint64 val);
    void on_horizontalSlider_valueChanged(int value);
    void videoend(QMediaPlayer::MediaStatus status);
    void on_horizontalSlider_sliderPressed();
    void on_horizontalSlider_sliderReleased();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::Widget *ui;
    MyVideoWidget *VideoWidget;
    QMediaPlayer *MediaPlay;
    QString filename;   //文件路径
    QVBoxLayout *layout;
    QTimer *timer;
    bool ispress;
    QSize size;     //保存全屏之前窗口的尺寸

signals:
    void changed();
};
#endif // WIDGET_H
