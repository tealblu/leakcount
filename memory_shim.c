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

// Shim destructor, which actually does all the printing
void shim_kill() {
    int leakcount = 0,
        leaksize = 0;
    
    struct Node *iterator = NULL;

    for (iterator = head; iterator != NULL; iterator = iterator->next) {
        // Count leaks
        leakcount++;

        //Track size
        int cursize = iterator->size;
        leaksize = cursize + iterator->size;

        // Print data
        fprintf(stderr, "LEAK\t%d\n", cursize);
    }

    // Print data
    fprintf(stderr, "TOTAL\t%d\t%d\n", leakcount, leaksize);
}

void* malloc(size_t size) {
    // Call original malloc and store pointer to allocated memory block
    void *mem = ogmalloc(size);

    // Create node
    struct Node *temp = original_malloc(sizeof(struct Node));
    temp->size = size;
    temp->val = mem;

    // Add temp node to top of list
    temp->next = head;
    head = temp;

    // Return pointer to allocated memory block
    return mem;
}