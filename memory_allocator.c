#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define MEMORY_SIZE 1024 * 1024 // 1 MB

typedef struct {
    size_t size;
    int free;
} Block;

static char *memory = NULL;
static size_t allocated = 0;

void init_memory() {
    if (memory == NULL) {
        // Reserve a large block of memory
        memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (memory == MAP_FAILED) {
            perror("Memory reservation failed");
            exit(EXIT_FAILURE);
        }
    }
}

void* mymalloc(size_t size) {
    init_memory();

    if (allocated + size + sizeof(Block) > MEMORY_SIZE) {
        // Not enough memory left
        return NULL;
    }

    // Allocate a chunk from the reserved block
    Block *block = (Block *)(memory + allocated);
    block->size = size;
    block->free = 0;

    allocated += size + sizeof(Block);

    return (void *)(block + 1);
}

void myfree(void* ptr) {
    // Mark the block as free
    Block *block = (Block *)ptr - 1;
    block->free = 1;
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

    // Free the memory
    myfree(ptr);

    return 0;
}