#include "s21_3d_viewer_1_0.h"

void centering(dataStruct *d) {
  double cX = d->minMaxX[MIN] + (d->minMaxX[MAX] - d->minMaxX[MIN]) / 2;
  double cY = d->minMaxY[MIN] + (d->minMaxY[MAX] - d->minMaxY[MIN]) / 2;
  double cZ = d->minMaxZ[MIN] + (d->minMaxZ[MAX] - d->minMaxZ[MIN]) / 2;

  for (int i = 0; i < d->count_of_vertexes; i++) {
    d->matrix_3d.matrix[i][X] -= cX;
    d->matrix_3d.matrix[i][Y] -= cY;
    d->matrix_3d.matrix[i][Z] -= cZ;
  }
}

void scaling(dataStruct *d, double value) {
  double x = d->minMaxX[MAX] - d->minMaxX[MIN];
  double y = d->minMaxY[MAX] - d->minMaxY[MIN];
  double z = d->minMaxZ[MAX] - d->minMaxZ[MIN];

  double maxV = x;
  if (y > maxV) {
    maxV = y;
  }
  if (z > maxV) {
    maxV = z;
  }

  double scale = (value - value * (-1)) / maxV;

  dyn_scaling(d, scale);
}

void dyn_scaling(dataStruct *d, double value) {
  for (int i = 0; i < d->count_of_vertexes; i++) {
    d->matrix_3d.matrix[i][X] *= value;
    d->matrix_3d.matrix[i][Y] *= value;
    d->matrix_3d.matrix[i][Z] *= value;
  }
}

// mode - X | Y | Z
void move_coord(dataStruct *d, double value, int mode) {
  if (mode == X || mode == Y || mode == Z) {
    for (int i = 0; i < d->count_of_vertexes; i++) {
      d->matrix_3d.matrix[i][mode] += value;
    }
  }
}

void rotateX(dataStruct *d, double value) {
  for (int i = 0; i < d->count_of_vertexes; i++) {
    double y = d->matrix_3d.matrix[i][Y];
    double z = d->matrix_3d.matrix[i][Z];
    d->matrix_3d.matrix[i][Y] = y * cos(value) - z * sin(value);
    d->matrix_3d.matrix[i][Z] = y * sin(value) + z * cos(value);
  }
}

void rotateY(dataStruct *d, double value) {
  for (int i = 0; i < d->count_of_vertexes; i++) {
    double x = d->matrix_3d.matrix[i][X];
    double z = d->matrix_3d.matrix[i][Z];
    d->matrix_3d.matrix[i][X] = x * cos(value) + z * sin(value);
    d->matrix_3d.matrix[i][Z] = z * cos(value) - x * sin(value);
  }
}

void rotateZ(dataStruct *d, double value) {
  for (int i = 0; i < d->count_of_vertexes; i++) {
    double x = d->matrix_3d.matrix[i][X];
    double y = d->matrix_3d.matrix[i][Y];
    d->matrix_3d.matrix[i][X] = x * cos(value) + y * sin(value);
    d->matrix_3d.matrix[i][Y] = y * cos(value) - x * sin(value);
  }
}
