#include "../s21_tests_runner.h"

Suite *s21_OtherFunctionsSuite() {
  Suite *suite = suite_create("Other Functions");

  // Создаём тестовый случай и добавляем его в Suite
  TCase *tc_negate = s21_NegateTCase();
  suite_add_tcase(suite, tc_negate);

  return suite;
}