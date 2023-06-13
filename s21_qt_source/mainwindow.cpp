#include "mainwindow.h"

#include <s21_gl_view.h>

#include <QString>
#include <QTimer>

#include "debugwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle("3d_viewer_1_0_may_2023");

  auto_y = false;
  timer = new QTimer(this);

  debugW = new DebugWindow(this);
  debugW->setWindowTitle("data");

  init_labels();

  init_radioButtons_vertex();
  init_radioButtons_line();
  init_radioButtons_projection();

  ui->gl_window->slider_scale = ui->horizontalSlider_scale->value();

  ui->gl_window->slider_XYZ[X] = ui->horizontalSlider_moveX->value();
  ui->gl_window->slider_XYZ[Y] = ui->horizontalSlider_moveY->value();
  ui->gl_window->slider_XYZ[Z] = ui->horizontalSlider_moveZ->value();

  ui->gl_window->slider_rotate_XYZ[X] = ui->horizontalSlider_rotateX->value();
  ui->gl_window->slider_rotate_XYZ[Y] = ui->horizontalSlider_rotateY->value();
  ui->gl_window->slider_rotate_XYZ[Z] = ui->horizontalSlider_rotateZ->value();

  readSettings();

  ui->gl_window->projection = ui->buttonGroup_projection->checkedId();
  ui->gl_window->line = ui->buttonGroup_line->checkedId();
  ui->gl_window->vertex = ui->buttonGroup_vertex->checkedId();
  ui->gl_window->bg_color[RED] =
      ((double)ui->horizontalSlider_bg_color->value()) / 100;
  ui->gl_window->bg_color[GREEN] =
      ((double)ui->horizontalSlider_bg_color->value() + 10) / 100;
  ui->gl_window->bg_color[BLUE] =
      ((double)ui->horizontalSlider_bg_color->value() + 20) / 100;

  int v = ui->horizontalSlider_l_color->value();

  ui->gl_window->l_color[RED] = (double)((v + 30) % 100) / 100;
  ui->gl_window->l_color[GREEN] = (double)((v + 60) % 100) / 100;
  ui->gl_window->l_color[BLUE] = (double)((v + 90) % 100) / 100;

  //    ui->label_scale->setText(QString::number(ui->horizontalSlider_scale->value()));
  connect(ui->pushButton_debug, SIGNAL(clicked()), debugW, SLOT(show()));
  connect(ui->pushButton_debug, SIGNAL(clicked()), this, SLOT(onButtonSend()));
  connect(this, SIGNAL(sendData(QString)), debugW, SLOT(recieveData(QString)));

  connect(ui->pushButton_gif, SIGNAL(clicked()), this, SLOT(start_gif()));

  //    ui->buttonGroup_vertex->checkedId();
}

MainWindow::~MainWindow() {
  delete ui;
  writeSettings();
}

void MainWindow::onTimeout() {
  ui->horizontalSlider_rotateY->setValue(
      (ui->horizontalSlider_rotateY->value() + 1) % 360);
}

void MainWindow::init_radioButtons_projection() {
  char v = ui->gl_window->projection;
  if (v == PARR) {
    ui->radioButton_parr->setChecked(true);
  } else if (v == CENTER) {
    ui->radioButton_center->setChecked(true);
  }
  ui->buttonGroup_projection->setId(ui->radioButton_parr, 0);
  ui->buttonGroup_projection->setId(ui->radioButton_center, 1);
}

void MainWindow::init_radioButtons_line() {
  char v = ui->gl_window->line;
  if (v == NO) {
    ui->radioButton_no_line->setChecked(true);
  } else if (v == SOLID) {
    ui->radioButton_solid_line->setChecked(true);
  } else if (v == DOTTED) {
    ui->radioButton_dotted_line->setChecked(true);
  }

  ui->buttonGroup_line->setId(ui->radioButton_no, 0);
  ui->buttonGroup_line->setId(ui->radioButton_solid_line, 1);
  ui->buttonGroup_line->setId(ui->radioButton_dotted_line, 2);
}

