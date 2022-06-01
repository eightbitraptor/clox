#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

#ifdef __APPLE__
#include <editline/readline.h>
#else
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include "common.h"
#include "chunk.h"
#include "vm.h"
#include "debug.h"

static inline vm_result_t
interpret(char *line)
{
    return INTERPRET_OK;
}

static inline char *
read_file(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file: \"%s\"\n", path);
        exit(EX_IOERR);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *buffer = (char *)clox_malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read file\n");
        exit(EX_IOERR);
    }
    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Could not read file: \"%s\"\n", path);
        exit(EX_IOERR);
    }
    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

static inline void
repl(void)
{
    puts("clox version 0.0.1");
    puts("Press Ctrl+c to exit");

    while (1) {
        char *line = readline("clox> ");
        if (!line) {
            break;
        }
        add_history(line);

        vm_interpret(line);
    }
}

static inline void
run_file(const char *path)
{
    char *source = read_file(path);
    vm_result_t result = vm_interpret(source);
    clox_free(source);

    if (result == INTERPRET_COMPILER_ERROR) exit(EX_DATAERR);
    if (result == INTERPRET_RUNTIME_ERROR)  exit(EX_SOFTWARE);
}

int main(int argc, const char* argv[])
{
    objspace_init();
    vm_init();

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
    }

    vm_free();

    objspace_free();
    return 0;
}
