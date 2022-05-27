#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "value.h"
#include "memory.h"

#define STACK_MAX 256

typedef struct {
    chunk_t *chunk;
    u8 *ip;
    value_t stack[STACK_MAX];
    value_t *stack_top;
} vm_t;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILER_ERROR,
    INTERPRET_RUNTIME_ERROR,
} vm_result_t;

vm_t *vm;

void vm_init();
void vm_free();
vm_result_t vm_interpret(chunk_t *chunk);

void stack_push(value_t v);
value_t stack_pop();

#endif
