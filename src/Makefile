CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lcheck -lm -lpthread

# Флаги для разных тестов
CFLAGS_NEGATE = $(CFLAGS) -DENABLE_OTHER_FUNCTIONS_TESTS
CFLAGS_CONVERSION = $(CFLAGS) -DENABLE_CONVERSION_TESTS
CFLAGS_ARITHMETIC_ADD = $(CFLAGS) -DENABLE_ARITHMETIC_ADD_TESTS
CFLAGS_ARITHMETIC_MUL = $(CFLAGS) -DENABLE_ARITHMETIC_MUL_TESTS

# Файлы для тестов negate
SRCS_NEGATE = tests/test_other_function/test_negate.c \
              tests/s21_tests_runner.c \
              other_function/s21_negate.c \
              tests/test_other_function/s21_main_other_function.c
OBJS_NEGATE = tests/test_other_function/test_negate.o \
              tests/s21_tests_runner_negate.o \
              other_function/s21_negate.o \
              tests/test_other_function/s21_main_other_function.o
TARGET_NEGATE = test_negate

# Файлы для тестов from_float_to_decimal
SRCS_CONVERSION = tests/test_conversion/s21_test_conversion_from_float.c \
                  tests/s21_tests_runner.c \
                  conversion/s21_from_float_to_decimal.c \
                  tests/test_conversion/s21_main_conversion.c
OBJS_CONVERSION = tests/test_conversion/s21_test_conversion_from_float.o \
                  tests/s21_tests_runner_conversion.o \
                  conversion/s21_from_float_to_decimal.o \
                  tests/test_conversion/s21_main_conversion.o
TARGET_CONVERSION = test_conversion

SRCS_ARITHMETIC_ADD = tests/tests_arithmetic/s21_test_add.c \
                      tests/s21_tests_runner.c \
                      arithmetic/s21_add.c \
                      tests/tests_arithmetic/s21_main_arithmetic.c
OBJS_ARITHMETIC_ADD = tests/tests_arithmetic/s21_test_add.o \
                      tests/s21_tests_runner_arithmetic_add.o \
                      arithmetic/s21_add.o \
                      tests/tests_arithmetic/s21_main_arithmetic.o

TARGET_ARITHMETIC_ADD = test_arithmetic_add
# Файлы для mul
SRCS_ARITHMETIC_MUL = tests/tests_arithmetic/s21_test_mul.c \
                      tests/s21_tests_runner.c \
                      arithmetic/s21_mul.c \
                      tests/tests_arithmetic/s21_main_arithmetic.c

OBJS_ARITHMETIC_MUL = tests/tests_arithmetic/s21_test_mul.o \
                      tests/s21_tests_runner_arithmetic_mul.o \
                      arithmetic/s21_mul.o \
                      tests/tests_arithmetic/s21_main_arithmetic.o

TARGET_ARITHMETIC_MUL = test_arithmetic_mul

# Компиляция runner-а с разными флагами
tests/s21_tests_runner_negate.o: tests/s21_tests_runner.c
	$(CC) $(CFLAGS_NEGATE) -c -o $@ $<

tests/s21_tests_runner_conversion.o: tests/s21_tests_runner.c
	$(CC) $(CFLAGS_CONVERSION) -c -o $@ $<

tests/s21_tests_runner_arithmetic.o: tests/s21_tests_runner.c
	$(CC) $(CFLAGS_ARITHMETIC) -c -o $@ $<

# Компиляция остальных файлов
tests/test_other_function/test_negate.o: tests/test_other_function/test_negate.c
	$(CC) $(CFLAGS_NEGATE) -c -o $@ $<

other_function/s21_negate.o: other_function/s21_negate.c
	$(CC) $(CFLAGS_NEGATE) -c -o $@ $<

tests/test_other_function/s21_main_other_function.o: tests/test_other_function/s21_main_other_function.c
	$(CC) $(CFLAGS_NEGATE) -c -o $@ $<

