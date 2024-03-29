#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

void* mymalloc(size_t size) {
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("Memory allocation failed");
        return NULL;
    }
    return ptr;
}

void myfree(void* ptr, size_t size) {
    if (munmap(ptr, size) == -1) {
        perror("Memory deallocation failed");
    }
}

int main() {
    // Allocate memory for an integer
    int *ptr = (int*) mymalloc(sizeof(int));
    if (ptr == NULL) {
        return 1;
    }

    // Use the allocated memory
    *ptr = 10;
    printf("Value of *ptr: %d\n", *ptr);

    // Deallocate the memory
    myfree(ptr, sizeof(int));

    return 0;
}