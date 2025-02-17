#include "../s21_tests_runner.h"

void test_add(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check);
void test_add_fail(s21_decimal decimal1, s21_decimal decimal2, int check);

int s21_is_equal(s21_decimal a, s21_decimal b) {
    return (a.bits[0] == b.bits[0] &&
            a.bits[1] == b.bits[1] &&
            a.bits[2] == b.bits[2] &&
            a.bits[3] == b.bits[3]);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Тесты на некорректные данные (ручные)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

START_TEST(test_add_fail_manual1) {
    // 792281625.14264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    // int code = s21_add(decimal1, decimal2, NULL);

    // ck_assert_int_ne(code, 0);
    s21_decimal result;
    
    int code = s21_add(decimal1, decimal2, &result);  // Передаём указатель!
    
    ck_assert_int_ne(code, 0); 

}

START_TEST(test_add_fail_manual2) {
    // степень 154 (показатель степени должен быть от 0 до 28)
    // биты 0-15 не нули
    // биты 24-30 не нули
    s21_decimal decimal1 = {{0, 0, 0, 1000000000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_add_fail_manual3) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal1 = {{-1, 0, 0, 0x1D0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST


START_TEST(test_add_fail_manual4) {
    // степень 28 (что корректно), но биты 0-15 не нули (младший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x1C0001}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST


START_TEST(test_add_fail_manual5) {
    // степень 28 (что корректно), но биты 24-30 не нули (старший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x401C0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_add_fail_manual6) {
    // Просто все единицы
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_add_fail_manual7) {
    // степень 154 (показатель степени должен быть от 0 до 28)
    // биты 0-15 не нули
    // биты 24-30 не нули
    s21_decimal decimal2 = {{0, 0, 0, 1000000000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_add_fail_manual8) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal2 = {{-1, 0, 0, 0x1D0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST

START_TEST(test_add_fail_manual9) {
    // степень 28 (что корректно), но биты 24-30 не нули (старший бит)
    s21_decimal decimal2 = {{-1, 0, 0, 0x401C0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
}
END_TEST


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Тесты на все типы данных (ручные)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

START_TEST(test_add_manual1) {
    // 7.922816251426433759354395034
    s21_decimal decimal1 = {{0x9999999A, 0x99999999, 0x19999999, 0x1B0000}};
    // 3.9614081257132168796771975168
    s21_decimal decimal2 = {{0x0, 0x0, 0x80000000, 0x1C0000}};
    // 11.884224377139650639031592551
    s21_decimal check = {{0x66666667, 0x66666666, 0x26666666, 0x1B0000}};

    test_add(decimal1, decimal2, check);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Tests all data (automatic)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

START_TEST(test_add1) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // 79228162514264337593543950335
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // overflow
    int check = 1;

    test_add_fail(decimal1, decimal2, check);
}

START_TEST(test_add2) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // -79228162514264337593543950335
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    // 0
    s21_decimal check = {{0x0, 0x0, 0x0, 0x0}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add3) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // 79228162514264337593543950334
    s21_decimal decimal2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // overflow
    int check = 1;

    test_add_fail(decimal1, decimal2, check);
}

START_TEST(test_add4) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // -79228162514264337593543950334
    s21_decimal decimal2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    // 1
    s21_decimal check = {{0x1, 0x0, 0x0, 0x0}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add5) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // 7922816251426433759354395033
    s21_decimal decimal2 = {{0x99999999, 0x99999999, 0x19999999, 0x0}};
    // overflow
    int check = 1;

    test_add_fail(decimal1, decimal2, check);
}

START_TEST(test_add6) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // -7922816251426433759354395033
    s21_decimal decimal2 = {{0x99999999, 0x99999999, 0x19999999, 0x80000000}};
    // 71305346262837903834189555302
    s21_decimal check = {{0x66666666, 0x66666666, 0xE6666666, 0x0}};

    test_add(decimal1, decimal2, check);
}


START_TEST(test_add7) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // -7922816251426433759354395033.5
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};
    // 71305346262837903834189555302
    s21_decimal check = {{0x66666666, 0x66666666, 0xE6666666, 0x0}};

    test_add(decimal1, decimal2, check);
}


START_TEST(test_add8) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // 1
    s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x0}};
    // overflow
    int check = 1;

    test_add_fail(decimal1, decimal2, check);
}

START_TEST(test_add9) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // -1
    s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x80000000}};
    // 79228162514264337593543950334
    s21_decimal check = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add10) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // 0.5
    s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x10000}};
    // overflow
    int check = 1;

    test_add_fail(decimal1, decimal2, check);
}

START_TEST(test_add11) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // -0.5
    s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x80010000}};
    // 79228162514264337593543950334
    s21_decimal check = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

    test_add(decimal1, decimal2, check);
}


START_TEST(test_add12) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // 0.5000000000000000000000000001
    s21_decimal decimal2 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}};
    // overflow
    int check = 1;

    test_add_fail(decimal1, decimal2, check);
}

