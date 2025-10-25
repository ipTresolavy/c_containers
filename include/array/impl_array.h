#ifndef H_IMPL_ARRAY
#define H_IMPL_ARRAY

#include "array/decl_array.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define CONT_PRIV_IMPL(T, op) CONT_CONCAT(CONT_CONCAT(CONT_PRIV_PREFIX(T), _), op)

#define CONT_PRIV_IMPL_OPERATOR_OF(T, T_operator_getter)                                                               \
        static inline CONT_OPERATOR_OF(T) *CONT_PRIV_GET_OPERATOR_OF(T)(void)                                          \
        {                                                                                                              \
                return (T_operator_getter)();                                                                          \
        }                                                                                                              \
        static inline CONT_OPERATOR_OF(T) *CONT_PRIV_IMPL(T, get_operator)(void)                                       \
        {                                                                                                              \
                return CONT_PRIV_GET_OPERATOR_OF(T)();                                                                 \
        }

#define CONT_PRIV_IMPL_ARRAY_OF(T)                                                                                     \
        struct CONT_PRIV_ARRAY_OF(T)                                                                                   \
        {                                                                                                              \
                size_t size;                                                                                           \
                T     *data;                                                                                           \
        };

#define CONT_PRIV_IMPL_ARRAY_HELPERS(T)                                                                                \
        static inline CONT_ARRAY_OF(T) *CONT_PRIV_IMPL(T, allocate_storage)(size_t size)                               \
        {                                                                                                              \
                CONT_ARRAY_OF(T) *array = malloc(sizeof(CONT_ARRAY_OF(T)));                                            \
                if(NULL == array)                                                                                      \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                if(0U == size)                                                                                         \
                {                                                                                                      \
                        array->data = NULL;                                                                            \
                        array->size = 0U;                                                                              \
                        return array;                                                                                  \
                }                                                                                                      \
                                                                                                                       \
                array->data = malloc(size * sizeof(T));                                                                \
                if(NULL == array->data)                                                                                \
                {                                                                                                      \
                        free(array);                                                                                   \
                        return NULL;                                                                                   \
                }                                                                                                      \
                array->size = size;                                                                                    \
                return array;                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        static inline void CONT_PRIV_IMPL(T, release_storage)(CONT_ARRAY_OF(T) *array)                                 \
        {                                                                                                              \
                if(NULL == array)                                                                                      \
                {                                                                                                      \
                        return;                                                                                        \
                }                                                                                                      \
                free(array->data);                                                                                     \
                free(array);                                                                                           \
        }                                                                                                              \
                                                                                                                       \
        static inline void CONT_PRIV_IMPL(T, force_deinit_range)(T *data, size_t count)                                \
        {                                                                                                              \
                if(NULL == data || 0U == count)                                                                        \
                {                                                                                                      \
                        return;                                                                                        \
                }                                                                                                      \
                                                                                                                       \
                CONT_OPERATOR_OF(T) *operator= CONT_PRIV_IMPL(T, get_operator)();                                      \
                for(size_t i = 0; i < count; ++i)                                                                      \
                {                                                                                                      \
                        operator->deinit(data + i);                                                                    \
                }                                                                                                      \
        }                                                                                                              \
                                                                                                                       \
        static inline cont_array_status_t CONT_PRIV_IMPL(T, try_deinit_range)(T *data, size_t count)                   \
        {                                                                                                              \
                if(NULL == data || 0U == count)                                                                        \
                {                                                                                                      \
                        return CONT_ARRAY_SUCCESS;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                CONT_OPERATOR_OF(T) *operator= CONT_PRIV_IMPL(T, get_operator)();                                      \
                for(size_t i = 0; i < count; ++i)                                                                      \
                {                                                                                                      \
                        cont_array_status_t status = operator->deinit(data + i);                                       \
                        if(CONT_ARRAY_SUCCESS != status)                                                               \
                        {                                                                                              \
                                return status;                                                                         \
                        }                                                                                              \
                }                                                                                                      \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }                                                                                                              \
                                                                                                                       \
        static inline cont_array_status_t CONT_PRIV_IMPL(T, init_range)(T *data, size_t count, size_t *initialized)    \
        {                                                                                                              \
                if(NULL == data || 0U == count)                                                                        \
                {                                                                                                      \
                        if(NULL != initialized)                                                                        \
                        {                                                                                              \
                                *initialized = 0U;                                                                     \
                        }                                                                                              \
                        return CONT_ARRAY_SUCCESS;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                CONT_OPERATOR_OF(T) *operator= CONT_PRIV_IMPL(T, get_operator)();                                      \
                size_t                  i        = 0U;                                                                 \
                for(; i < count; ++i)                                                                                  \
                {                                                                                                      \
                        cont_array_status_t status = operator->init(data + i);                                         \
                        if(CONT_ARRAY_SUCCESS != status)                                                               \
                        {                                                                                              \
                                if(NULL != initialized)                                                                \
                                {                                                                                      \
                                        *initialized = i;                                                              \
                                }                                                                                      \
                                return status;                                                                         \
                        }                                                                                              \
                }                                                                                                      \
                                                                                                                       \
                if(NULL != initialized)                                                                                \
                {                                                                                                      \
                        *initialized = i;                                                                              \
                }                                                                                                      \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }                                                                                                              \
                                                                                                                       \
        static inline cont_array_status_t CONT_PRIV_IMPL(T, copy_range)(T *dest, T *src, size_t count, size_t *copied) \
        {                                                                                                              \
                if(0U == count)                                                                                        \
                {                                                                                                      \
                        if(NULL != copied)                                                                             \
                        {                                                                                              \
                                *copied = 0U;                                                                          \
                        }                                                                                              \
                        return CONT_ARRAY_SUCCESS;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                CONT_OPERATOR_OF(T) *operator= CONT_PRIV_IMPL(T, get_operator)();                                      \
                size_t                  i        = 0U;                                                                 \
                for(; i < count; ++i)                                                                                  \
                {                                                                                                      \
                        cont_array_status_t status = operator->copy(dest + i, src + i);                                \
                        if(CONT_ARRAY_SUCCESS != status)                                                               \
                        {                                                                                              \
                                if(NULL != copied)                                                                     \
                                {                                                                                      \
                                        *copied = i;                                                                   \
                                }                                                                                      \
                                return status;                                                                         \
                        }                                                                                              \
                }                                                                                                      \
                if(NULL != copied)                                                                                     \
                {                                                                                                      \
                        *copied = i;                                                                                   \
                }                                                                                                      \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_CONSTRUCT_EMPTY(T)                                                                              \
        static CONT_ARRAY_OF(T) *CONT_PRIV_IMPL(T, construct_empty)(size_t size)                                       \
        {                                                                                                              \
                CONT_ARRAY_OF(T) *array = CONT_PRIV_IMPL(T, allocate_storage)(size);                                   \
                if(NULL == array)                                                                                      \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                size_t               initialized = 0U;                                                                 \
                cont_array_status_t status       = CONT_PRIV_IMPL(T, init_range)(array->data, size, &initialized);     \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        CONT_PRIV_IMPL(T, force_deinit_range)(array->data, initialized);                               \
                        CONT_PRIV_IMPL(T, release_storage)(array);                                                     \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                array->size = size;                                                                                    \
                return array;                                                                                          \
        }

#define CONT_PRIV_IMPL_CONSTRUCT_INITIALIZED(T)                                                                        \
        static CONT_ARRAY_OF(T) *CONT_PRIV_IMPL(T, construct_initialized)(size_t size, T *array_data,                  \
                                                                         size_t array_data_size)                       \
        {                                                                                                              \
                if(NULL == array_data)                                                                                 \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                CONT_ARRAY_OF(T) *array = CONT_PRIV_IMPL(T, allocate_storage)(size);                                   \
                if(NULL == array)                                                                                      \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                size_t               copied  = 0U;                                                                     \
                cont_array_status_t status =                                                                           \
                        CONT_PRIV_IMPL(T, copy_range)(array->data, array_data, array_data_size, &copied);             \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        CONT_PRIV_IMPL(T, force_deinit_range)(array->data, copied);                                    \
                        CONT_PRIV_IMPL(T, release_storage)(array);                                                     \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                size_t initialized = 0U;                                                                               \
                status             = CONT_PRIV_IMPL(T, init_range)(                                                    \
                        array->data + array_data_size, size - array_data_size, &initialized                            \
                );                                                                                                     \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        CONT_PRIV_IMPL(T, force_deinit_range)(array->data + array_data_size, initialized);             \
                        CONT_PRIV_IMPL(T, force_deinit_range)(array->data, array_data_size);                           \
                        CONT_PRIV_IMPL(T, release_storage)(array);                                                     \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                array->size = size;                                                                                    \
                return array;                                                                                          \
        }

#define CONT_PRIV_IMPL_CONSTRUCT(T)                                                                                    \
        static CONT_ARRAY_OF(T) *CONT_PRIV_IMPL(T, construct)(size_t size, T *array_data, size_t array_data_size)      \
        {                                                                                                              \
                if(NULL != array_data)                                                                                 \
                {                                                                                                      \
                        if(array_data_size > size)                                                                     \
                        {                                                                                              \
                                return NULL;                                                                           \
                        }                                                                                              \
                        return CONT_PRIV_IMPL(T, construct_initialized)(size, array_data, array_data_size);            \
                }                                                                                                      \
                                                                                                                       \
                return CONT_PRIV_IMPL(T, construct_empty)(size);                                                       \
        }

#define CONT_PRIV_IMPL_DESTRUCT(T)                                                                                     \
        static cont_array_status_t CONT_PRIV_IMPL(T, destruct)(CONT_ARRAY_OF(T) **array)                               \
        {                                                                                                              \
                if(NULL == array || NULL == *array)                                                                    \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                CONT_ARRAY_OF(T) *instance = *array;                                                                   \
                cont_array_status_t status   =                                                                         \
                        CONT_PRIV_IMPL(T, try_deinit_range)(instance->data, instance->size);                           \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        return status;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                CONT_PRIV_IMPL(T, release_storage)(instance);                                                          \
                *array = NULL;                                                                                         \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_ASSIGN(T)                                                                                       \
        static cont_array_status_t CONT_PRIV_IMPL(T, assign)(CONT_ARRAY_OF(T) *array, CONT_ARRAY_OF(T) *other)         \
        {                                                                                                              \
                if(NULL == array || NULL == other)                                                                     \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                if(array->size != other->size)                                                                         \
                {                                                                                                      \
                        return CONT_ARRAY_SIZE_MISMATCH;                                                               \
                }                                                                                                      \
                                                                                                                       \
                return CONT_PRIV_IMPL(T, copy_range)(array->data, other->data, array->size, NULL);                     \
        }

#define CONT_PRIV_IMPL_AT(T)                                                                                           \
        static T *CONT_PRIV_IMPL(T, at)(CONT_ARRAY_OF(T) *array, size_t index)                                         \
        {                                                                                                              \
                if(NULL == array || index >= array->size)                                                              \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return array->data + index;                                                                            \
        }

#define CONT_PRIV_IMPL_INDEX(T)                                                                                        \
        static T *CONT_PRIV_IMPL(T, index)(CONT_ARRAY_OF(T) *array, size_t index)                                      \
        {                                                                                                              \
                if(NULL == array)                                                                                      \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return array->data + index;                                                                            \
        }

#define CONT_PRIV_IMPL_FRONT(T)                                                                                        \
        static T *CONT_PRIV_IMPL(T, front)(CONT_ARRAY_OF(T) *array)                                                    \
        {                                                                                                              \
                if(NULL == array || 0U == array->size)                                                                 \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return array->data;                                                                                    \
        }

#define CONT_PRIV_IMPL_BACK(T)                                                                                         \
        static T *CONT_PRIV_IMPL(T, back)(CONT_ARRAY_OF(T) *array)                                                     \
        {                                                                                                              \
                if(NULL == array || 0U == array->size)                                                                 \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return array->data + array->size - 1U;                                                                 \
        }

#define CONT_PRIV_IMPL_DATA(T)                                                                                         \
        static T *CONT_PRIV_IMPL(T, data)(CONT_ARRAY_OF(T) *array)                                                     \
        {                                                                                                              \
                if(NULL == array)                                                                                      \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return array->data;                                                                                    \
        }

#define CONT_PRIV_IMPL_EMPTY(T)                                                                                        \
        static bool CONT_PRIV_IMPL(T, empty)(CONT_ARRAY_OF(T) *array)                                                  \
        {                                                                                                              \
                if(NULL == array)                                                                                      \
                {                                                                                                      \
                        return false;                                                                                  \
                }                                                                                                      \
                return array->size == 0U;                                                                              \
        }

#define CONT_PRIV_IMPL_SIZE(T)                                                                                         \
        static size_t CONT_PRIV_IMPL(T, size)(CONT_ARRAY_OF(T) *array)                                                 \
        {                                                                                                              \
                if(NULL == array)                                                                                      \
                {                                                                                                      \
                        return 0U;                                                                                     \
                }                                                                                                      \
                return array->size;                                                                                    \
        }

#define CONT_PRIV_IMPL_MAX_SIZE(T)                                                                                     \
        static size_t CONT_PRIV_IMPL(T, max_size)(CONT_ARRAY_OF(T) *array)                                             \
        {                                                                                                              \
                return CONT_PRIV_IMPL(T, size)(array);                                                                 \
        }

#define CONT_PRIV_IMPL_FILL(T)                                                                                         \
        static cont_array_status_t CONT_PRIV_IMPL(T, fill)(CONT_ARRAY_OF(T) *array, T value)                           \
        {                                                                                                              \
                if(NULL == array)                                                                                      \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                for(size_t i = 0; i < array->size; ++i)                                                                \
                {                                                                                                      \
                        array->data[i] = value;                                                                        \
                }                                                                                                      \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_SWAP(T)                                                                                         \
        static cont_array_status_t CONT_PRIV_IMPL(T, swap)(CONT_ARRAY_OF(T) *array, CONT_ARRAY_OF(T) *other)           \
        {                                                                                                              \
                if(NULL == array || NULL == other)                                                                     \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                if(array->size != other->size)                                                                         \
                {                                                                                                      \
                        return CONT_ARRAY_SIZE_MISMATCH;                                                               \
                }                                                                                                      \
                                                                                                                       \
                T *temp     = array->data;                                                                             \
                array->data = other->data;                                                                             \
                other->data = temp;                                                                                    \
                                                                                                                       \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_GET_ARRAY_OPERATOR_OF(T)                                                                        \
        CONT_ARRAY_OPERATOR_OF(T) *CONT_GET_ARRAY_OPERATOR_OF(T)(void)                                                 \
        {                                                                                                              \
                static CONT_ARRAY_OPERATOR_OF(T) operator= {                                                           \
                        .construct = CONT_PRIV_IMPL(T, construct),                                                     \
                        .destruct  = CONT_PRIV_IMPL(T, destruct),                                                      \
                        .assign    = CONT_PRIV_IMPL(T, assign),                                                        \
                        .at        = CONT_PRIV_IMPL(T, at),                                                            \
                        .index     = CONT_PRIV_IMPL(T, index),                                                         \
                        .front     = CONT_PRIV_IMPL(T, front),                                                         \
                        .back      = CONT_PRIV_IMPL(T, back),                                                          \
                        .data      = CONT_PRIV_IMPL(T, data),                                                          \
                        .empty     = CONT_PRIV_IMPL(T, empty),                                                         \
                        .size      = CONT_PRIV_IMPL(T, size),                                                          \
                        .max_size  = CONT_PRIV_IMPL(T, max_size),                                                      \
                        .fill      = CONT_PRIV_IMPL(T, fill),                                                          \
                        .swap      = CONT_PRIV_IMPL(T, swap)                                                           \
                };                                                                                                     \
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
        CONT_PRIV_IMPL_ARRAY_HELPERS(T)                                                                                \
        CONT_PRIV_IMPL_OPS(T)                                                                                          \
        CONT_PRIV_IMPL_GET_ARRAY_OPERATOR_OF(T)

#endif // !H_IMPL_ARRAY
