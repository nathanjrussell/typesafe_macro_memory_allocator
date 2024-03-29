#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stdlib.h>

void initializeMemory();
void free_memory();
void* mymalloc(size_t size);
void myfree(void* ptr);

#endif // MEMORY_ALLOCATOR_H