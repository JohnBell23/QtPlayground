#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(new QSerialPort(this))
{    
    ui->setupUi(this);
    LoadComportList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSend_clicked()
{    
    doSend(ui->txtToSend->toPlainText());
}

void MainWindow::LoadComportList()
{
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);

    ui->cboComports->clear();

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->cboComports->addItem(info.portName());
    }

    openDefaultComport();
}

void MainWindow::openDefaultComport()
{
    for (int i = 0; i < ui->cboComports->count(); ++i) {
        QString comportName = ui->cboComports->itemText(i);
        if (openComport(comportName))
        {
            if (tryInitSend())
            {
                setActiveComport(comportName);
                return;
            }
        }
    }
}

bool MainWindow::tryInitSend()
{
    m_received="";
    doSend(this->INIT_SEND);

    QThread::msleep(200);

    return m_received == this->INIT_EXPECT;
}

void MainWindow::doSend(QString data)
{
    if (serial->isOpen()) {
        QString dataToSend = data;
        QByteArray byteArray = dataToSend.toUtf8();
        serial->write(byteArray);
    }
}

bool MainWindow::openComport(QString comportName)
{
    serial->setPortName(comportName);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    return (serial->open(QIODevice::ReadWrite));
}

void MainWindow::setActiveComport(QString compoortName)
{
    int index = ui->cboComports->findText(compoortName);
    if (index != -1)
    {
        ui->cboComports->setCurrentIndex(index);
    } else {
        qDebug() << "Item nicht gefunden";
    }
}

void MainWindow::readSerialData()
{
    QByteArray data = serial->readAll();

    QString receivedData = QString::fromUtf8(data);
    m_received=receivedData;

    qDebug() << "Empfangene Daten: " << receivedData;

    ui->txtReceived->append(receivedData);
}

void MainWindow::on_btnOpen_clicked()
{
    if (openComport(ui->cboComports->currentText())) {
        qDebug() << "Serielle Schnittstelle geöffnet";  
    } else {
        qDebug() << "Fehler beim Öffnen der seriellen Schnittstelle: " << serial->errorString();
    }
}

void MainWindow::on_btnSendTu_clicked()
{
    doSend("#tu;" + ui->txtTu->toPlainText());
}

