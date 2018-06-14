/*
 * mem_utils.c contains the implementations
 * of various utility methods
 */

#include <assert.h>
#include "mem_impl.h"

void check_heap() {
  FreeMemoryNode* cur = free_block_list;
  while (cur != NULL) {
    if (cur->next != NULL) {
      assert((cur->next - cur) > 0);
    }
    if (cur != free_block_list) {
        assert(cur->size >= NODE_SIZE);
    }
    if (cur->next != NULL) {
      assert((cur + 1 + cur->size/NODE_SIZE) <= cur->next);
    }
    cur = cur->next;
  }
}
