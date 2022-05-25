#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[])
{
    chunk_t chunk;
    chunk_init(&chunk);

    int constant = chunk_add_constant(&chunk, 1.2);
    chunk_write(&chunk, OP_CONSTANT);
    chunk_write(&chunk, constant);
    chunk_write(&chunk, OP_RETURN);

    dissassemble_chunk(&chunk, "test chunk");

    chunk_free(&chunk);

    return 0;
}
