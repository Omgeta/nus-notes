#include <stdio.h>
#include "mymalloc.h"
#include "llist.h"

void testalloc(long size, char *ptrname, char **ptr) {
    printf("\nAllocating %ld KBytes to %s\n", size >> 10, ptrname);
    *ptr = mymalloc(size);

    if(*ptr == NULL) {
        printf("Allocation failed.\n");
    } else {
        printf("Allocated %ld KB successfully to %s\n", size >> LOG_MINIMUM_BLOCK, ptrname);
    }
    print_memlist();
}

void testfree(char *ptr, char *ptrname) {
    printf("\nFreeing %s\n", ptrname);
    myfree(ptr);
    print_memlist();
}

int main() {
    char *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;

    testalloc(512 * MINIMUM_BLOCK_SIZE, "ptr1", &ptr1);
    testalloc(120 * MINIMUM_BLOCK_SIZE, "ptr2", &ptr2);
    testalloc(100 * MINIMUM_BLOCK_SIZE, "ptr3", &ptr3);
    testfree(ptr2, "ptr2");
    testalloc(80 * MINIMUM_BLOCK_SIZE, "ptr2", &ptr2);
    testalloc(280 * MINIMUM_BLOCK_SIZE, "ptr4", &ptr4);
    testfree(ptr2, "ptr2");
    testalloc(30 * MINIMUM_BLOCK_SIZE, "ptr5", &ptr5);
    testfree(ptr1, "ptr1");
    testfree(ptr3, "ptr3");
    testfree(ptr4, "ptr4");
    testfree(ptr5, "ptr5");

}
