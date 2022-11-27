#include "array.h"
#include <check.h>

START_TEST(test_array_create)
{
    Array *array = array_create(0);

    ck_assert_int_eq(0, array_length(array));
    ck_assert_int_eq(0, array_capacity(array));

    array_free(array);
}
END_TEST

START_TEST(test_array_push)
{
    Array *array = array_create(sizeof(int));
    int value = 5;

    array_push(array, &value);

    ck_assert_int_eq(1, array_length(array));
    ck_assert_int_eq(2, array_capacity(array));

    array_free(array);
}
END_TEST

START_TEST(test_array_at)
{
    Array *array = array_create(sizeof(int));
    int value = 5;

    array_push(array, &value);

    ck_assert_int_eq(5, *(int *)array_at(array, 0));
    ck_assert_ptr_null(array_at(array, 1));

    array_free(array);
}
END_TEST

Suite *array_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Array");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_array_create);
    tcase_add_test(tc_core, test_array_push);
    tcase_add_test(tc_core, test_array_at);
    suite_add_tcase(s, tc_core);

    return s;
}
