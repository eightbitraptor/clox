#include <stdlib.h>
#include <sys/param.h>
#include <string.h>

#include "memory.h"
#include "vm.h"

#if HEAP_DEBUG
static void
dump_heap(const char *op, uintptr_t highlight) {
    heap_node_t *start = vm->heap;
    heap_node_t *node = (heap_node_t *)vm->heap;

    printf("--- HEAP_DUMP_START(%s):\n", op);
    while(node) {
        printf("%p [%8zu] Node { used: %i, size: %zu, next: %p, prev: %p }", 
                node, 
                (size_t)((uintptr_t)node - (uintptr_t)start),
                node->used, node->size, node->next, node->prev);
        if ((uintptr_t)node == highlight) {
            printf(" <<<<<");
        }
        printf("\n");

        node = node->next;
    }
    printf("\n");
}
#endif

void *
reallocate(void *pointer, size_t old_size, size_t new_size)
{
    if (new_size == 0) {
        clox_free(pointer);
        return NULL;
    }

    void *result = clox_realloc(pointer, new_size);
    if (!result) {
        fprintf(stderr, "couldn't reallocate memory (%p)", pointer);
        abort();
    }

    return result;
}

void *
heap_init()
{
    heap_node_t *heap_start = malloc(HEAP_INIT_SIZE);
    if (heap_start == NULL) {
        fprintf(stderr, "Error allocating memory!");
        abort();
    }

    heap_start->used = false;
    heap_start->size = (size_t)(HEAP_INIT_SIZE - HEAP_NODE_SIZE);
    heap_start->next = NULL;

    return (void *)heap_start;
}

void *
clox_malloc(size_t requested_size)
{
    heap_node_t *allocated_node;
    heap_node_t *node = (heap_node_t *)vm->heap;
    size_t allocation_size = requested_size + HEAP_NODE_SIZE;

    // get the first unused node in the heap
    // we should probably replace this with a freelist
    while (node) {
        if (!node->used && (node->size >= allocation_size)) {
            break;
        }
        node = node->next;
    }

    if (node) {
        // split the node, create a new node of the correct size at the end of the
        // current node 
        node->size = node->size - allocation_size;
        allocated_node = (heap_node_t *)((uintptr_t)node + HEAP_NODE_SIZE + node->size);
        allocated_node->size = requested_size;
        allocated_node->used = true;

        // insert the new node into the linked list
        allocated_node->next = node->next;
        allocated_node->prev = node;
        if (node->next != NULL) {
            node->next->prev = allocated_node;
        }
        node->next = allocated_node;

        return (void *)allocated_node->data;
    }

    return NULL;
}

void
clox_free(void *ptr)
{
    // find the node associated with the pointer
    heap_node_t *node = (heap_node_t *)((uintptr_t)ptr - HEAP_NODE_SIZE);
    CLOX_ASSERT(node->prev);
    // mark it as unused
    node->used = false;

    // if the node has a next node that is unused, we can merge the nodes.
    // increase the size of this node to consume the next node, and set the 
    // linked list pointers to skip the next node
    if (node->next != NULL && !node->next->used) {
        heap_node_t *next_node = node->next;

        node->size += (next_node->size + HEAP_NODE_SIZE);
        node->next = next_node->next;
        if (next_node->next != NULL) {
            next_node->next->prev = node;
        }
        CLOX_ASSERT(((uintptr_t)node + HEAP_NODE_SIZE + node->size) 
                == (uintptr_t)node->next);
        fprintf(stderr, "assert\n");
    }
    // same again, but with the previous node. If the previous node is unused
    // adjust the previous nodes size to consume this node, and set the linked
    // list pointers to skip this node
    if (node->prev != NULL && !node->prev->used) {
        heap_node_t *prev_node = node->prev;

        prev_node->size += (node->size + HEAP_NODE_SIZE);
        prev_node->next = node->next;
        if (node->next != NULL) {
            node->next->prev = prev_node;
            CLOX_ASSERT(((uintptr_t)prev_node->data + prev_node->size) 
                    == (uintptr_t)node->next);
        }
    }
#if HEAP_DEBUG
        dump_heap("free: end", (uintptr_t)node);
#endif
}

void *
clox_realloc(void *ptr, size_t size)
{
    heap_node_t *og_node;

    void *mem = clox_malloc(size);
    if (mem == NULL) {
        return mem;
    }

    if (ptr != NULL) {
        og_node = (heap_node_t *)((uintptr_t)ptr - HEAP_NODE_SIZE);
        
        memcpy(mem, ptr, MIN(size, og_node->size));
        clox_free(ptr);
    }

    return mem;
}
