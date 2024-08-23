#ifndef H_TYPES
#define H_TYPES

typedef enum
{
        CSTL_ARRAY_SUCCESS,
        CSTL_ARRAY_FAILURE,
        CSTL_ARRAY_IS_NULL,
        CSTL_ARRAY_COULD_NOT_DESRUCT_ELEMENT,
        CSTL_ARRAY_SIZE_MISMATCH
} cstl_array_status_t;

typedef enum
{
        CSTL_EQUAL,
        CSTL_DIFFERENT,
        CSTL_LESS,
        CSTL_GREATER
} cstl_comparison_t;

#endif // !H_TYPES
