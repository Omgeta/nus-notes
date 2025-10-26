#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "llist.h"

#define COUNT   10

void print_node(TNode *node) {
    if(node != NULL)
        printf("Key: %d\n", node->key);
    else
        printf("Unable to find key.\n");
}

void free_data(TNode *node) {
    free(node->pdata);
}

void trav_list(TNode *llist) {
    reset_traverser(llist, FRONT);
    TNode *node;

    do {
        node = succ(llist);
        if(node)
            print_node(node);
    } while(node != NULL);
}

void rev_trav_list(TNode *llist) {
    reset_traverser(llist, REAR);
    TNode *node;

    do{
        node = pred(llist);
        if(node)
            print_node(node);
    } while(node != NULL);
}


int main(){
    TNode *llist = NULL;

    int test_array[COUNT] = {10, 6, 2, 8, 1, 4, 5, 3, 9, 7};

    // Insert

    char c;
    printf("Hit enter to start ascending order test\n");
    scanf("%c", &c);

    int i;
    printf("Inserting in ascending order\n");
    TData *data;
    TNode *node;

    for(i=0; i<COUNT; i++) {
        data = (TData *) malloc(sizeof(TData));
        data->val = test_array[i];
        node = make_node(test_array[i], data);
        insert_node(&llist, node, ASCENDING);
    }

    printf("Printing entire list\n");
    process_list(llist, print_node);
    printf("\n");

    printf("Printing entire list using reverse traversal\n");
    rev_trav_list(llist);
    printf("\n");

    printf("Searching for 2\n");
    node = find_node(llist, 2);
    print_node(node);

    printf("Deleting node 2\n");
    delete_node(&llist, node);

    printf("Searching for 2\n");
    node = find_node(llist, 2);
    print_node(node);
    printf("\n");

    printf("Searching for 5\n");
    node = find_node(llist, 5);
    print_node(node);

    printf("Deleting node 5\n");
    delete_node(&llist, node);

    printf("Searching for 5\n");
    node = find_node(llist, 5);
    print_node(node);
    printf("\n");

    printf("Searching for 22\n");
    node = find_node(llist, 22);
    print_node(node);

    printf("Deleting node 22\n");
    delete_node(&llist, node);

    printf("Searching for 22\n");
    node = find_node(llist, 22);
    print_node(node);
    printf("\n");

    printf("Purging list\n");
    process_list(llist, free_data);
    purge_list(&llist);

    printf("Printing entire list\n");
    process_list(llist, print_node);
    printf("\n");

    printf("Hit enter to start descending order test\n");
    scanf("%c", &c);

    printf("Inserting in descending order\n");
    for(i=0; i<COUNT; i++) {
        TNode *node = make_node(test_array[i], NULL);
        insert_node(&llist, node, DESCENDING);
    }

    printf("Printing entire list\n");
    process_list(llist, print_node);
    printf("\n");

    printf("Printing entire list using reverse traversal\n");
    rev_trav_list(llist);
    printf("\n");

    printf("Searching for 2\n");
    node = find_node(llist, 2);
    print_node(node);

    printf("Deleting node 2\n");
    delete_node(&llist, node);

    printf("Searching for 2\n");
    node = find_node(llist, 2);
    print_node(node);
    printf("\n");

    printf("Searching for 5\n");
    node = find_node(llist, 5);
    print_node(node);

    printf("Deleting node 5\n");
    delete_node(&llist, node);

    printf("Searching for 5\n");
    node = find_node(llist, 5);
    print_node(node);
    printf("\n");

    printf("Searching for 22\n");
    node = find_node(llist, 22);
    print_node(node);

    printf("Deleting node 22\n");
    delete_node(&llist, node);

    printf("Searching for 22\n");
    node = find_node(llist, 22);
    print_node(node);
    printf("\n");

    printf("Purging list\n");
    process_list(llist, free_data);
    purge_list(&llist);

    printf("Printing entire list\n");
    process_list(llist, print_node);
    printf("\n");

}
