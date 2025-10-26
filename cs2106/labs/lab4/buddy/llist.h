
// Uncomment the next line to enable debug printing
#define DEBUG       // Enable debug printing

// The debug printer; used like a normal printf, except
// that printing can be turned off by commenting out the
// #define DEBUG above.
void dbprintf(char *format, ...);

// You should modify this structure to hold
// whatever you need to implement your
// memory manager. You can delete the
// val field. It is only used for testlist.c
typedef struct td {
    int val;
} TData;


/* ----------------------------------------- 
   BASIC ROUTINES

   Basic linked list routines

   ---------------------------------------- */

// Basic double linked list node. 

typedef struct tn {
    unsigned int key;
    TData *pdata; // Pointer to the data you want to store

    struct tn *trav; // Only used in the root for traversal
    struct tn *tail; // Only used in the root for finding the end of the list
    struct tn *prev;
    struct tn *next;
} TNode;

// Insert Direction
#define ASCENDING   0
#define DESCENDING  1

// Merge direction
#define PRECEDING   0
#define SUCCEEDING  1

// Traverser position
#define FRONT       0
#define REAR        1

// Create a new node. You need to
// Create your own TData node, populate
// it, then create a new node with a suitable
// key. Insertion into the link list is
// by ascending order of the key. An example key
// might be the starting address of a memory segment.

TNode *make_node(unsigned int key, TData *data);

// Inserts a node into the correct point of the
// double linked list. The list is sorted
// in ascending order of the key. Duplicate keys
// are permitted, though not recommended.
// llist = Pointer to link list
// node = Pointer to node created by make_node
// dir = 0: Insert in ascending order
// dir = 1: Insert in descending order

void insert_node(TNode **llist, TNode *node, int dir);


// Remove a given node from the linked list
void delete_node(TNode **llist, TNode *node);

// Find a node that has the value of key
// If there are duplicate keys, the first one encountered
// will be returned.
TNode *find_node(TNode *llist, unsigned int key);

// Merge the node provided with either the node after or the node before.
// You need to manage merging the data in node->pdata yourself. This code just
// deletes the larger of the two nodes.
// dir = 0: Merge with node before
// dir = 1: Merge with node after

void merge_node(TNode *llist, TNode *node, int dir);

// Purge the entire list. You must
// free any dynamic data in the TData
// struct yourself.
void purge_list(TNode **llist);

/* ----------------------------------------- 
   TRAVERSAL ROUTINES

   Lets you traverse the linked list

   ---------------------------------------- */

// Go over every element of llist, and call func
// func prototype is void func(TNode *);

void process_list(TNode *llist, void (*func)(TNode *));

// Reset traverser
// where=0 START: Resets traverser to start of list
// where=1 END: Rsets
void reset_traverser(TNode *llist, int where);

// Get the next node
TNode *succ(TNode *llist);

// Get the previous node
TNode *pred(TNode *llist);
