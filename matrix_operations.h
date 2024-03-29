#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

typedef struct {
    int matrixSize;
    int** matrixA;
    int** matrixB;
    int** matrixProductAB;
    char success; // 1 if the test was successful, 0 otherwise
} MatrixProduct;

MatrixProduct* buildMatrixTest(int matrixSize);
void verifySuccess(MatrixProduct* matrixProduct);
void destroyMatrixProduct(MatrixProduct* matrixProduct);

#endif // MATRIX_OPERATIONS_H