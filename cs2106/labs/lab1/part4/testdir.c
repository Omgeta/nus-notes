#include "dir.h"
#include <stdio.h>
#include <string.h>

typedef struct fstr {
    char filename[MAX_FNAME_LEN];
    int filesize;
    int startblock;
} TFileData;

// Number of entries in hash table
#define TABLE_LEN 4

// Number of files we are inserting
#define NUM_FNAMES 6

// Test missing file
#define MISSING_FILE "bork.jpg"
#define MISSING_LEN 15
#define MISSING_BLKNO 6

// Test new filename
#define NEW_FILE "work.jpg"

// Our hash function
int hash(char *filename, int len) {
    // Simple modulo hash function
    unsigned int sum = 0;
    int namelen = strlen(filename);

    for (int i = 0; i < namelen; i++) {
        sum += (int)filename[i];
    }

    return (sum % len);
}

int main() {
    // Same set of files as testlist
    TFileData fnames[NUM_FNAMES] = {
        {"test.txt", 32, 0},  {"hello.txt", 172, 93}, {"a.exe", 2384, 381},
        {"c.exe", 8475, 123}, {"d.tmp", 8374, 274},   {"e.bin", 283, 8472}};

    // Create the hashtable
    TLinkedList *hashtable[TABLE_LEN];

    // Initialize the hash table
    init_hashtable(hashtable, TABLE_LEN);

    int i;

    printf("\nAdding files.\n");

    for (i = 0; i < NUM_FNAMES; i++) {
        add_file(fnames[i].filename, fnames[i].filesize, fnames[i].startblock,
                 hash, hashtable, TABLE_LEN);
    }

    printf("\nListing files.\n");
    listdir(hashtable, TABLE_LEN);

    // Search for an existing file
    printf("\nSearching for existing file %s\n", fnames[2].filename);
    TLinkedList *filedata =
        find_file(fnames[2].filename, hash, hashtable, TABLE_LEN);
    if (!filedata) {
        printf("ERROR: Cannot find file!\n");
    } else {
        printf("OK!\n");
        printf("Filename: %s\tFile size: %d\tStarting Block: %d\n",
               filedata->filename, filedata->filesize, filedata->startblock);
    }

    printf("\nSearching for non-existing file\n");
    filedata = find_file(MISSING_FILE, hash, hashtable, TABLE_LEN);

    if (!filedata)
        printf("OK. File not found.\n");
    else
        printf("ERROR: File should not be found\n");

    // Add in the file and try again
    add_file(MISSING_FILE, MISSING_LEN, MISSING_BLKNO, hash, hashtable,
             TABLE_LEN);

    // Now search again
    printf("\nAdding the missing file\n");
    filedata = find_file(MISSING_FILE, hash, hashtable, TABLE_LEN);

    if (!filedata)
        printf("ERROR: File should now be found\n");
    else
        printf("Ok, file found.\n");

    // Now rename bork.jpg to work.jpg
    printf("\nRenaming %s to %s\n", MISSING_FILE, NEW_FILE);
    rename_file(MISSING_FILE, NEW_FILE, hash, hashtable, TABLE_LEN);

    // Now we should not be able to find bork.jpg

    printf("\nSearching for %s\n", MISSING_FILE);
    filedata = find_file(MISSING_FILE, hash, hashtable, TABLE_LEN);

    if (!filedata)
        printf("OK. File not found.\n");
    else
        printf("ERROR: File should not be found\n");

    // Now search again for work.jpg
    printf("\nSearhing for %s\n", NEW_FILE);
    filedata = find_file(NEW_FILE, hash, hashtable, TABLE_LEN);

    if (!filedata)
        printf("ERROR: File should now be found\n");
    else
        printf("Ok, file found.\n");

    printf("\nSearching for file Before deleting:\n");

    filedata = find_file(fnames[1].filename, hash, hashtable, TABLE_LEN);

    if (filedata)
        printf("OK! Found file!\n");
    else
        printf("ERROR: File not found!\n");

    // Delete file
    printf("\nDeleting file %s\n", fnames[1].filename);

    delete_file(fnames[1].filename, hash, hashtable, TABLE_LEN);

    filedata = find_file(fnames[1].filename, hash, hashtable, TABLE_LEN);

    if (!filedata)
        printf("OK! File no longer found!\n");
    else
        printf("ERROR: File still exists!\n");
}
