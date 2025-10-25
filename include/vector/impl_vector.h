#ifndef H_IMPL_VECTOR
#define H_IMPL_VECTOR

#include "vector/decl_vector.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define CONT_PRIV_VECTOR_IMPL(T, op) CONT_CONCAT(CONT_CONCAT(CONT_PRIV_VECTOR_PREFIX(T), _), op)

#define CONT_PRIV_IMPL_OPERATOR_OF(T, T_operator_getter) \
        static inline CONT_OPERATOR_OF(T) *CONT_PRIV_GET_OPERATOR_OF(T)(void)                                          \
        {                                                                                                              \
                return (T_operator_getter)();                                                                          \
        }                                                                                                              \
        static inline CONT_OPERATOR_OF(T) *CONT_PRIV_IMPL(T, get_operator)(void)                                       \
        {                                                                                                              \
                return CONT_PRIV_GET_OPERATOR_OF(T)();                                                                 \
        }

#define CONT_PRIV_IMPL_VECTOR_OF(T) \
        struct CONT_PRIV_VECTOR_OF(T)                                                                                  \
        {                                                                                                              \
                size_t size;                                                                                           \
                size_t capacity;                                                                                       \
                T     *data;                                                                                           \
        };

#define CONT_PRIV_IMPL_VECTOR_HELPERS(T) \
        static inline bool CONT_PRIV_IMPL(T, allocation_would_overflow)(size_t count)                                  \
        {                                                                                                              \
                return (count > 0U) && (SIZE_MAX / sizeof(T) < count);                                                 \
        }                                                                                                              \
                                                                                                                       \
        static inline CONT_VECTOR_OF(T) *CONT_PRIV_IMPL(T, allocate_storage)(size_t capacity)                          \
        {                                                                                                              \
                CONT_VECTOR_OF(T) *vector = malloc(sizeof(CONT_VECTOR_OF(T)));                                         \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                vector->size     = 0U;                                                                                 \
                vector->capacity = 0U;                                                                                 \
                vector->data     = NULL;                                                                               \
                                                                                                                       \
                if(0U == capacity)                                                                                     \
                {                                                                                                      \
                        return vector;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                if(CONT_PRIV_IMPL(T, allocation_would_overflow)(capacity))                                             \
                {                                                                                                      \
                        free(vector);                                                                                  \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                vector->data = malloc(capacity * sizeof(T));                                                           \
                if(NULL == vector->data)                                                                               \
                {                                                                                                      \
                        free(vector);                                                                                  \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                vector->capacity = capacity;                                                                           \
                return vector;                                                                                         \
        }                                                                                                              \
                                                                                                                       \
        static inline void CONT_PRIV_IMPL(T, release_storage)(CONT_VECTOR_OF(T) *vector)                               \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return;                                                                                        \
                }                                                                                                      \
                free(vector->data);                                                                                    \
                free(vector);                                                                                          \
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
                for(size_t i = 0U; i < count; ++i)                                                                     \
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
                for(size_t i = 0U; i < count; ++i)                                                                     \
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
                if(NULL == dest || NULL == src)                                                                        \
                {                                                                                                      \
                        return CONT_ARRAY_FAILURE;                                                                     \
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
                                                                                                                       \
                if(NULL != copied)                                                                                     \
                {                                                                                                      \
                        *copied = i;                                                                                   \
                }                                                                                                      \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_VECTOR_STORAGE(T) \
        static inline cont_array_status_t CONT_PRIV_IMPL(T, set_capacity)(CONT_VECTOR_OF(T) *vector, size_t capacity) \
        {                                                                                                             \
                if(NULL == vector)                                                                                    \
                {                                                                                                     \
                        return CONT_ARRAY_IS_NULL;                                                                    \
                }                                                                                                     \
                                                                                                                      \
                if(capacity == vector->capacity)                                                                      \
                {                                                                                                     \
                        return CONT_ARRAY_SUCCESS;                                                                    \
                }                                                                                                     \
                                                                                                                      \
                if(capacity < vector->size)                                                                           \
                {                                                                                                     \
                        return CONT_ARRAY_SIZE_MISMATCH;                                                              \
                }                                                                                                     \
                                                                                                                      \
                if(CONT_PRIV_IMPL(T, allocation_would_overflow)(capacity))                                            \
                {                                                                                                     \
                        return CONT_ARRAY_FAILURE;                                                                    \
                }                                                                                                     \
                                                                                                                      \
                T *new_data = NULL;                                                                                   \
                if(capacity > 0U)                                                                                     \
                {                                                                                                     \
                        new_data = malloc(capacity * sizeof(T));                                                      \
                        if(NULL == new_data)                                                                          \
                        {                                                                                             \
                                return CONT_ARRAY_FAILURE;                                                            \
                        }                                                                                             \
                }                                                                                                     \
                                                                                                                      \
                size_t copied = 0U;                                                                                   \
                cont_array_status_t status =                                                                           \
                        CONT_PRIV_IMPL(T, copy_range)(new_data, vector->data, vector->size, &copied);                 \
                if(CONT_ARRAY_SUCCESS != status)                                                                      \
                {                                                                                                     \
                        CONT_PRIV_IMPL(T, force_deinit_range)(new_data, copied);                                      \
                        free(new_data);                                                                               \
                        return status;                                                                                \
                }                                                                                                     \
                                                                                                                      \
                cont_array_status_t cleanup_status =                                                                  \
                        CONT_PRIV_IMPL(T, try_deinit_range)(vector->data, vector->size);                              \
                if(CONT_ARRAY_SUCCESS != cleanup_status)                                                              \
                {                                                                                                     \
                        CONT_PRIV_IMPL(T, force_deinit_range)(new_data, copied);                                      \
                        free(new_data);                                                                               \
                        return cleanup_status;                                                                        \
                }                                                                                                     \
                                                                                                                      \
                free(vector->data);                                                                                   \
                vector->data     = new_data;                                                                          \
                vector->capacity = capacity;                                                                          \
                return CONT_ARRAY_SUCCESS;                                                                            \
        }                                                                                                             \
                                                                                                                      \
        static inline cont_array_status_t CONT_PRIV_IMPL(T, ensure_capacity)(CONT_VECTOR_OF(T) *vector,               \
                                                                             size_t              min_capacity)       \
        {                                                                                                             \
                if(NULL == vector)                                                                                    \
                {                                                                                                     \
                        return CONT_ARRAY_IS_NULL;                                                                    \
                }                                                                                                     \
                                                                                                                      \
                if(vector->capacity >= min_capacity)                                                                  \
                {                                                                                                     \
                        return CONT_ARRAY_SUCCESS;                                                                    \
                }                                                                                                     \
                                                                                                                      \
                size_t new_capacity = (vector->capacity == 0U) ? 1U : vector->capacity;                               \
                while(new_capacity < min_capacity)                                                                    \
                {                                                                                                     \
                        if(new_capacity > SIZE_MAX / 2U)                                                              \
                        {                                                                                             \
                                new_capacity = min_capacity;                                                          \
                                break;                                                                                \
                        }                                                                                             \
                        new_capacity *= 2U;                                                                           \
                }                                                                                                     \
                                                                                                                      \
                if(new_capacity < min_capacity)                                                                       \
                {                                                                                                     \
                        new_capacity = min_capacity;                                                                  \
                }                                                                                                     \
                                                                                                                      \
                return CONT_PRIV_IMPL(T, set_capacity)(vector, new_capacity);                                         \
        }

#define CONT_PRIV_IMPL_CONSTRUCT(T) \
        static CONT_VECTOR_OF(T) *CONT_PRIV_IMPL(T, construct)(size_t capacity, T *array_data,                          \
                                                               size_t array_data_size)                                 \
        {                                                                                                              \
                if(array_data_size > capacity)                                                                         \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                CONT_VECTOR_OF(T) *vector = CONT_PRIV_IMPL(T, allocate_storage)(capacity);                             \
                if(NULL == vector)                                                                                    \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                vector->capacity = capacity;                                                                          \
                vector->size     = 0U;                                                                                \
                                                                                                                       \
                if(NULL == array_data || 0U == array_data_size)                                                       \
                {                                                                                                      \
                        return vector;                                                                                \
                }                                                                                                      \
                                                                                                                       \
                size_t               copied = 0U;                                                                     \
                cont_array_status_t status =                                                                           \
                        CONT_PRIV_IMPL(T, copy_range)(vector->data, array_data, array_data_size, &copied);            \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        CONT_PRIV_IMPL(T, force_deinit_range)(vector->data, copied);                                   \
                        CONT_PRIV_IMPL(T, release_storage)(vector);                                                    \
                        return NULL;                                                                                   \
                }                                                                                                      \
                                                                                                                       \
                vector->size = array_data_size;                                                                        \
                return vector;                                                                                         \
        }

#define CONT_PRIV_IMPL_DESTRUCT(T) \
        static cont_array_status_t CONT_PRIV_IMPL(T, destruct)(CONT_VECTOR_OF(T) **vector)                              \
        {                                                                                                              \
                if(NULL == vector || NULL == *vector)                                                                  \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                CONT_VECTOR_OF(T) *instance = *vector;                                                                 \
                cont_array_status_t status   =                                                                         \
                        CONT_PRIV_IMPL(T, try_deinit_range)(instance->data, instance->size);                           \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        return status;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                CONT_PRIV_IMPL(T, release_storage)(instance);                                                          \
                *vector = NULL;                                                                                        \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_ASSIGN(T) \
        static cont_array_status_t CONT_PRIV_IMPL(T, assign)(CONT_VECTOR_OF(T) *vector,                                 \
                                                             CONT_VECTOR_OF(T) *other)                                 \
        {                                                                                                              \
                if(NULL == vector || NULL == other)                                                                    \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                if(vector == other)                                                                                    \
                {                                                                                                      \
                        return CONT_ARRAY_SUCCESS;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                cont_array_status_t status = CONT_PRIV_IMPL(T, ensure_capacity)(vector, other->size);                  \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        return status;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                status = CONT_PRIV_IMPL(T, clear)(vector);                                                             \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        return status;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                size_t copied = 0U;                                                                                   \
                status       =                                                                                         \
                        CONT_PRIV_IMPL(T, copy_range)(vector->data, other->data, other->size, &copied);               \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        CONT_PRIV_IMPL(T, force_deinit_range)(vector->data, copied);                                   \
                        vector->size = 0U;                                                                             \
                        return status;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                vector->size = other->size;                                                                            \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_AT(T) \
        static T *CONT_PRIV_IMPL(T, at)(CONT_VECTOR_OF(T) *vector, size_t index)                                       \
        {                                                                                                              \
                if(NULL == vector || index >= vector->size)                                                            \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return vector->data + index;                                                                           \
        }

#define CONT_PRIV_IMPL_INDEX(T) \
        static T *CONT_PRIV_IMPL(T, index)(CONT_VECTOR_OF(T) *vector, size_t index)                                    \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return vector->data + index;                                                                           \
        }

#define CONT_PRIV_IMPL_FRONT(T) \
        static T *CONT_PRIV_IMPL(T, front)(CONT_VECTOR_OF(T) *vector)                                                  \
        {                                                                                                              \
                if(NULL == vector || 0U == vector->size)                                                               \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return vector->data;                                                                                   \
        }

#define CONT_PRIV_IMPL_BACK(T) \
        static T *CONT_PRIV_IMPL(T, back)(CONT_VECTOR_OF(T) *vector)                                                   \
        {                                                                                                              \
                if(NULL == vector || 0U == vector->size)                                                               \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return vector->data + vector->size - 1U;                                                               \
        }

#define CONT_PRIV_IMPL_DATA(T) \
        static T *CONT_PRIV_IMPL(T, data)(CONT_VECTOR_OF(T) *vector)                                                   \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return NULL;                                                                                   \
                }                                                                                                      \
                return vector->data;                                                                                   \
        }

#define CONT_PRIV_IMPL_EMPTY(T) \
        static bool CONT_PRIV_IMPL(T, empty)(CONT_VECTOR_OF(T) *vector)                                                \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return false;                                                                                  \
                }                                                                                                      \
                return 0U == vector->size;                                                                             \
        }

