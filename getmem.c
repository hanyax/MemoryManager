/*
 * getmem.c contains the internal
 * implementation for getmem 
 */

#include <stdlib.h>
#include "mem.h"
#include "mem_impl.h"

FreeMemoryNode* free_block_list = NULL;
uintptr_t total_mem = 0;
uintptr_t total_free_mem = 0;
uintptr_t num_free_blocks = 0;

//Searches for a free memory block that has at least a given size
//size: The size desired
FreeMemoryNode* search_free_block(uintptr_t size);

void* getmem(uintptr_t size) {

  check_heap();

  if (size < 0) {
    return NULL;
  }
  
  if (free_block_list == NULL) {
    free_block_list = (FreeMemoryNode*) malloc(sizeof(FreeMemoryNode));
    free_block_list->size=0;
    free_block_list->next=NULL;
    total_mem += sizeof(FreeMemoryNode);
    total_free_mem += sizeof(FreeMemoryNode);
  }
  
  // Calculate size needed to be allocated
  uintptr_t need_size;
  if (size % NODE_SIZE == 0) {
    need_size = size;
  } else {
    need_size = (size + 1) * NODE_SIZE;
  }

  FreeMemoryNode* prev_block =  search_free_block(need_size);
  FreeMemoryNode* found_block = prev_block->next;
  uintptr_t alloc_size;
  uintptr_t found_size = found_block->size;
  FreeMemoryNode* new_block;
 
  // calculate size need to be allocate
  if (found_size - need_size >= (NODE_SIZE * 2)) {
    // break that memory
    alloc_size = need_size;
    new_block = (found_block + 1 + need_size/NODE_SIZE);
    new_block->size = (found_size - alloc_size - NODE_SIZE);
    new_block->next = found_block->next;

  } else { // allocate that entire block
    alloc_size = found_size;
    new_block = found_block->next;
    num_free_blocks = num_free_blocks - 1;
  }

  // link new block 
  prev_block->next = new_block;
  
  // override found_block
  found_block->next = NULL;
  found_block->size = alloc_size;
  total_free_mem-=alloc_size;
  
  check_heap();
  return  ((void*)found_block + (uintptr_t)16 );
}

FreeMemoryNode* search_free_block(uintptr_t size) {

  FreeMemoryNode* current = free_block_list;  
  while (current->next != NULL) {
    if ((current->next)->size > size) {
      return current;
    } else {
      current = current->next;
    }
  }
  
  FreeMemoryNode* block_ptr;
  if (size <= (MALLOC_SIZE - NODE_SIZE)) {
    block_ptr = (FreeMemoryNode*) malloc(MALLOC_SIZE);
    block_ptr->size = MALLOC_SIZE - NODE_SIZE;
    total_mem += MALLOC_SIZE;
    total_free_mem += MALLOC_SIZE;
  } else {
    block_ptr = (FreeMemoryNode*) malloc(size + NODE_SIZE);
    block_ptr->size = size;
    total_mem += (size + NODE_SIZE);
    total_free_mem += (size + NODE_SIZE);
  }
  
  num_free_blocks = num_free_blocks + 1;
  block_ptr->next = NULL;
  current->next = block_ptr;
  return current;
}
