#include "matrix_operations.h"
#include "memory_allocator.h"
#include <stdio.h>


// Define MALLOC to use mymalloc or malloc based on the preprocessor flag -DMYMALLOC
#ifdef DISABLE_MYMALLOC
    #define MALLOC malloc
    #define FREE free
    #define MEMORY_POOL_SIZE 0
#else
    #define MALLOC mymalloc
    #define FREE myfree
    #define MEMORY_POOL_SIZE (4000*sizeof(int))*(4000*sizeof(int)) + 4000*3*sizeof(int*)

#endif

int main() {
    int seed = 123;
    srand(seed);
    initializeMemory(MEMORY_POOL_SIZE);
    int testCount = 10;
    int modValue = 1000;
    int offsetValue = 500;
    int *randomSizes = (int*)MALLOC(testCount * sizeof(int));
    MatrixProduct** matrixProducts = (MatrixProduct**)MALLOC(testCount * sizeof(MatrixProduct*));
    if (randomSizes == NULL) {
        return 1;
    }
    for (int i = 0; i < testCount; i++) {
        randomSizes[i] = rand() % modValue + offsetValue;
    }
    for (int i=0; i<testCount; i++) {
        int size = randomSizes[i];
        matrixProducts[i] = buildMatrixTest(size);
        verifySuccess(matrixProducts[i]);
        if (matrixProducts[i] != NULL && matrixProducts[i]->success) {
            printf("Test successful for size %d\n", size);
        } else {
            printf("Test Failed for size %d\n", size);
        }
        if (i >= 2) {
            destroyMatrixProduct(matrixProducts[i-2]);
        }
    }

    for (int i = testCount - 2; i < testCount; i++) {
        destroyMatrixProduct(matrixProducts[i]);
    }
    FREE(randomSizes);
    FREE(matrixProducts);
    free_memory();

    return 0;
}