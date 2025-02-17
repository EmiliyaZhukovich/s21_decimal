#include "../s21_tests_runner.h"

Suite *s21_AritmeticSuite() {
  Suite *suite = suite_create("s21_arithmetic");
  #ifdef ENABLE_ARITHMETIC_ADD_TESTS
  suite_add_tcase(suite, s21_AritmeticTCaseAdd());
  #endif
  //   suite_add_tcase(suite, s21_AritmeticTCaseSub());
  #ifdef ENABLE_ARITHMETIC_MUL_TESTS
  suite_add_tcase(suite, s21_AritmeticTCaseMul());
  #endif

  //   suite_add_tcase(suite, s21_AritmeticTCaseDiv());

  return suite;
}