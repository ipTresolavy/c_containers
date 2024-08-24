#ifndef H_OPERATOR
#define H_OPERATOR
// TODO: create general return types for operators and specific ones for arrays
// check how CMake can generate source files with macros expanded (one for each .c)
// maybe generalize the CONT_PRIV_PREFIX macro
// doc: explain what the project is (a bunch of macros) and usage. Begin with end result and explain what user has to
// do to use it. Also say it is type-safe
// check if it would be better to destruct the array elements on the reverse order, without using deinit

#define CONT_PRIV_CONCAT(x, y)   x##y
#define CONT_CONCAT(x, y)        CONT_PRIV_CONCAT(x, y)

#define CONT_PRIV_OPERATOR_OF(T) cont_##T##_operator
#define CONT_OPERATOR_OF(T)      CONT_CONCAT(CONT_PRIV_OPERATOR_OF(T), _t)

#define CONT_DECL_OPERATOR_OF(T)                                                                                       \
        typedef struct CONT_PRIV_OPERATOR_OF(T)                                                                        \
        {                                                                                                              \
                T *(*const construct)(void);                                                                           \
                cont_array_status_t (*const destruct)(T **);                                                           \
                cont_array_status_t (*const copy)(T *, T *);                                                           \
                cont_array_status_t (*const init)(T *);                                                                \
                cont_array_status_t (*const deinit)(T *);                                                              \
        } CONT_OPERATOR_OF(T)

#define CONT_PRIV_GET_OPERATOR_OF(T) cont_get_##T##_operator

#endif // !H_OPERATOR
