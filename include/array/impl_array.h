#ifndef H_IMPL_ARRAY
#define H_IMPL_ARRAY
#include "array/decl_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONT_PRIV_IMPL(T, op) CONT_CONCAT(CONT_CONCAT(CONT_PRIV_PREFIX(T), _), op)

#define CONT_PRIV_IMPL_OPERATOR_OF(T, T_operator_getter)                                                               \
        CONT_OPERATOR_OF(T) * (*const CONT_PRIV_GET_OPERATOR_OF(T))(void) = T_operator_getter;

#define CONT_PRIV_IMPL_ARRAY_OF(T)                                                                                     \
        struct CONT_PRIV_ARRAY_OF(T)                                                                                   \
        {                                                                                                              \
                size_t size;                                                                                           \
                T *data;                                                                                               \
        };

#define CONT_PRIV_IMPL_CONSTRUCT_EMPTY(T)                                                                              \
        static CONT_ARRAY_OF(T) * CONT_PRIV_IMPL(T, construct_empty)(size_t size)                                      \
        {                                                                                                              \
                CONT_ARRAY_OF(T) *array = malloc(sizeof(CONT_ARRAY_OF(T)));                                            \
                if (NULL == array)                                                                                     \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                array->data = malloc(size * sizeof(T));                                                                \
                if (NULL == array->data)                                                                               \
                {                                                                                                      \
                        free(array);                                                                                   \
                        return NULL;                                                                                   \
                }                                                                                                      \
                T *const data = array->data;                                                                           \
                CONT_OPERATOR_OF(T) *operator= CONT_PRIV_GET_OPERATOR_OF(T)();                                         \
                for (size_t i = 0; i < size; ++i)                                                                      \
                {                                                                                                      \
                        cont_array_status_t status = operator->init(data + i);                                         \
                        if (CONT_ARRAY_SUCCESS != status)                                                              \
                        {                                                                                              \
                                free(array->data);                                                                     \
                                free(array);                                                                           \
                                return NULL;                                                                           \
                        }                                                                                              \
                }                                                                                                      \
                array->size = size;                                                                                    \
                                                                                                                       \
                return array;                                                                                          \
        }

#define CONT_PRIV_IMPL_CONSTRUCT_INITIALIZED(T)                                                                        \
        static CONT_ARRAY_OF(T) *                                                                                      \
            CONT_PRIV_IMPL(T, construct_initialized)(size_t size, T * array_data, size_t array_data_size)              \
        {                                                                                                              \
                if (NULL == array_data)                                                                                \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                CONT_ARRAY_OF(T) *array = malloc(sizeof(CONT_ARRAY_OF(T)));                                            \
                if (NULL == array)                                                                                     \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                array->data = malloc(size * sizeof(T));                                                                \
                if (NULL == array->data)                                                                               \
                {                                                                                                      \
                        free(array);                                                                                   \
                        return NULL;                                                                                   \
                }                                                                                                      \
                T *const data = array->data;                                                                           \
                CONT_OPERATOR_OF(T) *operator= CONT_PRIV_GET_OPERATOR_OF(T)();                                         \
                for (size_t i = 0; i < array_data_size; ++i)                                                           \
                {                                                                                                      \
                        cont_array_status_t status = operator->copy(data + i, array_data + i);                         \
                        if (CONT_ARRAY_SUCCESS != status)                                                              \
                        {                                                                                              \
                                free(array->data);                                                                     \
                                free(array);                                                                           \
                                return NULL;                                                                           \
                        }                                                                                              \
                }                                                                                                      \
                for (size_t i = array_data_size; i < size; ++i)                                                        \
                {                                                                                                      \
                        cont_array_status_t status = operator->init(data + i);                                         \
                        if (CONT_ARRAY_SUCCESS != status)                                                              \
                        {                                                                                              \
                                free(array->data);                                                                     \
                                free(array);                                                                           \
                                return NULL;                                                                           \
                        }                                                                                              \
                }                                                                                                      \
                array->size = size;                                                                                    \
                return array;                                                                                          \
        }

#define CONT_PRIV_IMPL_CONSTRUCT(T)                                                                                    \
        static CONT_ARRAY_OF(T) * CONT_PRIV_IMPL(T, construct)(size_t size, T * array_data, size_t array_data_size)    \
        {                                                                                                              \
                if (NULL != array_data)                                                                                \
                {                                                                                                      \
                        if (array_data_size > size)                                                                    \
                        {                                                                                              \
                                return NULL;                                                                           \
                        }                                                                                              \
                        return CONT_PRIV_IMPL(T, construct_initialized)(size, array_data, array_data_size);            \
                }                                                                                                      \
                                                                                                                       \
                return CONT_PRIV_IMPL(T, construct_empty)(size);                                                       \
        }

