/*
 * freemem.c contains the internal
 * implementation for freemem
 */

#include "mem.h"
#include "mem_impl.h"

// Combines two given free memory blocks
// prev: The block before the desired block to combine
// block: The desired block to combine
void combine_blocks(FreeMemoryNode* prev, FreeMemoryNode* block);

void freemem(void* p) {
  check_heap();

  if (p == NULL) {
    return;
  }
  FreeMemoryNode* free_block = (FreeMemoryNode*) (p - (uintptr_t) NODE_SIZE);
  total_free_mem += (free_block->size + NODE_SIZE);
  num_free_blocks = num_free_blocks + 1;

  // free_block_list is empty (just contains "dummy" pointer)
  if (free_block_list->next == NULL) {

    free_block->next = NULL;
    free_block_list->next = free_block;
    return;

  // else free_block_list is not empty, loop through to find a spot to put block
  } else {

    // Place node at front of list
    if ((uintptr_t) ((void*) free_block_list->next) > (uintptr_t) p) {

      free_block->next = free_block_list->next;
      free_block_list->next = free_block;

      if (free_block_list->next->next != NULL) {
	       // Call two times to make sure special case
        combine_blocks(free_block_list->next, free_block_list->next->next);

        // If there is nothing left we might combine null
        combine_blocks(free_block_list->next, free_block_list->next->next);
      }

    // else iterate through list
    } else {

      // Skip past dummy pointer
      FreeMemoryNode* current = free_block_list->next;

      while (current->next != NULL) {

        // Suitable spot to put free block between
        if ((uintptr_t) ((void*) current) < (uintptr_t) p && (uintptr_t)
	    ((void*) current->next) > (uintptr_t) p) {

          free_block->next = current->next;
          current->next = free_block;

	  // Call two times to make sure special case
          combine_blocks(current, current->next);
	        combine_blocks(current, current->next);
          return;

        } else {
          // Keep looking for a spot to put free block
          current = current->next;
        }
      }

      // Reached end of list, free_block must be put at the end.
      free_block->next = NULL;
      current->next = free_block;

      // Call two times to make sure special case
      combine_blocks(current, current->next);
    }
  }
  check_heap();
}


void combine_blocks(FreeMemoryNode* prev, FreeMemoryNode* block) {

  if (block != NULL) {

    uintptr_t prev_end_address = (uintptr_t)
      (prev + 1 + prev->size/NODE_SIZE);

    uintptr_t block_start_address = (uintptr_t) (block);

    uintptr_t block_end_address = (uintptr_t)
      (block + 1 + block->size/NODE_SIZE);

    uintptr_t next_start_address = (uintptr_t) (block->next);
    int combineFront = 0;
    if (prev_end_address == block_start_address) {
      prev->size = prev->size + block->size + NODE_SIZE;
      prev->next = block->next;
      combineFront = 1;
    }
    int combineBack = 0;
    if (block_end_address == next_start_address) {
      block->size = block->size + block->next->size + NODE_SIZE;
      block->next = block->next->next;
      combineBack = 1;
    }
    if ((combineBack + combineFront) > 0) {
      num_free_blocks -= 1;
    }
  }
}
