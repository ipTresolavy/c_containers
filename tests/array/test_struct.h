#ifndef H_TEST_STRUCT
#define H_TEST_STRUCT

#include "array/decl_array.h"
#include "operator.h"
#include "types.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>

#define INIT_SIZE 512

struct test_struct;
typedef struct test_struct test_struct_t;

test_struct_t *new_test_struct(void);
char *get_test_struct_name(test_struct_t *test_struct);
cstl_array_status_t init_test_struct(test_struct_t *test_struct);
cstl_array_status_t set_test_struct_name(test_struct_t *test_struct, char *name, size_t size);
uint32_t get_test_struct_social_security(test_struct_t *test_struct);
cstl_array_status_t copy_test_struct(test_struct_t *test_struct, test_struct_t *other);
cstl_array_status_t set_test_struct_social_security(test_struct_t *test_struct, uint32_t social_security);
cstl_array_status_t deinit_test_struct(test_struct_t *test_struct);
cstl_array_status_t destruct_test_struct(test_struct_t **test_struct);

DECL_OPERATOR_OF(test_struct_t);

DECL_ARRAY_OF(test_struct_t);

#endif // !H_TEST_STRUCT
