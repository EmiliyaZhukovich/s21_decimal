#include "../s21_tests_runner.h"

#define TEST_CONVERSION_OK 0
#define TEST_CONVERSION_ERROR 1

void test_from_float_to_decimal(int f, s21_decimal decimal_check);
int s21_is_equal(s21_decimal a, s21_decimal b) {
    return (a.bits[0] == b.bits[0] &&
            a.bits[1] == b.bits[1] &&
            a.bits[2] == b.bits[2] &&
            a.bits[3] == b.bits[3]);
}

#include <string.h>


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Тесты на ненормальные float
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

START_TEST(test_from_float_to_decimal_unnormal1) {
    float f = 0.0;
    s21_decimal result;
    s21_decimal check = {{0x0, 0x0, 0x0, 0x0}};

    int code = s21_from_float_to_decimal(f, &result);
    ck_assert_msg(code == TEST_CONVERSION_OK, "Conversion failed for 0.0");

    for (int i = 0; i < 4; i++) {
        ck_assert_msg(result.bits[i] == check.bits[i], "Bit mismatch at index %d", i);
    }
}
END_TEST

START_TEST(test_from_float_to_decimal_unnormal2) {
    float f = -0.0;
    s21_decimal result;
    s21_decimal check = {{0x0, 0x0, 0x0, 0x0}};

    int code = s21_from_float_to_decimal(f, &result);
    ck_assert_msg(code == TEST_CONVERSION_OK, "Conversion failed for -0.0");

    for (int i = 0; i < 4; i++) {
        ck_assert_msg(result.bits[i] == check.bits[i], "Bit mismatch at index %d", i);
    }

    int sign_check = get_sign(check);
    int sign_result = get_sign(result);
    ck_assert_msg(sign_check == sign_result, "Sign mismatch: expected %d, got %d", sign_check, sign_result);
}
END_TEST

START_TEST(test_from_float_to_decimal_unnormal3) {
    float f = INFINITY;
    s21_decimal result;

    int code = s21_from_float_to_decimal(f, &result);
    ck_assert_msg(code == TEST_CONVERSION_ERROR, "Expected conversion error for INFINITY");
}
END_TEST

START_TEST(test_from_float_to_decimal_unnormal4) {
    float f = -INFINITY;
    s21_decimal result;

    int code = s21_from_float_to_decimal(f, &result);
    ck_assert_msg(code == TEST_CONVERSION_ERROR, "Expected conversion error for -INFINITY");
}
END_TEST

START_TEST(test_from_float_to_decimal_unnormal5) {
    float f = NAN;
    s21_decimal result;

    int code = s21_from_float_to_decimal(f, &result);
    ck_assert_msg(code == TEST_CONVERSION_ERROR, "Expected conversion error for NAN");
}
END_TEST

START_TEST(test_from_float_to_decimal_unnormal6) {
    float f = -NAN;
    s21_decimal result;

    int code = s21_from_float_to_decimal(f, &result);
    ck_assert_msg(code == TEST_CONVERSION_ERROR, "Expected conversion error for -NAN");
}
END_TEST

START_TEST(test_from_float_to_decimal_incorrect1) {
    float f = 1.0f;
    int code = s21_from_float_to_decimal(f, NULL);

    ck_assert_int_eq(code, TEST_CONVERSION_ERROR);
}
END_TEST

START_TEST(test_from_float_to_decimal_ok1) {
    // 2.28401628E-19
    int f = 545706772;
    // 0.0000000000000000002284016
    s21_decimal decimal_check = {{0x22D9F0, 0x0, 0x0, 0x190000}};

    test_from_float_to_decimal(f, decimal_check);
}

START_TEST(test_from_float_to_decimal_ok2) {
    // -2.1349025E-18
    int f = -1575123157;
    // -0.000000000000000002134902
    s21_decimal decimal_check = {{0x209376, 0x0, 0x0, 0x80180000}};

    test_from_float_to_decimal(f, decimal_check);
}

