#ifndef H_DECL_ARRAY
#define H_DECL_ARRAY

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
typedef int T;

typedef enum
{
	CSTL_SUCCESS,
	CSTL_ARRAY_IS_NULL,
	CSTL_SIZE_MISMATCH
} cstl_array_status_t;

typedef enum
{
	CSTL_EQUAL,
	CSTL_DIFFERENT,
	CSTL_LESS,
	CSTL_GREATER
} cstl_comparison_t;

#define __CONCAT__(x, y)	 x##y
#define CONCAT(x, y)		 __CONCAT__(x, y)

#define __ARRAY_OF__(T)		 cstl_##T##_array
#define __OPERATOR_OF__(T)	 cstl_##T##_operator
#define __ARRAY_OPERATOR_OF__(T) cstl_##T##_array_operator
#define ARRAY_OF(T)		 CONCAT(__ARRAY_OF__(T), _t)
#define OPERATOR_OF(T)		 CONCAT(__OPERATOR_OF__(T), _t)
#define ARRAY_OPERATOR_OF(T)	 CONCAT(__ARRAY_OPERATOR_OF__(T), _t)

#define NEW_ARRAY_OPERATOR_OF(T) cstl_new_##T##_array_operator

struct __ARRAY_OF__(T);
typedef struct __ARRAY_OF__(T) ARRAY_OF(T);

typedef struct __OPERATOR_OF__(T)
{
	cstl_comparison_t (*const is_equal)(T *lhs, T *rhs);
	cstl_comparison_t (*const compare)(T *lhs, T *rhs);
} OPERATOR_OF(T);

typedef struct __ARRAY_OPERATOR_OF__(T)
{
	ARRAY_OF(T) * (*const construct)(size_t size, ...);
	cstl_array_status_t *(*const destruct)(ARRAY_OF(T) * *array);
	cstl_array_status_t *(*const assign)(ARRAY_OF(T) * array, ARRAY_OF(T) * other);
	T *(*const at)(ARRAY_OF(T) * array, size_t index);
	T *(*const index)(ARRAY_OF(T) * array, size_t index);
	T *(*const front)(ARRAY_OF(T) * array, size_t index);
	T *(*const back)(ARRAY_OF(T) * array, size_t index);
	T *(*const data)(ARRAY_OF(T) * array, size_t index);
	bool (*const empty)(ARRAY_OF(T) * array);
	size_t (*const size)(ARRAY_OF(T) * array);
	size_t (*const max_size)(ARRAY_OF(T) * array);
	void (*const fill)(ARRAY_OF(T) * array, T *value);
	void (*const swap)(ARRAY_OF(T) * array, ARRAY_OF(T) * other);
} ARRAY_OPERATOR_OF(T);

ARRAY_OPERATOR_OF(T) * NEW_ARRAY_OPERATOR_OF(T)(void);

#endif // !H_DECL_ARRAY
