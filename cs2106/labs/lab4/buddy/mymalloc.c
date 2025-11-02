#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"
#define ORDER_TO_KB(X) (1 << (X))
#define BYTES_TO_KB(X) ((X) >> LOG_MINIMUM_BLOCK)
#define KB_TO_BYTES(X) ((X) << LOG_MINIMUM_BLOCK)
#define ALLOCATED 1
#define FREE 0

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length

static TNode* free_lists[MAX_ORDER] = {0};
int initialised = 0;

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
static void setupHeapRegion() {
  insert_node(&free_lists[MAX_ORDER - 1], make_node(0, NULL), ASCENDING);
  initialised = 1;
}

long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

long get_size(void *ptr) {
  long idx = get_index(ptr);
  if (idx == -1) return -1;
  TNode *block = find_node(_memlist, BYTES_TO_KB(idx));
  if (!block || block->pdata == NULL) return -1;
  return KB_TO_BYTES(block->pdata->val);
}

void print_memlist() {
  for (int order = MAX_ORDER - 1; order >= 0; order--) {
    unsigned sizeKB = ORDER_TO_KB(order);
    printf("Block size %u KB: ", sizeKB);
    
    // sorted list
    TNode *sorted = NULL;

    // iterate over blocks
    for (TNode* curr = _memlist; curr != NULL; curr = curr->next) {
      unsigned allocKB = curr->pdata->val; 
      unsigned offsetKB   = curr->key;
      if (allocKB <= sizeKB) {
        unsigned startKB = (offsetKB / sizeKB) * sizeKB;
        if (!find_node(sorted, startKB)) { // remove duplicates from shared ancestors
          TData *d = (TData*)malloc(sizeof(TData));
          d->val = ALLOCATED;
          insert_node(&sorted, make_node(startKB, d), ASCENDING);
        }
      }
    }

    for (TNode* curr = free_lists[order]; curr != NULL; curr = curr->next) {
      TData *d = (TData*)malloc(sizeof(TData));
      d->val = FREE;
      insert_node(&sorted, make_node(curr->key, d), ASCENDING);
    }

    // print out sorted order by offset, then cleanup
    for (TNode* curr = sorted; curr != NULL; curr = curr->next) {
      if (curr->pdata->val == ALLOCATED)
        printf("ALLOCATED, %u, %u -> ", curr->key, sizeKB);
      else
        printf("FREE, %u, %u -> ", curr->key, sizeKB);
      free(curr->pdata);
    }
    purge_list(&sorted);

    printf("\n");
  }
}

static unsigned minimum_order(size_t size) {
  int order = 0;
  int sizeKB = BYTES_TO_KB(size);
  while (ORDER_TO_KB(order) < sizeKB) order++;
  return order;
}

static unsigned buddy_of(unsigned offset, unsigned order) {
  return offset ^ ORDER_TO_KB(order); // flip bit at order
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
  if (size == 0) return NULL;
  if (!initialised) setupHeapRegion();

  unsigned need = minimum_order(size);
  unsigned order = need;
  while (order < MAX_ORDER && free_lists[order] == NULL) order++;
  if (order >= MAX_ORDER) return NULL;

  TNode *largest = free_lists[order];
  unsigned offsetKB = largest->key;
  delete_node(&free_lists[order], largest);

  // splitting down until minimum needed order is achieved
  while (order > need) {
    order--;
    unsigned right = offsetKB + ORDER_TO_KB(order);
    insert_node(&free_lists[order], make_node(right, NULL), ASCENDING);
  }

  TData* data = (TData*)malloc(sizeof(TData));
  data->val = ORDER_TO_KB(order);
  insert_node(&_memlist, make_node(offsetKB, data), ASCENDING);

  return (void*)(&_heap[0] + KB_TO_BYTES(offsetKB));
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
  if (!ptr) return;
  if (!initialised) setupHeapRegion();

  unsigned idxKB = BYTES_TO_KB(get_index(ptr));
  TNode* block = find_node(_memlist, idxKB);
  if (!block || !block->pdata) return;

  unsigned sizeKB = block->pdata->val;
  free(block->pdata);
  delete_node(&_memlist, block);

  // coalesce from current order up
  unsigned order = minimum_order(KB_TO_BYTES(sizeKB));
  while (order < MAX_ORDER - 1) {
    unsigned buddyKB = buddy_of(idxKB, order);
    TNode *buddy_block = find_node(free_lists[order], buddyKB);
    if (!buddy_block) break; // buddy not free, so stop coalesce chain
    delete_node(&free_lists[order], buddy_block); 
    idxKB = (buddyKB < idxKB) ? buddyKB : idxKB; // choose lower childs start as the start of the parent
    order++;
  }

  insert_node(&free_lists[order], make_node(idxKB, NULL), ASCENDING);
}

