#include "test.h"
#include "test_struct.h"

#include <check.h>
#include <stdlib.h>
#include <string.h>

START_TEST(test_vector_push_pop)
{
        CONT_VECTOR_OPERATOR_OF(test_struct_t) *vector_ops = CONT_GET_VECTOR_OPERATOR_OF(test_struct_t)();
        ck_assert_ptr_nonnull(vector_ops);

        CONT_OPERATOR_OF(test_struct_t) *element_ops = CONT_GET_OPERATOR_OF(test_struct_t)();
        ck_assert_ptr_nonnull(element_ops);

        CONT_VECTOR_OF(test_struct_t) *vector = vector_ops->construct(0, NULL, 0);
        ck_assert_ptr_nonnull(vector);

        test_struct_t *original_a = new_test_struct();
        ck_assert_ptr_nonnull(original_a);
        set_test_struct_name(original_a, "alpha", 6);
        set_test_struct_social_security(original_a, 111);

        test_struct_t *original_b = new_test_struct();
        ck_assert_ptr_nonnull(original_b);
        set_test_struct_name(original_b, "bravo", 6);
        set_test_struct_social_security(original_b, 222);

        ck_assert_int_eq(vector_ops->push_back(vector, *original_a), CONT_ARRAY_SUCCESS);
        ck_assert_int_eq(vector_ops->push_back(vector, *original_b), CONT_ARRAY_SUCCESS);
        ck_assert_uint_eq(vector_ops->size(vector), 2U);
        ck_assert_str_eq(get_test_struct_name(vector_ops->front(vector)), "alpha");
        ck_assert_str_eq(get_test_struct_name(vector_ops->back(vector)), "bravo");

        test_struct_t popped = {0};
        ck_assert_int_eq(vector_ops->pop_back(vector, &popped), CONT_ARRAY_SUCCESS);
        ck_assert_str_eq(get_test_struct_name(&popped), "bravo");
        ck_assert_uint_eq(vector_ops->size(vector), 1U);

        ck_assert_int_eq(element_ops->deinit(&popped), CONT_ARRAY_SUCCESS);

        ck_assert_int_eq(vector_ops->pop_back(vector, NULL), CONT_ARRAY_SUCCESS);
        ck_assert_uint_eq(vector_ops->size(vector), 0U);

        ck_assert_int_eq(vector_ops->destruct(&vector), CONT_ARRAY_SUCCESS);
        ck_assert_ptr_null(vector);

        ck_assert_int_eq(destruct_test_struct(&original_a), CONT_ARRAY_SUCCESS);
        ck_assert_int_eq(destruct_test_struct(&original_b), CONT_ARRAY_SUCCESS);
}
END_TEST

START_TEST(test_vector_assign_resize)
{
        CONT_VECTOR_OPERATOR_OF(test_struct_t) *vector_ops = CONT_GET_VECTOR_OPERATOR_OF(test_struct_t)();
        ck_assert_ptr_nonnull(vector_ops);

        CONT_VECTOR_OF(test_struct_t) *source = vector_ops->construct(0, NULL, 0);
        ck_assert_ptr_nonnull(source);
        CONT_VECTOR_OF(test_struct_t) *target = vector_ops->construct(0, NULL, 0);
        ck_assert_ptr_nonnull(target);

        test_struct_t *entry = new_test_struct();
        ck_assert_ptr_nonnull(entry);
        set_test_struct_name(entry, "source", 7);
        set_test_struct_social_security(entry, 333);

        ck_assert_int_eq(vector_ops->push_back(source, *entry), CONT_ARRAY_SUCCESS);
        ck_assert_int_eq(vector_ops->push_back(source, *entry), CONT_ARRAY_SUCCESS);
        ck_assert_uint_eq(vector_ops->size(source), 2U);

        ck_assert_int_eq(vector_ops->assign(target, source), CONT_ARRAY_SUCCESS);
        ck_assert_uint_eq(vector_ops->size(target), 2U);
        ck_assert_str_eq(get_test_struct_name(vector_ops->at(target, 0)), "source");
        ck_assert_str_eq(get_test_struct_name(vector_ops->at(target, 1)), "source");

        ck_assert_int_eq(vector_ops->resize(target, 5U), CONT_ARRAY_SUCCESS);
        ck_assert_uint_eq(vector_ops->size(target), 5U);
        for(size_t i = 2; i < 5; ++i)
        {
                ck_assert_str_eq(get_test_struct_name(vector_ops->at(target, i)), "no_name");
        }

        ck_assert_int_eq(vector_ops->clear(target), CONT_ARRAY_SUCCESS);
        ck_assert_uint_eq(vector_ops->size(target), 0U);

        ck_assert_int_eq(vector_ops->destruct(&source), CONT_ARRAY_SUCCESS);
        ck_assert_ptr_null(source);
        ck_assert_int_eq(vector_ops->destruct(&target), CONT_ARRAY_SUCCESS);
        ck_assert_ptr_null(target);

        ck_assert_int_eq(destruct_test_struct(&entry), CONT_ARRAY_SUCCESS);
}
END_TEST

