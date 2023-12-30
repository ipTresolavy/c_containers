#ifndef H_OPERATOR
#define H_OPERATOR

#define __CONCAT__(x, y)   x##y
#define CONCAT(x, y)	   __CONCAT__(x, y)

#define __OPERATOR_OF__(T) cstl_##T##_operator
#define OPERATOR_OF(T)	   CONCAT(__OPERATOR_OF__(T), _t)

#define DECL_T_OPERATOR_OF(T)                                                                                          \
	typedef struct __OPERATOR_OF__(T)                                                                              \
	{                                                                                                              \
		cstl_array_status_t (*const destruct)(T **);                                                           \
	} OPERATOR_OF(T)

#define __GET_OPERATOR_OF__(T) cstl_get_##T##_operator

#endif // !H_OPERATOR
