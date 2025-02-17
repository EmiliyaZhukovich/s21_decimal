#include "../s21_tests_runner.h"

void test_mul(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check);
void test_mul_fail1(s21_decimal decimal1, s21_decimal decimal2, int code_check);
void test_mul_fail2(s21_decimal decimal1, s21_decimal decimal2, s21_decimal decimal_check, int code_check);

int s21_is_equal(s21_decimal a, s21_decimal b) {
    return (a.bits[0] == b.bits[0] &&
            a.bits[1] == b.bits[1] &&
            a.bits[2] == b.bits[2] &&
            a.bits[3] == b.bits[3]);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Тесты на некорректные данные (ручные)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

START_TEST(test_mul_fail_manual1) {
    // 792281625.14264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    int code = s21_mul(decimal1, decimal2, NULL);

    ck_assert_int_ne(code, 0);
}

START_TEST(test_mul_fail_manual2) {
    // степень 154 (показатель степени должен быть от 0 до 28)
    // биты 0-15 не нули
    // биты 24-30 не нули
    s21_decimal decimal1 = {{0, 0, 0, 1000000000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_mul_fail_manual3) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal1 = {{-1, 0, 0, 0x1D0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST


START_TEST(test_mul_fail_manual4) {
    // степень 28 (что корректно), но биты 0-15 не нули (младший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x1C0001}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST


START_TEST(test_mul_fail_manual5) {
    // степень 28 (что корректно), но биты 24-30 не нули (младший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x11C0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST


START_TEST(test_mul_fail_manual6) {
    // Просто все единицы
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_mul_fail_manual7) {
    // степень 154 (показатель степени должен быть от 0 до 28)
    // биты 0-15 не нули
    // биты 24-30 не нули
    s21_decimal decimal2 = {{0, 0, 0, 1000000000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_mul_fail_manual8) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal2 = {{-1, 0, 0, 0x1D0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST


START_TEST(test_mul_fail_manual9) {
    // степень 28 (что корректно), но биты 24-30 не нули (младший бит)
    s21_decimal decimal2 = {{-1, 0, 0, 0x11C0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Тесты на все типы данных (ручные)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

START_TEST(test_mul_manual1) {
    // 7.9228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
    // 7.8228162514264337593543950335
    s21_decimal decimal2 = {{0x17FFFFFF, 0x602F7FC3, 0xFCC4D1C3, 0x1C0000}};
    // 61.978735728724164262422454727
    s21_decimal check = {{0xE07921C7, 0x3FD7ABE1, 0xC8439BCC, 0x1B0000}};

    test_mul(decimal1, decimal2, check);
}

START_TEST(test_mul_manual2) {
    // 7.9228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
    // -0.8228162514264337593543950335
    s21_decimal decimal2 = {{0x4F63FFFF, 0xBB0D25CF, 0x1A962D2F, 0x801C0000}};
    // -6.5190219687391279469416894926
    s21_decimal check = {{0x23B23CAE, 0xEC60363A, 0xD2A415FA, 0x801C0000}};

    test_mul(decimal1, decimal2, check);
}

START_TEST(test_mul_manual3) {
    // -7.000000000000025
    s21_decimal decimal1 = {{0x816D8019, 0x18DE76, 0x0, 0x800F0000}};
    // -0.00000000000005
    s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x800E0000}};
    // 0.0000000000003500000000000012
    s21_decimal check = {{0x40B6C00C, 0xC6F3B, 0x0, 0x1C0000}};

    test_mul(decimal1, decimal2, check);
}


START_TEST(test_mul_manual4) {
    // 79228162514265.1
    s21_decimal decimal1 = {{0x70D4257B, 0x2D093, 0x0, 0x10000}};
    // 78228162514264.5
    s21_decimal decimal2 = {{0x22618575, 0x2C77B, 0x0, 0x10000}};
    // 6197873572872488935063484519.0
    s21_decimal check = {{0xA1CDB406, 0x3FDA3F58, 0xC8439BCC, 0x10000}};

    test_mul(decimal1, decimal2, check);
}

START_TEST(test_mul_manual5) {
    // 5.0
    s21_decimal decimal1 = {{0x32, 0x0, 0x0, 0x10000}};
    // 4.5
    s21_decimal decimal2 = {{0x2D, 0x0, 0x0, 0x10000}};
    // 22.50
    s21_decimal check = {{0x8CA, 0x0, 0x0, 0x20000}};

    test_mul(decimal1, decimal2, check);
}

START_TEST(test_mul_manual6) {
    // -7.000000000000025
    s21_decimal decimal1 = {{0x816D8019, 0x18DE76, 0x0, 0x800F0000}};
    // -0.0000000000005
    s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x800D0000}};
    // 0.0000000000035000000000000125
    s21_decimal check = {{0x8723807D, 0x7C5850, 0x0, 0x1C0000}};

    test_mul(decimal1, decimal2, check);
}


START_TEST(test_mul_manual7) {
    // 7922816251426433759354395032.8
    s21_decimal decimal1 = {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};
    // -10.000000000000000000000000001
    s21_decimal decimal2 = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x801B0000}};
    // overflow
    int code_check = 2;

    test_mul_fail1(decimal1, decimal2, code_check);
}

START_TEST(test_mul_manual8) {
    // -0.0000000000000025
    s21_decimal decimal1 = {{0x19, 0x0, 0x0, 0x80100000}};
    // -0.0000000000000005
    s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x80100000}};
    // too small
    // 0.0000000000000000000000000000
    s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0x1C0000}};
    int code_check = 2;

    test_mul_fail2(decimal1, decimal2, decimal_check, code_check);
}


START_TEST(test_mul1) {
    // 26409387504754779197847983445
    s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x55555555, 0x0}};
    // -26409387504754779197847983445
    s21_decimal decimal2 = {{0x55555555, 0x55555555, 0x55555555, 0x80000000}};
    // overflow
    int code_check = 2;

    test_mul_fail1(decimal1, decimal2, code_check);
}

START_TEST(test_mul2) {
    // 26409387504754779197847983445
    s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x55555555, 0x0}};
    // 0.9999999999999999999999999999
    s21_decimal decimal2 = {{0xFFFFFFF, 0x3E250261, 0x204FCE5E, 0x1C0000}};
    // 26409387504754779197847983442
    s21_decimal decimal_check = {{0x55555552, 0x55555555, 0x55555555, 0x0}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul3) {
    // 26409387504754779197847983445
    s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x55555555, 0x0}};
    // -0.9999999999999999999999999999
    s21_decimal decimal2 = {{0xFFFFFFF, 0x3E250261, 0x204FCE5E, 0x801C0000}};
    // -26409387504754779197847983442
    s21_decimal decimal_check = {{0x55555552, 0x55555555, 0x55555555, 0x80000000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul4) {
    // 26409387504754779197847983445
    s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x55555555, 0x0}};
    // 79228162514264337593543950335
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // overflow
    int code_check = 1;

    test_mul_fail1(decimal1, decimal2, code_check);
}