#define CONT_PRIV_IMPL_SIZE(T) \
        static size_t CONT_PRIV_IMPL(T, size)(CONT_VECTOR_OF(T) *vector)                                               \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return 0U;                                                                                     \
                }                                                                                                      \
                return vector->size;                                                                                   \
        }

#define CONT_PRIV_IMPL_CAPACITY(T) \
        static size_t CONT_PRIV_IMPL(T, capacity)(CONT_VECTOR_OF(T) *vector)                                           \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return 0U;                                                                                     \
                }                                                                                                      \
                return vector->capacity;                                                                               \
        }

#define CONT_PRIV_IMPL_CLEAR(T) \
        static cont_array_status_t CONT_PRIV_IMPL(T, clear)(CONT_VECTOR_OF(T) *vector)                                 \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                cont_array_status_t status = CONT_PRIV_IMPL(T, try_deinit_range)(vector->data, vector->size);          \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        return status;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                vector->size = 0U;                                                                                    \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_RESERVE(T) \
        static cont_array_status_t CONT_PRIV_IMPL(T, reserve)(CONT_VECTOR_OF(T) *vector, size_t new_capacity)          \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                if(new_capacity <= vector->capacity)                                                                   \
                {                                                                                                      \
                        return CONT_ARRAY_SUCCESS;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                return CONT_PRIV_IMPL(T, set_capacity)(vector, new_capacity);                                          \
        }

