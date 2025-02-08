#ifndef S21_DECIMAL_H
#define S21_DECIMAL_h

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
   uint32_t bits[4];
} s21_decimal;

typedef struct {
    unsigned bits[8];  // 256 бит для операций с большим числом
} s21_big_decimal;


int s21_negate(s21_decimal value, s21_decimal *result);

int s21_from_float_to_decimal(float src, s21_decimal *dst);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

void PrintDecimal(s21_decimal d);
void PrintDecimalBinary(s21_decimal *value);
void set_sign(s21_decimal *result, int sign);
int get_sign(s21_decimal d);
int get_scale(s21_decimal d);
s21_big_decimal get_mantissa(s21_decimal d);
void set_decimal(s21_decimal* d, s21_big_decimal mantissa, int scale, int sign);
void normalize(s21_decimal *a, s21_decimal *b);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);



#endif