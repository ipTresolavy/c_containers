#ifndef H_DECL_ARRAY
#define H_DECL_ARRAY

#include "operator.h"

#include <stdbool.h>
#include <stddef.h>

#define CONT_PRIV_PREFIX(T)            cont_##T##_array
#define CONT_PRIV_ARRAY_OF(T)          CONT_PRIV_PREFIX(T)
#define CONT_PRIV_ARRAY_OPERATOR_OF(T) CONT_CONCAT(CONT_PRIV_ARRAY_OF(T), _operator)
#define CONT_ARRAY_OF(T)               CONT_CONCAT(CONT_PRIV_ARRAY_OF(T), _t)
#define CONT_ARRAY_OPERATOR_OF(T)      CONT_CONCAT(CONT_PRIV_ARRAY_OPERATOR_OF(T), _t)

#define CONT_GET_ARRAY_OPERATOR_OF(T)  cont_get_##T##_array_operator

#define CONT_PRIV_DECL_ARRAY_OF(T)                                                                                     \
        struct CONT_PRIV_ARRAY_OF(T);                                                                                  \
        typedef struct CONT_PRIV_ARRAY_OF(T) CONT_ARRAY_OF(T);

#define CONT_PRIV_DECL_ARRAY_OPERATOR_OF(T)                                                                            \
        typedef struct CONT_PRIV_ARRAY_OPERATOR_OF(T)                                                                  \
        {                                                                                                              \
                CONT_ARRAY_OF(T) *(*const construct)(size_t size, T *array_data, size_t array_data_size);              \
                cont_array_status_t (*const destruct)(CONT_ARRAY_OF(T) * * array);                                     \
                cont_array_status_t (*const assign)(CONT_ARRAY_OF(T) *array, CONT_ARRAY_OF(T) *other);                 \
                T *(*const at)(CONT_ARRAY_OF(T) *array, size_t index);                                                 \
                T *(*const index)(CONT_ARRAY_OF(T) *array, size_t index);                                              \
                T *(*const front)(CONT_ARRAY_OF(T) *array);                                                            \
                T *(*const back)(CONT_ARRAY_OF(T) *array);                                                             \
                T *(*const data)(CONT_ARRAY_OF(T) *array);                                                             \
                bool (*const empty)(CONT_ARRAY_OF(T) *array);                                                          \
                size_t (*const size)(CONT_ARRAY_OF(T) *array);                                                         \
                size_t (*const max_size)(CONT_ARRAY_OF(T) *array);                                                     \
                cont_array_status_t (*const fill)(CONT_ARRAY_OF(T) *array, T value);                                   \
                cont_array_status_t (*const swap)(CONT_ARRAY_OF(T) *array, CONT_ARRAY_OF(T) *other);                   \
        } CONT_ARRAY_OPERATOR_OF(T);

#define CONT_PRIV_DECL_GET_ARRAY_OPERATOR_OF(T) CONT_ARRAY_OPERATOR_OF(T) *CONT_GET_ARRAY_OPERATOR_OF(T)(void)

#define CONT_DECL_ARRAY_OF(T)                                                                                          \
        CONT_PRIV_DECL_ARRAY_OF(T)                                                                                     \
        CONT_PRIV_DECL_ARRAY_OPERATOR_OF(T)                                                                            \
        CONT_PRIV_DECL_GET_ARRAY_OPERATOR_OF(T)

#endif // !H_DECL_ARRAY
