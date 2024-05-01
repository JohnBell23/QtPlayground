#include "QtWidgetsApplication1.h"

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.btnSet, &QPushButton::clicked, this, &QtWidgetsApplication1::onButtonSetClicked);
    connect(ui.btnDelete, &QPushButton::clicked, this, &QtWidgetsApplication1::onButtonDeleteClicked);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}

void QtWidgetsApplication1::onButtonDeleteClicked()
{
    ui.textEdit->setText("");
}

void QtWidgetsApplication1::onButtonSetClicked() {
    ui.textEdit->setText("hello world");
}
