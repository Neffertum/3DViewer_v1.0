#include "test.h"

START_TEST(test1_3d_movingXYZ) {
    const char *str = "./examples_obj/Cube.obj";
    dataStruct res = start_c_obj_parsing(str);
    dataStruct ref = start_c_obj_parsing(str);

    move_coord(&res, 0.5, X);
    move_coord(&res, 0.7, Y);
    move_coord(&res, -0.5, Z);
    for (int i = 0; i < res.count_of_vertexes; i++) {
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][X], ref.matrix_3d.matrix[i][X] + 0.5, 1e-7);
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][Y], ref.matrix_3d.matrix[i][Y] + 0.7, 1e-7);
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][Z], ref.matrix_3d.matrix[i][Z] - 0.5, 1e-7);
    }
    for (int i = 0; i < res.count_of_facets; i++) {
        for (int k = 0; k < res.polygons->num_of_ver_in_facets; k++) {
            ck_assert_int_eq(res.polygons[i].vertexes[k], ref.polygons[i].vertexes[k]);
        }
    }
    ck_assert_int_eq(res.count_of_facets, ref.count_of_facets);
    ck_assert_int_eq(res.count_of_vertexes, ref.count_of_vertexes);
}
END_TEST

START_TEST(test2_3d_rotateX) {
    double alpha = 0.017; 
    const char *str = "./examples_obj/Cube.obj";
    dataStruct res = start_c_obj_parsing(str);
    dataStruct ref = start_c_obj_parsing(str);
    rotateX(&res, alpha);
    for (int i = 0; i < res.count_of_vertexes; i++) {
      double x = ref.matrix_3d.matrix[i][X];
      double y = ref.matrix_3d.matrix[i][Y];
      double z = ref.matrix_3d.matrix[i][Z];
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][X], x, 1e-7);
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][Y], y * cos(alpha) - z * sin(alpha), 1e-7);
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][Z], y * sin(alpha) + z * cos(alpha), 1e-7);
    }
    for (int i = 0; i < res.count_of_facets; i++) {
        for (int k = 0; k < res.polygons->num_of_ver_in_facets; k++) {
            ck_assert_int_eq(res.polygons[i].vertexes[k], ref.polygons[i].vertexes[k]);
        }
    }
}
END_TEST

START_TEST(test2_3d_rotateY) {
    double alpha = 0.017; 
    const char *str = "./examples_obj/Cube.obj";
    dataStruct res = start_c_obj_parsing(str);
    dataStruct ref = start_c_obj_parsing(str);
    rotateY(&res, alpha);
    for (int i = 0; i < res.count_of_vertexes; i++) {
      double x = ref.matrix_3d.matrix[i][X];
      double y = ref.matrix_3d.matrix[i][Y];
      double z = ref.matrix_3d.matrix[i][Z];
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][X], x * cos(alpha) + z * sin(alpha), 1e-7);
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][Y], y, 1e-7);
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][Z], z * cos(alpha) - x * sin(alpha), 1e-7);
    }
    for (int i = 0; i < res.count_of_facets; i++) {
        for (int k = 0; k < res.polygons->num_of_ver_in_facets; k++) {
            ck_assert_int_eq(res.polygons[i].vertexes[k], ref.polygons[i].vertexes[k]);
        }
    }
}
END_TEST

START_TEST(test2_3d_rotateZ) {
    double alpha = 0.017; 
    const char *str = "./examples_obj/Cube.obj";
    dataStruct res = start_c_obj_parsing(str);
    dataStruct ref = start_c_obj_parsing(str);
    rotateZ(&res, alpha);
    for (int i = 0; i < res.count_of_vertexes; i++) {
      double x = ref.matrix_3d.matrix[i][X];
      double y = ref.matrix_3d.matrix[i][Y];
      double z = ref.matrix_3d.matrix[i][Z];
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][X], x * cos(alpha) + y * sin(alpha), 1e-7);
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][Y], y * cos(alpha) - x * sin(alpha), 1e-7);
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][Z], z, 1e-7);
    }
    for (int i = 0; i < res.count_of_facets; i++) {
        for (int k = 0; k < res.polygons->num_of_ver_in_facets; k++) {
            ck_assert_int_eq(res.polygons[i].vertexes[k], ref.polygons[i].vertexes[k]);
        }
    }
}
END_TEST

Suite *test_3d_moving(void) {
  Suite *s;
  TCase *tc;

  s = suite_create("\033[43m-=S21_3D_MOVING=-\033[0m.");
  /* Core test case */
  tc = tcase_create("add_tc");

  tcase_add_test(tc, test1_3d_movingXYZ);
  tcase_add_test(tc, test2_3d_rotateX);
  tcase_add_test(tc, test2_3d_rotateY);
  tcase_add_test(tc, test2_3d_rotateZ);
 
  suite_add_tcase(s, tc);

  return s;
}