START_TEST(test_mul5) {
    // 26409387504754779197847983445
    s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x55555555, 0x0}};
    // 0.0000000000000000000000000000
    s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x1C0000}};
    // 0
    s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0x0}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul6) {
    // -26409387504754779197847983445
    s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x55555555, 0x80000000}};
    // 26409387.504754779197847983445
    s21_decimal decimal2 = {{0x55555555, 0x55555555, 0x55555555, 0x150000}};
    // overflow
    int code_check = 2;

    test_mul_fail1(decimal1, decimal2, code_check);
}

START_TEST(test_mul7) {
    // -6148914691236517205
    s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x0, 0x80000000}};
    // 1431655765
    s21_decimal decimal2 = {{0x55555555, 0x0, 0x0, 0x0}};
    // -8803129166201954835059936825
    s21_decimal decimal_check = {{0x38E38E39, 0x55555555, 0x1C71C71C, 0x80000000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul8) {
    // -6148914691236517205
    s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x0, 0x80000000}};
    // -1431655765
    s21_decimal decimal2 = {{0x55555555, 0x0, 0x0, 0x80000000}};
    // 8803129166201954835059936825
    s21_decimal decimal_check = {{0x38E38E39, 0x55555555, 0x1C71C71C, 0x0}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul9) {
    // 1431655765
    s21_decimal decimal1 = {{0x55555555, 0x0, 0x0, 0x0}};
    // -0.5
    s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x80010000}};
    // -715827882.5
    s21_decimal decimal_check = {{0xAAAAAAA9, 0x1, 0x0, 0x80010000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul10) {
    // 1431655765
    s21_decimal decimal1 = {{0x55555555, 0x0, 0x0, 0x0}};
    // 0.4999999999999999999999999999
    s21_decimal decimal2 = {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x1C0000}};
    // 715827882.49999999999999999986
    s21_decimal decimal_check = {{0x1A27FFF2, 0xA936C5A3, 0xE74BE506, 0x140000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul11) {
    // 1431655765
    s21_decimal decimal1 = {{0x55555555, 0x0, 0x0, 0x0}};
    // -79228162514264337593543950335
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    // overflow
    int code_check = 2;

    test_mul_fail1(decimal1, decimal2, code_check);
}

START_TEST(test_mul12) {
    // 1431655765
    s21_decimal decimal1 = {{0x55555555, 0x0, 0x0, 0x0}};
    // 0
    s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x0}};
    // 0
    s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0x0}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul13) {
    // 1431655765
    s21_decimal decimal1 = {{0x55555555, 0x0, 0x0, 0x0}};
    // -0.0000000000000000000000000000
    s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x801C0000}};
    // -0.0000000000000000000000000000
    s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0x801C0000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul14) {
    // 1431655765
    s21_decimal decimal1 = {{0x55555555, 0x0, 0x0, 0x0}};
    // 0.0000000000000000000000000000
    s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x1C0000}};
    // 0.0000000000000000000000000000
    s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0x1C0000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul15) {
    // -8
    s21_decimal decimal1 = {{0x8, 0x0, 0x0, 0x80000000}};
    // 1
    s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x0}};
    // -8
    s21_decimal decimal_check = {{0x8, 0x0, 0x0, 0x80000000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul16) {
    // -8
    s21_decimal decimal1 = {{0x8, 0x0, 0x0, 0x80000000}};
    // -1
    s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x80000000}};
    // 8
    s21_decimal decimal_check = {{0x8, 0x0, 0x0, 0x0}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul17) {
    // -8
    s21_decimal decimal1 = {{0x8, 0x0, 0x0, 0x80000000}};
    // 0.5
    s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x10000}};
    // -4.0
    s21_decimal decimal_check = {{0x28, 0x0, 0x0, 0x80010000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul18) {
    // -8
    s21_decimal decimal1 = {{0x8, 0x0, 0x0, 0x80000000}};
    // -0.5
    s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x80010000}};
    // 4.0
    s21_decimal decimal_check = {{0x28, 0x0, 0x0, 0x10000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul19) {
    // -8
    s21_decimal decimal1 = {{0x8, 0x0, 0x0, 0x80000000}};
    // 0.4999999999999999999999999999
    s21_decimal decimal2 = {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x1C0000}};
    // -3.9999999999999999999999999992
    s21_decimal decimal_check = {{0x3FFFFFF8, 0xF8940984, 0x813F3978, 0x801C0000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul20) {
    // -8
    s21_decimal decimal1 = {{0x8, 0x0, 0x0, 0x80000000}};
    // -0.4999999999999999999999999999
    s21_decimal decimal2 = {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x801C0000}};
    // 3.9999999999999999999999999992
    s21_decimal decimal_check = {{0x3FFFFFF8, 0xF8940984, 0x813F3978, 0x1C0000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul21) {
    // -8
    s21_decimal decimal1 = {{0x8, 0x0, 0x0, 0x80000000}};
    // 0.5000000000000000000000000001
    s21_decimal decimal2 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}};
    // -4.0000000000000000000000000008
    s21_decimal decimal_check = {{0x40000008, 0xF8940984, 0x813F3978, 0x801C0000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul22) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // 0.9999999999999999999999999999
    s21_decimal decimal2 = {{0xFFFFFFF, 0x3E250261, 0x204FCE5E, 0x1C0000}};
    // 79228162514264337593543950327
    s21_decimal decimal_check = {{0xFFFFFFF7, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

    test_mul(decimal1, decimal2, decimal_check);
}


START_TEST(test_mul23) {
    // 7055.820576069728775806815669
    s21_decimal decimal1 = {{0x69168DB5, 0xDCB4F4B1, 0x16CC701F, 0x180000}};
    // -7055.820576069728775806815669
    s21_decimal decimal2 = {{0x69168DB5, 0xDCB4F4B1, 0x16CC701F, 0x80180000}};
    // -49784604.001688959238158904099
    s21_decimal decimal_check = {{0x40136B23, 0xBF8285B9, 0xA0DCDBF6, 0x80150000}};

    test_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul24) {
    // 7055.820576069728775806815669
    s21_decimal decimal1 = {{0x69168DB5, 0xDCB4F4B1, 0x16CC701F, 0x180000}};
    // 7055.820576069728775806815669
    s21_decimal decimal2 = {{0x69168DB5, 0xDCB4F4B1, 0x16CC701F, 0x180000}};
    // 49784604.001688959238158904099
    s21_decimal decimal_check = {{0x40136B23, 0xBF8285B9, 0xA0DCDBF6, 0x150000}};

    test_mul(decimal1, decimal2, decimal_check);
}


void test_mul(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(code, 0);
}

void test_mul_fail1(s21_decimal decimal1, s21_decimal decimal2, int code_check) {
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);


    ck_assert_int_eq(code, code_check);
}

void test_mul_fail2(s21_decimal decimal1, s21_decimal decimal2, s21_decimal decimal_check, int code_check) {
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);


    ck_assert_int_eq(code, code_check);
    ck_assert_int_eq(s21_is_equal(result, decimal_check), 1);
}


TCase *s21_AritmeticTCaseMul() {
  TCase *tcase = tcase_create("s21_AritmeticTCaseMul");

  tcase_add_test(tcase, test_mul_fail_manual1);
    tcase_add_test(tcase, test_mul_fail_manual2);
    tcase_add_test(tcase, test_mul_fail_manual3);
    tcase_add_test(tcase, test_mul_fail_manual4);
    tcase_add_test(tcase, test_mul_fail_manual5);
    tcase_add_test(tcase, test_mul_fail_manual6);
    tcase_add_test(tcase, test_mul_fail_manual7);
    tcase_add_test(tcase, test_mul_fail_manual8);
    tcase_add_test(tcase, test_mul_fail_manual9);


    tcase_add_test(tcase, test_mul_manual1);
    tcase_add_test(tcase, test_mul_manual2);
    tcase_add_test(tcase, test_mul_manual3);
    tcase_add_test(tcase, test_mul_manual4);
    tcase_add_test(tcase, test_mul_manual5);
    tcase_add_test(tcase, test_mul_manual6);
    tcase_add_test(tcase, test_mul_manual7);
    tcase_add_test(tcase, test_mul_manual8);

    tcase_add_test(tcase, test_mul1);
    tcase_add_test(tcase, test_mul2);
    tcase_add_test(tcase, test_mul3);
    tcase_add_test(tcase, test_mul4);
    tcase_add_test(tcase, test_mul5);
    tcase_add_test(tcase, test_mul6);
    tcase_add_test(tcase, test_mul7);
    tcase_add_test(tcase, test_mul8);
    tcase_add_test(tcase, test_mul9);
    tcase_add_test(tcase, test_mul10);
    tcase_add_test(tcase, test_mul11);
    tcase_add_test(tcase, test_mul12);
    tcase_add_test(tcase, test_mul13);
    tcase_add_test(tcase, test_mul14);
    tcase_add_test(tcase, test_mul15);
    tcase_add_test(tcase, test_mul16);
    tcase_add_test(tcase, test_mul17);
    tcase_add_test(tcase, test_mul18);
    tcase_add_test(tcase, test_mul19);
    tcase_add_test(tcase, test_mul20);
    tcase_add_test(tcase, test_mul21);
    tcase_add_test(tcase, test_mul22);
    tcase_add_test(tcase, test_mul23);
    tcase_add_test(tcase, test_mul24);

  return tcase;
}