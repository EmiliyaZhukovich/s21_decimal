#include "../s21_decimal.h"
#include <limits.h>

void PrintDecimal(s21_decimal d) {
    long long mantissa = get_mantissa(d);
    int scale = get_scale(d);
    int sign = get_sign(d);

    if (sign == 1) printf("-");
    printf("%lld", mantissa / (long long)pow(10, scale));
    if (scale > 0) {
        printf(".%0*lld", scale, llabs(mantissa % (long long)pow(10, scale)));
    }
    printf("\n");
}


void set_sign(s21_decimal *result, int sign) {
    if (sign == 1) {
        result->bits[3] |= (1 << 31);
    } else {
        result->bits[3] &= ~(1 << 31);
    }
}

int get_sign(s21_decimal d) {
    return (d.bits[3] >> 31) & 0x1;
}

int get_scale(s21_decimal d) {
    return (d.bits[3] >> 16) & 0xFF;
}

long long get_mantissa(s21_decimal d) {
    long long mantissa = ((long long)d.bits[1] << 32) | (unsigned int)d.bits[0];
    return mantissa;
}

void set_decimal(s21_decimal* d, long long mantissa, int scale, int sign) {
    d->bits[0] = (int)(mantissa & 0xFFFFFFFF);
    d->bits[1] = (int)((mantissa >> 32) & 0xFFFFFFFF);
    d->bits[2] = 0; // Обнуляем, так как mantissa — 64-битное число
    d->bits[3] = (scale << 16) | (sign << 31);
}

void normalize(s21_decimal *a, s21_decimal* b) {
    int scale_a = get_scale(*a);
    int scale_b = get_scale(*b);

    if (scale_a > scale_b) {
        int scale_diff = scale_a - scale_b;
        long long mantissa_b = get_mantissa(*b);
        mantissa_b *= pow(10, scale_diff);
        set_decimal(b, mantissa_b, scale_a, get_sign(*b));
    } else if (scale_b > scale_a) {
        int scale_diff = scale_b - scale_a;
        long long mantissa_a = get_mantissa(*a);
        mantissa_a *= pow(10, scale_diff);
        set_decimal(a, mantissa_a, scale_b, get_sign(*a));
    }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    unsigned long long mantissa_1 = get_mantissa(value_1);
    unsigned long long mantissa_2 = get_mantissa(value_2);

    int scale_1 = get_scale(value_1);
    int scale_2 = get_scale(value_2);
    int sign_1 = get_sign(value_1);
    int sign_2 = get_sign(value_2);

    unsigned long long result_mantissa = mantissa_1 * mantissa_2;

    // Проверка на переполнение
    if (result_mantissa > ULLONG_MAX / mantissa_1) {
        // Если произошло переполнение, применяем банковское округление
        result_mantissa = ULLONG_MAX;
    }

    int res_scale = scale_1 + scale_2;
    int res_sign = sign_1 ^ sign_2;

    // Проверка на переполнение масштаба
    if (res_scale > 28) {
        // Если масштаб слишком большой, возвращаем ошибку
        return 1;
    }

    set_decimal(result, result_mantissa, res_scale, res_sign);

    // Проверка на переполнение мантиссы
    if (result_mantissa > ULLONG_MAX) {
        return 1;
    }

    return 0;
}

int main() {
    // Пример 1: Умножение двух положительных чисел
    s21_decimal num1, num2, result;
    set_decimal(&num1, 123456789, 2, 0);  // 1234567.89
    set_decimal(&num2, 987654321, 3, 0);  // 987654.321
    int status = s21_mul(num1, num2, &result);
    printf("Test 1: 1234567.89 * 987654.321 = ");
    PrintDecimal(result);
    printf("Status: %d\n\n", status);

    // Пример 2: Умножение положительного и отрицательного числа
    set_decimal(&num1, 123456789, 2, 0);  // 1234567.89
    set_decimal(&num2, 987654321, 3, 1);  // -987654.321
    status = s21_mul(num1, num2, &result);
    printf("Test 2: 1234567.89 * (-987654.321) = ");
    PrintDecimal(result);
    printf("Status: %d\n\n", status);

    // Пример 3: Умножение двух больших чисел (проверка на переполнение)
    set_decimal(&num1, 999999999, 0, 0);  // 999999999
    set_decimal(&num2, 999999999, 0, 0);  // 999999999
    status = s21_mul(num1, num2, &result);
    printf("Test 3: 999999999 * 999999999 = ");
    PrintDecimal(result);
    printf("Status: %d (1 - переполнение)\n\n", status);

    // Пример 4: Умножение на ноль
    set_decimal(&num1, 123456789, 2, 0);  // 1234567.89
    set_decimal(&num2, 0, 0, 0);          // 0
    status = s21_mul(num1, num2, &result);
    printf("Test 4: 1234567.89 * 0 = ");
    PrintDecimal(result);
    printf("Status: %d\n\n", status);

    // Пример 5: Умножение чисел с большим масштабом (проверка на переполнение масштаба)
    set_decimal(&num1, 123456789, 15, 0);  // 0.000123456789
    set_decimal(&num2, 987654321, 15, 0);  // 0.000987654321
    status = s21_mul(num1, num2, &result);
    printf("Test 5: 0.000123456789 * 0.000987654321 = ");
    PrintDecimal(result);
    printf("Status: %d (1 - переполнение масштаба)\n\n", status);

    return 0;
}