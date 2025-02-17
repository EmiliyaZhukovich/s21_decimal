#include "../s21_decimal.h"

int is_zero(s21_decimal value) {
  return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}

void PrintDecimal(s21_decimal value) {
  if (is_zero(value)) {
    printf("0\n");
    return;
  }

  int sign = get_sign(value);
  int scale = get_scale(value);

  __uint128_t low = value.bits[0];
  __uint128_t mid = value.bits[1];
  __uint128_t high = value.bits[2];

  __uint128_t full_value = ((__uint128_t)high << 64) | (mid << 32) | low;
  double result = (double)full_value / pow(10, scale);

  if (sign) {
    printf("-");
  }
  printf("%e\n", result);
}

void set_sign(s21_decimal *result, int sign) {
  if (sign == 1) {
    result->bits[3] |= (1U << 31);
  } else {
    result->bits[3] &= ~(1U << 31);
  }
}

int get_sign(s21_decimal d) { return (d.bits[3] >> 31) & 0x1; }

int get_scale(s21_decimal d) { return (d.bits[3] >> 16) & 0xFF; }

void set_scale(s21_decimal *d, int scale) {
  d->bits[3] &= ~(0xFF << 16);
  d->bits[3] |= (scale << 16);
}

void normalize(s21_decimal *a, s21_decimal *b) {
  int scale_a = get_scale(*a);
  int scale_b = get_scale(*b);

  while (scale_a < scale_b) {
    if (scale_a >= 28) break;
    __uint128_t low = (__uint128_t)a->bits[0] * 10;
    __uint128_t mid = (__uint128_t)a->bits[1] * 10 + (low >> 32);
    __uint128_t high = (__uint128_t)a->bits[2] * 10 + (mid >> 32);

    a->bits[0] = (int)low;
    a->bits[1] = (int)mid;
    a->bits[2] = (int)high;

    scale_a++;
  }

  while (scale_b < scale_a) {
    if (scale_b >= 28) break;
    __uint128_t low = (__uint128_t)b->bits[0] * 10;
    __uint128_t mid = (__uint128_t)b->bits[1] * 10 + (low >> 32);
    __uint128_t high = (__uint128_t)b->bits[2] * 10 + (mid >> 32);

    b->bits[0] = (int)low;
    b->bits[1] = (int)mid;
    b->bits[2] = (int)high;

    scale_b++;
  }

  set_scale(a, scale_a);
  set_scale(b, scale_b);
}

int round_bank(s21_decimal *value) {
  int code = 0;
  int scale = get_scale(*value);
  if (scale == 0) code = 1;  // Дальше уменьшать нельзя

  __uint128_t remainder = value->bits[0] % 10;
  __uint128_t carry = 0;

  for (int i = 2; i >= 0; i--) {
    __uint128_t current = ((__uint128_t)value->bits[i] + (carry << 32));
    value->bits[i] = current / 10;
    carry = current % 10;
  }

  // Банковское округление (округляем к четному)
  if (remainder > 5 || (remainder == 5 && (value->bits[0] & 1))) {
    value->bits[0]++;

    // Если прибавление 1 вызвало переполнение
    if (value->bits[0] == 0 && value->bits[1] == 0 && value->bits[2] == 0) {
      return 1;  // Переполнение, число не вмещается
    }
  }

  set_scale(value, scale - 1);
  return code;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = 0;
  if (!result) code = 1;  // Проверка на NULL

  int scale_1 = get_scale(value_1);
  int scale_2 = get_scale(value_2);
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);

  *result = (s21_decimal){{0, 0, 0, 0}};  // Обнуление результата

  // Приведение к одинаковой степени
  normalize(&value_1, &value_2);

  __uint128_t low, mid, high;

  do {
    if (sign_1 == sign_2) {
      // Сложение чисел с одинаковыми знаками
      low = (__uint128_t)value_1.bits[0] + value_2.bits[0];
      mid = (__uint128_t)value_1.bits[1] + value_2.bits[1] + (low >> 32);
      high = (__uint128_t)value_1.bits[2] + value_2.bits[2] + (mid >> 32);
    } else {
      // Вычитание (если знаки разные)
      if (value_1.bits[2] < value_2.bits[2] ||
          (value_1.bits[2] == value_2.bits[2] &&
           value_1.bits[1] < value_2.bits[1]) ||
          (value_1.bits[2] == value_2.bits[2] &&
           value_1.bits[1] == value_2.bits[1] &&
           value_1.bits[0] < value_2.bits[0])) {
        // Меняем местами если `|value_2| > |value_1|`
        s21_decimal temp = value_1;
        value_1 = value_2;
        value_2 = temp;
        sign_1 = !sign_1;
      }

      low = (__uint128_t)value_1.bits[0] - value_2.bits[0];
      mid = (__uint128_t)value_1.bits[1] - value_2.bits[1] - (low >> 63);
      high = (__uint128_t)value_1.bits[2] - value_2.bits[2] - (mid >> 63);
    }

    // Проверяем, есть ли переполнение
    if (high >> 32) {
      if (round_bank(&value_1) || round_bank(&value_2)) {
        return (sign_1 == 0) ? 1 : 2;  // Если scale == 0, то переполнение
      }
      // Еще раз проверяем после округления
      if (value_1.bits[2] >> 31 || value_2.bits[2] >> 31) {
        return (sign_1 == 0) ? 1 : 2;
      }
    }

  } while (!code &&
           high >> 32);  // Повторяем, пока число не помещается в мантиссу

  // Записываем результат
  if (!code) {
    result->bits[0] = (int)low;
    result->bits[1] = (int)mid;
    result->bits[2] = (int)high;
    set_sign(result, sign_1);
    set_scale(result, scale_1 > scale_2
                          ? scale_1
                          : scale_2);  // Выставляем максимальный scale
  }

  return code;
}

void run_test(s21_decimal value_1, s21_decimal value_2, const char *test_name) {
  s21_decimal result = {{0, 0, 0, 0}};
  int error_code = s21_add(value_1, value_2, &result);

  printf("Тест: %s\n", test_name);
  if (error_code) {
    printf("Ошибка при сложении: %d\n", error_code);
  } else {
    printf("Результат: ");
    PrintDecimal(result);
  }
  printf("------------------------------------------------\n");
}

int main() {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  s21_decimal value_2 = {{50, 0, 0, 0}};
  run_test(value_1, value_2, "100 + 50");

  value_1.bits[3] = (1U << 31);
  run_test(value_1, value_2, "-100 + 50");

  value_2.bits[3] = (1U << 31);
  run_test(value_1, value_2, "-100 + -50");

  s21_decimal big_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal big_2 = {{1, 0, 0, 0}};
  run_test(big_1, big_2, "MAX + 1 (overflow)");

  s21_decimal dec_1 = {{123, 0, 0, (2 << 16)}};
  s21_decimal dec_2 = {{277, 0, 0, (2 << 16)}};
  run_test(dec_1, dec_2, "1.23 + 2.77");

  s21_decimal max_value = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, (1U << 31)}};
  s21_decimal one = {{1, 0, 0, (1U << 31)}};
  run_test(max_value, one, "-MAX - 1");

  s21_decimal tiny_value = {{1, 0, 0, (28 << 16) | (1U << 31)}};
  run_test(tiny_value, tiny_value,
           "Сложение очень маленьких отрицательных чисел");

  s21_decimal zero = {{0, 0, 0, 0}};
  run_test(value_1, zero, "-100 + 0");

  return 0;
}