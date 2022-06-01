#pragma once

#include "common.h"

typedef double value_t;

typedef struct {
    int capa;
    int count;
    value_t *values;
} value_array_t;

void value_array_init(value_array_t *arr);
void value_array_write(value_array_t *arr, value_t value);
void value_array_free(value_array_t *arr);

void value_print(value_t value);

