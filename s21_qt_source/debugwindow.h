#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QDialog>

namespace Ui {
class DebugWindow;
}

class DebugWindow : public QDialog {
  Q_OBJECT

 public:
  explicit DebugWindow(QWidget *parent = nullptr);
  ~DebugWindow();
  static void print_data(QString);

 private:
  Ui::DebugWindow *ui;

 public slots:
  void recieveData(QString str);
};

#endif  // DEBUGWINDOW_H
