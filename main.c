#include "memory_allocator.h"
#include <stdio.h>

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