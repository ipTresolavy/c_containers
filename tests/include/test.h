#ifndef H_TEST
#define H_TEST

#include <check.h>
typedef struct test_case
{
        char *name;
        const TTest *function;
} test_case_t;

#define CONT_CASE_CREATE(s, tc, tc_name, tc_func)                                                                      \
        do                                                                                                             \
        {                                                                                                              \
                (tc) = tcase_create((tc_name));                                                                        \
                tcase_add_test((tc), (tc_func));                                                                       \
                suite_add_tcase((s), (tc));                                                                            \
        } while (0)

#endif // !H_TEST