void MainWindow::init_radioButtons_vertex() {
  char v = ui->gl_window->vertex;
  if (v == NO) {
    ui->radioButton_no->setChecked(true);
  } else if (v == CIRCLE) {
    ui->radioButton_circle->setChecked(true);
  } else if (v == SQUARE) {
    ui->radioButton_square->setChecked(true);
  }

  ui->buttonGroup_vertex->setId(ui->radioButton_no, 0);
  ui->buttonGroup_vertex->setId(ui->radioButton_circle, 1);
  ui->buttonGroup_vertex->setId(ui->radioButton_square, 2);
}

void MainWindow::sliders_to_middle() {
  ui->horizontalSlider_scale->setValue(50);
  ui->horizontalSlider_moveX->setValue(50);
  ui->horizontalSlider_moveY->setValue(50);
  ui->horizontalSlider_moveZ->setValue(50);
  ui->horizontalSlider_rotateX->setValue(180);
  ui->horizontalSlider_rotateY->setValue(180);
  ui->horizontalSlider_rotateZ->setValue(180);
}

void MainWindow::init_labels() {
  ui->label_scale->setText(
      "Scale: " + QString::number(ui->horizontalSlider_scale->value()) + " %");

  ui->label_X->setText("X: " +
                       QString::number(ui->horizontalSlider_moveX->value()));
  ui->label_Y->setText("Y: " +
                       QString::number(ui->horizontalSlider_moveY->value()));
  ui->label_Z->setText("Z: " +
                       QString::number(ui->horizontalSlider_moveZ->value()));

  ui->label_X_r->setText(
      "X: " + QString::number(round(ui->horizontalSlider_rotateX->value())));
  ui->label_Y_r->setText(
      "Y: " + QString::number(round(ui->horizontalSlider_rotateY->value())));
  ui->label_Z_r->setText(
      "Z: " + QString::number(round(ui->horizontalSlider_rotateZ->value())));
}

void MainWindow::on_pushButton_file_select_clicked() {
  const QString currentExecDir = QCoreApplication::applicationDirPath();
  QString str;
  str = QFileDialog::getOpenFileName(
      this, "Выбрать файл", currentExecDir + "/../../../../examples_obj",
      ".obj files (*.obj)");

  ui->statusbar->showMessage(str);

  QByteArray ba = str.toLocal8Bit();
  const char *file = ba.data();

  st_data = start_c_obj_parsing(file);
  if (st_data.count_of_facets <= 0 && st_data.count_of_vertexes <= 0) {
    ui->label_error->setText("WRONG DATA");
  } else {
    ui->label_error->setText("OK");
  }

  sliders_to_middle();
  auto_y = false;
  ui->pushButton_a->setText("A-off");
  disconnect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

  ui->gl_window->st_data = st_data;

  //    print_struct();
}

QString MainWindow::print_struct() {
  QString debug = "";
  debug +=
      "Вершин: " + QString::number(ui->gl_window->st_data.count_of_vertexes) +
      ",\n";
  debug += "Ребер: " + QString::number(ui->gl_window->st_data.count_of_facets) +
           ",\n";
  debug += "МАТРИЦА ВЕРШИН:\n";
  for (int i = 0; i < st_data.count_of_vertexes; i++) {
    debug += "<" + QString::number(i) + ">  ";
    debug +=
        QString::number(ui->gl_window->st_data.matrix_3d.matrix[i][0]) + "  ";
    debug +=
        QString::number(ui->gl_window->st_data.matrix_3d.matrix[i][1]) + "  ";
    debug +=
        QString::number(ui->gl_window->st_data.matrix_3d.matrix[i][2]) + "\n";
  }
  debug += "МАССИВ ГРАНЕЙ:\n";
  for (int i = 0; i < ui->gl_window->st_data.count_of_facets; i++) {
    debug += "<" + QString::number(i) + ">  ВЕРШИН: ";
    int edges = ui->gl_window->st_data.polygons[i].num_of_ver_in_facets;
    debug += QString::number(edges) + " : ";
    for (int k = 0; k < edges; k++) {
      debug += QString::number(ui->gl_window->st_data.polygons[i].vertexes[k]);
      if (k < edges - 1) {
        debug += "->";
      } else {
        debug += "\n";
      }
    }
  }
  debug += "minX: " + QString::number(ui->gl_window->st_data.minMaxX[0]) + "\n";
  debug += "maxX: " + QString::number(ui->gl_window->st_data.minMaxX[1]) + "\n";

  debug += "minY: " + QString::number(ui->gl_window->st_data.minMaxY[0]) + "\n";
  debug += "maxY: " + QString::number(ui->gl_window->st_data.minMaxY[1]) + "\n";

  debug += "minZ: " + QString::number(ui->gl_window->st_data.minMaxZ[0]) + "\n";
  debug += "maxZ: " + QString::number(ui->gl_window->st_data.minMaxZ[1]) + "\n";

  return debug;
}

