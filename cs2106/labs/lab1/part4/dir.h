#include "llist.h"

// Add prototypes and other declarations below
void writelog(char *msg);
void init_hashtable(TLinkedList *hashtable[], int len);
TLinkedList *get_filelist(char *filename, int (*hashfun)(char *, int), TLinkedList *hashtable[], int len);
void update_hashtable(char *filename, int (*hashfun)(char *, int), TLinkedList *hashtable[], int len, TLinkedList *newentry);
TLinkedList *find_file(char *filename, int (*hashfun)(char *, int), TLinkedList *hashtable[], int len);
void add_file(char *filename, int filesize, int startblock, int (*hashfun)(char *, int), TLinkedList *hashtable[], int len);
void delete_file(char *filename, int (*hashfun)(char *, int), TLinkedList *hashtable[], int len);
void rename_file(char *old_filename, char *new_filename, int (*hashfun)(char *, int), TLinkedList *hashtable[], int len);
void listdir(TLinkedList *hashtable[], int len);
void print_node(TLinkedList *node);