tests/test_conversion/s21_test_conversion_from_float.o: tests/test_conversion/s21_test_conversion_from_float.c
	$(CC) $(CFLAGS_CONVERSION) -c -o $@ $<

conversion/s21_from_float_to_decimal.o: conversion/s21_from_float_to_decimal.c
	$(CC) $(CFLAGS_CONVERSION) -c -o $@ $<

tests/test_conversion/s21_main_conversion.o: tests/test_conversion/s21_main_conversion.c
	$(CC) $(CFLAGS_CONVERSION) -c -o $@ $<

tests/s21_tests_runner_arithmetic_add.o: tests/s21_tests_runner.c
	$(CC) $(CFLAGS_ARITHMETIC_ADD) -c -o $@ $<

tests/s21_tests_runner_arithmetic_mul.o: tests/s21_tests_runner.c
	$(CC) $(CFLAGS_ARITHMETIC_MUL) -c -o $@ $<

# Компиляция отдельных тестов
tests/tests_arithmetic/s21_test_add.o: tests/tests_arithmetic/s21_test_add.c
	$(CC) $(CFLAGS_ARITHMETIC_ADD) -c -o $@ $<

arithmetic/s21_add.o: arithmetic/s21_add.c
	$(CC) $(CFLAGS_ARITHMETIC_ADD) -c -o $@ $<

tests/tests_arithmetic/s21_test_mul.o: tests/tests_arithmetic/s21_test_mul.c
	$(CC) $(CFLAGS_ARITHMETIC_MUL) -c -o $@ $<

arithmetic/s21_mul.o: arithmetic/s21_mul.c
	$(CC) $(CFLAGS_ARITHMETIC_MUL) -c -o $@ $<

tests/tests_arithmetic/s21_main_arithmetic.o: tests/tests_arithmetic/s21_main_arithmetic.c
	$(CC) $(CFLAGS) -c -o $@ $<


# Линковка исполняемых файлов
$(TARGET_NEGATE): $(OBJS_NEGATE)
	$(CC) $(CFLAGS_NEGATE) $(OBJS_NEGATE) -o $(TARGET_NEGATE) $(LDFLAGS)

$(TARGET_CONVERSION): $(OBJS_CONVERSION)
	$(CC) $(CFLAGS_CONVERSION) $(OBJS_CONVERSION) -o $(TARGET_CONVERSION) $(LDFLAGS)

$(TARGET_ARITHMETIC_ADD): $(OBJS_ARITHMETIC_ADD)
	$(CC) $(CFLAGS_ARITHMETIC_ADD) $(OBJS_ARITHMETIC_ADD) -o $(TARGET_ARITHMETIC_ADD) $(LDFLAGS)

$(TARGET_ARITHMETIC_MUL): $(OBJS_ARITHMETIC_MUL)
	$(CC) $(CFLAGS_ARITHMETIC_MUL) $(OBJS_ARITHMETIC_MUL) -o $(TARGET_ARITHMETIC_MUL) $(LDFLAGS)


clean:
	rm -f tests/s21_tests_runner_negate.o tests/s21_tests_runner_conversion.o tests/s21_tests_runner.o  tests/s21_tests_runner_arithmetic.o
	rm -f $(OBJS_NEGATE) $(OBJS_CONVERSION) $(OBJS_ARITHMETIC_ADD) $(OBJS_ARITHMETIC_MUL) $(TARGET_NEGATE) $(TARGET_CONVERSION) $(TARGET_ARITHMETIC_ADD) $(TARGET_ARITHMETIC_MUL)


run_negate: $(TARGET_NEGATE)
	./$(TARGET_NEGATE)

run_conversion: $(TARGET_CONVERSION)
	./$(TARGET_CONVERSION)

# Запуск отдельных тестов
run_arithmetic_add: $(TARGET_ARITHMETIC_ADD)
	./$(TARGET_ARITHMETIC_ADD)

run_arithmetic_mul: $(TARGET_ARITHMETIC_MUL)
	./$(TARGET_ARITHMETIC_MUL)
