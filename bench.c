/*
 * This program executes a large number
 * of calls to functions getmem and freemem
 * to allocate and free blocks of random
 * sizes and in random order
 */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "mem.h"
#include "mem_impl.h"

// Performs benchmark trials
// based on the given parameters
// parameters[]: The parameters to be used
void perform_trials(int parameters[]); 

// Performs a memory allocation trial for the benchmark
// allocated_mem_ptrs: The pointers to allocated memory so far
// used_mem_blocks: The number of allocated pointers so far
// small_limit: The largest size in bytes of a small block
// large_limit: The largest size in bytes of a large block
// pctlarge: The percent of blocks that will be large blocks
void allocate_trial(void** allocated_mem_ptrs, int *used_mem_blocks,
                    int small_limit, int large_limit, int pctlarge);

// Performs a free memory trial for the benchmark
// allocated_mem_ptrs: The pointers to allocated memory so far
// used_mem_blocks: The number of allocated pointers so far
void free_trial(void** allocated_mem_ptrs, int *used_mem_blocks);

int main(int argc, char** argv) {
  if (argc > 7) {
    fprintf(stderr, "%s\n", "Too many arguments");
    fprintf(stderr, "Expected use: bench %s %s\n",
            "[ntrials] [pctget] [pctlarge]",
            "[small_limit] [large_limit] [random_seed]");
    exit(EXIT_FAILURE);
  }

  time_t time_of_day;

  int parameters[6] = {10000,               // 0: ntrials
                       50,                  // 1: pctget
                       10,                  // 2: pctlarge
                       200,                 // 3: small_limit
                       20000,               // 4: large_limit
                       time(&time_of_day)}; // 5: random_seed

  // Initialize user set parameters
  for (int i = 0; i < argc - 1; i++) {
    int value = atoi(argv[i + 1]);
    parameters[i] = value;
  }

  perform_trials(parameters);

  return 0;
}

void perform_trials(int parameters[]) {

  int used_mem_blocks = 0;
  void** allocated_mem_ptrs = (void**) 
                               malloc(sizeof(void*) * parameters[0]);

  srand(parameters[5]);
  clock_t start = clock();
  
  for (int i = 0; i < parameters[0]; i++) {

    int random_value = (rand() % 100) + 1;
    int checkpoint = parameters[0] / 10;

    if (random_value <= parameters[1]) {
      // get mem case
      allocate_trial(allocated_mem_ptrs, &used_mem_blocks,
                     parameters[3], parameters[4], parameters[2]);

    } else {
      // free mem case
      free_trial(allocated_mem_ptrs, &used_mem_blocks);
    }
    
    if ((i != 0 && i % checkpoint == 0) || i == parameters[0] - 1) {
      // Print
      clock_t current = clock();
      fprintf(stdout, "Total CPU time: %f ms\n",
              (double) ((current - start) * 1000 / CLOCKS_PER_SEC));

      uintptr_t total_size = 0;
      uintptr_t total_free = 0;
      uintptr_t n_free_blocks = 0;
      get_mem_stats(&total_size, &total_free, &n_free_blocks);

      fprintf(stdout, "Total memory: %lu\n", total_size);
      fprintf(stdout, "Number of free blocks: %lu\n", n_free_blocks);
      if (n_free_blocks != 0) {
	fprintf(stdout, "Average memory per block: %lu\n", 
		(total_free)/(n_free_blocks));
      } else {
	fprintf(stdout, "Average memory per block: 0\n");
      }
      fprintf(stdout, "\n");
    }
  }
}

void allocate_trial(void** allocated_mem_ptrs, int *used_mem_blocks,
                    int small_limit, int large_limit, int pctlarge) {

  int random_value = (rand() % 100) + 1;
  uintptr_t random_size;

  if (random_value <= pctlarge) {

    // Allocate large block, set size for large block 
    random_size = (rand() % (large_limit - small_limit + 1)) + small_limit;

  } else {

    // Allocate small block, set size for small block
    random_size = (rand() % small_limit) + 1;
  }

  unsigned char* address = getmem(random_size);

  if (address != NULL) {
    
    if (random_size < 16) {
      for (int i = 0; i < random_size; i++) {
	*(address+i) = 0xFE;
      }
    } else {
      for (int i = 0; i < 16; i++) {
	*(address+i) = 0xFE;
      }
    }

    allocated_mem_ptrs[*used_mem_blocks] = address;
    (*used_mem_blocks)++;
  }
}

void free_trial(void** allocated_mem_ptrs, int *used_mem_blocks) {

  if (*used_mem_blocks != 0) {

    int random_index = (rand() % *used_mem_blocks);
    void* random_ptr = allocated_mem_ptrs[random_index];

    freemem(random_ptr);

    allocated_mem_ptrs[random_index] = allocated_mem_ptrs[*used_mem_blocks - 1];
    allocated_mem_ptrs[*used_mem_blocks - 1] = NULL;
    (*used_mem_blocks)--;
  }
}
