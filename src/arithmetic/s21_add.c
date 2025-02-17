#include "../s21_decimal.h"

int is_zero(s21_decimal value) {
  return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}

void PrintDecimal(s21_decimal value) {
    printf("%u %u %u %u\n", value.bits[3], value.bits[2], value.bits[1], value.bits[0]);
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

int multiply_by_10(s21_decimal *value) {
  unsigned long long carry = 0;
  for (int i = 0; i < 3; i++) {
    unsigned long long temp = (unsigned long long)value->bits[i] * 10 + carry;
    value->bits[i] = (unsigned int)temp;
    carry = temp >> 32;
  }
  return carry == 0;
}

void normalize(s21_decimal *a, s21_decimal *b) {
  int scale_a = get_scale(*a);
  int scale_b = get_scale(*b);

  while (scale_a < scale_b) {
    if (!round_bank(b)) break;  // Уменьшаем b, если его scale больше
    scale_b--;
  }

  while (scale_b < scale_a) {
    if (!round_bank(a)) break;  // Уменьшаем a, если его scale больше
    scale_a--;
  }

  set_scale(a, scale_a);
  set_scale(b, scale_b);
}

int round_bank(s21_decimal *value) {
  int scale = get_scale(*value);
  if (scale == 0) return 1;

  unsigned int remainder = value->bits[0] % 10;
  unsigned long long carry = 0;

  for (int i = 2; i >= 0; i--) {
    unsigned long long current =
        ((unsigned long long)value->bits[i] + (carry << 32));
    value->bits[i] = current / 10;
    carry = current % 10;
  }

  if (remainder > 5 || (remainder == 5 && (value->bits[0] & 1))) {
    for (int i = 0; i < 3; i++) {
      if (++value->bits[i] != 0) break;
    }
  }

  set_scale(value, scale - 1);
  return 0;
}

int compare(s21_decimal a, s21_decimal b) {
  for (int i = 2; i >= 0; i--) {
    if (a.bits[i] != b.bits[i]) {
      return (a.bits[i] > b.bits[i]) ? 1 : -1;
    }
  }
  return 0;
}

int is_valid_decimal(s21_decimal value) {
    int scale = get_scale(value);
    if (scale < 0 || scale > 28) return 0;
    if (value.bits[3] & 0x007F0000) return 0;
    return 1;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return 1;
  if (!is_valid_decimal(value_1) || !is_valid_decimal(value_2)) return 1;

  normalize(&value_1, &value_2);

  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);
  *result = (s21_decimal){{0, 0, 0, 0}};

  unsigned long long carry = 0;
  if (sign_1 == sign_2) {
    for (int i = 0; i < 3; i++) {
      unsigned long long sum = (unsigned long long)value_1.bits[i] + value_2.bits[i] + carry;
      result->bits[i] = (unsigned int)sum;
      carry = sum >> 32;
    }
    if (carry) return sign_1 ? 2 : 1; 
  } else {
    if (compare(value_1, value_2) < 0) {
      s21_decimal temp = value_1;
      value_1 = value_2;
      value_2 = temp;
      sign_1 = !sign_1;
    }
    for (int i = 0; i < 3; i++) {
      if (value_1.bits[i] < value_2.bits[i] + carry) {
        result->bits[i] = (unsigned int)(value_1.bits[i] - value_2.bits[i] - carry);
        carry = 1;
      } else {
        result->bits[i] = (unsigned int)(value_1.bits[i] - value_2.bits[i] - carry);
        carry = 0;
      }
    }
  }

  if (!is_valid_decimal(*result)) return 1;
  set_sign(result, sign_1);
  set_scale(result, get_scale(value_1));

  return 0;
}