void MainWindow::on_horizontalSlider_scale_valueChanged(int value) {
  int curr_v = ui->gl_window->slider_scale;
  ui->gl_window->slider_scale = value;

  double v = (double)value / (double)curr_v;

  dyn_scaling(&(ui->gl_window->st_data), v);

  ui->label_scale->setText("Scale: " + QString::number(value) + " %");

  ui->gl_window->update();
}

void MainWindow::on_horizontalSlider_moveX_valueChanged(int value) {
  move_c(value, X);

  ui->label_X->setText("X: " + QString::number(value));

  //    print_struct();
}

void MainWindow::on_horizontalSlider_moveY_valueChanged(int value) {
  move_c(value, Y);

  ui->label_Y->setText("Y: " + QString::number(value));
}

void MainWindow::on_horizontalSlider_moveZ_valueChanged(int value) {
  move_c(value, Z);

  ui->label_Z->setText("Z: " + QString::number(value));
}

void MainWindow::move_c(int value, int mode) {
  int curr = ui->gl_window->slider_XYZ[mode];
  ui->gl_window->slider_XYZ[mode] = value;
  double v = ((double)value - (double)curr) / 100;
  move_coord(&(ui->gl_window->st_data), v, mode);
  ui->gl_window->update();
}

void MainWindow::on_horizontalSlider_rotateX_valueChanged(int value) {
  int curr = ui->gl_window->slider_rotate_XYZ[X];
  ui->gl_window->slider_rotate_XYZ[X] = value;
  double v = (double)value - (double)curr;
  rotateX(&(ui->gl_window->st_data), v * 0.01745329);
  ui->label_X_r->setText("X: " + QString::number(round(value)));
  ui->gl_window->update();
}

void MainWindow::on_horizontalSlider_rotateY_valueChanged(int value) {
  int curr = ui->gl_window->slider_rotate_XYZ[Y];
  ui->gl_window->slider_rotate_XYZ[Y] = value;
  double v = (double)value - (double)curr;
  rotateY(&(ui->gl_window->st_data), v * 0.01745329);
  ui->label_Y_r->setText("Y: " + QString::number(round(value)));
  ui->gl_window->update();
}

void MainWindow::on_horizontalSlider_rotateZ_valueChanged(int value) {
  int curr = ui->gl_window->slider_rotate_XYZ[Z];
  ui->gl_window->slider_rotate_XYZ[Z] = value;
  double v = (double)value - (double)curr;
  rotateZ(&(ui->gl_window->st_data), v * 0.01745329);
  ui->label_Z_r->setText("Z: " + QString::number(round(value)));
  ui->gl_window->update();
}

void MainWindow::on_pushButton_ok_scale_clicked() {
  int v = ui->horizontalSlider_scale->value();
  int vv = ui->spinBox_scale->value();
  ui->horizontalSlider_scale->setValue(v + vv);
  ui->spinBox_scale->setValue(0);
}

void MainWindow::on_pushButton_center_clicked() {
  if (ui->gl_window->st_data.count_of_vertexes > 0) {
    //        sliders_to_middle();
    ui->horizontalSlider_moveX->setValue(50);
    ui->horizontalSlider_moveY->setValue(50);
    ui->horizontalSlider_moveZ->setValue(50);
    find_min_max(&(ui->gl_window->st_data));
    centering(&(ui->gl_window->st_data));
    ui->gl_window->update();
    //        print_struct();
  }
}

