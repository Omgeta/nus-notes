
#define MAX_FNAME_LEN 64

typedef struct llist {
    char filename[MAX_FNAME_LEN];
    int filesize;
    int startblock;
    struct llist *prev, *next;
} TLinkedList;

// Fill in the prototypes below
void init_llist(TLinkedList **head);
TLinkedList *create_node(char *filename, int filesize, int startblock);
void insert_llist(TLinkedList **head, TLinkedList *node);
void delete_llist(TLinkedList **head, TLinkedList *node);
TLinkedList *find_llist(TLinkedList *head, char *fname);
void traverse(TLinkedList **head, void (*fn)(TLinkedList *));
