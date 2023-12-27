#include "array/decl_array.h"
#include <stdbool.h>

struct __ARRAY_OF__(T)
{
	size_t size;
	T *data;
	const T *const_data;
	// TODO: implement iterators
	OPERATOR_OF(T) * T_operator;
};
