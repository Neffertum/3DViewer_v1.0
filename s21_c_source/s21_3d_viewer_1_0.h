/*! \mainpage S21_3d_viewer_v1.0 by CityOrde, FeniXnec, YungSqua
 *
 * \section Project Description
 * Implementation a program to view 3D wireframe models (3D Viewer)
 * in the C programming language.
 * The models themselves will be loaded from .obj files
 * and be viewable on the screen with the ability to rotate, scale and
 * translate.
 *
 * \section Commands
 * Run \a "make" or \a "make all" to build 3dviewer\n
 * Run \a "make install" to install application to folder "build"\n
 * Run \a "make dvi" to get documentation\n
 * Run \a "make test" to run tests\n
 * Run \a "make gcov_report" to get coverage report\n
 * Run \a "make style" to chek code style
 *
 */

/*!
\file
This file contains definitions of all C functions used in project
*/

#ifndef SRC_S21_C_SOURCE_S21_3D_VIEWER_1_0_H_
#define SRC_S21_C_SOURCE_S21_3D_VIEWER_1_0_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Enum of errors
 */
enum errors { OK, INCORRECT_MATRIX, CALC_ERROR, ALLOC_ERROR };
enum { MIN, MAX };
enum { X, Y, Z };
/**
 * Enum types of point
 */
enum { NO, CIRCLE, SQUARE };
/**
 * Enum types of line
 */
enum { SOLID = 1, DOTTED };
enum { RED, GREEN, BLUE };
/**
 * Enum types of projection
 */
enum { PARR, CENTER };

/**
 * Structure for matrix of vertexes
 */
typedef struct Matrix {
  double **matrix;
  int rows;
  int cols;
} matrix_t;

/**
 * Structure for one facet
 */
typedef struct facets {
  int *vertexes;
  int num_of_ver_in_facets;
} polygon_t;

/**
 * Structure for all data from .obj
 */
typedef struct data_struct {
  int count_of_vertexes;
  int count_of_facets;
  matrix_t matrix_3d;
  polygon_t *polygons;
  double minMaxX[2];
  double minMaxY[2];
  double minMaxZ[2];
} dataStruct;

//! main c function for parsing .obj file
dataStruct start_c_obj_parsing(const char *file);
void get_count_of_vertex_and_facets(const char *file, dataStruct *st_data);
//! allocation memory for matrix of vertexes
int s21_create_matrix(int rows, int columns, matrix_t *result);
int s21_init_polygon_struct(polygon_t **polygons, int len);
void get_matrix_of_vertexes(const char *file, matrix_t *matrix_3d,
                            polygon_t *polygons);
int get_count_of_edges(char *line);
void init_polygon(polygon_t *polygons, int i_pol, char *line);
void find_min_max(dataStruct *d);
// ---

//! function for model centering
void centering(dataStruct *d);
//! function for model scaling
void scaling(dataStruct *d, double value);

void dyn_scaling(dataStruct *d, double value);
void move_coord(dataStruct *d, double value, int mode);

void rotateX(dataStruct *d, double value);
void rotateY(dataStruct *d, double value);
void rotateZ(dataStruct *d, double value);
//---

#ifdef __cplusplus
}
#endif

#endif  // SRC_S21_C_SOURCE_S21_3D_VIEWER_1_0_H_
