#include <limits.h>

#include "../s21_decimal.h"

#define HIGH_PART_INDEX 5

typedef struct {
  uint32_t parts[6];  // 192-битное число
} big_decimal;

int is_zero(s21_decimal value) {
  return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}

int get_sign(s21_decimal d) { return (d.bits[3] >> 31) & 0x1; }

int get_scale(s21_decimal d) { return (d.bits[3] >> 16) & 0xFF; }

big_decimal get_mantissa(s21_decimal d) {
  big_decimal mantissa = {0};
  mantissa.parts[0] = d.bits[0];
  mantissa.parts[1] = d.bits[1];
  mantissa.parts[2] = d.bits[2];
  return mantissa;
}

void print_big_decimal(big_decimal value) {
  for (int i = HIGH_PART_INDEX; i >= 0; i--) {
    printf("%08X ", value.parts[i]);
  }
  printf("\n");
}

void shift_left(big_decimal *value) {
  uint64_t carry = 0;
  for (int i = 0; i <= HIGH_PART_INDEX; i++) {
    uint64_t temp = ((uint64_t)value->parts[i] << 1) | carry;
    value->parts[i] = (uint32_t)temp;
    carry = temp >> 32;
  }
}

void add_big_decimals(big_decimal *result, big_decimal a, big_decimal b) {
  uint64_t carry = 0;
  for (int i = 0; i <= HIGH_PART_INDEX; i++) {
    uint64_t temp = (uint64_t)a.parts[i] + b.parts[i] + carry;
    result->parts[i] = (uint32_t)temp;
    carry = temp >> 32;
  }
}

big_decimal multiply_big_decimals(big_decimal a, big_decimal b) {
  big_decimal result = {0};
  for (int i = 0; i < 96; i++) {
    if (b.parts[i / 32] & (1U << (i % 32))) {
      big_decimal temp = a;
      for (int j = 0; j < i; j++) shift_left(&temp);
      add_big_decimals(&result, result, temp);
    }
  }
  return result;
}

void set_decimal(s21_decimal *d, big_decimal mantissa, int scale, int sign) {
  d->bits[0] = mantissa.parts[0];
  d->bits[1] = mantissa.parts[1];
  d->bits[2] = mantissa.parts[2];
  d->bits[3] = (scale << 16) | (sign << 31);
}

void PrintDecimal(s21_decimal value) {
  if (get_sign(value)) {
    printf("-");
  }
  big_decimal mantissa = get_mantissa(value);
  print_big_decimal(mantissa);
  printf("\n");
}

void bank_rounding(big_decimal *value, int scale) {
  if (scale == 0) return;

  int last_bit_index = scale * 3; // Примерное место в битах (scale * log2(10) ≈ scale * 3)
  int round_bit = (value->parts[last_bit_index / 32] >> (last_bit_index % 32)) & 1;
  int next_bit = (value->parts[(last_bit_index - 1) / 32] >> ((last_bit_index - 1) % 32)) & 1;

  int round_up = 0;

  if (round_bit == 1) {
    if (next_bit == 1) {
      round_up = 1;  // Округляем вверх
    } else {
      int lowest_bit = value->parts[0] & 1;  // Четность младшего разряда
      if (lowest_bit == 1) {
        round_up = 1;  // Банковское округление
      }
    }
  }

  // Если нужно округлить вверх, добавляем 1
  if (round_up) {
    uint64_t carry = 1;
    for (int i = 0; i < 6 && carry; i++) {
      uint64_t temp = (uint64_t)value->parts[i] + carry;
      value->parts[i] = (uint32_t)temp;
      carry = temp >> 32;
    }
  }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = 0;

  // Проверяем, что result не NULL
  if (!result) return 1;

  // Извлекаем мантиссы
  big_decimal mantissa_1 = get_mantissa(value_1);
  big_decimal mantissa_2 = get_mantissa(value_2);

  // Умножаем мантиссы
  big_decimal result_mantissa = multiply_big_decimals(mantissa_1, mantissa_2);

  // Извлекаем знак и степень
  int scale_1 = get_scale(value_1);
  int scale_2 = get_scale(value_2);
  int res_scale = scale_1 + scale_2;
  int res_sign = get_sign(value_1) ^ get_sign(value_2);

  // Проверяем корректность шкалы (максимум 28)
  if (res_scale > 28) {
    return 1; // Ошибка переполнения
  }

  // Округляем до 96 бит
  bank_rounding(&result_mantissa, res_scale);

  // Проверяем переполнение
  if (result_mantissa.parts[3] || result_mantissa.parts[4] || result_mantissa.parts[5]) {
    code = res_sign ? 2 : 1;  // 1 - слишком большое число, 2 - слишком маленькое
  } else {
    set_decimal(result, result_mantissa, res_scale, res_sign);
  }

  return code;
}
