// Blue Hartsell
// CPSC 3220 F22
// project_1

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#define GNU_SOURCE

extern int leakcount;
extern int *leaksizes;

void *(*original_malloc)(size_t size) = NULL;
void (*original_free)(void *point) = NULL;