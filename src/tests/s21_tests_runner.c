#include "s21_tests_runner.h"

int main() {
  RunAllTests();
  return 0;
}

void RunAllTests() {
  SRunner *runner = srunner_create(NULL);

  AddAllSuitesIntoRunner(&runner);

  srunner_run_all(runner, CK_NORMAL);

  srunner_free(runner);
}


void AddAllSuitesIntoRunner(SRunner **runner) {
#ifdef ENABLE_CONVERSION_TESTS
  AddConversionSuitesIntoRunner(runner);
#endif
#if defined(ENABLE_ARITHMETIC_ADD_TESTS) || defined(ENABLE_ARITHMETIC_MUL_TESTS)
  #ifdef ENABLE_ARITHMETIC_ADD_TESTS
    AddAritmeticSuitesIntoRunner(runner);
    #endif
      
  #ifdef ENABLE_ARITHMETIC_MUL_TESTS
    AddAritmeticSuitesIntoRunner(runner);
  #endif
#endif
#ifdef ENABLE_OTHER_FUNCTIONS_TESTS
  AddOtherFunctionsSuitesIntoRunner(runner);
#endif
}

#ifdef ENABLE_CONVERSION_TESTS
void AddConversionSuitesIntoRunner(SRunner **runner) {
  srunner_add_suite(*runner, s21_ConversionSuite());
}
#endif

#if defined(ENABLE_ARITHMETIC_TESTS) || defined(ENABLE_ARITHMETIC_ADD_TESTS) || defined(ENABLE_ARITHMETIC_MUL_TESTS)
void AddAritmeticSuitesIntoRunner(SRunner **runner) {
 Suite *suite = suite_create("s21_arithmetic");

  #ifdef ENABLE_ARITHMETIC_ADD_TESTS
  suite_add_tcase(suite, s21_AritmeticTCaseAdd());
  #endif

  #ifdef ENABLE_ARITHMETIC_MUL_TESTS
  suite_add_tcase(suite, s21_AritmeticTCaseMul());
  #endif

  srunner_add_suite(*runner, suite);
}
#endif

#ifdef ENABLE_OTHER_FUNCTIONS_TESTS
void AddOtherFunctionsSuitesIntoRunner(SRunner **runner) {
  srunner_add_suite(*runner, s21_OtherFunctionsSuite());
}
#endif
