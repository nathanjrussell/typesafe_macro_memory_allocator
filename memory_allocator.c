#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define MEMORY_SIZE 1024 * 1024 // 1 MB

static char *memory = NULL;
static size_t allocated = 0;

void* mymalloc(size_t size) {
    if (memory == NULL) {
        // Reserve a large block of memory
        memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (memory == MAP_FAILED) {
            perror("Memory reservation failed");
            return NULL;
        }
    }

    if (allocated + size > MEMORY_SIZE) {
        // Not enough memory left
        return NULL;
    }

    // Allocate a chunk from the reserved block
    void *ptr = memory + allocated;
    allocated += size;

    return ptr;
}

void myfree(void* ptr, size_t size) {
    // This function doesn't actually free the memory, it just moves the allocation pointer back
    if (ptr >= memory && ptr < memory + allocated) {
        allocated -= size;
    }
}

int main() {
    // Allocate memory for an integer
    void *ptr = mymalloc(sizeof(int));
    if (ptr == NULL) {
        return 1;
    }

    // Use the allocated memory
    *(int*)ptr = 10;
    printf("Value of *ptr: %d\n", *(int*)ptr);

    // "Free" the memory
    myfree(ptr, sizeof(int));

    return 0;
}