#define CONT_PRIV_IMPL_SHRINK_TO_FIT(T) \
        static cont_array_status_t CONT_PRIV_IMPL(T, shrink_to_fit)(CONT_VECTOR_OF(T) *vector)                         \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                return CONT_PRIV_IMPL(T, set_capacity)(vector, vector->size);                                          \
        }

#define CONT_PRIV_IMPL_RESIZE(T) \
        static cont_array_status_t CONT_PRIV_IMPL(T, resize)(CONT_VECTOR_OF(T) *vector, size_t new_size)               \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                if(new_size == vector->size)                                                                           \
                {                                                                                                      \
                        return CONT_ARRAY_SUCCESS;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                if(new_size < vector->size)                                                                            \
                {                                                                                                      \
                        cont_array_status_t status =                                                                   \
                                CONT_PRIV_IMPL(T, try_deinit_range)(vector->data + new_size,                           \
                                                                    vector->size - new_size);                          \
                        if(CONT_ARRAY_SUCCESS != status)                                                               \
                        {                                                                                              \
                                return status;                                                                         \
                        }                                                                                              \
                                                                                                                       \
                        vector->size = new_size;                                                                       \
                        return CONT_ARRAY_SUCCESS;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                cont_array_status_t status =                                                                           \
                        CONT_PRIV_IMPL(T, ensure_capacity)(vector, new_size);                                          \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        return status;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                size_t initialized = 0U;                                                                               \
                status             = CONT_PRIV_IMPL(T, init_range)(vector->data + vector->size,                        \
                                                                   new_size - vector->size, &initialized);             \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        CONT_PRIV_IMPL(T, force_deinit_range)(vector->data + vector->size, initialized);               \
                        return status;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                vector->size = new_size;                                                                               \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_PUSH_BACK(T) \
        static cont_array_status_t CONT_PRIV_IMPL(T, push_back)(CONT_VECTOR_OF(T) *vector, T value)                    \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                cont_array_status_t status =                                                                           \
                        CONT_PRIV_IMPL(T, ensure_capacity)(vector, vector->size + 1U);                                 \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        return status;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                CONT_OPERATOR_OF(T) *operator= CONT_PRIV_IMPL(T, get_operator)();                                      \
                status                     = operator->copy(vector->data + vector->size, &value);                      \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        return status;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                vector->size += 1U;                                                                                   \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_POP_BACK(T) \
        static cont_array_status_t CONT_PRIV_IMPL(T, pop_back)(CONT_VECTOR_OF(T) *vector, T *out_value)                \
        {                                                                                                              \
                if(NULL == vector)                                                                                     \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                if(0U == vector->size)                                                                                \
                {                                                                                                      \
                        return CONT_ARRAY_FAILURE;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                CONT_OPERATOR_OF(T) *operator= CONT_PRIV_IMPL(T, get_operator)();                                      \
                T                   *element = vector->data + vector->size - 1U;                                       \
                                                                                                                       \
                if(NULL != out_value)                                                                                  \
                {                                                                                                      \
                        cont_array_status_t status = operator->copy(out_value, element);                               \
                        if(CONT_ARRAY_SUCCESS != status)                                                               \
                        {                                                                                              \
                                return status;                                                                         \
                        }                                                                                              \
                }                                                                                                      \
                                                                                                                       \
                cont_array_status_t status = operator->deinit(element);                                                \
                if(CONT_ARRAY_SUCCESS != status)                                                                       \
                {                                                                                                      \
                        return status;                                                                                 \
                }                                                                                                      \
                                                                                                                       \
                vector->size -= 1U;                                                                                   \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_SWAP(T) \
        static cont_array_status_t CONT_PRIV_IMPL(T, swap)(CONT_VECTOR_OF(T) *vector, CONT_VECTOR_OF(T) *other)        \
        {                                                                                                              \
                if(NULL == vector || NULL == other)                                                                    \
                {                                                                                                      \
                        return CONT_ARRAY_IS_NULL;                                                                     \
                }                                                                                                      \
                                                                                                                       \
                T     *tmp_data     = vector->data;                                                                    \
                size_t tmp_size     = vector->size;                                                                    \
                size_t tmp_capacity = vector->capacity;                                                                \
                                                                                                                       \
                vector->data     = other->data;                                                                        \
                vector->size     = other->size;                                                                        \
                vector->capacity = other->capacity;                                                                    \
                                                                                                                       \
                other->data     = tmp_data;                                                                            \
                other->size     = tmp_size;                                                                            \
                other->capacity = tmp_capacity;                                                                        \
                                                                                                                       \
                return CONT_ARRAY_SUCCESS;                                                                             \
        }

#define CONT_PRIV_IMPL_GET_VECTOR_OPERATOR_OF(T) \
        CONT_VECTOR_OPERATOR_OF(T) *CONT_GET_VECTOR_OPERATOR_OF(T)(void)                                               \
        {                                                                                                              \
                static CONT_VECTOR_OPERATOR_OF(T) operator= {                                                          \
                        .construct     = CONT_PRIV_IMPL(T, construct),                                                 \
                        .destruct      = CONT_PRIV_IMPL(T, destruct),                                                  \
                        .assign        = CONT_PRIV_IMPL(T, assign),                                                    \
                        .at            = CONT_PRIV_IMPL(T, at),                                                        \
                        .index         = CONT_PRIV_IMPL(T, index),                                                     \
                        .front         = CONT_PRIV_IMPL(T, front),                                                     \
                        .back          = CONT_PRIV_IMPL(T, back),                                                      \
                        .data          = CONT_PRIV_IMPL(T, data),                                                      \
                        .empty         = CONT_PRIV_IMPL(T, empty),                                                     \
                        .size          = CONT_PRIV_IMPL(T, size),                                                      \
                        .capacity      = CONT_PRIV_IMPL(T, capacity),                                                  \
                        .reserve       = CONT_PRIV_IMPL(T, reserve),                                                   \
                        .shrink_to_fit = CONT_PRIV_IMPL(T, shrink_to_fit),                                             \
                        .resize        = CONT_PRIV_IMPL(T, resize),                                                    \
                        .push_back     = CONT_PRIV_IMPL(T, push_back),                                                 \
                        .pop_back      = CONT_PRIV_IMPL(T, pop_back),                                                  \
                        .clear         = CONT_PRIV_IMPL(T, clear),                                                     \
                        .swap          = CONT_PRIV_IMPL(T, swap)                                                       \
                };                                                                                                     \
                return &operator;                                                                                      \
        }

#define CONT_PRIV_IMPL_OPS(T) \
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
        CONT_PRIV_IMPL_CAPACITY(T)                                                                                     \
        CONT_PRIV_IMPL_CLEAR(T)                                                                                        \
        CONT_PRIV_IMPL_RESERVE(T)                                                                                      \
        CONT_PRIV_IMPL_SHRINK_TO_FIT(T)                                                                                \
        CONT_PRIV_IMPL_RESIZE(T)                                                                                       \
        CONT_PRIV_IMPL_PUSH_BACK(T)                                                                                    \
        CONT_PRIV_IMPL_POP_BACK(T)                                                                                     \
        CONT_PRIV_IMPL_SWAP(T)

#define IMPL_VECTOR_OF(T, T_operator_getter) \
        CONT_PRIV_IMPL_OPERATOR_OF(T, T_operator_getter)                                                               \
        CONT_PRIV_IMPL_VECTOR_OF(T)                                                                                    \
        CONT_PRIV_IMPL_VECTOR_HELPERS(T)                                                                               \
        CONT_PRIV_IMPL_VECTOR_STORAGE(T)                                                                               \
        CONT_PRIV_IMPL_OPS(T)                                                                                          \
        CONT_PRIV_IMPL_GET_VECTOR_OPERATOR_OF(T)

#endif // !H_IMPL_VECTOR