void MainWindow::on_pushButton_change_clicked() {
  if (ui->spinBox_x->value() != 0) {
    ui->horizontalSlider_moveX->setValue(ui->horizontalSlider_moveX->value() +
                                         ui->spinBox_x->value());
    ui->spinBox_x->setValue(0);
  }
  if (ui->spinBox_y->value() != 0) {
    ui->horizontalSlider_moveY->setValue(ui->horizontalSlider_moveY->value() +
                                         ui->spinBox_y->value());
    ui->spinBox_y->setValue(0);
  }
  if (ui->spinBox_z->value() != 0) {
    ui->horizontalSlider_moveZ->setValue(ui->horizontalSlider_moveZ->value() +
                                         ui->spinBox_z->value());
    ui->spinBox_z->setValue(0);
  }
  if (ui->spinBox_x_r->value() != 0) {
    ui->horizontalSlider_rotateX->setValue(
        ui->horizontalSlider_rotateX->value() + ui->spinBox_x_r->value());
    ui->spinBox_x_r->setValue(0);
  }
  if (ui->spinBox_y_r->value() != 0) {
    ui->horizontalSlider_rotateY->setValue(
        ui->horizontalSlider_rotateY->value() + ui->spinBox_y_r->value());
    ui->spinBox_y_r->setValue(0);
  }
  if (ui->spinBox_z_r->value() != 0) {
    ui->horizontalSlider_rotateZ->setValue(
        ui->horizontalSlider_rotateZ->value() + ui->spinBox_z_r->value());
    ui->spinBox_z_r->setValue(0);
  }
}

void MainWindow::on_pushButton_reset_clicked() {
  QString str = ui->statusbar->currentMessage();
  QByteArray ba = str.toLocal8Bit();
  const char *file = ba.data();
  sliders_to_middle();
  ui->gl_window->st_data = start_c_obj_parsing(file);
}

// void MainWindow::on_pushButton_debug_clicked()
//{

//    debugW->show();
//    DebugWindow::print_data("LALALA");
//}

void MainWindow::onButtonSend() {
  if (ui->gl_window->st_data.count_of_vertexes > 0 &&
      ui->gl_window->st_data.count_of_facets > 0) {
    emit sendData(print_struct());
  } else {
    emit sendData("WRONG DATA");
  }
}

void MainWindow::on_horizontalSlider_vertexSize_valueChanged(int value) {
  ui->gl_window->vertex_size = (double)value / 10;
  ui->gl_window->update();
}

void MainWindow::on_radioButton_no_clicked() {
  ui->gl_window->vertex = NO;
  ui->gl_window->update();
}

void MainWindow::on_radioButton_circle_clicked() {
  ui->gl_window->vertex = CIRCLE;
  ui->gl_window->update();
}

void MainWindow::on_radioButton_square_clicked() {
  ui->gl_window->vertex = SQUARE;
  ui->gl_window->update();
}

void MainWindow::on_pushButton_a_clicked() {
  if (auto_y == false) {
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start(50);
    ui->pushButton_a->setText("A-on");
    auto_y = true;
  } else {
    disconnect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->stop();
    ui->pushButton_a->setText("A-off");
    auto_y = false;
  }
}

void MainWindow::on_horizontalSlider_l_color_valueChanged(int value) {
  int r, g, b;
  r = (30 + value) % 100;
  g = (60 + value) % 100;
  b = (90 + value) % 100;

  ui->gl_window->l_color[RED] = (double)r / 100;
  ui->gl_window->l_color[GREEN] = (double)g / 100;
  ui->gl_window->l_color[BLUE] = (double)b / 100;

  ui->gl_window->update();
}

void MainWindow::on_horizontalSlider_p_color_valueChanged(int value) {
  int r, g, b;
  r = (30 + value) % 100;
  g = (60 + value) % 100;
  b = (90 + value) % 100;

  ui->gl_window->p_color[RED] = (double)r / 100;
  ui->gl_window->p_color[GREEN] = (double)g / 100;
  ui->gl_window->p_color[BLUE] = (double)b / 100;
  ui->gl_window->update();
}

