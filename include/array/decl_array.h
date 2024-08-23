#ifndef H_DECL_ARRAY
#define H_DECL_ARRAY

#include "operator.h"

#include <stdbool.h>
#include <stddef.h>

#define __PREFIX__(T)            cont_##T##_array
#define __ARRAY_OF__(T)          __PREFIX__(T)
#define __ARRAY_OPERATOR_OF__(T) CONCAT(__ARRAY_OF__(T), _operator)
#define ARRAY_OF(T)              CONCAT(__ARRAY_OF__(T), _t)
#define ARRAY_OPERATOR_OF(T)     CONCAT(__ARRAY_OPERATOR_OF__(T), _t)

#define GET_ARRAY_OPERATOR_OF(T) cont_get_##T##_array_operator

#define __DECL_ARRAY_OF__(T)                                                                                           \
        struct __ARRAY_OF__(T);                                                                                        \
        typedef struct __ARRAY_OF__(T) ARRAY_OF(T);

#define __DECL_ARRAY_OPERATOR_OF__(T)                                                                                  \
        typedef struct __ARRAY_OPERATOR_OF__(T)                                                                        \
        {                                                                                                              \
                ARRAY_OF(T) * (*const construct)(size_t size, T *array_data, size_t array_data_size);                  \
                cont_array_status_t (*const destruct)(ARRAY_OF(T) * *array);                                           \
                cont_array_status_t (*const assign)(ARRAY_OF(T) * array, ARRAY_OF(T) * other);                         \
                T *(*const at)(ARRAY_OF(T) * array, size_t index);                                                     \
                T *(*const index)(ARRAY_OF(T) * array, size_t index);                                                  \
                T *(*const front)(ARRAY_OF(T) * array);                                                                \
                T *(*const back)(ARRAY_OF(T) * array);                                                                 \
                T *(*const data)(ARRAY_OF(T) * array);                                                                 \
                bool (*const empty)(ARRAY_OF(T) * array);                                                              \
                size_t (*const size)(ARRAY_OF(T) * array);                                                             \
                size_t (*const max_size)(ARRAY_OF(T) * array);                                                         \
                cont_array_status_t (*const fill)(ARRAY_OF(T) * array, T value);                                       \
                cont_array_status_t (*const swap)(ARRAY_OF(T) * array, ARRAY_OF(T) * other);                           \
        } ARRAY_OPERATOR_OF(T);

#define __DECL_GET_ARRAY_OPERATOR_OF__(T) ARRAY_OPERATOR_OF(T) * GET_ARRAY_OPERATOR_OF(T)(void)

#define DECL_ARRAY_OF(T)                                                                                               \
        __DECL_ARRAY_OF__(T)                                                                                           \
        __DECL_ARRAY_OPERATOR_OF__(T)                                                                                  \
        __DECL_GET_ARRAY_OPERATOR_OF__(T)

#endif // !H_DECL_ARRAY
