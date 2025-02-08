#include "../s21_decimal.h"

#define MAX_MANTISSA 79228162514264337593543950335ULL

void PrintDecimal(s21_decimal value) {
    int sign = (value.bits[3] >> 31) & 1;
    int scale = (value.bits[3] >> 16) & 0xFF;

    unsigned long long low = value.bits[0];
    unsigned long long mid = value.bits[1];
    unsigned long long high = value.bits[2];

    unsigned __int128 full_value = ((unsigned __int128)high << 64) | (mid << 32) | low;
    double result = (double)full_value / pow(10, scale);

    if (sign) {
        printf("-");
    }
    printf("%g\n", result);
}

void set_sign(s21_decimal *result, int sign) {
    if (sign == 1) {
        result->bits[3] |= (1U << 31);
    } else {
        result->bits[3] &= ~(1U << 31);
    }
}

int get_sign(s21_decimal d) {
    return (d.bits[3] >> 31) & 0x1;
}

int get_scale(s21_decimal d) {
    return (d.bits[3] >> 16) & 0xFF;
}

void set_scale(s21_decimal *d, int scale) {
    d->bits[3] &= ~(0xFF << 16);
    d->bits[3] |= (scale << 16);
}


void normalize(s21_decimal *a, s21_decimal *b) {
    int scale_a = get_scale(*a);
    int scale_b = get_scale(*b);

    while (scale_a < scale_b) {
        if (scale_a >= 28) break;
        uint64_t low = (uint64_t)a->bits[0] * 10;
        uint64_t mid = (uint64_t)a->bits[1] * 10 + (low >> 32);
        uint64_t high = (uint64_t)a->bits[2] * 10 + (mid >> 32);

        a->bits[0] = (uint32_t)low;
        a->bits[1] = (uint32_t)mid;
        a->bits[2] = (uint32_t)high;

        scale_a++;
    }

    while (scale_b < scale_a) {
        if (scale_b >= 28) break;
        uint64_t low = (uint64_t)b->bits[0] * 10;
        uint64_t mid = (uint64_t)b->bits[1] * 10 + (low >> 32);
        uint64_t high = (uint64_t)b->bits[2] * 10 + (mid >> 32);

        b->bits[0] = (uint32_t)low;
        b->bits[1] = (uint32_t)mid;
        b->bits[2] = (uint32_t)high;

        scale_b++;
    }

    set_scale(a, scale_a);
    set_scale(b, scale_b);
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (!result) return 1;  // Проверка на NULL

    int scale_1 = get_scale(value_1);
    int scale_2 = get_scale(value_2);
    int sign_1 = get_sign(value_1);
    int sign_2 = get_sign(value_2);

    *result = (s21_decimal){{0, 0, 0, 0}}; // Обнуление результата

    // Приведение к одинаковой степени
    normalize(&value_1, &value_2);

    if (sign_1 == sign_2) { 
        // Сложение чисел с одинаковыми знаками
        unsigned long long low = (unsigned long long)value_1.bits[0] + value_2.bits[0];
        unsigned long long mid = (unsigned long long)value_1.bits[1] + value_2.bits[1] + (low >> 32);
        unsigned long long high = (unsigned long long)value_1.bits[2] + value_2.bits[2] + (mid >> 32);

        if (high >> 32) {
            return (sign_1 == 0) ? 1 : 2;  // Переполнение
        }

        result->bits[0] = (int)low;
        result->bits[1] = (int)mid;
        result->bits[2] = (int)high;
        set_sign(result, sign_1);
    } else { 
        // Вычитание (если знаки разные)
        if (value_1.bits[2] < value_2.bits[2] || 
            (value_1.bits[2] == value_2.bits[2] && value_1.bits[1] < value_2.bits[1]) ||
            (value_1.bits[2] == value_2.bits[2] && value_1.bits[1] == value_2.bits[1] && value_1.bits[0] < value_2.bits[0])) {
            // Меняем местами если `|value_2| > |value_1|`
            s21_decimal temp = value_1;
            value_1 = value_2;
            value_2 = temp;
            sign_1 = !sign_1;
        }

        unsigned long long low = (unsigned long long)value_1.bits[0] - value_2.bits[0];
        unsigned long long mid = (unsigned long long)value_1.bits[1] - value_2.bits[1] - (low >> 63);
        unsigned long long high = (unsigned long long)value_1.bits[2] - value_2.bits[2] - (mid >> 63);

        result->bits[0] = (int)low;
        result->bits[1] = (int)mid;
        result->bits[2] = (int)high;
        set_sign(result, sign_1);
    }

    set_scale(result, scale_1);
    return 0;
}


void run_test(s21_decimal value_1, s21_decimal value_2, const char* test_name) {
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
    run_test(tiny_value, tiny_value, "Сложение очень маленьких отрицательных чисел");

    s21_decimal zero = {{0, 0, 0, 0}};
    run_test(value_1, zero, "-100 + 0");

    return 0;
}