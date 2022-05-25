#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include "chunk.h"

void dissassembleChunk(chunk_t *chunk, const char *name);
int dissassembleInstruction(chunk_t *chunk, int offset);

#endif
