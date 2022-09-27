// Blue Hartsell
// CPSC 3220 F22
// project_1

#define _GNU_SOURCE

// Library inclusions
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void __attribute__ ((constructor)) shim_start();
void __attribute__ ((destructor)) shim_kill();

void *(*ogmalloc)(size_t size) = NULL;
void (*ogfree)(void *point) = NULL;

// Node struct for linked list
struct Node {
    void *val;
    int size;
    struct Node *next; // ptr to next node in list
};

// Set up head pointer
struct Node *head = NULL;

// Shim constructor
void shim_start() {
    ogmalloc = dlsym(RTLD_NEXT, "malloc");
    ogfree = dlsym(RTLD_NEXT, "free");
}

// Shim destructor, which actually does all the printing (to avoid inter-process communication)
void shim_kill() {
    // Initialize
    int leakcount = 0,
        leaksize = 0;
    
    // Traverse list
    struct Node *iterator = NULL;
    for (iterator = head; iterator != NULL; iterator = iterator->next) {
        // Count leaks
        leakcount++;

        //Track size
        int cursize = iterator->size;
        leaksize += iterator->size;

        // Print data
        fprintf(stderr, "LEAK\t%d\n", cursize);
    }

    // Print data
    fprintf(stderr, "TOTAL\t%d\t%d\n", leakcount, leaksize);

    // Cleanup
    ogfree(iterator);
}

// New malloc, which now tracks itself
void* malloc(size_t size) {
    // Call original malloc and store pointer to allocated memory block
    void *mem = ogmalloc(size);

    // Create node
    struct Node *temp = ogmalloc(sizeof(struct Node));
    temp->size = size;
    temp->val = mem;

    // Add temp node to top of list
    temp->next = head;
    head = temp;

    // Return pointer to allocated memory block
    return mem;
}

// New free, which also tracks itself
void free(void *toFree) {
    // Call original free
    ogfree(toFree);

    // Traverse list
    struct Node *iterator = NULL;
    struct Node *prev = NULL;
    for (iterator = head; iterator != NULL; iterator = iterator->next) {
        // Find block toFree is pointing to
        if(iterator->val == toFree) {
            // Adjust list
            if(prev == NULL) {
                head = iterator->next;
            } else {
                prev->next = iterator->next;
            }

            // Remove node
            ogfree(iterator);

            // Done
            return;
        }
    }

    // toFree is not pointing towards a block of memory recorded in the list
    return;
}