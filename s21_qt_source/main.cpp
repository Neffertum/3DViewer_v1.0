#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  //        w.readSettings();
  w.show();
  //        a.exec();
  return a.exec();
}
