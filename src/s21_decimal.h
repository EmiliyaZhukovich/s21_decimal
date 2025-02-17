#ifndef S21_DECIMAL_H
#define S21_DECIMAL_h

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECIMAL 7.92281625e+28  // 2^96 - 1
#define MIN_DECIMAL 1e-28

typedef struct {
  unsigned bits[4];
} s21_decimal;


int s21_negate(s21_decimal value, s21_decimal *result);
int s21_is_equal(s21_decimal a, s21_decimal b);
int is_valid_decimal(s21_decimal value);


int s21_from_float_to_decimal(float src, s21_decimal *dst);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

void PrintDecimal(s21_decimal value);
void set_sign(s21_decimal *result, int sign);
int is_zero(s21_decimal value);
int get_sign(s21_decimal d);
int get_scale(s21_decimal d);
void set_scale(s21_decimal *d, int scale);
void normalize(s21_decimal *a, s21_decimal *b);
void null_decimal(s21_decimal *num);
int round_bank(s21_decimal *value);




#endif