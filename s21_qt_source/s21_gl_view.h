#ifndef S21_GL_VIEW_H
#define S21_GL_VIEW_H

#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <QtOpenGLWidgets/QtOpenGLWidgets>

#include "../s21_c_source/s21_3d_viewer_1_0.h"

class s21_gl_view : public QOpenGLWidget, public QOpenGLFunctions {
  Q_OBJECT
 private:
  void drawCube(double a);

 public:
  explicit s21_gl_view(QWidget *parent = 0);
  ~s21_gl_view();

  dataStruct st_data;

  // settings
  double vertex_size;
  double line_width;

  char projection;
  char vertex;
  char line;

  double l_color[3];
  double p_color[3];
  double bg_color[3];
  // ---

  int slider_scale;
  int slider_XYZ[3];
  int slider_rotate_XYZ[3];

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
};

#endif  // S21_GL_VIEW_H
