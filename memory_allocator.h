#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stdlib.h>

// Block structure to keep track of the size of the block and if it is free
typedef struct {
    size_t size;
    char free;
} Block;

void initializeMemory(size_t size); // Initialize the memory allocator
void free_memory(); // Free the memory allocated by the memory allocator
void* mymalloc(size_t size); // Allocate memory from the memory pool
void myfree(void* mem); // Free memory from the memory pool

#endif // MEMORY_ALLOCATOR_H