#ifndef H_TYPES
#define H_TYPES

typedef enum
{
        CONT_ARRAY_SUCCESS,
        CONT_ARRAY_FAILURE,
        CONT_ARRAY_IS_NULL,
        CONT_ARRAY_COULD_NOT_DESRUCT_ELEMENT,
        CONT_ARRAY_SIZE_MISMATCH
} cont_array_status_t;

typedef enum
{
        CONT_EQUAL,
        CONT_DIFFERENT,
        CONT_LESS,
        CONT_GREATER
} cont_comparison_t;

#endif // !H_TYPES
