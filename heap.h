/*
 * heap.h
 *
 *  Created on: May 9, 2025
 *      Author: khogaly
 */

#ifndef MIDDLELAYER_HEAP_H_
#define MIDDLELAYER_HEAP_H_
#include "Std_types.h"



/**
 * @file heap.h
 * @brief Simple dynamic memory allocator using a static heap and free list.
 *
 * This heap implementation provides `pvmalloc()` and `vfree()` functions
 * similar to `malloc()` and `free()` but uses a statically allocated memory
 * region. It maintains a doubly linked list of memory blocks with headers
 * containing metadata.
 *
 * - Each block contains size, previous, and next pointers.
 * - Allocation uses a first-fit strategy and may split blocks.
 * - Deallocation coalesces adjacent free blocks to reduce fragmentation.
 *.
 */
// === Configuration Macros ===


#define HEAP_SIZE		(1024)
#define HEAP_ALIGN        (8UL)  // Alignment size in bytes






/**
 * @brief Allocate memory from the static heap
 * @param Wantedbytes Number of user-requested bytes
 * 		 note: allocated size = alignedup(wantedbytes + list header size)
 * @return Pointer to allocated memory, or NULL on failure
 */
void* pvmalloc(u32_t Wantedbytes);

/**
 * @brief Free a previously allocated block
 * @param usr_ptr Pointer returned by pvmalloc()
 */
void vfree(void* usr_ptr);

#endif /* MIDDLELAYER_HEAP_H_ */
