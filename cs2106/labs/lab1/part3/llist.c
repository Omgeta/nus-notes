// Implement a double linked-list

#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize the linked list by setting head to NULL
// PRE: head is the pointer variable that points to the
//      first node of the linked list
// POST: head is set to NULL

void init_llist(TLinkedList **head) {
  *head = NULL;
}

// Create a new node
// PRE: filename = name of file to be inserted
//      filesize = size of file in blocks
//      startblock = File's starting block
// RETURNS: A new node containing the file information is created.

TLinkedList *create_node(char *filename, int filesize, int startblock) {
  TLinkedList *node = (TLinkedList *)malloc(sizeof(TLinkedList));
  strcpy(node->filename, filename);
  node->filesize = filesize;
  node->startblock = startblock;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

// Insert node into the end of the linkedlist indicated by head
// PRE: head = Pointer variable pointing to the start of the linked list
//      node = Node created using create_node
// POST: node is inserted into the linked list.

void insert_llist(TLinkedList **head, TLinkedList *node) {
  if (*head == NULL) {
    *head = node;
  } else {
    TLinkedList *cur = *head;
    while (cur->next != NULL) cur = cur->next;
    cur->next = node;
    node->prev = cur;
  }
}

// Delete node from the linkedlist
// PRE: head = Pointer variable pointing to the start of the linked list
//      node = An existing node in the linked list to be deleted.
// POST: node is deleted from the linked list

void delete_llist(TLinkedList **head, TLinkedList *node) {
  if (*head == NULL) return;

  if (node -> prev) 
    node->prev->next = node->next;
  else 
    *head = node->next;

  if (node -> next)
    node->next->prev = node->prev;  

  free(node);
}

// Find node in the linkedlist
// PRE: head = Variable that points to the first node of the linked list
//      fname = Name of file to look for
// RETURNS: The node that contains fname, or NULL if not found.

TLinkedList *find_llist(TLinkedList *head, char *fname) {
  for (TLinkedList *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(cur->filename, fname) == 0) return cur;
  }
  return NULL;
}

// Traverse the entire linked list calling a function
// PRE: head = Variable pointing to the first node of the linked list
//      fn = Pointer to function to be called for each node
// POST: fn is called with every node of the linked list.

void traverse(TLinkedList **head, void (*fn)(TLinkedList *)) {
  for (TLinkedList *cur = *head; cur != NULL; cur = cur->next) {
    fn(cur);
  }
}
