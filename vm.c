#include <stdio.h>
#include "common.h"
#include "vm.h"

vm_t vm;

static vm_result_t
vm_run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        double b = stack_pop(); \
        double a = stack_pop(); \
        stack_push(a op b); \
    } while(false)

    for (;;) {
#if DEBUG_TRACE_EXECUTION
#include "debug.h"
        printf("          ");
        for (value_t *slot = vm.stack; slot < vm.stack_top; slot++) {
            printf("[ ");
            value_print(*slot);
            printf(" ]");
        }
        printf("\n");
        dissassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        u8 instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                value_t constant = READ_CONSTANT();
                stack_push(constant);
                break;
            }
            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;
            case OP_NEGATE: {
                stack_push(-stack_pop());
                break;
            }
            case OP_RETURN: {
                value_print(stack_pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

static void
stack_reset()
{
    vm.stack_top = vm.stack;
}

void
vm_init()
{
    stack_reset();
}

void
vm_free()
{
}

vm_result_t
vm_interpret(chunk_t *chunk)
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return vm_run();
}

void
stack_push(value_t v)
{
    *vm.stack_top = v;
    vm.stack_top++;
}

value_t
stack_pop()
{
    vm.stack_top--;
    return *vm.stack_top;
}
