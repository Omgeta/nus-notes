#define MEMSIZE 1024 * 1024         // Size of memory in bytes
#define MAX_ORDER 11                // Max memory size is 1024KB, smallest block is 1KB
#define LOG_MINIMUM_BLOCK 10        // 1 KB minimum block size order
#define MINIMUM_BLOCK_SIZE 1024

long get_index(void *ptr);
long get_size(void *ptr);
void *mymalloc(size_t);
void myfree(void *);
