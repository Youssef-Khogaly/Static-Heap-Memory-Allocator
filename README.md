# Static Heap Memory Allocator

A lightweight dynamic memory allocator implemented in C using a statically allocated heap and a free list. This project provides `pvmalloc()` and `vfree()` functions with similar behavior to the standard `malloc()` and `free()`, but without relying on the system heap or standard library memory functions.

## Features

- **Statically allocated heap** — No external dependencies on system-level memory allocators.
- **Doubly linked free list** — Efficient traversal and block coalescing.
- **Minimal overhead** — Each memory block contains a compact metadata header.
- **Memory splitting and coalescing** — To reduce fragmentation and reuse freed blocks.

## File Overview

- `heap.c`: Core implementation of memory allocation and deallocation.
- `heap.h`: Public interface to the allocator.
- `Std_types.h`: Generic type definitions used throughout the project.

## API

```c
/**
 * Allocates a block of memory of the requested size.
 * Returns a pointer to the allocated memory or NULL on failure.
 */
void* pvmalloc(uint32_t size);

/**
 * Frees a previously allocated block of memory.
 * Safe to call with NULL.
 */
void vfree(void* ptr);

/**
 * Initializes the heap memory system.
 * Must be called before any allocation.
 */
void heap_init(void);
```

## How It Works

1. **Static Heap**: A fixed-size array is defined at compile-time to serve as the heap space.
2. **Block Headers**: Each block contains metadata (`size`, and pointers to previous and next blocks).
3. **Allocation**: When `pvmalloc()` is called, it searches the free list for a suitable block, optionally splitting it and setting the MSB of the size field to mark the block as *not free*.
4. **Deallocation**: `vfree()` marks a block as free and attempts to coalesce adjacent free blocks.

## Limitations

- No thread safety (intended for educational or embedded use).
- Heap size is fixed at compile-time.

## Usage Example

```c
#include "heap.h"

int main(void) {
    heap_init();

    int* arr = (int*)pvmalloc(10 * sizeof(int));
    if (arr) {
        // Use the allocated array...
        vfree(arr);
    }

    return 0;
}
```


