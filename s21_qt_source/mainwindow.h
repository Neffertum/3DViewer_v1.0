#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QSettings>

#include "../s21_c_source/s21_3d_viewer_1_0.h"
#include "debugwindow.h"
#include "qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  dataStruct st_data;

  void readSettings();
  void writeSettings();

 private slots:

  void on_pushButton_file_select_clicked();

  void on_horizontalSlider_scale_valueChanged(int value);

  void on_horizontalSlider_moveX_valueChanged(int value);

  void on_horizontalSlider_moveY_valueChanged(int value);

  void on_horizontalSlider_moveZ_valueChanged(int value);

  void on_horizontalSlider_rotateX_valueChanged(int value);

  void on_horizontalSlider_rotateY_valueChanged(int value);

  void on_horizontalSlider_rotateZ_valueChanged(int value);

  void on_pushButton_ok_scale_clicked();

  void on_pushButton_center_clicked();

  void on_pushButton_change_clicked();

  void on_pushButton_reset_clicked();

 private:
  Ui::MainWindow *ui;
  DebugWindow *debugW;

  void move_c(int value, int mode);
  QString print_struct();
  void sliders_to_middle();
  void init_labels();
  void init_radioButtons_vertex();
  void init_radioButtons_line();
  void init_radioButtons_projection();

  bool auto_y;

  QTimer *timer;

  QGifImage *my_gif;
  QTimer *gif_timer;
  int start_time;
  int temp_time;
  float print_time;

 signals:

  void signal();

  void sendData(QString str);

 private slots:

  void onButtonSend();
  void on_horizontalSlider_vertexSize_valueChanged(int value);
  void on_radioButton_no_clicked();
  void on_radioButton_circle_clicked();
  void on_radioButton_square_clicked();
  void on_pushButton_a_clicked();
  void onTimeout();
  void on_horizontalSlider_l_color_valueChanged(int value);
  void on_horizontalSlider_p_color_valueChanged(int value);
  void on_horizontalSlider_bg_color_valueChanged(int value);
  void on_radioButton_no_line_clicked();
  void on_radioButton_solid_line_clicked();
  void on_radioButton_dotted_line_clicked();
  void on_horizontalSlider_line_width_valueChanged(int value);
  void on_radioButton_parr_clicked();
  void on_radioButton_center_clicked();
  void on_pushButton_img_clicked();

  void start_gif();
  void make_gif();
};
#endif  // MAINWINDOW_H