START_TEST(test_from_float_to_decimal_ok3) {
    // -0.000115481133
    int f = -1192087946;
    // -0.0001154811
    s21_decimal decimal_check = {{0x119EFB, 0x0, 0x0, 0x800A0000}};

    test_from_float_to_decimal(f, decimal_check);
}

START_TEST(test_from_float_to_decimal_ok4) {
    // 3574.10864
    int f = 1163878845;
    // 3574.109
    s21_decimal decimal_check = {{0x36895D, 0x0, 0x0, 0x30000}};

    test_from_float_to_decimal(f, decimal_check);
}

START_TEST(test_from_float_to_decimal_ok5) {
    // -3.9575282E-24
    int f = -1734796938;
    // -0.0000000000000000000000039575
    s21_decimal decimal_check = {{0x9A97, 0x0, 0x0, 0x801C0000}};

    test_from_float_to_decimal(f, decimal_check);
}

START_TEST(test_from_float_to_decimal_ok6) {
    // 4.27844415E-17
    int f = 608521972;
    // 0.00000000000000004278444
    s21_decimal decimal_check = {{0x4148AC, 0x0, 0x0, 0x170000}};

    test_from_float_to_decimal(f, decimal_check);
}

START_TEST(test_from_float_to_decimal_ok7) {
    // -12419649
    int f = -885161407;
    // -12419650
    s21_decimal decimal_check = {{0xBD8242, 0x0, 0x0, 0x80000000}};

    test_from_float_to_decimal(f, decimal_check);
}

START_TEST(test_from_float_to_decimal_ok8) {
    // 0.00048828125
    int f = 973078528;
    // 0.0004882812
    s21_decimal decimal_check = {{0x4A817C, 0x0, 0x0, 0xA0000}};

    test_from_float_to_decimal(f, decimal_check);
}

START_TEST(test_from_float_to_decimal_ok9) {
    // 0.100000001
    int f = 1036831949;
    // 0.1
    s21_decimal decimal_check = {{0x1, 0x0, 0x0, 0x10000}};

    test_from_float_to_decimal(f, decimal_check);
}

START_TEST(test_from_float_to_decimal_ok10) {
    // 1
    int f = 1065353216;
    // 1
    s21_decimal decimal_check = {{0x1, 0x0, 0x0, 0x0}};

    test_from_float_to_decimal(f, decimal_check);
}

START_TEST(test_from_float_to_decimal_ok11) {
    // -1
    int f = -1082130432;
    // -1
    s21_decimal decimal_check = {{0x1, 0x0, 0x0, 0x80000000}};

    test_from_float_to_decimal(f, decimal_check);
}

START_TEST(test_from_float_to_decimal_ok12) {
    // -1.00000012
    int f = -1082130431;
    // -1
    s21_decimal decimal_check = {{0x1, 0x0, 0x0, 0x80000000}};

    test_from_float_to_decimal(f, decimal_check);
}



START_TEST(test_from_float_to_decimal_max) {
    float f = 79228162514264337593543950335.0f; // Примерно 7.9228e+28
    s21_decimal empty_decimal = {{0}};
    test_from_float_to_decimal(f, empty_decimal); // Ожидаем ошибку
}
END_TEST


START_TEST(test_from_float_to_decimal_min) {
    float f = 1e-28f; // -(2^96 - 1)
    s21_decimal empty_decimal = {{0}};
    test_from_float_to_decimal(f, empty_decimal);
}
END_TEST

START_TEST(test_from_float_to_decimal_overflow) {
    float f = 1E30; // Число слишком велико
    s21_decimal empty_decimal = {{0}};
    test_from_float_to_decimal(f, empty_decimal); // Ожидаем код ошибки 1
}
END_TEST


START_TEST(test_from_float_to_decimal_underflow) {
    float f = -1E30; // Число слишком велико по модулю
    s21_decimal empty_decimal = {{0}};
    test_from_float_to_decimal(f, empty_decimal); // Ожидаем ошибку
}
END_TEST


