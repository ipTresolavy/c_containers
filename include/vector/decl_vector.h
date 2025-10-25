#ifndef H_DECL_VECTOR
#define H_DECL_VECTOR

#include "operator.h"

#include <stdbool.h>
#include <stddef.h>

#define CONT_PRIV_VECTOR_PREFIX(T)            cont_##T##_vector
#define CONT_PRIV_VECTOR_OF(T)                CONT_PRIV_VECTOR_PREFIX(T)
#define CONT_PRIV_VECTOR_OPERATOR_OF(T)       CONT_CONCAT(CONT_PRIV_VECTOR_OF(T), _operator)
#define CONT_VECTOR_OF(T)                     CONT_CONCAT(CONT_PRIV_VECTOR_OF(T), _t)
#define CONT_VECTOR_OPERATOR_OF(T)            CONT_CONCAT(CONT_PRIV_VECTOR_OPERATOR_OF(T), _t)

#define CONT_GET_VECTOR_OPERATOR_OF(T)        cont_get_##T##_vector_operator

#define CONT_PRIV_DECL_VECTOR_OF(T)                                                                           \
        struct CONT_PRIV_VECTOR_OF(T);                                                                        \
        typedef struct CONT_PRIV_VECTOR_OF(T) CONT_VECTOR_OF(T)

#define CONT_PRIV_DECL_VECTOR_OPERATOR_OF(T)                                                                  \
        typedef struct CONT_PRIV_VECTOR_OPERATOR_OF(T)                                                        \
        {                                                                                                      \
                CONT_VECTOR_OF(T) *(*const construct)(size_t capacity, T *array_data, size_t array_data_size); \
                cont_array_status_t (*const destruct)(CONT_VECTOR_OF(T) **vector);                             \
                cont_array_status_t (*const assign)(CONT_VECTOR_OF(T) *vector, CONT_VECTOR_OF(T) *other);       \
                T *(*const at)(CONT_VECTOR_OF(T) *vector, size_t index);                                       \
                T *(*const index)(CONT_VECTOR_OF(T) *vector, size_t index);                                    \
                T *(*const front)(CONT_VECTOR_OF(T) *vector);                                                  \
                T *(*const back)(CONT_VECTOR_OF(T) *vector);                                                   \
                T *(*const data)(CONT_VECTOR_OF(T) *vector);                                                   \
                bool (*const empty)(CONT_VECTOR_OF(T) *vector);                                                \
                size_t (*const size)(CONT_VECTOR_OF(T) *vector);                                               \
                size_t (*const capacity)(CONT_VECTOR_OF(T) *vector);                                           \
                cont_array_status_t (*const reserve)(CONT_VECTOR_OF(T) *vector, size_t new_capacity);          \
                cont_array_status_t (*const shrink_to_fit)(CONT_VECTOR_OF(T) *vector);                         \
                cont_array_status_t (*const resize)(CONT_VECTOR_OF(T) *vector, size_t new_size);               \
                cont_array_status_t (*const push_back)(CONT_VECTOR_OF(T) *vector, T value);                    \
                cont_array_status_t (*const pop_back)(CONT_VECTOR_OF(T) *vector, T *out_value);                \
                cont_array_status_t (*const clear)(CONT_VECTOR_OF(T) *vector);                                 \
                cont_array_status_t (*const swap)(CONT_VECTOR_OF(T) *vector, CONT_VECTOR_OF(T) *other);         \
        } CONT_VECTOR_OPERATOR_OF(T)

#define CONT_PRIV_DECL_GET_VECTOR_OPERATOR_OF(T) CONT_VECTOR_OPERATOR_OF(T) *CONT_GET_VECTOR_OPERATOR_OF(T)(void)

#define CONT_DECL_VECTOR_OF(T)                                                                                 \
        CONT_PRIV_DECL_VECTOR_OF(T);                                                                           \
        CONT_PRIV_DECL_VECTOR_OPERATOR_OF(T);                                                                  \
        CONT_PRIV_DECL_GET_VECTOR_OPERATOR_OF(T)

#endif // !H_DECL_VECTOR
