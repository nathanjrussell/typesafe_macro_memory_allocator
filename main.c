#include "matrix_operations.h"
#include "memory_allocator.h"
#include <stdio.h>

int main() {
    initializeMemory((4000*sizeof(int))*(4000*sizeof(int)) + 4000*3*sizeof(int*));
    int testCount = 1;
    int modValue = 4000;
    int offsetValue = 500;
    int *randomSizes = (int*)mymalloc(testCount * sizeof(int));
    MatrixProduct** matrixProducts = (MatrixProduct**)mymalloc(testCount * sizeof(MatrixProduct*));
    if (randomSizes == NULL) {
        return 1;
    }
    for (int i = 0; i < testCount; i++) {
        randomSizes[i] = rand() % modValue + offsetValue;
    }
    for (int i=0; i<testCount; i++) {
        int size = 4000;
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
    myfree(randomSizes);
    myfree(matrixProducts);
    free_memory();

    return 0;
}