#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "common.h"

typedef struct heap_node{
    size_t size;
    bool used;
    struct heap_node *next;
    struct heap_node *prev;

    void * data[];
} heap_node_t;

typedef struct {
    heap_node_t *heap;
} objspace_t;

extern objspace_t objspace;

#define HEAP_INIT_SIZE 1024 * 1024
#define HEAP_NODE_SIZE sizeof(heap_node_t)
 
#define GROW_CAPA(capa) \
    ((capa) < 8 ? 8 : (capa) * 2)

#define GROW_ARRAY(type, pointer, old_count, new_count)    \
    (type*)reallocate(pointer, sizeof(type) * (old_count), \
            sizeof(type) * (new_count))

#define FREE_ARRAY(type, pointer, old_count) \
    reallocate(pointer, sizeof(type) * old_count, 0)

void objspace_init(void);
void objspace_free(void);
void * heap_init(void);

void * clox_malloc(size_t);
void * clox_calloc(int c, size_t size);
void * clox_realloc(void *pointer, size_t size);
void clox_free(void *ptr);

void * reallocate(void *pointer, size_t old_size, size_t new_size);