void MainWindow::on_horizontalSlider_bg_color_valueChanged(int value) {
  int r, g, b;
  r = (value) % 100;
  g = (10 + value) % 100;
  b = (20 + value) % 100;

  ui->gl_window->bg_color[RED] =
      ((double)r + ui->gl_window->bg_color[RED]) / 100;
  ui->gl_window->bg_color[GREEN] =
      ((double)g + ui->gl_window->bg_color[GREEN]) / 100;
  ui->gl_window->bg_color[BLUE] =
      ((double)b + ui->gl_window->bg_color[BLUE]) / 100;

  QString deb = "";
  deb += "R: " + QString::number(ui->gl_window->bg_color[RED]);
  deb += " G: " + QString::number(ui->gl_window->bg_color[GREEN]);
  deb += " B: " + QString::number(ui->gl_window->bg_color[BLUE]);
  //    ui->label_color->setText(deb);

  ui->gl_window->update();
}

void MainWindow::on_radioButton_no_line_clicked() {
  ui->gl_window->line = NO;
  ui->gl_window->update();
}

void MainWindow::on_radioButton_solid_line_clicked() {
  ui->gl_window->line = SOLID;
  ui->gl_window->update();
}

void MainWindow::on_radioButton_dotted_line_clicked() {
  ui->gl_window->line = DOTTED;
  ui->gl_window->update();
}

void MainWindow::on_horizontalSlider_line_width_valueChanged(int value) {
  ui->gl_window->line_width = (double)value / 10;
  ui->gl_window->update();
}

void MainWindow::on_radioButton_parr_clicked() {
  ui->gl_window->projection = PARR;
  ui->gl_window->update();
}

void MainWindow::on_radioButton_center_clicked() {
  ui->gl_window->projection = CENTER;
  ui->gl_window->update();
}

void MainWindow::on_pushButton_img_clicked() {
  //    writeSettings();
  QString file = QFileDialog::getSaveFileName(this, "Save as...", "name.bmp",
                                              "BMP (*.bmp);; JPEG (*.jpeg)");
  ui->gl_window->grabFramebuffer().save(file, NULL, -1);
}

void MainWindow::writeSettings() {
  const QString currentExecDir = QCoreApplication::applicationDirPath();
  QFile file(currentExecDir + '/' + "cnf.txt");
  //    QFile file = QFile(":/config/conf.txt");
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    if (ui->radioButton_parr->isChecked()) {
      file.write("Proj_1: 1\n");
    } else {
      file.write("Proj_1: 0\n");
    }
    if (ui->radioButton_center->isChecked()) {
      file.write("Proj_2: 1\n");
    } else {
      file.write("Proj_2: 0\n");
    }
    if (ui->radioButton_no_line->isChecked()) {
      file.write("Line_1: 1\n");
    } else {
      file.write("Line_1: 0\n");
    }
    if (ui->radioButton_solid_line->isChecked()) {
      file.write("Line_2: 1\n");
    } else {
      file.write("Line_3: 0\n");
    }
    if (ui->radioButton_dotted_line->isChecked()) {
      file.write("Line_3: 1\n");
    } else {
      file.write("Line_3: 0\n");
    }
    if (ui->radioButton_no->isChecked()) {
      file.write("Vert_1: 1\n");
    } else {
      file.write("Vert_1: 0\n");
    }
    if (ui->radioButton_circle->isChecked()) {
      file.write("Vert_2: 1\n");
    } else {
      file.write("Vert_2: 0\n");
    }
    if (ui->radioButton_square->isChecked()) {
      file.write("Vert_3: 1\n");
    } else {
      file.write("Vert_3: 0\n");
    }
    file.write(
        "LColor: " + QByteArray::number(ui->horizontalSlider_l_color->value()) +
        "\n");
    file.write("LWidth: " +
               QByteArray::number(ui->horizontalSlider_line_width->value()) +
               "\n");
    file.write(
        "PColor: " + QByteArray::number(ui->horizontalSlider_p_color->value()) +
        "\n");
    file.write("VerSiz: " +
               QByteArray::number(ui->horizontalSlider_vertexSize->value()) +
               "\n");
    file.write("BColor: " +
               QByteArray::number(ui->horizontalSlider_bg_color->value()) +
               "\n");

    file.close();
  }
}