START_TEST(test_add13) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // -0.5000000000000000000000000001
    s21_decimal decimal2 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x801C0000}};
    // 79228162514264337593543950334
    s21_decimal check = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add14) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // 0
    s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x0}};
    // 79228162514264337593543950335
    s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add15) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // 0.0000000000000000000000000001
    s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x1C0000}};
    // 79228162514264337593543950335
    s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add16) {
    // 79228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // -0.0000000000000000000000000001
    s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x801C0000}};
    // 79228162514264337593543950335
    s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add17) {
    // 7922816251426433759354395033
    s21_decimal decimal1 = {{0x99999999, 0x99999999, 0x19999999, 0x0}};
    // -7922816251426433759354395034
    s21_decimal decimal2 = {{0x9999999A, 0x99999999, 0x19999999, 0x80000000}};
    // -1
    s21_decimal check = {{0x1, 0x0, 0x0, 0x80000000}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add18) {
    // -1
    s21_decimal decimal1 = {{0x1, 0x0, 0x0, 0x80000000}};
    // 0.0000000000000000000000000000
    s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x1C0000}};
    // -1.0000000000000000000000000000
    s21_decimal check = {{0x10000000, 0x3E250261, 0x204FCE5E, 0x801C0000}};

    test_add(decimal1, decimal2, check);
}


START_TEST(test_add19) {
    // 0.5
    s21_decimal decimal1 = {{0x5, 0x0, 0x0, 0x10000}};
    // -1
    s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x80000000}};
    // -0.5
    s21_decimal check = {{0x5, 0x0, 0x0, 0x80010000}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add20) {
    // -1.9999999999999999999999999000
    s21_decimal decimal1 = {{0x1FFFFC18, 0x7C4A04C2, 0x409F9CBC, 0x801C0000}};
    // -1.0000000000000000000000000001
    s21_decimal decimal2 = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x801C0000}};
    // -2.9999999999999999999999999001
    s21_decimal check = {{0x2FFFFC19, 0xBA6F0723, 0x60EF6B1A, 0x801C0000}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add21) {
    // 7.9228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
    // 26409387
    s21_decimal decimal2 = {{0x192F9AB, 0x0, 0x0, 0x0}};
    // 26409394.922816251426433759354
    s21_decimal check = {{0x4965787A, 0x779DCBC0, 0x555556E7, 0x150000}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add22) {
    // 7.9228162514264337593543950335
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
    // -26409387
    s21_decimal decimal2 = {{0x192F9AB, 0x0, 0x0, 0x80000000}};
    // -26409379.077183748573566240646
    s21_decimal check = {{0x605A8786, 0x794A742A, 0x5555538C, 0x80150000}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add23) {
    // 649244002.20841517182548587502
    s21_decimal decimal1 = {{0x122233EE, 0x5675EBE6, 0xD1C83484, 0x140000}};
    // -0
    s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x80000000}};
    // 649244002.20841517182548587502
    s21_decimal check = {{0x122233EE, 0x5675EBE6, 0xD1C83484, 0x140000}};

    test_add(decimal1, decimal2, check);
}

START_TEST(test_add24) {
    // 649244002.20841517182548587502
    s21_decimal decimal1 = {{0x122233EE, 0x5675EBE6, 0xD1C83484, 0x140000}};
    // -649244002.20841517182548587502
    s21_decimal decimal2 = {{0x122233EE, 0x5675EBE6, 0xD1C83484, 0x80140000}};
    // 0.00000000000000000000
    s21_decimal check = {{0x0, 0x0, 0x0, 0x140000}};

    test_add(decimal1, decimal2, check);
}


void test_add(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

   if (s21_is_equal(result, check) != 1 || code != 0) {  
        printf("\nTest failed!\n");
        printf("Expected:\n");
        PrintDecimal(check);
        printf("Actual:\n");
        PrintDecimal(result);
    }


    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(code, 0);
}

void test_add_fail(s21_decimal decimal1, s21_decimal decimal2, int check) {
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_eq(code, check);
}

TCase *s21_AritmeticTCaseAdd() {
  TCase *tcase = tcase_create("s21_AritmeticTCaseAdd");

   tcase_add_test(tcase, test_add_fail_manual1);
    tcase_add_test(tcase, test_add_fail_manual2);
    tcase_add_test(tcase, test_add_fail_manual3);
    tcase_add_test(tcase, test_add_fail_manual4);
    tcase_add_test(tcase, test_add_fail_manual5);
    tcase_add_test(tcase, test_add_fail_manual6);
    tcase_add_test(tcase, test_add_fail_manual7);
    tcase_add_test(tcase, test_add_fail_manual8);
    tcase_add_test(tcase, test_add_fail_manual9);
    tcase_add_test(tcase, test_add_manual1);
    tcase_add_test(tcase, test_add1);
    tcase_add_test(tcase, test_add2);
    tcase_add_test(tcase, test_add3);
    tcase_add_test(tcase, test_add4);
    tcase_add_test(tcase, test_add5);
    tcase_add_test(tcase, test_add6);
    tcase_add_test(tcase, test_add7);
    tcase_add_test(tcase, test_add8);
    tcase_add_test(tcase, test_add9);
    tcase_add_test(tcase, test_add10);
    tcase_add_test(tcase, test_add11);
    tcase_add_test(tcase, test_add12);
    tcase_add_test(tcase, test_add13);
    tcase_add_test(tcase, test_add14);
    tcase_add_test(tcase, test_add15);
    tcase_add_test(tcase, test_add16);
    tcase_add_test(tcase, test_add17);
    tcase_add_test(tcase, test_add18);
    tcase_add_test(tcase, test_add19);
    tcase_add_test(tcase, test_add20);
    tcase_add_test(tcase, test_add21);
    tcase_add_test(tcase, test_add22);
    tcase_add_test(tcase, test_add23);
    tcase_add_test(tcase, test_add24);

  return tcase;
}