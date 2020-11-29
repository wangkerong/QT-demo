#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraViewfinderSettings>
#include <QFileDialog>
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
class QComboBox;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void addViewfinderToCombox(QComboBox *p);
    void switchViewfinderSize(int val);
    void save(int id, const QString &fileName);
protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void SHOUIMAGE(int val,const QImage &image);

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
};
#endif // WIDGET_H
