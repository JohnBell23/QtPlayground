#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSend_clicked();
    void on_btnOpen_clicked();
    void readSerialData();

    void on_pushButton_clicked();

private:
    const QString INIT_SEND="#gst";
    const QString INIT_EXPECT="#gst";

    Ui::MainWindow *ui;
    QSerialPort *serial;

    void LoadComportList();
    void sendTu();
};
#endif // MAINWINDOW_H
