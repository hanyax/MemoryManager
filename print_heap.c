/*
 * print_heap.c contains the internal
 * implementation of print_heap
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "mem_impl.h"

void print_heap(FILE* f) {
  
  if (f == NULL) {
    fprintf(stderr, "Invalid file given\n");
    return;
  }
  
  if (free_block_list != NULL) {    
    //Start at next to ignore initial dummy node
    FreeMemoryNode* current = free_block_list->next;
    while (current != NULL) {
      fprintf(f, "Address: %p | Length: %lu\n",
              current, current->size);
      current = current->next;
    }
  }
}
