#include "matrix_operations.h"
#include "memory_allocator.h"
#include <stdio.h>

static int** create_identity_matrix(int size);
static int** multiply_matrices(int** matrix1, int** matrix2, int size);
static int isIdentityMatrix(int** matrix, int size);

MatrixProduct* buildMatrixTest(int matrixSize) {
    MatrixProduct* matrixProduct = (MatrixProduct*)mymalloc(sizeof(MatrixProduct));
    if (matrixProduct == NULL) {
        return NULL;
    }
    matrixProduct->matrixSize = matrixSize;
    matrixProduct->matrixA = create_identity_matrix(matrixSize);
    if (matrixProduct->matrixA == NULL) {
        myfree(matrixProduct);
        return NULL;
    }
    matrixProduct->matrixB = create_identity_matrix(matrixSize);
    if (matrixProduct->matrixB == NULL) {
        for (int i = 0; i < matrixSize; i++) {
            myfree(matrixProduct->matrixA[i]);
        }
        myfree(matrixProduct);
        return NULL;
    }
    matrixProduct->matrixProductAB = multiply_matrices(matrixProduct->matrixA, matrixProduct->matrixB, matrixSize);
    if (matrixProduct->matrixProductAB == NULL) {
        for (int i = 0; i < matrixSize; i++) {
            myfree(matrixProduct->matrixA[i]);
            myfree(matrixProduct->matrixB[i]);
        }
        myfree(matrixProduct->matrixA);
        myfree(matrixProduct->matrixB);
        myfree(matrixProduct);
        return NULL;
    }

    return matrixProduct;
}

static int isIdentityMatrix(int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j && matrix[i][j] != 1) {
                return 0;
            } else if (i != j && matrix[i][j] != 0) {
                return 0;
            }
        }
    }

    return 1;
}

static int** create_identity_matrix(int size) {
    // Allocate memory for the rows
    int** matrix = (int**)mymalloc(size * sizeof(int*));
    if (matrix == NULL) {
        return NULL;
    }

    // For each row, allocate memory for the columns
    for (int i = 0; i < size; i++) {
        matrix[i] = (int*)mymalloc(size * sizeof(int));
        if (matrix[i] == NULL) {
            // If allocation fails, free any previously allocated memory
            for (int j = 0; j < i; j++) {
                myfree(matrix[j]);
            }
            myfree(matrix);
            return NULL;
        }

        // Initialize the elements to 0
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
        }

        // Set the diagonal element to 1
        matrix[i][i] = 1;
    }

    return matrix;
}


static int** multiply_matrices(int** matrix1, int** matrix2, int size) {
    // Allocate memory for the result matrix
    int** result = (int**)mymalloc(size * sizeof(int*));
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        result[i] = (int*)mymalloc(size * sizeof(int));
        if (result[i] == NULL) {
            // If allocation fails, free any previously allocated memory
            for (int j = 0; j < i; j++) {
                myfree(result[j]);
            }
            myfree(result);
            return NULL;
        }

        // Initialize the elements to 0
        for (int j = 0; j < size; j++) {
            result[i][j] = 0;
        }
    }

    // Perform the multiplication
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}

void verifySuccess(MatrixProduct* matrixProduct) {
    if (matrixProduct == NULL) {
        matrixProduct->success = 0;
        return;
    }

    matrixProduct->success  =   isIdentityMatrix(matrixProduct->matrixA, matrixProduct->matrixSize) &&
                                isIdentityMatrix(matrixProduct->matrixB, matrixProduct->matrixSize) &&
                                isIdentityMatrix(matrixProduct->matrixProductAB, matrixProduct->matrixSize); 
}

void destroyMatrixProduct(MatrixProduct* matrixProduct) {
    if (matrixProduct == NULL) {
        return;
    }
    for (int i = 0; i < matrixProduct->matrixSize; i++) {
        myfree(matrixProduct->matrixA[i]);
        myfree(matrixProduct->matrixB[i]);
        myfree(matrixProduct->matrixProductAB[i]);
    }
    myfree(matrixProduct->matrixA);
    myfree(matrixProduct->matrixB);
    myfree(matrixProduct->matrixProductAB);
    myfree(matrixProduct);
}