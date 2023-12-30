#ifndef H_IMPL_ARRAY
#define H_IMPL_ARRAY
#include "array/decl_array.h"
#include <stdlib.h>
#include <string.h>

#define __IMPL__(T, op) CONCAT(PREFIX(T), op)

#define __IMPL_ARRAY_OF__(T)                                                                                           \
	struct __ARRAY_OF__(T)                                                                                         \
	{                                                                                                              \
		size_t size;                                                                                           \
		T *data;                                                                                               \
	};

#define __IMPL_CONSTRUCT_EMPTY__(T)                                                                                    \
	static ARRAY_OF(T) * __IMPL__(T, construct_empty)(size_t size)                                                 \
	{                                                                                                              \
		ARRAY_OF(T) *array = malloc(sizeof(ARRAY_OF(T)));                                                      \
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
                                                                                                                       \
		array->size = size;                                                                                    \
                                                                                                                       \
		return array;                                                                                          \
	}

#define __IMPL_CONSTRUCT_INITIALIZED__(T)                                                                              \
	static ARRAY_OF(T) * __IMPL__(T, construct_initialized)(size_t size, T * array_data, size_t array_data_size)   \
	{                                                                                                              \
		ARRAY_OF(T) *array = malloc(sizeof(ARRAY_OF(T)));                                                      \
		if (NULL == array)                                                                                     \
		{                                                                                                      \
			return NULL;                                                                                   \
		}                                                                                                      \
                                                                                                                       \
		array->data = calloc(size, sizeof(T));                                                                 \
		if (NULL == array->data)                                                                               \
		{                                                                                                      \
			free(array);                                                                                   \
			return NULL;                                                                                   \
		}                                                                                                      \
		if (array_data_size > size)                                                                            \
		{                                                                                                      \
			free(array);                                                                                   \
			free(array->data);                                                                             \
			return NULL;                                                                                   \
		}                                                                                                      \
		memcpy(array->data, array_data, array_data_size * sizeof(T));                                          \
		array->size = size;                                                                                    \
                                                                                                                       \
		return array;                                                                                          \
	}

#define __IMPL_CONSTRUCT__(T)                                                                                          \
	static ARRAY_OF(T) * __IMPL__(T, construct)(size_t size, ...)                                                  \
	{                                                                                                              \
		va_list args;                                                                                          \
		va_start(args, size);                                                                                  \
		T *array_data = va_arg(args, T *);                                                                     \
		if (NULL != array_data)                                                                                \
		{                                                                                                      \
			size_t array_data_size = va_arg(args, size_t);                                                 \
			return __IMPL__(T, construct_initialized)(size, array_data, array_data_size);                  \
		}                                                                                                      \
                                                                                                                       \
		return __IMPL__(T, construct_empty)(size);                                                             \
	}

#define __IMPL_DESTRUCT__(T)                                                                                           \
	static cstl_array_status_t __IMPL__(T, destruct)(ARRAY_OF(T) * *array)                                         \
	{                                                                                                              \
		if (NULL == array || NULL == *array)                                                                   \
		{                                                                                                      \
			return CSTL_ARRAY_IS_NULL;                                                                     \
		}                                                                                                      \
		ARRAY_OF(T) *_array = *array;                                                                          \
                                                                                                                       \
		free(_array->data);                                                                                    \
		free(_array);                                                                                          \
		_array = NULL;                                                                                         \
                                                                                                                       \
		return CSTL_SUCCESS;                                                                                   \
	}

#define __IMPL_ASSIGN__(T)                                                                                             \
	static cstl_array_status_t __IMPL__(T, assign)(ARRAY_OF(T) * array, ARRAY_OF(T) * other)                       \
	{                                                                                                              \
		if (NULL == array || NULL == other)                                                                    \
		{                                                                                                      \
			return CSTL_ARRAY_IS_NULL;                                                                     \
		}                                                                                                      \
		size_t size = array->size;                                                                             \
		if (size != other->size)                                                                               \
		{                                                                                                      \
			return CSTL_SIZE_MISMATCH;                                                                     \
		}                                                                                                      \
                                                                                                                       \
		memcpy(array->data, other->data, size * sizeof(T));                                                    \
                                                                                                                       \
		return CSTL_SUCCESS;                                                                                   \
	}

#define __IMPL_AT__(T)                                                                                                 \
	static T *__IMPL__(T, at)(ARRAY_OF(T) * array, size_t index)                                                   \
	{                                                                                                              \
		if (index >= array->size || NULL == array)                                                             \
		{                                                                                                      \
			return NULL;                                                                                   \
		}                                                                                                      \
		return &(array->data[index]);                                                                          \
	}

#define __IMPL_INDEX__(T)                                                                                              \
	static T *__IMPL__(T, index)(ARRAY_OF(T) * array, size_t index)                                                \
	{                                                                                                              \
		if (NULL == array)                                                                                     \
		{                                                                                                      \
			return NULL;                                                                                   \
		}                                                                                                      \
		return &(array->data[index]);                                                                          \
	}

#define __IMPL_FRONT__(T)                                                                                              \
	static T *__IMPL__(T, front)(ARRAY_OF(T) * array)                                                              \
	{                                                                                                              \
		if (NULL == array)                                                                                     \
		{                                                                                                      \
			return NULL;                                                                                   \
		}                                                                                                      \
		return &(array->data[0]);                                                                              \
	}

