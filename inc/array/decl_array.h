#ifndef H_DECL_ARRAY
#define H_DECL_ARRAY

#include "types.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#define __CONCAT__(x, y)	 x##y
#define CONCAT(x, y)		 __CONCAT__(x, y)

#define PREFIX(T)		 cstl_##T##_array
#define __ARRAY_OF__(T)		 PREFIX(T)
#define __ARRAY_OPERATOR_OF__(T) CONCAT(__ARRAY_OF__(T), _operator)
#define ARRAY_OF(T)		 CONCAT(__ARRAY_OF__(T), _t)
#define ARRAY_OPERATOR_OF(T)	 CONCAT(__ARRAY_OPERATOR_OF__(T), _t)

#define NEW_ARRAY_OPERATOR_OF(T) cstl_new_##T##_array_operator

#define __DECL_ARRAY_OF__(T)                                                                                           \
	struct __ARRAY_OF__(T);                                                                                        \
	typedef struct __ARRAY_OF__(T) ARRAY_OF(T);

#define __DECL_ARRAY_OPERATOR_OF__(T)                                                                                  \
	typedef struct __ARRAY_OPERATOR_OF__(T)                                                                        \
	{                                                                                                              \
		ARRAY_OF(T) * (*const construct)(size_t size, ...);                                                    \
		cstl_array_status_t (*const destruct)(ARRAY_OF(T) * *array);                                           \
		cstl_array_status_t (*const assign)(ARRAY_OF(T) * array, ARRAY_OF(T) * other);                         \
		T *(*const at)(ARRAY_OF(T) * array, size_t index);                                                     \
		T *(*const index)(ARRAY_OF(T) * array, size_t index);                                                  \
		T *(*const front)(ARRAY_OF(T) * array);                                                                \
		T *(*const back)(ARRAY_OF(T) * array);                                                                 \
		T *(*const data)(ARRAY_OF(T) * array);                                                                 \
		bool (*const empty)(ARRAY_OF(T) * array);                                                              \
		size_t (*const size)(ARRAY_OF(T) * array);                                                             \
		size_t (*const max_size)(ARRAY_OF(T) * array);                                                         \
		cstl_array_status_t (*const fill)(ARRAY_OF(T) * array, T value);                                       \
		cstl_array_status_t (*const swap)(ARRAY_OF(T) * array, ARRAY_OF(T) * other);                           \
	} ARRAY_OPERATOR_OF(T);

#define __DECL_NEW_ARRAY_OPERATOR_OF__(T) ARRAY_OPERATOR_OF(T) * NEW_ARRAY_OPERATOR_OF(T)(void)

#define DECL_ARRAY_OF(T)                                                                                               \
	__DECL_ARRAY_OF__(T)                                                                                           \
	__DECL_ARRAY_OPERATOR_OF__(T)                                                                                  \
	__DECL_NEW_ARRAY_OPERATOR_OF__(T)

#endif // !H_DECL_ARRAY
