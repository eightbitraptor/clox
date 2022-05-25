#include <stdio.h>

#include "debug.h"

static int
simpleInstruction(const char *name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

void
dissassembleChunk(chunk_t *chunk, const char *name)
{
    printf("== %s ==\n", name);
    for (int offset = 0; offset < chunk->count;)
    {
        offset = dissassembleInstruction(chunk, offset);
    }
}

int
dissassembleInstruction(chunk_t *chunk, int offset)
{
    u8 instruction = chunk->code[offset];
    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("unknown opcode %d\n", instruction);
            return offset + 1;
    }
}

