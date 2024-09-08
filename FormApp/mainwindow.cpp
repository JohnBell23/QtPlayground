#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>

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
    if (serial->isOpen()) {
        QString dataToSend = ui->txtToSend->toPlainText();
        QByteArray byteArray = dataToSend.toUtf8();  // Konvertiere String in Byte-Array
        serial->write(byteArray);  // Sende die Daten
        qDebug() << "Daten gesendet: " << dataToSend;
    } else {
        qDebug() << "Serielle Schnittstelle ist nicht geöffnet.";
    }
}

void MainWindow::LoadComportList()
{
    ui->cboComports->clear();

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->cboComports->addItem(info.portName());
    }
}

void MainWindow::readSerialData()
{
    QByteArray data = serial->readAll();

    QString receivedData = QString::fromUtf8(data);

    qDebug() << "Empfangene Daten: " << receivedData;

    ui->txtReceived->append(receivedData);
}

void MainWindow::on_btnOpen_clicked()
{
    serial->setPortName(ui->cboComports->currentText()); // Beispiel: COM1 (oder eine andere verfügbare Schnittstelle)
    serial->setBaudRate(QSerialPort::Baud115200);  // Baudrate setzen
    serial->setDataBits(QSerialPort::Data8);     // 8 Datenbits
    serial->setParity(QSerialPort::NoParity);    // Keine Parität
    serial->setStopBits(QSerialPort::OneStop);   // 1 Stopbit
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Serielle Schnittstelle geöffnet";
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    } else {
        qDebug() << "Fehler beim Öffnen der seriellen Schnittstelle: " << serial->errorString();
    }
}
