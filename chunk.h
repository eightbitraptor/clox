#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"

typedef enum {
    OP_RETURN,
} opcode_t;

typedef struct {
    int count;
    int capacity;
    u8* code;
} chunk_t;

void initChunk(chunk_t *chunk);
void freeChunk(chunk_t *chunk);
void writeChunk(chunk_t *chunk, u8 byte);

#endif
