#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stdlib.h>

typedef struct {
    size_t size;
    char free;
} Block;

void initializeMemory(size_t size);
void free_memory();
void* mymalloc(size_t size);
void myfree(void* ptr);

#endif // MEMORY_ALLOCATOR_H