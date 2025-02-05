#include "../s21_decimal.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DECIMAL 79228162514264337593543950335.0 // 2^96 - 1
#define MIN_DECIMAL 1e-28

void null_decimal(s21_decimal *num) {
    num->bits[0] = num->bits[1] = num->bits[2] = num->bits[3] = 0;
}

void set_sign(s21_decimal *num, int sign) {
    if (sign)
        num->bits[3] |= (1 << 31);
    else
        num->bits[3] &= ~(1 << 31);
}

void set_scale(s21_decimal *num, int scale) {
    num->bits[3] &= ~(0xFF << 16);
    num->bits[3] |= (scale << 16);
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    if (!dst || isnan(src) || isinf(src) || fabs(src) > MAX_DECIMAL || fabs(src) < MIN_DECIMAL) {
        return 1;  // Ошибка конвертации
    }

    if (src == 0.0f) {
        null_decimal(dst);
        set_scale(dst, 0);
        printf("Округленное значение: 0.0\n");
        return 0;
    }

    int sign = signbit(src) ? 1 : 0;
    src = fabs(src);

    // **Округление до 7 значащих цифр**
    int exponent = (int)floor(log10f(src));  // Определяем порядок числа
    int scale = 0;
    float rounded_value;

    if (exponent > 6) {
        rounded_value = roundf(src / powf(10, exponent - 6));  // Оставляем 7 значащих цифр
    } else {
        scale = 6 - exponent;  // Смещение запятой
        rounded_value = roundf(src * powf(10, scale));
    }

    printf("Округленное значение: %.7g\n", rounded_value);  // Вывод округленного значения

    // Преобразуем округленное значение в целое
    unsigned long long int_part = (unsigned long long)rounded_value;

    // Записываем в decimal
    dst->bits[0] = (int)(int_part & 0xFFFFFFFF);
    dst->bits[1] = (int)((int_part >> 32) & 0xFFFFFFFF);
    dst->bits[2] = 0;

    set_sign(dst, sign);
    set_scale(dst, scale);

    return 0;  // Успешная конвертация
}

void print_decimal(s21_decimal *dec) {
    printf("[%d, %d, %d, %d]\n", dec->bits[0], dec->bits[1], dec->bits[2], dec->bits[3]);
}

int main() {
    s21_decimal dec;
    float test_values[] = {0.0f, 12345.6789f, -98765.4321f, 1e38f, 1e-40f, NAN, INFINITY, -INFINITY};

    for (int i = 0; i < 8; i++) {
        printf("Test %d: %g -> ", i + 1, test_values[i]);
        int res = s21_from_float_to_decimal(test_values[i], &dec);
        printf("result: %d ", res);
        if (res == 0)
            print_decimal(&dec);
        else
            printf("[ошибка конвертации]\n");
    }
    return 0;
}