void MainWindow::readSettings() {
  const QString currentExecDir = QCoreApplication::applicationDirPath();
  QFile file(currentExecDir + '/' + "cnf.txt");
  //    QFile file(":/config/conf.txt");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    int temp = 0;
    while (!file.atEnd()) {
      QString str = file.readLine();
      if (str[0] == 'P' && str[5] == '1') {
        ui->radioButton_parr->setChecked(str[8].digitValue());
      }
      if (str[0] == 'P' && str[5] == '2') {
        ui->radioButton_center->setChecked(str[8].digitValue());
      }
      if (str[0] == 'L' && str[5] == '1') {
        ui->radioButton_no_line->setChecked(str[8].digitValue());
      }
      if (str[0] == 'L' && str[5] == '2') {
        ui->radioButton_solid_line->setChecked(str[8].digitValue());
      }
      if (str[0] == 'L' && str[5] == '3') {
        ui->radioButton_dotted_line->setChecked(str[8].digitValue());
      }
      if (str[0] == 'V' && str[5] == '1') {
        ui->radioButton_no->setChecked(str[8].digitValue());
      }
      if (str[0] == 'V' && str[5] == '2') {
        ui->radioButton_circle->setChecked(str[8].digitValue());
      }
      if (str[0] == 'V' && str[5] == '3') {
        ui->radioButton_square->setChecked(str[8].digitValue());
      }
      if (str[0] == 'L' && str[5] == 'r') {
        temp = str[8].digitValue();
        if (str[9] != '\n') {
          temp = temp * 10 + str[9].digitValue();
          if (str[10] != '\n') {
            temp = temp * 10 + str[10].digitValue();
          }
        }
        ui->horizontalSlider_l_color->setValue(temp);
      }
      if (str[0] == 'L' && str[5] == 'h') {
        temp = str[8].digitValue();
        if (str[9] != '\n') {
          temp = temp * 10 + str[9].digitValue();
          if (str[10] != '\n') {
            temp = temp * 10 + str[10].digitValue();
          }
        }
        ui->horizontalSlider_line_width->setValue(temp);
      }
      if (str[0] == 'P' && str[5] == 'r') {
        temp = str[8].digitValue();
        if (str[9] != '\n') {
          temp = temp * 10 + str[9].digitValue();
          if (str[10] != '\n') {
            temp = temp * 10 + str[10].digitValue();
          }
        }
        ui->horizontalSlider_p_color->setValue(temp);
      }
      if (str[0] == 'V' && str[5] == 'z') {
        temp = str[8].digitValue();
        if (str[9] != '\n') {
          temp = temp * 10 + str[9].digitValue();
          if (str[10] != '\n') {
            temp = temp * 10 + str[10].digitValue();
          }
        }
        ui->horizontalSlider_vertexSize->setValue(temp);
      }
      if (str[0] == 'B' && str[5] == 'r') {
        temp = str[8].digitValue();
        if (str[9] != '\n') {
          temp = temp * 10 + str[9].digitValue();
          if (str[10] != '\n') {
            temp = temp * 10 + str[10].digitValue();
          }
        }
        ui->horizontalSlider_bg_color->setValue(temp);
      }
    }
  }
  file.close();
  ui->gl_window->update();
}

void MainWindow::start_gif() {
  my_gif = new QGifImage;
  gif_timer = new QTimer(this);
  start_time = 0;
  temp_time = 100;
  my_gif->setDefaultDelay(100);
  connect(gif_timer, SIGNAL(timeout()), this, SLOT(make_gif()));
  gif_timer->start(100);
}

void MainWindow::make_gif() {
  if (start_time == temp_time) {
    QPixmap my_screen(ui->gl_window->size());
    ui->gl_window->render(&my_screen);
    QImage my_image = my_screen.toImage();
    my_gif->addFrame(my_image, 100);
    print_time = (float)start_time / 1e3;
    temp_time = temp_time + 100;
    ui->label_error->setText("GIF IN PROCESS");
  }
  if (start_time == 5000) {
    time_t this_time = time(0);
    tm *time = localtime(&this_time);
    QString my_path = QCoreApplication::applicationDirPath();
    QString gif_name = my_path + "/" + QString::number(time->tm_hour) +
                       QString::number(time->tm_min) +
                       QString::number(time->tm_sec) + ".gif";
    my_gif->save(gif_name);
    free(my_gif);
    gif_timer->stop();
    ui->label_error->setText("GIF DONE");
  }

  start_time = start_time + 100;
}
