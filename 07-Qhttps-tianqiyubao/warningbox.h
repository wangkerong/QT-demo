#ifndef WARNINGBOX_H
#define WARNINGBOX_H


#include <QLabel>
#include <QTimer>
#include <QString>
#include <QDebug>
class warningbox : public QLabel
{
    Q_OBJECT
public:
    explicit warningbox(QWidget *parent = nullptr);
    ~warningbox(void);
    void showMessage(int sec, const char *text,int x,int y);
private:
    QTimer timer;

signals:

};

#endif // WARNINGBOX_H
