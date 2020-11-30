#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Port = new QSerialPort();
    this->setWindowTitle("串口助手");
    PortBuf.resize(1024*PortBufSize);

    ui->label_7->SetBackground(LED::red);
    connect(ui->comboBox,&MyComboBox::clicked,[=](){UPDataSerialPortList(ui->comboBox);});
    connect(ui->open_btn,&QPushButton::clicked,this,&MainWindow::OpenSeialPort);
    connect(ui->send_btn,&QPushButton::clicked,this,&MainWindow::SendData);
    connect(Port,&QSerialPort::readyRead,this,&MainWindow::RecvDataTimeDelay);
    connect(&PortTimer,&QTimer::timeout,this,&MainWindow::RecvData);
    connect(ui->clear_btn,&QPushButton::clicked,[=](){ui->textBrowser->clear();});
    connect(ui->clear_btn_2,&QPushButton::clicked,[=](){ui->textEdit->clear();});
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Port;

}
void MainWindow::SendData()
{
    QString str = ui->textEdit->toPlainText();
    if(ui->newline_chk->checkState()==Qt::Checked)
    {
        str.append("\r\n");
    }
    if(ui->newline_chk_2->checkState() == Qt::Checked)
    {
        QByteArray TEMP = str.toLocal8Bit();
        QByteArray data = TEMP.toHex();
        Port->write(data);

    }else
    {
        QByteArray data = str.toLocal8Bit() ;
        Port->write(data);
    }

}

void MainWindow::RecvDataTimeDelay(void)
{
    PortTimer.stop();
    PortTimer.start(50);

}
void MainWindow::RecvData()
{
    PortTimer.stop();
    PortBuf = Port->readAll();
    QTextCodec *tc = QTextCodec::codecForName("GBK");//解决接收中文字符乱码的问题
    ui->textBrowser->moveCursor(QTextCursor::End);
    if(ui->data_chk->isChecked())
    {
         ui->textBrowser->insertPlainText(QTime::currentTime().toString("hh:mm:ss").append("\n"));
    }
    if(ui->hex_chk->isChecked())
    {
        QByteArray temp;
        temp = PortBuf.toHex().toUpper();
        for (int i =0;i<temp.size();i+=2) {
            ui->textBrowser->insertPlainText(QString("%1%2 ").arg(temp.at(i)).arg((temp.at(i+1))));
            QScrollBar *s =ui->textBrowser->verticalScrollBar();//实现QTextBrowser内容自动滚动的效果
            s->setValue(s->maximum());
        }
        ui->textBrowser->insertPlainText("\r\n");
    }
    else
    {
        ui->textBrowser->insertPlainText(tc->toUnicode(PortBuf));
        QScrollBar *s =ui->textBrowser->verticalScrollBar();//实现QTextBrowser内容自动滚动的效果
        s->setValue(s->maximum());
    }
    PortBuf.clear();

}


void MainWindow::OpenSeialPort()
{
    if(!(ui->open_btn->isChecked()))
    {
        SerialPortConfig();
        bool rel = Port->open(QIODevice::ReadWrite);
        if(!rel)
        {
            QMessageBox::critical(NULL,"打开串口失败","串口被占用，或其他错误");
            ui->open_btn->setChecked(true);
        }
        else
        {
            ui->open_btn->setText("关闭串口");
            ui->label_7->SetBackground(LED::green);

            ui->comboBox->setEnabled(false);
            ui->baud_cbb->setEnabled(false);
            ui->party_cbb->setEnabled(false);
            ui->databit_cbb->setEnabled(false);
            ui->stopbit_cbb->setEnabled(false);

        }
    }
    else
    {
        ui->open_btn->setText("打开串口");
        ui->label_7->SetBackground(LED::red);
        Port->close();
        ui->comboBox->setEnabled(true);
        ui->baud_cbb->setEnabled(true);
        ui->party_cbb->setEnabled(true);
        ui->databit_cbb->setEnabled(true);
        ui->stopbit_cbb->setEnabled(true);
    }

}
void MainWindow::SerialPortConfig(void)
{
    QString PortNameInfo = ui->comboBox->currentText();
    QString PortName = PortNameInfo.split("-").at(0);
    Port->setPortName(PortName);
#ifdef DEBUG
    qDebug()<<PortName;
#endif

    switch (ui->baud_cbb->currentIndex()) {
    case 0: Port->setBaudRate(QSerialPort::Baud9600);   break;
    case 1: Port->setBaudRate(QSerialPort::Baud4800);   break;
    case 3: Port->setBaudRate(QSerialPort::Baud2400);   break;
    default:
        Port->setBaudRate(QSerialPort::Baud9600);
    }
#ifdef DEBUG
    qDebug()<<"波特率"<<ui->baud_cbb->currentIndex();
#endif

    switch (ui->party_cbb->currentIndex()) {
    case 0: Port->setParity(QSerialPort::NoParity);     break;
    case 1: Port->setParity(QSerialPort::OddParity);    break;
    case 2: Port->setParity(QSerialPort::EvenParity);   break;
    default:
        Port->setParity(QSerialPort::NoParity);
    }
#ifdef DEBUG
    qDebug()<<"校验位"<<ui->party_cbb->currentIndex();
#endif

    switch (ui->databit_cbb->currentIndex()) {
    case 0: Port->setDataBits(QSerialPort::Data8);  break;
    case 1: Port->setDataBits(QSerialPort::Data7);  break;
    case 2: Port->setDataBits(QSerialPort::Data6);  break;
    case 3: Port->setDataBits(QSerialPort::Data5);  break;
    default:
        Port->setDataBits(QSerialPort::Data8);
    }
#ifdef DEBUG
    qDebug()<<"数据位"<<ui->databit_cbb->currentIndex();
#endif
    switch (ui->stopbit_cbb->currentIndex()) {
    case 0: Port->setStopBits(QSerialPort::OneStop);            break;
    case 1: Port->setStopBits(QSerialPort::OneAndHalfStop);     break;
    case 2: Port->setStopBits(QSerialPort::TwoStop);            break;
    default:
        Port->setStopBits(QSerialPort::OneStop);
    }
#ifdef DEBUG
    qDebug()<<"停止位"<<ui->stopbit_cbb->currentIndex();
#endif
}

//p的格式：COMXX-XXXXX,获取串口号COM14,获取14
uint8_t MainWindow::GetSerialPortNumber(QString *p)
{
    QString S = p->split("-").at(0);
    return S.remove("COM").toInt();
}

//获取串口号和串口描述符
uint8_t MainWindow::GetSerialPortName(QVector<QString> &v)
{
    uint8_t num = 0;
    v.clear();
    foreach(auto port, QSerialPortInfo::availablePorts())
    {
        num++;
        v.push_back(QString(port.portName().append("-").append(port.description())));
    }
    return num;
}
//更新串口列表
void MainWindow::UPDataSerialPortList(QComboBox *p)
{
        QVector<QString>ComName;
        uint8_t num = GetSerialPortName(ComName);
        //可用串口大于2时，对可用串口排序
        if(num>2)
        {
            //使用选择排序
            for(QVector<QString>::iterator it = ComName.begin();it != ComName.end();it++)
            {
                auto min = it;
                for(auto j = it+1; j!=ComName.end();j++)
                {
                    if(GetSerialPortNumber(min) >GetSerialPortNumber(j))
                    {
                        min = j;
                    }
                }
                std::swap(*it,*min);
            }
        }
        //将串口号等信息加入到ui->combox
        p->clear();
        for(QVector<QString>::iterator it = ComName.begin();it != ComName.end();it++)
        {
            p->addItems(QStringList(*it));
        }

}

