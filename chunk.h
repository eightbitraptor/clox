#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} opcode_t;

/* 
 * chunk_t -> this is basically an iseq in Ruby parlance I think
 */
typedef struct {
    int count;
    int capacity;
    u8 *code;
    int *lines;
    value_array_t constants;
} chunk_t;

void chunk_init(chunk_t *chunk);
void chunk_free(chunk_t *chunk);
void chunk_write(chunk_t *chunk, u8 byte, int line);
int chunk_add_constant(chunk_t *chunk, value_t value);

#endif
