#include "test_struct.h"
#include "array/impl_array.h"
#include "operator.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_struct
{
        char *name;
        size_t name_buffer_size;
        uint32_t social_security;
};

cont_array_status_t init_test_struct(test_struct_t *test_struct)
{
        if (NULL == test_struct)
        {
                return CONT_ARRAY_FAILURE;
        }
        test_struct->name = malloc(INIT_SIZE * sizeof(char));
        if (NULL == test_struct->name)
        {
                return CONT_ARRAY_FAILURE;
        }
        test_struct->name_buffer_size = INIT_SIZE;
        strcpy(test_struct->name, "no_name");
        test_struct->social_security = 0;
        return CONT_ARRAY_SUCCESS;
}

cont_array_status_t copy_test_struct(test_struct_t *test_struct, test_struct_t *other)
{
        if (NULL == test_struct || NULL == other)
        {
                return CONT_ARRAY_FAILURE;
        }
        init_test_struct(test_struct);
        set_test_struct_social_security(test_struct, other->social_security);
        set_test_struct_name(test_struct, other->name, other->name_buffer_size);
        test_struct->name_buffer_size = other->name_buffer_size;
        return CONT_ARRAY_SUCCESS;
}

test_struct_t *new_test_struct(void)
{
        test_struct_t *test_struct = malloc(sizeof(test_struct_t));
        if (NULL == test_struct)
        {
                return NULL;
        }
        if (CONT_ARRAY_SUCCESS != init_test_struct(test_struct))
        {
                free(test_struct);
                return NULL;
        }
        return test_struct;
}

char *get_test_struct_name(test_struct_t *test_struct)
{
        if (NULL == test_struct)
        {
                return NULL;
        }
        return test_struct->name;
}

cont_array_status_t set_test_struct_name(test_struct_t *test_struct, char *name, size_t size)
{
        if (NULL == test_struct || NULL == name)
        {
                return CONT_ARRAY_FAILURE;
        }
        if (size > test_struct->name_buffer_size)
        {
                const char *const old_name = test_struct->name;
                test_struct->name = realloc(test_struct->name, size);
                if (old_name == test_struct->name)
                {
                        return CONT_ARRAY_FAILURE;
                }
                test_struct->name_buffer_size = size;
        }
        strcpy(test_struct->name, name);
        test_struct->name[size - 1] = '\0';
        return CONT_ARRAY_SUCCESS;
}

uint32_t get_test_struct_social_security(test_struct_t *test_struct)
{
        if (NULL == test_struct)
        {
                return 0;
        }
        return test_struct->social_security;
}
cont_array_status_t set_test_struct_social_security(test_struct_t *test_struct, uint32_t social_security)
{
        if (NULL == test_struct)
        {
                return CONT_ARRAY_FAILURE;
        }
        test_struct->social_security = social_security;
        return CONT_ARRAY_SUCCESS;
}

cont_array_status_t deinit_test_struct(test_struct_t *test_struct)
{
        if (NULL == test_struct)
        {
                return CONT_ARRAY_FAILURE;
        }
        free(test_struct->name);
        memset(test_struct, 0, sizeof(test_struct_t));
        return CONT_ARRAY_SUCCESS;
}
cont_array_status_t destruct_test_struct(test_struct_t **test_struct)
{
        if (NULL == test_struct || NULL == *test_struct)
        {
                return CONT_ARRAY_FAILURE;
        }
        test_struct_t *_test_struct = *test_struct;
        free(_test_struct->name);
        free(_test_struct);
        *test_struct = NULL;
        return CONT_ARRAY_SUCCESS;
}

static OPERATOR_OF(test_struct_t) * get_operator_of_test_struct(void)
{
        static OPERATOR_OF(test_struct_t) operator= {.construct = new_test_struct,
                                                     .destruct = destruct_test_struct,
                                                     .copy = copy_test_struct,
                                                     .init = init_test_struct,
                                                     .deinit = deinit_test_struct};
        return &operator;
}

IMPL_ARRAY_OF(test_struct_t, get_operator_of_test_struct);
