#include "dir.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// Logging function: Here we just output to stderr
// Call this to print out any error message,
// e.g. file not found, file already exists, etc.

void writelog(char *msg) {

    // Buffer
    char buffer[512];

    // Get current date and time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    sprintf(buffer, "%d-%d-%d-%d-%d-%d: %s", tm.tm_year + 1980, tm.tm_mon + 1,
            tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, msg);

    fprintf(stderr, "%s\n", buffer);
}

// Initializes all entries of the hashtable to NULL
// PRE: hashtable = Hash table to be initialized
//      len = Number of entries in the hashtable.
//          The hash table is thus indexed from 0 to len-1
// POST: Every entry of the hash table is set to NULL.
//

void init_hashtable(TLinkedList *hashtable[], int len) {
  for (int i = 0; i < len; i++) hashtable[i] = NULL;
}

// Returns the head of the linked list that MAY contain  the file identified
// by "filename". You need this because the llist routines require a
// head variable in order to access the linked list. This function has
// been done for you. If the linked list does not contain "filename",
// then you can insert data about filename into this list. This is
// very much needed when you are adding files to the directory.
// If the list contains "filename", you can do things like delete a file,
// rename a file, etc.
//
// PRE: filename = name of file to search.
//      hashfun = Hash funtion that takes a string and the number of entries
//          in the hash table.
//      hashtable = The hash table iteself
//      len = Number of entries in the hash table.
// RETURNS: The linked list that may contain "filename". If not, the
//          data for "filename" can be inserted into this list.
//

TLinkedList *get_filelist(char *filename, int (*hashfun)(char *, int),
                          TLinkedList *hashtable[], int len) {
    int ndx = hashfun(filename, len);

    TLinkedList *filelist = hashtable[ndx];

    return filelist;
}

// Updates the head for a linked list that may contain filename.
// This is needed because llist may sometimes change the head
// variable. Thus this function must be called every time you
// insert or delete a file. This function is implemented for you.
//
// PRE: filename = Name of file
//      hashfun = Hash function
//      hashtable = Hash table
//      newentry = The head value returned by insert, delete
//          Note that this value may not necessarily change
//          but we should call update_hashtable anyway just in case.
//
// POST: newentry is written to hashtable[ndx], where ndx
//      is the index value returned by hashfun
//

void update_hashtable(char *filename, int (*hashfun)(char *, int),
                      TLinkedList *hashtable[], int len,
                      TLinkedList *newentry) {

    int ndx = hashfun(filename, len);
    hashtable[ndx] = newentry;
}

// Find file and return node containing filename
// Use the functions in llist.c and the functions defined above
// to implement.
//
// PRE: filename = name of file to find.
//      hashfun = Hash function
//      hashtable = Hash table
//      len = # of entries in hash table.
//
// RETURNS: The TLinkedList node containing data on filename if found,
//          NULL otherwise. Note that we should not call writelog to
//          print an error if the file is not found because sometimes
//          this is what we want, e.g. when adding a new file.
//

TLinkedList *find_file(char *filename, int (*hashfun)(char *, int),
                       TLinkedList *hashtable[], int len) {
  TLinkedList *head = get_filelist(filename, hashfun, hashtable, len);
  return find_llist(head, filename);
}

// Add a new file
// PRE: filename = name of file to add
//      filesize = Size of file to add
//      startblock = Starting block number of file
//      hashfun = Hash function
//      hashtable = Hash table
//      len = # of entries in hash table
//
// POST: Error is printed if file exists. Otherwise a new node
//       is created and added to the appropriate linked list
//       HINT: Use get_filelist. Also remember that inserting
//              into a linked list may result in the head
//              being changed.
//

void add_file(char *filename, int filesize, int startblock,
              int (*hashfun)(char *, int), TLinkedList *hashtable[], int len) {
  TLinkedList *head = get_filelist(filename, hashfun, hashtable, len);
  if (find_llist(head, filename)) {
    writelog("Error: file already exists");
    return;
  }

  TLinkedList *node = create_node(filename, filesize, startblock);
  insert_llist(&head, node);
  update_hashtable(filename, hashfun, hashtable, len, head);
}

// Delete file. Remove the file's entry from the directory
// PRE: filename = name of file to delete.
//      hashfun = Hash function
//      hashtable = Hash table
//      len = # of entries in hash table.
// POST: An error message if the file is not found.
//      Otherwise the file's entry is deleted.
//      HINT: The head might be changed.

void delete_file(char *filename, int (*hashfun)(char *, int),
                 TLinkedList *hashtable[], int len) {
  TLinkedList *head = get_filelist(filename, hashfun, hashtable, len);
  TLinkedList *node = find_llist(head, filename);

  if (node == NULL) {
    writelog("Error: file not found");
    return;
  }

  delete_llist(&head, node);
  update_hashtable(filename, hashfun, hashtable, len, head);
}

// Rename a file.
// PRE: old_filename = Current filename
//      new_filename = Name to change to
//      hashfun = Hash function
//      hashtable = Hash table
//      len = # of entries in hash table
// POST: Error message if old_filename is not found. Otherwise
//      The directory is updated with the new filename.
//

void rename_file(char *old_filename, char *new_filename,
                 int (*hashfun)(char *, int), TLinkedList *hashtable[],
                 int len) {
  TLinkedList *head = get_filelist(old_filename, hashfun, hashtable, len);
  TLinkedList *node = find_llist(head, old_filename);

  if (node == NULL) {
    writelog("Error: file not found");
    return;
  }

  TLinkedList *new_head = get_filelist(new_filename, hashfun, hashtable, len);
  if (find_llist(head, new_filename) != NULL) {
    writelog("Error: new filename already exists");
    return;
  }
  
  int old_idx = hashfun(old_filename, len);
  int new_idx = hashfun(new_filename, len);

  // relink if needed
  if (old_idx != new_idx) {
      if (node->prev)
        node->prev->next = node->next;
      else
        head = node->next;
      
      if (node->next)
        node->next->prev = node->prev;

      // Clean links before reinserting
      node->prev = NULL;
      node->next = NULL;

      insert_llist(&new_head, node);

      update_hashtable(old_filename, hashfun, hashtable, len, head);
      update_hashtable(new_filename, hashfun, hashtable, len, new_head);
  }

  // rename in place
  strcpy(node->filename, new_filename);
}

// Prints the details of a file. Implemented for you.
// PRE: node = node to be printed.
// POST: The contents of node are printed.
void print_node(TLinkedList *node) {
    printf("%-30s\t\t%6d\t\t%6d\n", node->filename, node->filesize,
           node->startblock);
}

// List the directory. Does not need to be in any order.
// PRE: hashtable = Hash table
//      len = # of entries in hash table
// POST: List of files in hashtable is printed.
// HINT: You can treat hashtable as an array
//      and access its elements directly.
//      There is a useful function in llist that
//      traverses the linked list and applies a function
//      to it.

void listdir(TLinkedList *hashtable[], int len) {
    printf("\nFilename\t\t\tFile Size\t\tStart Block\n\n");

    // Implement the rest of this function below.
    for (int i = 0; i < len; i++) {
        if (hashtable[i]) {
            traverse(&hashtable[i], print_node);
        }
    }
}
