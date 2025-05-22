/*
 * heap.c
 *
 *  Created on: May 9, 2025
 *      Author: khogaly
 */

#include "heap.h"


#define ALIGN_UP(_n)      ( ( (_n) + (HEAP_ALIGN - 1) ) & ~(HEAP_ALIGN - 1) ) // Align value up to HEAP_ALIGN

#define ALLOCATION_BIT_MSK      (1UL << 31UL) // High bit of blockSize used to indicate allocation status

// Macros to manipulate allocation flags
#define SET_BLOCK_FREE(BLOCK)           ((BLOCK)->blockSize &= ~(ALLOCATION_BIT_MSK))  // Clear allocation bit
#define SET_BLOCK_ALLOCATED(BLOCK)      ((BLOCK)->blockSize |=  (ALLOCATION_BIT_MSK))  // Set allocation bit
#define IS_BLOCK_FREE(BLOCK)            ((((BLOCK)->blockSize) & ALLOCATION_BIT_MSK) == 0) // Check if block is free

// === Heap Data Structures ===

static u8_t u8Heap[HEAP_SIZE]; // Static heap memory buffer

typedef struct heap_node_t {
    struct heap_node_t* next;   // Pointer to next block in memory
    struct heap_node_t* prev;   // Pointer to previous block in memory
    u32_t blockSize;            // Size of block, MSB is used as allocation flag
} heap_node_t;

static heap_node_t* head;        // Head of the heap (first block)
static u8_t heapInitialized;     // Initialization flag

// === Memory Allocation Function ===

/**
 * @brief Allocate memory from the static heap
 * @param Wantedbytes Number of user-requested bytes
 * @return Pointer to allocated memory, or NULL on failure
 */
void* pvmalloc(u32_t Wantedbytes)
{
    // Include space for block metadata
    Wantedbytes += sizeof(heap_node_t);
    Wantedbytes = ALIGN_UP(Wantedbytes);

    void* usrPtr = NULL;

    // First-time heap initialization
    if (heapInitialized == 0)
    {
        heapInitialized = 1;
        head = (heap_node_t*)u8Heap;
        head->blockSize = HEAP_SIZE;
        head->next = NULL;
        head->prev = NULL;
    }
    else if (head == NULL || Wantedbytes > HEAP_SIZE)
    {
        return usrPtr; // No available memory
    }

    // Find first fit block in memory
    heap_node_t* freeBlock = head;
    while (freeBlock)
    {
        if (IS_BLOCK_FREE(freeBlock) && freeBlock->blockSize >= Wantedbytes)
        {
            break;
        }
        freeBlock = freeBlock->next;
    }

    if (freeBlock != NULL)
    {
        heap_node_t* newBlock;

        // If the block is large enough to be split
        if ((freeBlock->blockSize - (s32_t)Wantedbytes) > 8L)
        {
            // Create a new block at the end of the free block
            newBlock = (heap_node_t*)((u8_t*)freeBlock + (freeBlock->blockSize - Wantedbytes));
            newBlock->blockSize = Wantedbytes;

            newBlock->prev = freeBlock;
            newBlock->next = freeBlock->next;
            if (newBlock->next)
            {
                newBlock->next->prev = newBlock;
            }

            // Shrink the original free block
            freeBlock->blockSize -= Wantedbytes;
        }
        else // Use the whole block without splitting
        {
            newBlock = freeBlock;
        }

        SET_BLOCK_ALLOCATED(newBlock);
        usrPtr = ((u8_t*)newBlock + sizeof(heap_node_t)); // Return pointer past the header
    }

    return usrPtr;
}

// === Memory Freeing Function ===

/**
 * @brief Free a previously allocated block
 * @param usr_ptr Pointer returned by pvmalloc()
 */
void vfree(void* usr_ptr)
{
    if (usr_ptr == NULL)
        return;

    // Get the block header
    heap_node_t* block = (heap_node_t*)((u8_t*)usr_ptr - sizeof(heap_node_t));
    if(! IS_BLOCK_FREE(block))
    	return;  // double free
    SET_BLOCK_FREE(block);

    // Merge with previous block if it's free
    if (block->prev != NULL && IS_BLOCK_FREE(block->prev))
    {
        block->prev->blockSize += block->blockSize;
        block->prev->next = block->next;

        block = block->prev; // Update block to merged one

        if (block->next)
        {
            block->next->prev = block;
        }
    }

    // Merge with next block if it's free
    if (block->next != NULL && IS_BLOCK_FREE(block->next))
    {
        block->blockSize += block->next->blockSize;
        block->next = block->next->next;

        if (block->next != NULL)
        {
            block->next->prev = block;
        }
    }
}


















