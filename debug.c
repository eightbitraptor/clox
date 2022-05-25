#include <stdio.h>

#include "debug.h"

static int
simple_instruction(const char *name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

void
dissassemble_chunk(chunk_t *chunk, const char *name)
{
    printf("== %s ==\n", name);
    for (int offset = 0; offset < chunk->count;)
    {
        offset = dissassemble_instruction(chunk, offset);
    }
}

int
dissassemble_instruction(chunk_t *chunk, int offset)
{
    u8 instruction = chunk->code[offset];
    switch (instruction) {
        case OP_RETURN:
            return simple_instruction("OP_RETURN", offset);
        default:
            printf("unknown opcode %d\n", instruction);
            return offset + 1;
    }
}

