#include <stdio.h>
#include <math.h>
#include "../s21_decimal.h"

// Проверка, является ли число нулем
int is_zero(s21_decimal value) {
    return (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0);
}
int get_scale(s21_decimal d) {
    return (d.bits[3] >> 16) & 0xFF; 
}

// Получение знака числа
int get_sign(s21_decimal d) {
    return (d.bits[3] >> 31) & 0x1;
}

// Функция для вывода числа в удобном формате
void PrintDecimal(s21_decimal value) {
    if (is_zero(value)) {
        printf("0\n");
        return;
    }

    int sign = get_sign(value);
    int scale = get_scale(value);

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

// Функция negate
int s21_negate(s21_decimal value, s21_decimal *result) {
    if (!result) return 1;  // Проверка указателя result

    *result = value;  // Копируем значение

    if (is_zero(value)) {  // Если число 0, то всегда делаем его положительным
        result->bits[3] &= ~(1U << 31);
    } else {
        result->bits[3] ^= (1U << 31);  // Инвертируем знак
    }

    return 0;  // Операция успешна
}

int main() {
    s21_decimal num1 = {{123456789, 0, 0, 0}};  // 123456789
    s21_decimal num2 = {{987654321, 0, 0, (1U << 31)}};  // -987654321
    s21_decimal num3 = {{0, 0, 0, 0}};  // 0
    s21_decimal num4 = {{4294967295, 4294967295, 4294967295, 0}};  // Максимальное число (2^96 - 1)
    s21_decimal num5 = {{0, 0, 0, (1U << 31)}};  // -0 (особый случай)
    s21_decimal num6 = {{500, 0, 0, (5 << 16)}}; // 5.00 (scale = 5)

    s21_decimal result;

    printf("Исходные числа и их инверсии:\n");

    printf("num1: ");
    PrintDecimal(num1);
    s21_negate(num1, &result);
    printf("negate(num1): ");
    PrintDecimal(result);

    printf("\nnum2: ");
    PrintDecimal(num2);
    s21_negate(num2, &result);
    printf("negate(num2): ");
    PrintDecimal(result);

    printf("\nnum3: ");
    PrintDecimal(num3);
    s21_negate(num3, &result);
    printf("negate(num3): ");
    PrintDecimal(result);

    printf("\nnum4 (максимальное число): ");
    PrintDecimal(num4);
    s21_negate(num4, &result);
    printf("negate(num4): ");
    PrintDecimal(result);

    printf("\nnum5 (-0): ");
    PrintDecimal(num5);
    s21_negate(num5, &result);
    printf("negate(num5): ");
    PrintDecimal(result);

    printf("\nnum6 (5.00): ");
    PrintDecimal(num6);
    s21_negate(num6, &result);
    printf("negate(num6): ");
    PrintDecimal(result);

    int error_code = s21_negate(num1, NULL);
    printf("\nТест ошибки (NULL указатель): ");
    if (error_code == 1) {
        printf("ОК (функция вернула 1, как ожидалось)\n");
    } else {
        printf("Ошибка (функция должна вернуть 1!)\n");
    }


    return 0;
}
