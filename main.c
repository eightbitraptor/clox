#include "common.h"
#include "chunk.h"
#include "vm.h"
#include "debug.h"

int main(int argc, const char* argv[])
{
    chunk_t chunk;

    vm_init();
    chunk_init(&chunk);

    int constant = chunk_add_constant(&chunk, 1.2);
    chunk_write(&chunk, OP_CONSTANT, 123);
    chunk_write(&chunk, constant, 123);

    constant = chunk_add_constant(&chunk, 3.4);
    chunk_write(&chunk, OP_CONSTANT, 123);
    chunk_write(&chunk, constant, 123);

    chunk_write(&chunk, OP_ADD, 123);

    constant = chunk_add_constant(&chunk, 5.6);
    chunk_write(&chunk, OP_CONSTANT, 123);
    chunk_write(&chunk, constant, 123);

    chunk_write(&chunk, OP_DIVIDE, 123);
    chunk_write(&chunk, OP_NEGATE, 123);

    chunk_write(&chunk, OP_RETURN, 123);

    //dissassemble_chunk(&chunk, "test chunk");
    vm_interpret(&chunk);

    vm_free();
    chunk_free(&chunk);

    return 0;
}