START_TEST(test_vector_reserve_shrink)
{
        CONT_VECTOR_OPERATOR_OF(test_struct_t) *vector_ops = CONT_GET_VECTOR_OPERATOR_OF(test_struct_t)();
        ck_assert_ptr_nonnull(vector_ops);

        CONT_VECTOR_OF(test_struct_t) *vector = vector_ops->construct(2U, NULL, 0);
        ck_assert_ptr_nonnull(vector);

        test_struct_t *first = new_test_struct();
        ck_assert_ptr_nonnull(first);
        set_test_struct_name(first, "first", 6);

        test_struct_t *second = new_test_struct();
        ck_assert_ptr_nonnull(second);
        set_test_struct_name(second, "second", 7);

        ck_assert_int_eq(vector_ops->push_back(vector, *first), CONT_ARRAY_SUCCESS);
        ck_assert_int_eq(vector_ops->push_back(vector, *second), CONT_ARRAY_SUCCESS);
        ck_assert(vector_ops->capacity(vector) >= 2U);

        ck_assert_int_eq(vector_ops->reserve(vector, 10U), CONT_ARRAY_SUCCESS);
        ck_assert(vector_ops->capacity(vector) >= 10U);
        ck_assert_uint_eq(vector_ops->size(vector), 2U);

        ck_assert_int_eq(vector_ops->resize(vector, 1U), CONT_ARRAY_SUCCESS);
        ck_assert_uint_eq(vector_ops->size(vector), 1U);

        ck_assert_int_eq(vector_ops->shrink_to_fit(vector), CONT_ARRAY_SUCCESS);
        ck_assert_uint_eq(vector_ops->capacity(vector), vector_ops->size(vector));

        ck_assert_int_eq(vector_ops->destruct(&vector), CONT_ARRAY_SUCCESS);
        ck_assert_ptr_null(vector);

        ck_assert_int_eq(destruct_test_struct(&first), CONT_ARRAY_SUCCESS);
        ck_assert_int_eq(destruct_test_struct(&second), CONT_ARRAY_SUCCESS);
}
END_TEST

Suite *test_suite(void)
{
        Suite *s;
        TCase *tc;

        s = suite_create("Vector Test Suite");

        test_case_t test_cases[] = {
                {"push_pop",       test_vector_push_pop      },
                {"assign_resize",  test_vector_assign_resize },
                {"reserve_shrink", test_vector_reserve_shrink},
        };

        for(size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); ++i)
        {
                CONT_CASE_CREATE(s, tc, test_cases[i].name, test_cases[i].function);
        }

        return s;
}

int main(void)
{
        int      number_failed;
        Suite   *s;
        SRunner *sr;

        s  = test_suite();
        sr = srunner_create(s);

        srunner_run_all(sr, CK_NORMAL);
        number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
