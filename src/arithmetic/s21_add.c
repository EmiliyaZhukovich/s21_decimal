#include "../s21_decimal.h"
#include <stdio.h>

void PrintDecimal(s21_decimal *value) {
    int scale = (value->bits[3] >> 16) & 0xFF;  // Масштаб (scale)
    int sign = (value->bits[3] & (1 << 31)) ? -1 : 1;  // Знак числа
    long long int integer_part = 0;

    // Извлекаем целую часть
    for (int i = 0; i < 3; i++) {
        integer_part |= (long long int)(value->bits[i]) << (32 * i);
    }

    // Печать числа с учетом знака и масштаба
    if (scale == 0) {
        printf("Decimal: %lld\n", sign * integer_part);
    } else {
        // Выводим дробную часть
        long long int divisor = 1;
        for (int i = 0; i < scale; i++) {
            divisor *= 10;
        }
        long long int fractional_part = integer_part % divisor;
        integer_part /= divisor;
        printf("Decimal: %lld.%0*lld (scaled by %d)\n", sign * integer_part, scale, fractional_part, scale);
    }
}

void PrintDecimalBinary(s21_decimal *value) {
    // Печать 4-х слов по 32 бита в двоичном виде
    for (int i = 3; i >= 0; i--) {
        printf("bits[%d]: ", i);
        for (int j = 31; j >= 0; j--) {
            printf("%d", (value->bits[i] >> j) & 1);  // Печать каждого бита
        }
        printf("\n");
    }
}

int decimal_normalize(s21_decimal *value_1, s21_decimal *value_2) {
    int scale_1 = (value_1->bits[3] >> 16) & 0xFF;  // Масштаб первого числа
    int scale_2 = (value_2->bits[3] >> 16) & 0xFF;  // Масштаб второго числа

    // Если масштабы одинаковые, не нужно ничего делать
    if (scale_1 == scale_2) {
        return 0;
    }

    // Приводим оба числа к максимальной точности
    int max_scale = scale_1 > scale_2 ? scale_1 : scale_2;

    // Если scale_1 < max_scale, умножаем number_1 на 10^(max_scale - scale_1)
    if (scale_1 < max_scale) {
        int shift = max_scale - scale_1;
        for (int i = 0; i < shift; i++) {
            // Умножаем на 10
            long long int carry = 0;
            for (int j = 0; j < 3; j++) {
                long long int temp = (long long int)value_1->bits[j] * 10 + carry;
                value_1->bits[j] = (int)(temp & 0xFFFFFFFF);
                carry = temp >> 32;
            }
        }
        value_1->bits[3] = (value_1->bits[3] & 0x80000000) | (max_scale << 16);  // Обновляем scale
    }

    // Если scale_2 < max_scale, умножаем number_2 на 10^(max_scale - scale_2)
    if (scale_2 < max_scale) {
        int shift = max_scale - scale_2;
        for (int i = 0; i < shift; i++) {
            // Умножаем на 10
            long long int carry = 0;
            for (int j = 0; j < 3; j++) {
                long long int temp = (long long int)value_2->bits[j] * 10 + carry;
                value_2->bits[j] = (int)(temp & 0xFFFFFFFF);
                carry = temp >> 32;
            }
        }
        value_2->bits[3] = (value_2->bits[3] & 0x80000000) | (max_scale << 16);  // Обновляем scale
    }

    return 0;  
}

int is_less(s21_decimal value_1, s21_decimal value_2) {
    // Сравнение двух чисел
    if (value_1.bits[3] < value_2.bits[3]) return 1;
    if (value_1.bits[3] > value_2.bits[3]) return 0;

    // Если знаки одинаковые, сравниваем младшие биты.
    for (int i = 2; i >= 0; i--) {
        if (value_1.bits[i] < value_2.bits[i]) return 1;
        if (value_1.bits[i] > value_2.bits[i]) return 0;
    }
    return 0;  
}

void set_sign(s21_decimal *result, int sign) {
    if (sign == 1) {
        result->bits[3] |= (1 << 31); // Устанавливаем старший бит в 1 для отрицательного числа
    } else {
        result->bits[3] &= ~(1 << 31); // Убираем старший бит для положительного числа
    }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
    // Нормализация чисел перед сложением
    int normalisation = decimal_normalize(&value_1, &value_2);
    
    // Извлекаем знаки чисел
    int sign1 = (value_1.bits[3] & (1 << 31)) ? 1 : 0;
    int sign2 = (value_2.bits[3] & (1 << 31)) ? 1 : 0;

    if (sign1 == sign2) {
        // Если знаки одинаковые, складываем их.
        int carry = 0;
        for (int i = 0; i < 3; i++) {
            long long int temp = (long long int)value_1.bits[i] + value_2.bits[i] + carry;
            result->bits[i] = (int)(temp & 0xFFFFFFFF);
            carry = temp >> 32;
        }
        result->bits[3] = (value_1.bits[3] & 0x80000000) | ((value_1.bits[3] >> 16) << 16);  // Применяем знак и масштаб
    } else {
        // Если знаки разные, вычитаем одно число из другого.
        s21_decimal temp1 = value_1;
        s21_decimal temp2 = value_2;
        if (sign1 == 1) {
            set_sign(&temp1, 0);  // Инвертируем знак первого числа
        } else {
            set_sign(&temp2, 0);  // Инвертируем знак второго числа
        }

        if (is_less(temp1, temp2)) {
            s21_decimal copy = value_2;
            int carry = 0;
            for (int i = 0; i < 3; i++) {
                long long int temp = (long long int)copy.bits[i] - value_1.bits[i] - carry;
                if (temp < 0) {
                    temp += (1LL << 32);
                    carry = 1;
                } else {
                    carry = 0;
                }
                copy.bits[i] = (int)(temp & 0xFFFFFFFF);
            }
            *result = copy;
            set_sign(result, sign2);
        } else {
            s21_decimal copy = value_1;
            int carry = 0;
            for (int i = 0; i < 3; i++) {
                long long int temp = (long long int)copy.bits[i] - value_2.bits[i] - carry;
                if (temp < 0) {
                    temp += (1LL << 32);
                    carry = 1;
                } else {
                    carry = 0;
                }
                copy.bits[i] = (int)(temp & 0xFFFFFFFF);
            }
            *result = copy;
            set_sign(result, sign1);
        }
    }

    return normalisation; 
}

int main() {
    s21_decimal value_1 = {{0, 0, 0, 0}}; 
    s21_decimal value_2 = {{0, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};  

    // Пример использования
    value_1.bits[0] = 100; 
    value_1.bits[3] = 0;   

    value_2.bits[0] = 50;   
    value_2.bits[3] = 0;   

    // Выполнение операции сложения
    if (s21_add(value_1, value_2, &result)) {
        printf("Ошибка при сложении\n");
    } else {
        printf("Результат сложения:\n");
        PrintDecimalBinary(&result);  
        PrintDecimal(&result); 
    }

    return 0;
}