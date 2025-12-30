// mymalloc.h
// Lab 4 Exercise 1
// Your implementation goes in this file.
// Do not change the function prototypes given.
// You may add helper functions as necessary.

#ifndef MYMALLOC_H
#define MYMALLOC_H

#include <unistd.h>

#define MEMSIZE (64 << 10)    // Size of memory in bytes
#define FREE 0
#define OCCUPIED 1

// Meta-data about the heap
// Please change this structure to suit your needs.
typedef struct BlockInfo {
  struct BlockInfo *next;
  unsigned int size;
  char status;
} BlockInfo;

typedef struct HeapInfo {
    struct BlockInfo *first;
    void *base_address;
    unsigned int total_size;
} HeapInfo;


int setupHeapRegion();
unsigned int get_index(void *ptr);
void *mymalloc(size_t);
void myfree(void *);

// Debugging routine
void print_memlist();

#endif // MYMALLOC_H
