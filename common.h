#pragma once

#ifndef DEBUG_TRACE_EXECUTION
#define DEBUG_TRACE_EXECUTION 0
#endif

#ifndef DEBUG_HEAP
#define DEBUG_HEAP 0
#endif

#ifndef CLOX_CHECK_MODE
#define CLOX_CHECK_MODE 0
#endif

#if CLOX_CHECK_MODE
#include "assert.h"
#define CLOX_ASSERT(expr) \
    do { \
        assert(expr); \
    } while(false);
#else
#define CLOX_ASSERT(expr) \
    do { } while (false);
#endif


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "assert.h"

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
