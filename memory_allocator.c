#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define MEMORY_SIZE 200000000 // 1 MB

typedef struct {
    size_t size;
    char free;
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

void free_memory() {
    if (memory != NULL) {
        if (munmap(memory, MEMORY_SIZE) == -1) {
            perror("Memory deallocation failed");
            exit(EXIT_FAILURE);
        }
        memory = NULL;
        allocated = 0;
    }
}

void* mymalloc(size_t size) {
    init_memory();

    // Search for a free block that can fit the requested size
    Block *block = (Block *)memory;
    while ((char *)block < memory + allocated) {
        if (block->free && block->size >= size) {
            // Found a free block that can fit the requested size
            block->free = 0;

            // If the block is larger than the requested size, split it into two blocks
            if (block->size > size + sizeof(Block)) {
                Block *newBlock = (Block *)((char *)(block + 1) + size);
                newBlock->size = block->size - size - sizeof(Block);
                newBlock->free = 1;

                block->size = size;
                allocated += sizeof(Block);
            }

            return (void *)(block + 1);
        }

        // Move to the next block
        block = (Block *)((char *)(block + 1) + block->size);
    }

    // No free block found, allocate a new block
    if ((char *)block + sizeof(Block) + size > memory + MEMORY_SIZE) {
        // Not enough memory left
        return NULL;
    }

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

    free_memory();

    return 0;
}