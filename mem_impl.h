#ifndef MEM_IMPL_H
#define MEM_IMPL_H

#define MALLOC_SIZE 8000
#define NODE_SIZE sizeof(FreeMemoryNode)

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// A single free memory node to be
// used for a free memory list.
// next: The next free memory block
// size: The size of free memory on this block in bytes
typedef struct FreeMemoryNode {
  struct FreeMemoryNode* next;
  uintptr_t size;
} FreeMemoryNode;

extern FreeMemoryNode* free_block_list;

// Functions prototypes

// Check for possible problems with the free list data structure. 
void check_heap();

// Prints information about the heap's
// free blocks to a given file f
void print_heap(FILE* f);

#endif
