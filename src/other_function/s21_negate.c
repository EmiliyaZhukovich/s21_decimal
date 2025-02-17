#include "../s21_decimal.h"

#define ERROR 1  // Код ошибки для некорректных входных данных

// Проверка, является ли число нулем
int is_zero(s21_decimal value) {
  return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}

// Получение степени числа (scale)
int get_scale(s21_decimal d) { return (d.bits[3] >> 16) & 0xFF; }

// Получение знака числа
int get_sign(s21_decimal d) { return (d.bits[3] >> 31) & 0x1; }

// Проверка корректности s21_decimal
int is_valid_decimal(s21_decimal value) {
  int is_valid = 1;
  int scale = get_scale(value);

  // Проверка степени (должна быть от 0 до 28)
  if (scale > 28) {
    is_valid = 0;
  }

  // Проверка, что биты 0-15 и 24-30 в bits[3] нулевые
  int reserved_bits = value.bits[3] & ~(0x80000000 | 0x00FF0000);
  if (reserved_bits != 0) {
    is_valid = 0;
  }

  return is_valid;
}

void PrintDecimal(s21_decimal value) {
    printf("%u %u %u %u\n", value.bits[3], value.bits[2], value.bits[1], value.bits[0]);
}

// Функция negate
int s21_negate(s21_decimal value, s21_decimal *result) {
  int status = 0;

  if (!result || !is_valid_decimal(value)) {
    status = 1;
  } else {
    *result = value;  // Копируем значение

    if (is_zero(value)) {
      result->bits[3] &= ~(1U << 31);  // Делаем 0 положительным
    } else {
      result->bits[3] ^= (1U << 31);  // Инвертируем знак
    }
  }

  return status;
}