START_TEST(test_from_float_to_decimal_tiny) {
    float f = 1E-29; // Меньше 1E-28
    s21_decimal empty_decimal = {{0}};
    test_from_float_to_decimal(f, empty_decimal); // Ожидаем ошибку
}
END_TEST


START_TEST(test_from_float_to_decimal_precision_loss) {
    float f = 1.123456789f; // В `float` это представится как 1.123457
    s21_decimal decimal_check = {{1123457, 0x0, 0x0, 0x60000}}; // Округляется до 1.123457 (6 знаков после запятой)
    test_from_float_to_decimal(f, decimal_check);
}
END_TEST


START_TEST(test_from_float_to_decimal_rounding) {
    float f = 0.09999999; // Близко к 0.1, но не точно 0.1
    s21_decimal decimal_check = {{0x1, 0x0, 0x0, 0x10000}}; // Должно округлиться в 0.1

    test_from_float_to_decimal(f, decimal_check);
}
END_TEST

START_TEST(test_from_float_to_decimal_powers_of_10) {
    float f = 10000.0;
    s21_decimal decimal_check = {{10000, 0x0, 0x0, 0x0}};

    test_from_float_to_decimal(f, decimal_check);
}
END_TEST


void test_from_float_to_decimal(int f, s21_decimal decimal_check) {
    s21_decimal result;
    float value;

    // Преобразуем int → float (безопасно через memcpy)
    memcpy(&value, &f, sizeof(float));

    // Вызываем тестируемую функцию
    int code = s21_from_float_to_decimal(value, &result);

    // Проверка кода возврата
    if (code != 0) {
        printf("Ошибка: s21_from_float_to_decimal вернула %d\n", code);
        exit(1);  // Завершаем тест с ошибкой
    }

    // Проверка, что результат соответствует ожидаемому
    if (!s21_is_equal(result, decimal_check)) {
        printf("Тест не пройден!\n");
        printf("Ожидаемый результат: [%u, %u, %u, %u]\n",
               decimal_check.bits[0], decimal_check.bits[1], 
               decimal_check.bits[2], decimal_check.bits[3]);
        printf("Фактический результат: [%u, %u, %u, %u]\n",
               result.bits[0], result.bits[1], 
               result.bits[2], result.bits[3]);
        exit(1);
    }

    printf("Тест успешно пройден!\n");
}

TCase *s21_ConversionTCaseFromFloat() {

    TCase *tc_core = tcase_create("ConversionFromFloat");
    tcase_add_test(tc_core, test_from_float_to_decimal_incorrect1);
    tcase_add_test(tc_core, test_from_float_to_decimal_unnormal1);
    tcase_add_test(tc_core, test_from_float_to_decimal_unnormal2);
    tcase_add_test(tc_core, test_from_float_to_decimal_unnormal3);
    tcase_add_test(tc_core, test_from_float_to_decimal_unnormal4);
    tcase_add_test(tc_core, test_from_float_to_decimal_unnormal5);
    tcase_add_test(tc_core, test_from_float_to_decimal_unnormal6);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok1);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok2);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok3);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok4);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok5);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok6);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok7);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok8);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok9);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok10);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok11);
    tcase_add_test(tc_core, test_from_float_to_decimal_ok12);

    tcase_add_test(tc_core, test_from_float_to_decimal_max);
    tcase_add_test(tc_core, test_from_float_to_decimal_min);
    tcase_add_test(tc_core, test_from_float_to_decimal_overflow);
    tcase_add_test(tc_core, test_from_float_to_decimal_underflow);
    tcase_add_test(tc_core, test_from_float_to_decimal_tiny);
    tcase_add_test(tc_core, test_from_float_to_decimal_precision_loss);
    tcase_add_test(tc_core, test_from_float_to_decimal_rounding);
    tcase_add_test(tc_core, test_from_float_to_decimal_powers_of_10);

    return tc_core;
}