#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void 
initChunk(Chunk *chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
}

void
freeChunk(Chunk *chunk)
{
    FREE_ARRAY(u8, chunk->code, chunk->capacity);
    initChunk(chunk);
}

void
writeChunk(Chunk *chunk, u8 byte)
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

