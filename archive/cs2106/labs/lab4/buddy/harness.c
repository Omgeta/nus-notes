#include <stdio.h>
#include <assert.h>
#include "mymalloc.h"

void *runmalloc(size_t len, long expected_ndx) {

    long ndx;
    void *ptr;

    printf("Allocate %ld KB.\n", len / MINIMUM_BLOCK_SIZE);
    printf("Before: \n");
    print_memlist();
    printf("\n");
    ptr = mymalloc(len);
    ndx = get_index((void *) ptr);
    printf("EXPECTED: %ld ACTUAL: %ld\n", expected_ndx / MINIMUM_BLOCK_SIZE, ndx / MINIMUM_BLOCK_SIZE);
    printf("After:  \n");
    print_memlist();
    printf("\n");
    assert(ndx == expected_ndx);
    return ptr;
}

void runfree(void *ptr) {
    printf("Free at address %ld of size %ld KB\n", get_index(ptr) / MINIMUM_BLOCK_SIZE, get_size(ptr) / MINIMUM_BLOCK_SIZE);
    printf("Before: \n");
    print_memlist();
    printf("\n");
    myfree(ptr);
    printf("After:  \n");
    print_memlist();
    printf("\n");
}

int main() {
    
    void *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;

    ptr1 = runmalloc(512 * MINIMUM_BLOCK_SIZE, 0 * MINIMUM_BLOCK_SIZE);
    ptr2 = runmalloc(120 * MINIMUM_BLOCK_SIZE, 512 * MINIMUM_BLOCK_SIZE);
    ptr3 = runmalloc(100 * MINIMUM_BLOCK_SIZE, 640 * MINIMUM_BLOCK_SIZE);
    runfree(ptr2);
    ptr2 = runmalloc(80 * MINIMUM_BLOCK_SIZE, 512 * MINIMUM_BLOCK_SIZE);
    ptr4 = runmalloc(60 * MINIMUM_BLOCK_SIZE, 768 * MINIMUM_BLOCK_SIZE);
    ptr5 = runmalloc(30 * MINIMUM_BLOCK_SIZE, 832 * MINIMUM_BLOCK_SIZE);
    runfree(ptr1);
    ptr1 = runmalloc(2 * MINIMUM_BLOCK_SIZE, 864 * MINIMUM_BLOCK_SIZE);
    runfree(ptr1);
    runfree(ptr2);
    runfree(ptr3);
    runfree(ptr4);
    runfree(ptr5);
}


