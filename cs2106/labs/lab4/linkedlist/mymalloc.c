// mymalloc.c
// Lab 4 Exercise 1
// Your implementation goes in this file.
// You may add helper functions as necessary.

#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

// Meta-data about the heap
// This structure holds information about the entire heap region, including
// the base address, total size, and size of each block's meta-data.
static HeapInfo _heap;

static HeapInfo* first_block() {
  return _heap.next;
}

static HeapInfo* split_block(HeapInfo* block, unsigned int need) {
  unsigned int remainder_size = block->total_size - need;
  if (remainder_size > sizeof(HeapInfo)) {  // atleast 1 byte left over after space
    HeapInfo* remainder = (HeapInfo*)((char*)block->base_address + need); 
    remainder->base_address = (void*)remainder;
    remainder->total_size = remainder_size;
    remainder->next = block->next;
    remainder->status = FREE;

    block->total_size = need; 
    block->next = remainder;
  } 

  return block;
} 

static void coalesce_blocks() {
  HeapInfo *curr = first_block();
  while (curr != NULL) {
    if (curr->status == FREE) {
      while (curr->next != NULL && curr->next->status == FREE) {
        curr->total_size += curr->next->total_size;
        curr->next = curr->next->next;
      }
    }
    curr = curr->next;
  }
}

static HeapInfo* find_best_fit(unsigned int need) {
  HeapInfo *best = NULL;
  for (HeapInfo *curr = first_block(); curr != NULL; curr = curr->next) {
    if (curr->status == FREE && curr->total_size >= need &&
      (best == NULL || curr->total_size < best->total_size))
      best = curr;
  }
  return best;
}

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
unsigned int get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (unsigned int) (ptr - (void *)_heap.base_address);
}


// Setup the heap region. Returns 1 on success, 0 on failure.
int setupHeapRegion() {
  void* base_address;
	base_address = sbrk(0);
	if(sbrk(MEMSIZE) == (void*) - 1 ) {
		printf("Cannot set break! Behavior undefined!\n");
		return 0;
	}

  _heap.base_address = base_address;
  _heap.total_size = MEMSIZE;

  HeapInfo *first = (HeapInfo*) base_address;
  first->base_address = base_address;
  first->total_size = MEMSIZE;
  first->next = NULL;
  first->status = FREE;

  _heap.next = first;
  _heap.status = FREE;

  return 1;
}

void print_memlist() {
  // To implement your own print_memlist function.
  // This function should print out the status (FREE or ALLOCATED),
  // starting index (offset from the base address), and length of each block.
  HeapInfo *curr = first_block(); 
  while (curr != NULL) {
    printf("Status: %s Start index: + %u Length: %lu\n", 
           curr->status == FREE ? "FREE" : "ALLOCATED", 
           get_index(curr->base_address), 
           curr->total_size - (curr->status == FREE ? 0 : sizeof(HeapInfo)));
    curr = curr->next; 
  }
}

static void print_heap_info() {
  printf("Total Size = %u bytes\nStart Address = %p\nPartition Meta Size = %lu bytes\n", 
       _heap.total_size, (void*)_heap.base_address, sizeof(HeapInfo));
}



// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
  // To implement your own myfree function.
  if (size == 0 || size > MEMSIZE - sizeof(HeapInfo)) return NULL;
  if (first_block() == NULL) setupHeapRegion();

  unsigned int need = sizeof(HeapInfo) + size;
  HeapInfo *best = find_best_fit(need);
  if (best == NULL) return NULL;

  print_heap_info();
  
  best = split_block(best, need);
  best->status = OCCUPIED;
  return (void*) ((char*)best->base_address + sizeof(HeapInfo));
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
  // To implement your own myfree function. 
  if (ptr == NULL) return;
  if (ptr < _heap.base_address || ptr >= _heap.base_address + _heap.total_size) return;

  print_heap_info();

  HeapInfo *block = (HeapInfo*)(ptr - sizeof(HeapInfo));
  block->status = FREE;
  
  coalesce_blocks();
}


