#include "test.h"
#include "test_struct.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

START_TEST(test_construct_destruct)
{
        CONT_ARRAY_OPERATOR_OF(test_struct_t) *operator= CONT_GET_ARRAY_OPERATOR_OF(test_struct_t)();
        ck_assert_ptr_nonnull(operator);

        CONT_ARRAY_OF(test_struct_t) *array = operator->construct(3, NULL, 0);
        ck_assert_ptr_nonnull(array);

        operator->destruct(&array);
        ck_assert_ptr_null(array);
}
END_TEST

START_TEST(test_assign)
{
        CONT_ARRAY_OPERATOR_OF(test_struct_t) *operator= CONT_GET_ARRAY_OPERATOR_OF(test_struct_t)();
        ck_assert_ptr_nonnull(operator);

        test_struct_t *test1 = new_test_struct();
        ck_assert_ptr_nonnull(test1);
        test_struct_t *test2 = new_test_struct();
        ck_assert_ptr_nonnull(test2);
        set_test_struct_name(test2, "test2", 6);
        set_test_struct_social_security(test2, 999);

        CONT_ARRAY_OF(test_struct_t) *array1 = operator->construct(3, test1, 1);
        ck_assert_ptr_nonnull(array1);
        CONT_ARRAY_OF(test_struct_t) *array2 = operator->construct(3, test2, 1);
        ck_assert_ptr_nonnull(array2);
        operator->assign(array1, array2);

        ck_assert_str_eq(get_test_struct_name(operator->at(array1, 0)), get_test_struct_name(operator->at(array2, 0)));
        ck_assert_int_eq(get_test_struct_social_security(operator->at(array1, 0)),
                         get_test_struct_social_security(operator->at(array2, 0)));

        operator->destruct(&array1);
        ck_assert_ptr_null(array1);
        operator->destruct(&array2);
        ck_assert_ptr_null(array2);
        destruct_test_struct(&test1);
        ck_assert_ptr_null(test1);
        destruct_test_struct(&test2);
        ck_assert_ptr_null(test2);
}
END_TEST

START_TEST(test_at)
{
        CONT_ARRAY_OPERATOR_OF(test_struct_t) *operator= CONT_GET_ARRAY_OPERATOR_OF(test_struct_t)();
        ck_assert_ptr_nonnull(operator);

        test_struct_t *test2 = new_test_struct();
        ck_assert_ptr_nonnull(test2);

        CONT_ARRAY_OF(test_struct_t) *array = operator->construct(3, test2, 1);
        ck_assert_ptr_nonnull(array);

        test_struct_t *test1 = operator->at(array, 0);
        ck_assert_ptr_nonnull(test1);

        ck_assert_ptr_ne(test1, test2);
        ck_assert_ptr_ne(get_test_struct_name(test1), get_test_struct_name(test2));
        ck_assert_str_eq(get_test_struct_name(test1), get_test_struct_name(test2));
        ck_assert_uint_eq(get_test_struct_social_security(test1), get_test_struct_social_security(test2));
        operator->destruct(&array);
        ck_assert_ptr_null(array);
        destruct_test_struct(&test2);
        ck_assert_ptr_null(test2);
}
END_TEST

START_TEST(test_empty_size)
{
        CONT_ARRAY_OPERATOR_OF(test_struct_t) *operator= CONT_GET_ARRAY_OPERATOR_OF(test_struct_t)();
        ck_assert_ptr_nonnull(operator);

        CONT_ARRAY_OF(test_struct_t) *array1 = operator->construct(3, NULL, 0);
        ck_assert_ptr_nonnull(array1);
        ck_assert(false == operator->empty(array1));
        ck_assert_int_eq(operator->size(array1), 3);

        operator->destruct(&array1);
        ck_assert_ptr_null(array1);

        CONT_ARRAY_OF(test_struct_t) *array2 = operator->construct(0, NULL, 0);
        ck_assert_ptr_nonnull(array2);
        ck_assert(true == operator->empty(array2));
        ck_assert_int_eq(operator->size(array1), 0);

        operator->destruct(&array2);
        ck_assert_ptr_null(array2);
}
END_TEST

Suite *test_suite(void)
{
        Suite *s;
        TCase *tc;

        s = suite_create("Array Test Suite");

        test_case_t test_cases[] = {
            {"construct",  test_construct_destruct},
            {"at",         test_at                },
            {"assign",     test_assign            },
            {"empty_size", test_empty_size        },
        };

        for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); ++i)
        {
                CONT_CASE_CREATE(s, tc, test_cases[i].name, test_cases[i].function);
        }

        return s;
}

int main(void)
{
        int number_failed;
        Suite *s;
        SRunner *sr;

        s = test_suite();
        sr = srunner_create(s);

        srunner_run_all(sr, CK_NORMAL);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
