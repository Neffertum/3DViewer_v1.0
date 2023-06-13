#include "s21_gl_view.h"

s21_gl_view::s21_gl_view(QWidget *parent) : QOpenGLWidget(parent) {
  st_data.count_of_facets = 0;
  st_data.count_of_vertexes = 0;
  vertex_size = 1;
  line_width = 1;
  vertex = NO;
  line = SOLID;
  l_color[RED] = 0.5;
  l_color[GREEN] = 0.5;
  l_color[BLUE] = 0.5;

  p_color[RED] = 1;
  p_color[GREEN] = 0;
  p_color[BLUE] = 0;

  bg_color[RED] = 0.2;
  bg_color[GREEN] = 0.2;
  bg_color[BLUE] = 0.3;

  projection = PARR;
}

s21_gl_view::~s21_gl_view() {}

void s21_gl_view::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
}

void s21_gl_view::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void s21_gl_view::paintGL() {
  if (projection == CENTER) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 2, 6);
    glTranslatef(0, 0, -3);
  } else {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
  }

  glClearColor(bg_color[RED], bg_color[GREEN], bg_color[BLUE], 0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (line) {
    for (int i = 0; i < st_data.count_of_facets; i++) {
      for (int k = 0; k < st_data.polygons[i].num_of_ver_in_facets; k++) {
        double x = 0, y = 0, z = 0;
        int r = 0;
        r = st_data.polygons[i].vertexes[k] - 1;
        x = st_data.matrix_3d.matrix[r][X];
        y = st_data.matrix_3d.matrix[r][Y];
        z = st_data.matrix_3d.matrix[r][Z];
        glLineWidth(line_width);
        if (line == DOTTED) {
          glEnable(GL_LINE_STIPPLE);
          glLineStipple(2, 0X00FF);
        }
        glBegin(GL_LINE_LOOP);

        glColor3d(l_color[RED], l_color[GREEN], l_color[BLUE]);  // line color
        glVertex3d(x, y, z);
      }
      glEnd();
    }
    glDisable(GL_LINE_STIPPLE);
  }

  if (vertex) {
    for (int i = 0; i < st_data.count_of_facets; i++) {
      for (int k = 0; k < st_data.polygons[i].num_of_ver_in_facets; k++) {
        double x = 0, y = 0, z = 0;
        int r = 0;
        r = st_data.polygons[i].vertexes[k] - 1;
        x = st_data.matrix_3d.matrix[r][X];
        y = st_data.matrix_3d.matrix[r][Y];
        z = st_data.matrix_3d.matrix[r][Z];

        glEnable(GL_BLEND);
        if (vertex == CIRCLE) {
          glEnable(GL_POINT_SMOOTH);
        } else if (vertex == SQUARE) {
          glDisable(GL_POINT_SMOOTH);
        }
        glPointSize(vertex_size);
        glBegin(GL_POINTS);

        glColor3d(p_color[RED], p_color[GREEN], p_color[BLUE]);  // point color
        glVertex3d(x, y, z);
      }
      glEnd();
    }
  }
}
