#include "s21_3d_viewer_1_0.h"

/**
 * @brief start_c_obj_parsing
 * @param const char* path of obj file
 * @return dataStruct structure
 */
dataStruct start_c_obj_parsing(const char *file) {
  dataStruct st_data = {
      .count_of_facets = 0,
      .count_of_vertexes = 0,
      .matrix_3d = {.matrix = NULL, .cols = 0, .rows = 0},
      .polygons = NULL,
      .minMaxX[0] = 0,
      .minMaxX[1] = 0,
      .minMaxY[0] = 0,
      .minMaxY[1] = 0,
      .minMaxZ[0] = 0,
      .minMaxZ[1] = 0,
  };

  get_count_of_vertex_and_facets(file, &st_data);
  int error =
      s21_create_matrix(st_data.count_of_vertexes, 3, &st_data.matrix_3d);
  error += s21_init_polygon_struct(&st_data.polygons, st_data.count_of_facets);
  if (error == OK) {
    get_matrix_of_vertexes(file, &st_data.matrix_3d, st_data.polygons);
    find_min_max(&st_data);
    centering(&st_data);
    scaling(&st_data, 0.5);
  } else {
    st_data.count_of_facets = 0;
    st_data.count_of_vertexes = 0;
  }
  return st_data;
}

/**
 * @brief get_count_of_vertex_and_facets
 * @param file
 * @param st_data
 */
void get_count_of_vertex_and_facets(const char *file, dataStruct *st_data) {
  const char *pattern_v = "^v( +-?[0-9]+((,|.)([0-9]+))?){3}";
  const char *pattern_f = "^f ";

  int count_v = 0, count_f = 0;
  char *line = NULL;
  size_t len = 0;
  int success = 0;
  int cflags = REG_EXTENDED;
  regex_t regex_v, regex_f;
  regcomp(&regex_v, pattern_v, cflags);
  regcomp(&regex_f, pattern_f, cflags);

  FILE *f = fopen(file, "r");
  if (f == NULL) {
    fprintf(stderr, "ERROR FILE *f == NULL\n");
  } else {
    while (getline(&line, &len, f) != -1) {
      success = regexec(&regex_v, line, 0, NULL, 0);
      if (success == 0) {
        count_v++;
      }
      success = regexec(&regex_f, line, 0, NULL, 0);
      if (success == 0) {
        count_f++;
      }
    }
  }
  free(line);
  fclose(f);
  regfree(&regex_v);
  regfree(&regex_f);

  st_data->count_of_vertexes = count_v;
  st_data->count_of_facets = count_f;
}

/**
 * @brief s21_create_matrix
 * @param rows
 * @param columns
 * @param result
 * @return error
 */
int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int error = OK;
  if (rows <= 0 || columns <= 0 || result == NULL) {
    error = INCORRECT_MATRIX;
  } else {
    result->matrix =
        calloc(1, rows * sizeof(double *) + rows * columns * sizeof(double));
    if (result->matrix) {
      double *ptr = (double *)(result->matrix + rows);
      for (int i = 0; i < rows; i++) {
        result->matrix[i] = ptr + columns * i;
      }
      result->cols = columns;
      result->rows = rows;
    } else {
      error = CALC_ERROR;
    }
  }
  return error;
}

/**
 * @brief s21_init_polygon_struct
 * @param polygons
 * @param len
 * @return error
 */
int s21_init_polygon_struct(polygon_t **polygons, int len) {
  int error = OK;
  if (len <= 0) {
    error = ALLOC_ERROR;
  } else {
    polygon_t *memory = NULL;
    memory = (polygon_t *)calloc(len, sizeof(polygon_t));
    if (memory == NULL) {
      error = ALLOC_ERROR;
    } else {
      *polygons = memory;
    }
  }
  return error;
}

/**
 * @brief get_matrix_of_vertexes
 * @param file
 * @param matrix_3d
 * @param polygons
 */
