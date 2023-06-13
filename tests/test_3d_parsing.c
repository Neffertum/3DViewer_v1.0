#include "test.h"

START_TEST(test1_3d_parsing) {
    char *str = "f 1/1/1 2/2/1 3/3/1 3/ 6/5/ 1///   ";
    int res = 6;
    ck_assert_int_eq(get_count_of_edges(str), res);
}
END_TEST

START_TEST(test2_3d_parsing) {
    const char *str = "./examples_obj/Empty.obj";
    dataStruct res = start_c_obj_parsing(str);
    ck_assert_int_eq(res.count_of_facets, 0);
    ck_assert_int_eq(res.count_of_vertexes, 0);
    ck_assert_double_eq_tol(res.minMaxX[0], 0, 1e-7);
    ck_assert_double_eq_tol(res.minMaxX[1], 0, 1e-7);
    ck_assert_double_eq_tol(res.minMaxY[0], 0, 1e-7);
    ck_assert_double_eq_tol(res.minMaxY[1], 0, 1e-7);
    ck_assert_double_eq_tol(res.minMaxZ[0], 0, 1e-7);
    ck_assert_double_eq_tol(res.minMaxZ[1], 0, 1e-7);
    ck_assert_ptr_null(res.matrix_3d.matrix);
    ck_assert_ptr_null(res.polygons);
}
END_TEST

START_TEST(test3_3d_parsing) {
    const char *str = "./examples_obj/Cube.obj";
    dataStruct res = start_c_obj_parsing(str);
    double ref[8][3] = {{-0.5, -0.5, 0.5},
                          {0.5, -0.5, 0.5},
                          {-0.5, 0.5, 0.5},
                          {0.5, 0.5, 0.5},
                          {-0.5, 0.5, -0.5},
                          {0.5, 0.5, -0.5},
                          {-0.5, -0.5, -0.5},
                          {0.5, -0.5, -0.5},
                          };
    ck_assert_double_eq_tol(res.minMaxX[0], -0.5, 1e-7);
    ck_assert_double_eq_tol(res.minMaxX[1], 0.5, 1e-7);
    ck_assert_double_eq_tol(res.minMaxY[0], -0.5, 1e-7);
    ck_assert_double_eq_tol(res.minMaxY[1], 0.5, 1e-7);
    ck_assert_double_eq_tol(res.minMaxZ[0], -0.5, 1e-7);
    ck_assert_double_eq_tol(res.minMaxZ[1], 0.5, 1e-7);
    for (int i = 0; i < res.count_of_vertexes; i++) {
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][X], ref[i][X], 1e-7);
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][Y], ref[i][Y], 1e-7);
      ck_assert_double_eq_tol(res.matrix_3d.matrix[i][Z], ref[i][Z], 1e-7);
    }
    ck_assert_int_eq(res.count_of_facets, 12);
    ck_assert_int_eq(res.count_of_vertexes, 8);
}
END_TEST

START_TEST(test4_3d_parsing) {
    const char *str = "./examples_obj/NoFile.obj";
    dataStruct res = start_c_obj_parsing(str);
    ck_assert_int_eq(res.count_of_facets, 0);
    ck_assert_int_eq(res.count_of_vertexes, 0);
    int err = s21_create_matrix(res.matrix_3d.rows, res.matrix_3d.cols, &res.matrix_3d);
    ck_assert_int_eq(err, INCORRECT_MATRIX);
}
END_TEST

Suite *test_3d_parsing(void) {
  Suite *s;
  TCase *tc;

  s = suite_create("\033[43m-=S21_3D_PARSING=-\033[0m.");
  /* Core test case */
  tc = tcase_create("add_tc");

  tcase_add_test(tc, test1_3d_parsing);
  tcase_add_test(tc, test2_3d_parsing);
  tcase_add_test(tc, test3_3d_parsing);
  tcase_add_test(tc, test4_3d_parsing);

  suite_add_tcase(s, tc);

  return s;
}
