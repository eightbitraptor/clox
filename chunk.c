#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void 
chunk_init(chunk_t *chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    value_array_init(&chunk->constants);
}

void
chunk_free(chunk_t *chunk)
{
    FREE_ARRAY(u8, chunk->code, chunk->capacity);
    value_array_free(&chunk->constants);
    chunk_init(chunk);
}

void
chunk_write(chunk_t *chunk, u8 byte)
{
    if (chunk->capacity < chunk->count + 1)
    {
        int old_capa = chunk->capacity;
        chunk->capacity = GROW_CAPA(old_capa);
        chunk->code = GROW_ARRAY(u8, chunk->code, old_capa, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;
}

int
chunk_add_constant(chunk_t *chunk, value_t value)
{
    value_array_write(&chunk->constants, value);
    return chunk->constants.count - 1;
}