#define CONT_PRIV_IMPL_DESTRUCT(T)                                                                                     \
        static cont_array_status_t CONT_PRIV_IMPL(T, destruct)(CONT_ARRAY_OF(T) * *array)                              \
        {                                                                                                              \
                if (NULL == array || NULL == *array)                                                                   \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                CONT_ARRAY_OF(T) *_array = *array;                                                                     \
                                                                                                                       \
                const size_t size = _array->size;                                                                      \
                T *const data = _array->data;                                                                          \
                CONT_OPERATOR_OF(T) *operator= CONT_PRIV_GET_OPERATOR_OF(T)();                                         \
                for (size_t i = 0; i < size; ++i)                                                                      \
                {                                                                                                      \
                        cont_array_status_t status = operator->deinit(data + i);                                       \
                        if (CONT_ARRAY_SUCCESS != status)                                                              \
                        {                                                                                              \
                                return status;                                                                         \
                        }                                                                                              \
                }                                                                                                      \
                free(_array->data);                                                                                    \
                free(_array);                                                                                          \
                *array = NULL;                                                                                         \
                                                                                                                       \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_ASSIGN(T)                                                                                       \
        static cont_array_status_t CONT_PRIV_IMPL(T, assign)(CONT_ARRAY_OF(T) * array, CONT_ARRAY_OF(T) * other)       \
        {                                                                                                              \
                if (NULL == array || NULL == other)                                                                    \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                const size_t size = array->size;                                                                       \
                if (size != other->size)                                                                               \
                {                                                                                                      \
                        return CONT_ARRAY_SIZE_MISMATCH;                                                               \
                }                                                                                                      \
                                                                                                                       \
                T *const data = array->data;                                                                           \
                T *const other_data = other->data;                                                                     \
                CONT_OPERATOR_OF(T) *operator= CONT_PRIV_GET_OPERATOR_OF(T)();                                         \
                for (size_t i = 0; i < size; ++i)                                                                      \
                {                                                                                                      \
                        cont_array_status_t status = operator->copy(data + i, other_data + i);                         \
                        if (CONT_ARRAY_SUCCESS != status)                                                              \
                        {                                                                                              \
                                return status;                                                                         \
                        }                                                                                              \
                }                                                                                                      \
                                                                                                                       \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_AT(T)                                                                                           \
        static T *CONT_PRIV_IMPL(T, at)(CONT_ARRAY_OF(T) * array, size_t index)                                        \
        {                                                                                                              \
                if (NULL == array || index >= array->size)                                                             \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return array->data + index;                                                                            \
        }

#define CONT_PRIV_IMPL_INDEX(T)                                                                                        \
        static T *CONT_PRIV_IMPL(T, index)(CONT_ARRAY_OF(T) * array, size_t index)                                     \
        {                                                                                                              \
                if (NULL == array)                                                                                     \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return array->data + index;                                                                            \
        }

#define CONT_PRIV_IMPL_FRONT(T)                                                                                        \
        static T *CONT_PRIV_IMPL(T, front)(CONT_ARRAY_OF(T) * array)                                                   \
        {                                                                                                              \
                if (NULL == array)                                                                                     \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return array->data;                                                                                    \
        }

#define CONT_PRIV_IMPL_BACK(T)                                                                                         \
        static T *CONT_PRIV_IMPL(T, back)(CONT_ARRAY_OF(T) * array)                                                    \
        {                                                                                                              \
                if (NULL == array)                                                                                     \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return array->data + array->size - 1;                                                                  \
        }

#define CONT_PRIV_IMPL_DATA(T)                                                                                         \
        static T *CONT_PRIV_IMPL(T, data)(CONT_ARRAY_OF(T) * array)                                                    \
        {                                                                                                              \
                if (NULL == array)                                                                                     \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return array->data;                                                                                    \
        }

#define CONT_PRIV_IMPL_EMPTY(T)                                                                                        \
        static bool CONT_PRIV_IMPL(T, empty)(CONT_ARRAY_OF(T) * array)                                                 \
        {                                                                                                              \
                if (NULL == array)                                                                                     \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return array->size == 0;                                                                               \
        }

#define CONT_PRIV_IMPL_SIZE(T)                                                                                         \
        static size_t CONT_PRIV_IMPL(T, size)(CONT_ARRAY_OF(T) * array)                                                \
        {                                                                                                              \
                if (NULL == array)                                                                                     \
                {                                                                                                      \
                        return 0;                                                                                      \
                }                                                                                                      \
                return array->size;                                                                                    \
        }

#define CONT_PRIV_IMPL_MAX_SIZE(T)                                                                                     \
        static size_t CONT_PRIV_IMPL(T, max_size)(CONT_ARRAY_OF(T) * array)                                            \
        {                                                                                                              \
                return CONT_PRIV_IMPL(T, size)(array);                                                                 \
        }

#define CONT_PRIV_IMPL_FILL(T)                                                                                         \
        static cont_array_status_t CONT_PRIV_IMPL(T, fill)(CONT_ARRAY_OF(T) * array, T value)                          \
        {                                                                                                              \
                if (NULL == array)                                                                                     \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                const size_t size = array->size;                                                                       \
                T *const data = array->data;                                                                           \
                for (T *data_i = data; data_i < data + size; ++data_i)                                                 \
                {                                                                                                      \
                        *data_i = value;                                                                               \
                }                                                                                                      \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_SWAP(T)                                                                                         \
        static cont_array_status_t CONT_PRIV_IMPL(T, swap)(CONT_ARRAY_OF(T) * array, CONT_ARRAY_OF(T) * other)         \
        {                                                                                                              \
                if (NULL == array || NULL == other)                                                                    \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                if (array->size != other->size)                                                                        \
                {                                                                                                      \
                        return CONT_ARRAY_SIZE_MISMATCH;                                                               \
                }                                                                                                      \
                                                                                                                       \
                T *temp = array->data;                                                                                 \
                array->data = other->data;                                                                             \
                other->data = temp;                                                                                    \
                                                                                                                       \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_GET_ARRAY_OPERATOR_OF(T)                                                                        \
        CONT_ARRAY_OPERATOR_OF(T) * CONT_GET_ARRAY_OPERATOR_OF(T)(void)                                                \
        {                                                                                                              \
                static CONT_ARRAY_OPERATOR_OF(T) operator= {.construct = CONT_PRIV_IMPL(T, construct),                 \
                                                            .destruct = CONT_PRIV_IMPL(T, destruct),                   \
                                                            .assign = CONT_PRIV_IMPL(T, assign),                       \
                                                            .at = CONT_PRIV_IMPL(T, at),                               \
                                                            .index = CONT_PRIV_IMPL(T, index),                         \
                                                            .front = CONT_PRIV_IMPL(T, front),                         \
                                                            .back = CONT_PRIV_IMPL(T, back),                           \
                                                            .data = CONT_PRIV_IMPL(T, data),                           \
                                                            .empty = CONT_PRIV_IMPL(T, empty),                         \
                                                            .size = CONT_PRIV_IMPL(T, size),                           \
                                                            .max_size = CONT_PRIV_IMPL(T, max_size),                   \
                                                            .fill = CONT_PRIV_IMPL(T, fill),                           \
                                                            .swap = CONT_PRIV_IMPL(T, swap)};                          \
                return &operator;                                                                                      \
        }

#define CONT_PRIV_IMPL_OPS(T)                                                                                          \
        CONT_PRIV_IMPL_CONSTRUCT_EMPTY(T)                                                                              \
        CONT_PRIV_IMPL_CONSTRUCT_INITIALIZED(T)                                                                        \
        CONT_PRIV_IMPL_CONSTRUCT(T)                                                                                    \
        CONT_PRIV_IMPL_DESTRUCT(T)                                                                                     \
        CONT_PRIV_IMPL_ASSIGN(T)                                                                                       \
        CONT_PRIV_IMPL_AT(T)                                                                                           \
        CONT_PRIV_IMPL_INDEX(T)                                                                                        \
        CONT_PRIV_IMPL_FRONT(T)                                                                                        \
        CONT_PRIV_IMPL_BACK(T)                                                                                         \
        CONT_PRIV_IMPL_DATA(T)                                                                                         \
        CONT_PRIV_IMPL_EMPTY(T)                                                                                        \
        CONT_PRIV_IMPL_SIZE(T)                                                                                         \
        CONT_PRIV_IMPL_MAX_SIZE(T)                                                                                     \
        CONT_PRIV_IMPL_FILL(T)                                                                                         \
        CONT_PRIV_IMPL_SWAP(T)

#define IMPL_ARRAY_OF(T, T_operator_getter)                                                                            \
        CONT_PRIV_IMPL_OPERATOR_OF(T, T_operator_getter)                                                               \
        CONT_PRIV_IMPL_ARRAY_OF(T)                                                                                     \
        CONT_PRIV_IMPL_OPS(T)                                                                                          \
        CONT_PRIV_IMPL_GET_ARRAY_OPERATOR_OF(T)

#endif // !H_IMPL_ARRAY
