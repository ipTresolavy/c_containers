#include "array/decl_array.h"
#include <stdlib.h>
#include <string.h>

OPERATOR_OF(T) * T_operator(void);

struct __ARRAY_OF__(T)
{
	size_t size;
	// TODO: implement iterators
	OPERATOR_OF(T) * T_operator;
	T *data;
};

static OPERATOR_OF(T) * (*const cstl_get_T_operator)(void) = T_operator;

static ARRAY_OF(T) * cstl_T_array_construct_empty(size_t size)
{
	ARRAY_OF(T) *array = malloc(sizeof(ARRAY_OF(T)));
	if (NULL == array)
	{
		return NULL;
	}

	array->data = malloc(size * sizeof(T));
	if (NULL == array->data)
	{
		free(array);
		return NULL;
	}

	if (NULL == cstl_get_T_operator)
	{
		free(array);
		free(array->data);
		return NULL;
	}
	array->T_operator = cstl_get_T_operator();
	array->size = size;

	return array;
}

static ARRAY_OF(T) * cstl_T_array_construct_initialized(size_t size, T *array_data, size_t array_data_size)
{
	ARRAY_OF(T) *array = malloc(sizeof(ARRAY_OF(T)));
	if (NULL == array)
	{
		return NULL;
	}

	array->data = calloc(size, sizeof(T));
	if (NULL == array->data)
	{
		free(array);
		return NULL;
	}
	if (array_data_size > size || NULL == cstl_get_T_operator)
	{
		free(array);
		free(array->data);
		return NULL;
	}
	memcpy(array->data, array_data, array_data_size * sizeof(T));
	array->T_operator = cstl_get_T_operator();
	array->size = size;

	return array;
}

static ARRAY_OF(T) * cstl_T_array_construct(size_t size, ...)
{
	va_list args;
	va_start(args, size);
	T *array_data = va_arg(args, T *);
	if (NULL != array_data)
	{
		size_t array_data_size = va_arg(args, size_t);
		return cstl_T_array_construct_initialized(size, array_data, array_data_size);
	}

	return cstl_T_array_construct_empty(size);
}

static cstl_array_status_t cstl_T_array_destruct(ARRAY_OF(T) * *array)
{
	if (NULL == array || NULL == *array)
	{
		return CSTL_ARRAY_IS_NULL;
	}
	ARRAY_OF(T) *_array = *array;

	// if "free" receives a NULL arg., no operation is performed
	free(_array->data);
	free(_array);
	_array = NULL;

	return CSTL_SUCCESS;
}

static cstl_array_status_t cstl_T_array_assign(ARRAY_OF(T) * array, ARRAY_OF(T) * other)
{
	if (NULL == array || NULL == other)
	{
		return CSTL_ARRAY_IS_NULL;
	}
	size_t size = array->size;
	if (size != other->size)
	{
		return CSTL_SIZE_MISMATCH;
	}

	memcpy(array->data, other->data, size * sizeof(T));

	return CSTL_SUCCESS;
}

static T *cstl_T_array_at(ARRAY_OF(T) * array, size_t index)
{
	if (NULL == array || index >= array->size)
	{
		return NULL;
	}
	return &(array->data[index]);
}

static T *cstl_T_array_index(ARRAY_OF(T) * array, size_t index)
{
	if (NULL == array)
	{
		return NULL;
	}
	return &(array->data[index]);
}
