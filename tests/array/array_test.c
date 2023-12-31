#include "test_struct.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#define CASE_CREATE(s, tc, tc_name, tc_func)                                                                           \
	tc = tcase_create(tc_name);                                                                                    \
	tcase_add_test(tc, tc_func);                                                                                   \
	suite_add_tcase(s, tc)

START_TEST(test_construct_destruct)
{
	ARRAY_OPERATOR_OF(test_struct_t) *operator= GET_ARRAY_OPERATOR_OF(test_struct_t)();
	ck_assert_ptr_nonnull(operator);

	ARRAY_OF(test_struct_t) *array = operator->construct(3, NULL, 0);
	ck_assert_ptr_nonnull(array);

	operator->destruct(&array);
	ck_assert_ptr_null(array);
}
END_TEST

START_TEST(test_at)
{
	ARRAY_OPERATOR_OF(test_struct_t) *operator= GET_ARRAY_OPERATOR_OF(test_struct_t)();
	ck_assert_ptr_nonnull(operator);

	test_struct_t *test2 = new_test_struct();
	ck_assert_ptr_nonnull(test2);

	ARRAY_OF(test_struct_t) *array = operator->construct(3, test2, 1);
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

Suite *test_suite(void)
{
	Suite *s;
	TCase *tc;

	s = suite_create("Array Test Suite");

	CASE_CREATE(s, tc, "construct_destruct", test_construct_destruct);
	CASE_CREATE(s, tc, "at", test_at);

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
