#ifndef TESTS_RUN_H
#define TESTS_RUN_H
#define TEST_OTHER_ERROR 1
#define TEST_OTHER_OK 0

#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_decimal.h"

int s21_is_equal(s21_decimal a, s21_decimal b);
Suite *s21_ConversionSuite();
TCase *s21_ConversionTCaseFromFloat();


// tests for arithmetic
Suite *s21_AritmeticSuite();

#ifdef ENABLE_ARITHMETIC_ADD_TESTS
TCase *s21_AritmeticTCaseAdd();
#endif
#ifdef ENABLE_ARITHMETIC_MUL_TESTS
TCase *s21_AritmeticTCaseMul();
#endif

// tests for other functions
Suite *s21_OtherFunctionsSuite();
TCase *s21_NegateTCase();



// run tests
void RunAllTests();

void AddAllSuitesIntoRunner(SRunner **runner);
void AddConversionSuitesIntoRunner(SRunner **runner);
void AddAritmeticSuitesIntoRunner(SRunner **runner);
void AddOtherFunctionsSuitesIntoRunner(SRunner **runner);

#endif