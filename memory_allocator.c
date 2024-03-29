#include <stdio.h>
#include <sys/mman.h>
#include "memory_allocator.h"

//variables to stroes information about broad allocated memory (mmap)
static char *memory = NULL;  //pointer to start of memory pool
static size_t allocated = 0;  //size of memory allocated from pool
static size_t MEMORY_SIZE; //size of memory pool

// initialize memory pool
void initializeMemory(size_t size) {
    if (size == 0) { //check if size is 0
        return; //do nothing if size is 0
    }
    if (memory == NULL) { //check if memory pool is already initialized
        MEMORY_SIZE = size; //set the size of the memory pool
        memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (memory == MAP_FAILED) {
            perror("Memory reservation failed");
            exit(EXIT_FAILURE);
        }
    }
} //close initializeMemory function

// free memory pool
void free_memory() {
    if (memory != NULL) { //check if memory pool is initialized
        if (munmap(memory, MEMORY_SIZE) == -1) {
            perror("Memory deallocation failed");
            exit(EXIT_FAILURE);
        }
        memory = NULL; //reset memory pool pointer
        allocated = 0; //reset allocated memory size
    }
} //close free_memory function

// Behaves like malloc but allocates memory from the memory pool
void *mymalloc(size_t size) {
    Block *block = (Block *)memory;  //point to the first block in the memory pool
    while ((char *)block < memory + allocated) { //loop through the memory pool
        if (block->free && block->size >= size) { //check if block is free and has enough space
            block->free = 0; //mark the block as allocated
            if (block->size > size + sizeof(Block)) { //check if the block is larger than the requested size
                // Split the block into two blocks
                Block *newBlock = (Block *)((char *)(block + 1) + size); //create new block at end of required size in large block
                newBlock->size = block->size - size - sizeof(Block); //set the size of the new block
                newBlock->free = 1; //mark the new block as free
                block->size = size; //set the size of the original block to the requested size
                allocated += sizeof(Block); //update the allocated memory size variable
            }
            return (void *)(block + 1); 
        }
        block = (Block *)((char *)(block + 1) + block->size); //move to the next block in search of free block
    } //close while loop

    // No free block found, allocate new block
    if ((char *)block + sizeof(Block) + size > memory + MEMORY_SIZE) {  //check if there is enough space for the new block
        return NULL; //return NULL if there is not enough space
    }
    block->size = size; //set the size of the new block
    block->free = 0; //mark the block as allocated
    allocated += size + sizeof(Block); //update the allocated memory size variable
    return (void *)(block + 1); //return the pointer to the allocated memory
} //close mymalloc function


void myfree(void* mem) {
    Block *block = (Block *)mem - 1; //point to the block header
    block->free = 1; // Mark the block as free

    // If next block is free, consolidate the two blocks as one
    Block *nextBlock = (Block *)((char *)(block + 1) + block->size);  //point to the next block header
    if ((char *)nextBlock < memory + allocated && nextBlock->free) { // verify block is within the memory range and is free
        block->size += sizeof(Block) + nextBlock->size; // increase the size of the current block to include the next block
        allocated -= sizeof(Block); // update the allocated memory size variable
    }

    // If previous block is free, consolidate the two blocks as one
    Block *prevBlock = NULL; // ptr for the previous block
    if ((char *)block != memory) { // verify the block is not the first block
        prevBlock = (Block *)memory; // point to the first block
        while (
                (char *)prevBlock < (char *)memory + allocated &&  // verify the block is within the memory range
                (char *)((prevBlock + 1) + prevBlock->size) != (char *)block // verify the block is not the current block
            ) {
            prevBlock = (Block *)((char *)(prevBlock + 1) + prevBlock->size); // update to next block
        } // close while loop

        if (prevBlock->free) { // verify the previous block is free
            prevBlock->size += sizeof(Block) + block->size; // increase the size of the previous block to include the current block
            allocated -= sizeof(Block); // update the allocated memory size variable
            block = prevBlock; // update the block pointer to the previous block after condensing
        } 
    } // close if statement (verify the block is not the first block)
} //close myfree function