void get_matrix_of_vertexes(const char *file, matrix_t *matrix_3d,
                            polygon_t *polygons) {
  const char *pattern_v = "^v( +-?[0-9]+((,|.)([0-9]+))?){3}";
  const char *pattern_f = "^f ";

  char *line = NULL;
  size_t len = 0;
  int success = 0;
  int cflags = REG_EXTENDED;
  regex_t regex_v, regex_f;
  ;
  regcomp(&regex_v, pattern_v, cflags);
  regcomp(&regex_f, pattern_f, cflags);

  FILE *f = fopen(file, "r");
  if (f == NULL) {
    fprintf(stderr, "ERROR FILE *f == NULL\n");
  } else {
    int i = 0, i_pol = 0;
    while (getline(&line, &len, f) != -1) {
      success = regexec(&regex_v, line, 0, NULL, 0);
      if (success == 0) {
        char *start = line;
        while (!(*start >= '0' && *start <= '9') && *start != ' ') {
          start++;
        }
        matrix_3d->matrix[i][0] = strtod(start, &start);
        matrix_3d->matrix[i][1] = strtod(start, &start);
        matrix_3d->matrix[i][2] = strtod(start, &start);
        i++;
      }
      success = regexec(&regex_f, line, 0, NULL, 0);
      if (success == 0) {
        int count_edges = get_count_of_edges(line);

        if (polygons + i_pol) {
          polygons[i_pol].num_of_ver_in_facets = count_edges;
          init_polygon(polygons, i_pol, line);
        }
        i_pol++;
      }
    }
  }
  free(line);
  fclose(f);
  regfree(&regex_v);
  regfree(&regex_f);
}

/**
 * @brief get_count_of_edges
 * @param line
 * @return count of edges
 */
int get_count_of_edges(char *line) {
  int count = 0;
  char *start = line;
  while (*start != '\0') {
    if ((*start >= '0' && *start <= '9') && *(start - 1) == ' ') {
      count++;
    }
    start++;
  }
  return count;
}

/**
 * @brief init_polygon
 * @param polygons
 * @param i_pol
 * @param line
 */
void init_polygon(polygon_t *polygons, int i_pol, char *line) {
  if (polygons[i_pol].num_of_ver_in_facets > 0) {
    int *mem = (int *)calloc(polygons[i_pol].num_of_ver_in_facets, sizeof(int));
    if (mem == NULL) {
    } else {
      polygons[i_pol].vertexes = mem;
      char *start = line;
      int i = 0;
      while (*start != '\0') {
        if (*start == ' ' && (*(start + 1) >= '0' && *(start + 1) <= '9')) {
          polygons[i_pol].vertexes[i++] = strtod(start, &start);
        }
        start++;
      }
    }
  }
}

/**
 * @brief find_min_max
 * @param d
 */
void find_min_max(dataStruct *d) {
  double minX = d->matrix_3d.matrix[0][X];
  double minY = d->matrix_3d.matrix[0][Y];
  double minZ = d->matrix_3d.matrix[0][Z];
  double maxX = minX;
  double maxY = minY;
  double maxZ = minZ;
  for (int i = 0; i < d->count_of_vertexes; i++) {
    if (d->matrix_3d.matrix[i][X] < minX) {
      minX = d->matrix_3d.matrix[i][X];
    }
    if (d->matrix_3d.matrix[i][X] > maxX) {
      maxX = d->matrix_3d.matrix[i][X];
    }

    if (d->matrix_3d.matrix[i][Y] < minY) {
      minY = d->matrix_3d.matrix[i][Y];
    }
    if (d->matrix_3d.matrix[i][Y] > maxY) {
      maxY = d->matrix_3d.matrix[i][Y];
    }

    if (d->matrix_3d.matrix[i][Z] < minZ) {
      minZ = d->matrix_3d.matrix[i][Z];
    }
    if (d->matrix_3d.matrix[i][Z] > maxZ) {
      maxZ = d->matrix_3d.matrix[i][Z];
    }
  }
  d->minMaxX[0] = minX;
  d->minMaxX[1] = maxX;

  d->minMaxY[0] = minY;
  d->minMaxY[1] = maxY;

  d->minMaxZ[0] = minZ;
  d->minMaxZ[1] = maxZ;
}