#define __IMPL_BACK__(T)                                                                                               \
	static T *__IMPL__(T, back)(ARRAY_OF(T) * array)                                                               \
	{                                                                                                              \
		if (NULL == array)                                                                                     \
		{                                                                                                      \
			return NULL;                                                                                   \
		}                                                                                                      \
		return &(array->data[array->size - 1]);                                                                \
	}

#define __IMPL_DATA__(T)                                                                                               \
	static T *__IMPL__(T, data)(ARRAY_OF(T) * array)                                                               \
	{                                                                                                              \
		if (NULL == array)                                                                                     \
		{                                                                                                      \
			return NULL;                                                                                   \
		}                                                                                                      \
		return array->data;                                                                                    \
	}

#define __IMPL_EMPTY__(T)                                                                                              \
	static bool __IMPL__(T, empty)(ARRAY_OF(T) * array)                                                            \
	{                                                                                                              \
		if (NULL == array)                                                                                     \
		{                                                                                                      \
			return NULL;                                                                                   \
		}                                                                                                      \
		return array->size == 0;                                                                               \
	}

#define __IMPL_SIZE__(T)                                                                                               \
	static size_t __IMPL__(T, size)(ARRAY_OF(T) * array)                                                           \
	{                                                                                                              \
		if (NULL == array)                                                                                     \
		{                                                                                                      \
			return NULL;                                                                                   \
		}                                                                                                      \
		return array->size;                                                                                    \
	}

#define __IMPL_MAX_SIZE__(T)                                                                                           \
	static size_t __IMPL__(T, max_size)(ARRAY_OF(T) * array)                                                       \
	{                                                                                                              \
		return __IMPL__(T, size)(array);                                                                       \
	}

#define __IMPL_FILL__(T)                                                                                               \
	static cstl_array_status_t __IMPL__(T, fill)(ARRAY_OF(T) * array, T value)                                     \
	{                                                                                                              \
		if (NULL == array)                                                                                     \
		{                                                                                                      \
			return CSTL_ARRAY_IS_NULL;                                                                     \
		}                                                                                                      \
		const size_t size = array->size;                                                                       \
		T *data = array->data;                                                                                 \
		for (T *data_i = data; data_i < data + size; ++data_i)                                                 \
		{                                                                                                      \
			*data_i = value;                                                                               \
		}                                                                                                      \
		return CSTL_SUCCESS;                                                                                   \
	}

#define __IMPL_SWAP__(T)                                                                                               \
	static cstl_array_status_t __IMPL__(T, swap)(ARRAY_OF(T) * array, ARRAY_OF(T) * other)                         \
	{                                                                                                              \
		if (NULL == array || NULL == other)                                                                    \
		{                                                                                                      \
			return CSTL_ARRAY_IS_NULL;                                                                     \
		}                                                                                                      \
                                                                                                                       \
		if (array->size != other->size)                                                                        \
		{                                                                                                      \
			return CSTL_SIZE_MISMATCH;                                                                     \
		}                                                                                                      \
                                                                                                                       \
		T *temp = array->data;                                                                                 \
		array->data = other->data;                                                                             \
		other->data = temp;                                                                                    \
                                                                                                                       \
		return CSTL_SUCCESS;                                                                                   \
	}

#define __IMPL_NEW_ARRAY_OPERATOR_OF(T)                                                                                \
	ARRAY_OPERATOR_OF(T) * NEW_ARRAY_OPERATOR_OF(T)(void)                                                          \
	{                                                                                                              \
		static ARRAY_OPERATOR_OF(T) operator= {.construct = __IMPL__(T, construct),                            \
						       .destruct = __IMPL__(T, destruct),                              \
						       .assign = __IMPL__(T, assign),                                  \
						       .at = __IMPL__(T, at),                                          \
						       .index = __IMPL__(T, index),                                    \
						       .front = __IMPL__(T, front),                                    \
						       .back = __IMPL__(T, back),                                      \
						       .data = __IMPL__(T, data),                                      \
						       .empty = __IMPL__(T, empty),                                    \
						       .size = __IMPL__(T, size),                                      \
						       .max_size = __IMPL__(T, max_size),                              \
						       .fill = __IMPL__(T, fill),                                      \
						       .swap = __IMPL__(T, swap)};                                     \
		return &operator;                                                                                      \
	}

#define __IMPL_OPS__(T)                                                                                                \
	__IMPL_CONSTRUCT_EMPTY__(T)                                                                                    \
	__IMPL_CONSTRUCT_INITIALIZED__(T)                                                                              \
	__IMPL_CONSTRUCT__(T)                                                                                          \
	__IMPL_DESTRUCT__(T)                                                                                           \
	__IMPL_ASSIGN__(T)                                                                                             \
	__IMPL_AT__(T)                                                                                                 \
	__IMPL_INDEX__(T)                                                                                              \
	__IMPL_FRONT__(T)                                                                                              \
	__IMPL_BACK__(T)                                                                                               \
	__IMPL_DATA__(T)                                                                                               \
	__IMPL_EMPTY__(T)                                                                                              \
	__IMPL_SIZE__(T)                                                                                               \
	__IMPL_MAX_SIZE__(T)                                                                                           \
	__IMPL_FILL__(T)                                                                                               \
	__IMPL_SWAP__(T)

#define IMPL_ARRAY_OF(T)                                                                                               \
	__IMPL_ARRAY_OF__(T)                                                                                           \
	__IMPL_OPS__(T)                                                                                                \
	__IMPL_NEW_ARRAY_OPERATOR_OF(T)

#endif // !H_IMPL_ARRAY
