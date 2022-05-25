#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[])
{
    chunk_t chunk;
    chunk_init(&chunk);
    chunk_write(&chunk, OP_RETURN);

    dissassemble_chunk(&chunk, "test chunk");

    chunk_free(&chunk);

    return 0;
}
