#ifndef H_OPERATOR
#define H_OPERATOR
// TODO: create general return types for operators and specific ones for arrays
// check how CMake can generate source files with macros expanded (one for each .c)
// maybe generalize the __PREFIX__ macro
// doc: explain what the project is (a bunch of macros) and usage
// begin with end result and explain what user has to do to use it
// check if it would be better to destruct the array elements on the reverse order, without using deinit

#define __CONCAT__(x, y)   x##y
#define CONCAT(x, y)       __CONCAT__(x, y)

#define __OPERATOR_OF__(T) cont_##T##_operator
#define OPERATOR_OF(T)     CONCAT(__OPERATOR_OF__(T), _t)

#define DECL_OPERATOR_OF(T)                                                                                            \
        typedef struct __OPERATOR_OF__(T)                                                                              \
        {                                                                                                              \
                T *(*const construct)(void);                                                                           \
                cont_array_status_t (*const destruct)(T **);                                                           \
                cont_array_status_t (*const copy)(T *, T *);                                                           \
                cont_array_status_t (*const init)(T *);                                                                \
                cont_array_status_t (*const deinit)(T *);                                                              \
        } OPERATOR_OF(T)

#define __GET_OPERATOR_OF__(T) cont_get_##T##_operator

#endif // !H_OPERATOR
