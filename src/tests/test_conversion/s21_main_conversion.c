#include "../s21_tests_runner.h"

TCase *s21_ConversionTCaseFromFloat();

Suite *s21_ConversionSuite() {
  Suite *suite = suite_create("s21_conversion");

  TCase *tc_core = s21_ConversionTCaseFromFloat();
  if (tc_core) {
    suite_add_tcase(suite, tc_core);
  }

  return suite;
}
