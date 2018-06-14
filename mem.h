#ifndef MEM_H
#define MEM_H


#include <stdio.h>
#include <inttypes.h>

extern uintptr_t total_mem;
extern uintptr_t total_free_mem;
extern uintptr_t num_free_blocks;

// Functions prototype

// Return a pointer to a block of memeory with the address
// being the multiple of 16 and the size equal or slightly
// larger than size field
void* getmem(uintptr_t size);

// Return the block of storage at location p to the pool 
// of available free storage. 
void freemem(void* p);

// Store statistics about the current state of the memory manager
// in the three integer variables whose addresses are given as arguments.
void get_mem_stats(uintptr_t* total_size, 
		   uintptr_t* total_free,
		   uintptr_t* n_free_blocks);

// Print a formatted listing on file f showing the blocks on the free list. 
void print_heap(FILE* f);

#endif


