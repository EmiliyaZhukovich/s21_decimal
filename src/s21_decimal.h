#ifndef S21_DECIMAL_H
#define S21_DECIMAL_h

#include <stdio.h>

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  int bits[8];
} s21_big_decimal;


int s21_from_int_to_decimal(int src, s21_decimal* dst);

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_negate(s21_decimal value, s21_decimal *result);

int s21_from_float_to_decimal(float src, s21_decimal *dst);

void PrintIntBinary(int number);
int GetSign(int number);
void PrintDecimalBinary(s21_decimal* number);

#endif