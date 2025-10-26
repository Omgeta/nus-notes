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

    return 1;
}

void print_memlist() {
    // To implement your own print_memlist function.
    // This function should print out the status (FREE or ALLOCATED),
    // starting index (offset from the base address), and length of each block.
}



// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    // To implement your own myfree function.
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    // To implement your own myfree function.
    
}


