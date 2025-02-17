#include <string.h>

#include "../s21_decimal.h"

#define MAX_DECIMAL 7.92281625e+28  // 2^96 - 1
#define MIN_DECIMAL 1e-28

void null_decimal(s21_decimal *num) { memset(num, 0, sizeof(s21_decimal)); }

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

int get_sign(s21_decimal d) { return (d.bits[3] >> 31) & 0x1; }

int get_scale(s21_decimal d) { return (d.bits[3] >> 16) & 0xFF; }

void PrintDecimal(s21_decimal value) {
  printf("[%u, %u, %u, %u]\n", value.bits[0], value.bits[1], value.bits[2],
         value.bits[3]);
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int code = 0;  // Переменная для хранения кода ошибки

  if (!dst) {
    code = 1;  // Проверка на NULL
  } else {
    null_decimal(dst);  // Обнуляем decimal
                        // Проверяем на NaN, Infinity, слишком большие и слишком
                        // маленькие значения
    if (isnan(src) || isinf(src) || src > (float)MAX_DECIMAL ||
        src < -(float)MAX_DECIMAL || (fabs(src) < MIN_DECIMAL && src != 0.0f)) {
      code = 1;
    } else {
      int sign = signbit(src);  // Определяем знак числа
      src = fabs(src);          // Работаем с модулем числа

      // Округление до 7 значащих цифр
      char buffer[64];
      snprintf(buffer, sizeof(buffer), "%.7g", src);
      long double value = strtold(buffer, NULL);  // Переводим в long double

      // Определяем scale (количество знаков после запятой)
      int scale = 0;
      while (fmod(value, 1.0) != 0.0 && scale < 28) {
        value *= 10;
        scale++;
      }

      if (scale > 28) {
        code = 1;  // Ошибка, если scale слишком большой
      } else {
        // Преобразуем в 96-битное целое число
        uint64_t low =
            (uint64_t)fmod(value, 18446744073709551616.0);  // Младшие 64 бита
        uint32_t high =
            (uint32_t)(value / 18446744073709551616.0);  // Старшие 32 бита

        // Проверка переполнения (если число больше 2^96 - 1)
        if (high > 0xFFFFFFFF) {
          code = 1;
        } else {
          dst->bits[0] = (uint32_t)(low & 0xFFFFFFFF);  // Младшие 32 бита
          dst->bits[1] =
              (uint32_t)((low >> 32) & 0xFFFFFFFF);  // Средние 32 бита
          dst->bits[2] = high;  // Старшие 32 бита

          set_scale(dst, scale);
          if (!(dst->bits[0] == 0 && dst->bits[1] == 0 && dst->bits[2] == 0)) {
            set_sign(dst, sign);
          }
        }
      }
    }
  }

  return code;  // Единственный return в конце
}

// int s21_from_float_to_decimal(float src, s21_decimal *dst) {
//   int code = 0;  // Код результата

//   if (!dst) {
//     code = 1;
//   } else {
//     null_decimal(dst);  // Обнуляем decimal перед заполнением

//     if (src == 0.0f) {
//       set_sign(dst, signbit(src));
//       code = 0;
//       // Для 0.0f просто обнуляем decimal и оставляем code = 0
//     } else if (isnan(src) || isinf(src) || fabs(src) > MAX_DECIMAL ||
//     fabs(src) < MIN_DECIMAL) {
//       null_decimal(dst);
//       code = 1;  // Ошибка конвертации
//     } else {
//       int sign = signbit(src) ? 1 : 0;
//       src = fabs(src);

//       // **Округление до 7 значащих цифр**
//       int exponent = (int)floor(log10f(src));  // Определяем порядок числа
//       int scale = 0;
//       float rounded_value;

//       if (exponent > 28) {
//         code = 1;  // Число слишком велико для представления в s21_decimal
//       }else{

//         if (exponent > 6) {
//           rounded_value = roundf(src / powf(10, exponent - 6)) * powf(10,
//           exponent - 6);  // Оставляем 7 значащих цифр
//         } else {
//           scale = 6 - exponent;  // Смещение запятой
//           rounded_value = roundf(src * powf(10, scale));
//         }

//       // Преобразуем округленное значение в целое (96 бит)
//       unsigned long long low = (unsigned long long)rounded_value;
//       unsigned long long high = (unsigned long long)(rounded_value / powl(2,
//       64));

//       // Записываем в decimal
//       dst->bits[0] = (int)(low & 0xFFFFFFFF);
//       dst->bits[1] = (int)((low >> 32) & 0xFFFFFFFF);
//       dst->bits[2] = (int)(high & 0xFFFFFFFF);

//       set_sign(dst, sign);
//       set_scale(dst, scale);
//     }
//   }
//   }

//   return code;  // Один return в конце
// }
