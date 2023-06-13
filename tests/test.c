#include "test.h"

int run_tests(Suite *test_case) {
  int number_failed;
  SRunner *sr = srunner_create(test_case);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed;
}

int main() {
  int number_failed = 0;
  Suite *suites_list[] = {test_3d_parsing(),
                          test_3d_moving(),
                          NULL};
  for (Suite **current = suites_list; *current != NULL; current++) {
    printf("_______________________________________\n");
    number_failed += run_tests(*current);
  }

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
