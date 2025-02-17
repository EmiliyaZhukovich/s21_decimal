#include "../s21_tests_runner.h"

void test_negate(s21_decimal decimal, s21_decimal decimal_check);

int s21_is_equal(s21_decimal a, s21_decimal b) {
    return (a.bits[0] == b.bits[0] &&
            a.bits[1] == b.bits[1] &&
            a.bits[2] == b.bits[2] &&
            a.bits[3] == b.bits[3]);
}


START_TEST(test_negate_fail1) {
    // 792281625.14264337593543950335
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};

    int code = s21_negate(decimal, NULL);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}

START_TEST(test_negate_fail2) {
    // степень 154 (показатель степени должен быть от 0 до 28)
    // биты 0-15 не нули
    // биты 24-30 не нули
    s21_decimal decimal = {{0, 0, 0, 1000000000}};
    s21_decimal result;

    int code = s21_negate(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_negate_fail3) {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal = {{0, 0, 0, 0x1D0000}};
    s21_decimal result;

    int code = s21_negate(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST

START_TEST(test_negate_fail4) {
    // степень 28 (что корректно), но биты 0-15 не нули (младший бит)
    s21_decimal decimal = {{-1, 0, 0, 0x1C0001}};
    s21_decimal result;

    int code = s21_negate(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST


START_TEST(test_negate_fail5) {
    // Просто все единицы
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal result;

    int code = s21_negate(decimal, &result);

    ck_assert_int_eq(code, TEST_OTHER_ERROR);
}
END_TEST



START_TEST(test_negate_ok1) {
    // 79228162514264337593543950335
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
    // -79228162514264337593543950335
    s21_decimal decimal_check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok2) {
    // -79228162514264337593543950335
    s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    // 79228162514264337593543950335
    s21_decimal decimal_check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok3) {
    // -429496.7297
    s21_decimal decimal = {{0x1, 0x1, 0x0, 0x80040000}};
    // 429496.7297
    s21_decimal decimal_check = {{0x1, 0x1, 0x0, 0x40000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok4) {
    // -4294967295
    s21_decimal decimal = {{0xFFFFFFFF, 0x0, 0x0, 0x80000000}};
    // 4294967295
    s21_decimal decimal_check = {{0xFFFFFFFF, 0x0, 0x0, 0x0}};

    test_negate(decimal, decimal_check);
}
START_TEST(test_negate_ok5) {
    // 1.1
    s21_decimal decimal = {{0xB, 0x0, 0x0, 0x10000}};
    // -1.1
    s21_decimal decimal_check = {{0xB, 0x0, 0x0, 0x80010000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok6) {
    // 9
    s21_decimal decimal = {{0x9, 0x0, 0x0, 0x0}};
    // -9
    s21_decimal decimal_check = {{0x9, 0x0, 0x0, 0x80000000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok7) {
    // -0
    s21_decimal decimal = {{0x0, 0x0, 0x0, 0x80000000}};
    // -0
    s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0x0}};

    test_negate(decimal, decimal_check);
}


START_TEST(test_negate_ok8) {
    // -0.0
    s21_decimal decimal = {{0x0, 0x0, 0x0, 0x80010000}};
    // -0.0
    s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0x10000}};

    test_negate(decimal, decimal_check);
}


START_TEST(test_negate_ok9) {
    // -0.00
    s21_decimal decimal = {{0x0, 0x0, 0x0, 0x80020000}};
    // -0.00
    s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0x20000}};

    test_negate(decimal, decimal_check);
}


START_TEST(test_negate_ok10) {
    // -0.00000000000000
    s21_decimal decimal = {{0x0, 0x0, 0x0, 0x800E0000}};
    // -0.00000000000000
    s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0xE0000}};

    test_negate(decimal, decimal_check);
}


START_TEST(test_negate_ok11) {
    // -0.000000000000000000000000000
    s21_decimal decimal = {{0x0, 0x0, 0x0, 0x801B0000}};
    // -0.000000000000000000000000000
    s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0x1B0000}};

    test_negate(decimal, decimal_check);
}


START_TEST(test_negate_ok12) {
    // -0.5
    s21_decimal decimal = {{0x5, 0x0, 0x0, 0x80010000}};
    // 0.5
    s21_decimal decimal_check = {{0x5, 0x0, 0x0, 0x10000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok13) {
    // -0.999999999
    s21_decimal decimal = {{0x3B9AC9FF, 0x0, 0x0, 0x80090000}};
    // 0.999999999
    s21_decimal decimal_check = {{0x3B9AC9FF, 0x0, 0x0, 0x90000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok14) {
    // -1.0000000000000000000000000001
    s21_decimal decimal = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x801C0000}};
    // 1.0000000000000000000000000001
    s21_decimal decimal_check = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x1C0000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok15) {
    // -1.51
    s21_decimal decimal = {{0x97, 0x0, 0x0, 0x80020000}};
    // 1.51
    s21_decimal decimal_check = {{0x97, 0x0, 0x0, 0x20000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok16) {
    // 1.49999999
    s21_decimal decimal = {{0x8F0D17F, 0x0, 0x0, 0x80000}};
    // -1.49999999
    s21_decimal decimal_check = {{0x8F0D17F, 0x0, 0x0, 0x80080000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok17) {
    // -1.9999999999999999999999999999
    s21_decimal decimal = {{0x1FFFFFFF, 0x7C4A04C2, 0x409F9CBC, 0x801C0000}};
    // 1.9999999999999999999999999999
    s21_decimal decimal_check = {{0x1FFFFFFF, 0x7C4A04C2, 0x409F9CBC, 0x1C0000}};

    test_negate(decimal, decimal_check);
}


START_TEST(test_negate_ok18) {
    // -28661776774289094351.326636307
    s21_decimal decimal = {{0xE7BD6913, 0x4C875CF8, 0x5C9C7750, 0x80090000}};
    // 28661776774289094351.326636307
    s21_decimal decimal_check = {{0xE7BD6913, 0x4C875CF8, 0x5C9C7750, 0x90000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok19) {
    // 54701583.980450434134055537457
    s21_decimal decimal = {{0xF1474B31, 0x80089C5A, 0xB0C016F9, 0x150000}};
    // -54701583.980450434134055537457
    s21_decimal decimal_check = {{0xF1474B31, 0x80089C5A, 0xB0C016F9, 0x80150000}};

    test_negate(decimal, decimal_check);
}


START_TEST(test_negate_ok20) {
    // -2.596597038203724290724477478
    s21_decimal decimal = {{0x64B83A26, 0x7E973CF9, 0x863DACF, 0x801B0000}};
    // 2.596597038203724290724477478
    s21_decimal decimal_check = {{0x64B83A26, 0x7E973CF9, 0x863DACF, 0x1B0000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok21) {
    // -2341110502190.8440369730403507
    s21_decimal decimal = {{0x5F3BBCB3, 0x40813793, 0x4BA53652, 0x80100000}};
    // 2341110502190.8440369730403507
    s21_decimal decimal_check = {{0x5F3BBCB3, 0x40813793, 0x4BA53652, 0x100000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok22) {
    // -69212599648938134433703383.617
    s21_decimal decimal = {{0x81F3D241, 0x232D423B, 0xDFA35211, 0x80030000}};
    // 69212599648938134433703383.617
    s21_decimal decimal_check = {{0x81F3D241, 0x232D423B, 0xDFA35211, 0x30000}};

    test_negate(decimal, decimal_check);
}


START_TEST(test_negate_ok23) {
    // 2540106587315049901576149858.3
    s21_decimal decimal = {{0xE92691D7, 0x27E65E1D, 0x521344EC, 0x10000}};
    // -2540106587315049901576149858.3
    s21_decimal decimal_check = {{0xE92691D7, 0x27E65E1D, 0x521344EC, 0x80010000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok24) {
    // 50042765294467513495253976855
    s21_decimal decimal = {{0x3F488717, 0x78E1B68A, 0xA1B267BE, 0x0}};
    // -50042765294467513495253976855
    s21_decimal decimal_check = {{0x3F488717, 0x78E1B68A, 0xA1B267BE, 0x80000000}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok25) {
    // -50042765294467513495253976855
    s21_decimal decimal = {{0x3F488717, 0x78E1B68A, 0xA1B267BE, 0x80000000}};
    // 50042765294467513495253976855
    s21_decimal decimal_check = {{0x3F488717, 0x78E1B68A, 0xA1B267BE, 0x0}};

    test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok26) {
    // 5694011593981989243301.7721765
    s21_decimal decimal = {{0xD1B953A5, 0x2D2C928A, 0xB7FBC28E, 0x70000}};
    // -5694011593981989243301.7721765
    s21_decimal decimal_check = {{0xD1B953A5, 0x2D2C928A, 0xB7FBC28E, 0x80070000}};

    test_negate(decimal, decimal_check);
}


void test_negate(s21_decimal decimal, s21_decimal decimal_check) {
    s21_decimal result;

    int code = s21_negate(decimal, &result);
    int sign_check = get_sign(decimal_check);
    int sign_result = get_sign(result);

    if (s21_is_equal(result, decimal_check) != 1 || code != 0) {  
        printf("\nTest failed!\n");
        printf("Expected:\n");
        PrintDecimal(decimal_check);
        printf("Actual:\n");
        PrintDecimal(result);
    }
    else{
        printf("\nTest succes!\n");
        printf("Expected:\n");
        PrintDecimal(decimal_check);
        printf("Actual:\n");
        PrintDecimal(result);
    }

    ck_assert_int_eq(code, TEST_OTHER_OK);
    ck_assert_int_eq(s21_is_equal(result, decimal_check), 1);
    ck_assert_int_eq(sign_check, sign_result);
}

TCase *s21_NegateTCase() {
    TCase *tcase = tcase_create("s21_NegateTests");

    tcase_add_test(tcase, test_negate_fail1);
    tcase_add_test(tcase, test_negate_fail2);
    tcase_add_test(tcase, test_negate_fail3);
    tcase_add_test(tcase, test_negate_fail4);
    tcase_add_test(tcase, test_negate_fail5);
    tcase_add_test(tcase, test_negate_ok1);
    tcase_add_test(tcase, test_negate_ok2);
    tcase_add_test(tcase, test_negate_ok3);
    tcase_add_test(tcase, test_negate_ok4);
    tcase_add_test(tcase, test_negate_ok5);
    tcase_add_test(tcase, test_negate_ok6);
    tcase_add_test(tcase, test_negate_ok7);
    tcase_add_test(tcase, test_negate_ok8);
    tcase_add_test(tcase, test_negate_ok9);
    tcase_add_test(tcase, test_negate_ok10);
    tcase_add_test(tcase, test_negate_ok11);
    tcase_add_test(tcase, test_negate_ok12);
    tcase_add_test(tcase, test_negate_ok13);
    tcase_add_test(tcase, test_negate_ok14);
    tcase_add_test(tcase, test_negate_ok15);
    tcase_add_test(tcase, test_negate_ok16);
    tcase_add_test(tcase, test_negate_ok17);
    tcase_add_test(tcase, test_negate_ok18);
    tcase_add_test(tcase, test_negate_ok19);
    tcase_add_test(tcase, test_negate_ok20);
    tcase_add_test(tcase, test_negate_ok21);
    tcase_add_test(tcase, test_negate_ok22);
    tcase_add_test(tcase, test_negate_ok23);
    tcase_add_test(tcase, test_negate_ok24);
    tcase_add_test(tcase, test_negate_ok25);
    tcase_add_test(tcase, test_negate_ok26);

    return tcase;
}
