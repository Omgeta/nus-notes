#include "llist.h"
#include <stdio.h>
#include <string.h>

#define NUM_FNAMES 6

// Data structure to store file information
typedef struct fstr {
    char filename[MAX_FNAME_LEN];
    int filesize;
    int startblock;
} TFileData;

// Helper function to print out every node
void print_node(TLinkedList *node) {
    printf("Filename: %s Filesize: %d Starting Block: %d\n", node->filename,
           node->filesize, node->startblock);
}

int main() {
    TLinkedList *head = NULL;

    TFileData fnames[NUM_FNAMES] = {
        {"test.txt", 32, 0},  {"hello.txt", 172, 93}, {"a.exe", 2384, 381},
        {"c.exe", 8475, 123}, {"d.tmp", 8374, 274},   {"e.bin", 283, 8472}};

    printf("Initializing.\n");
    // Initialize the head. Redundant here but good practice anyway.
    init_llist(&head);

    // Try to traverse the list
    printf("Traversing empty list\n");
    traverse(&head, print_node);

    printf("\nEntering filenames\n");
    // Now enter the filenames
    for (int i = 0; i < NUM_FNAMES; i++) {
        TLinkedList *node = create_node(fnames[i].filename, fnames[i].filesize,
                                        fnames[i].startblock);
        printf("Inserting %s\n", node->filename);
        insert_llist(&head, node);
    }

    // Now print out everything
    printf("\nFilenames after insertion: \n");
    traverse(&head, print_node);

    // Now delete everything
    TLinkedList *trav = head, *next;

    printf("\nDeleting list.\n");
    while (trav != NULL) {
        printf("Deleting %s\n", trav->filename);

        // Save the next pointer before deleting trav
        next = trav->next;

        // Delete trav
        delete_llist(&head, trav);

        // Move to the next node
        trav = next;
    }

    printf("\nPrinting after deletion: \n");
    traverse(&head, print_node);
}
