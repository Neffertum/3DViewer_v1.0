#include "debugwindow.h"

#include "ui_debugwindow.h"

DebugWindow::DebugWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::DebugWindow) {
  ui->setupUi(this);
}

DebugWindow::~DebugWindow() { delete ui; }

void DebugWindow::print_data(QString) { qDebug() << "Hello"; }

void DebugWindow::recieveData(QString str) {
  //    ui->label->setText(str);
  ui->textBrowser->setText(str);
}
