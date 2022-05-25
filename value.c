#include <stdio.h>

#include "memory.h"
#include "value.h"

void 
value_array_init(value_array_t *arr) 
{
    arr->capa = 0;
    arr->count = 0;
    arr->values = NULL;
}

void
value_array_free(value_array_t *arr)
{
    FREE_ARRAY(value_t, arr->values, arr->capa);
    value_array_init(arr);
}

void
value_array_write(value_array_t *arr, value_t value)
{
    if (arr->capa < arr->count + 1) {
        int old_capa = arr->capa;
        arr->capa = GROW_CAPA(old_capa);
        arr->values = GROW_ARRAY(value_t, arr->values, old_capa, arr->capa);
    }

    arr->values[arr->count] = value;
    arr->count++;
}

void
value_print(value_t value)
{
    printf("%g", value);
}
