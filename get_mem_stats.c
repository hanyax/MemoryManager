/*
 * get_mem_stats.c contains the internal
 * implementation of get_mem_stats
 */

#include "mem.h"

void get_mem_stats(uintptr_t* total_size,
                   uintptr_t* total_free,
                   uintptr_t* n_free_blocks) {
  *(total_size) = total_mem;
  *(total_free) = total_free_mem;
  *(n_free_blocks) = num_free_blocks;
}
