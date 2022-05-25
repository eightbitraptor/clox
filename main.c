#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[])
{
    chunk_t chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_RETURN);

    dissassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);

    return 0;
}
