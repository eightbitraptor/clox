#pragma once

#include "chunk.h"

void dissassemble_chunk(chunk_t *chunk, const char *name);
int dissassemble_instruction(chunk_t *chunk, int offset);
