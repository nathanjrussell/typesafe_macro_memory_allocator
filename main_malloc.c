#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int matrixSize;
    int** matrixA;
    int** matrixB;
    int** matrixProductAB;
    char success; // 1 if the test was successful, 0 otherwise
} MatrixProduct;

int** create_identity_matrix(int size);
int** multiply_matrices(int** matrix1, int** matrix2, int size);

MatrixProduct* buildMatrixTest(int matrixSize) {
    MatrixProduct* matrixProduct = (MatrixProduct*)malloc(sizeof(MatrixProduct));
    if (matrixProduct == NULL) {
        return NULL;
    }
    matrixProduct->matrixSize = matrixSize;
    matrixProduct->matrixA = create_identity_matrix(matrixSize);
    if (matrixProduct->matrixA == NULL) {
        free(matrixProduct);
        return NULL;
    }
    matrixProduct->matrixB = create_identity_matrix(matrixSize);
    if (matrixProduct->matrixB == NULL) {
        for (int i = 0; i < matrixSize; i++) {
            free(matrixProduct->matrixA[i]);
        }
        free(matrixProduct);
        return NULL;
    }
    matrixProduct->matrixProductAB = multiply_matrices(matrixProduct->matrixA, matrixProduct->matrixB, matrixSize);
    if (matrixProduct->matrixProductAB == NULL) {
        for (int i = 0; i < matrixSize; i++) {
            free(matrixProduct->matrixA[i]);
            free(matrixProduct->matrixB[i]);
        }
        free(matrixProduct->matrixA);
        free(matrixProduct->matrixB);
        free(matrixProduct);
        return NULL;
    }

    return matrixProduct;
}

int isIdentityMatrix(int** matrix, int size) {
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

int** create_identity_matrix(int size) {
    // Allocate memory for the rows
    int** matrix = (int**)malloc(size * sizeof(int*));
    if (matrix == NULL) {
        return NULL;
    }

    // For each row, allocate memory for the columns
    for (int i = 0; i < size; i++) {
        matrix[i] = (int*)malloc(size * sizeof(int));
        if (matrix[i] == NULL) {
            // If allocation fails, free any previously allocated memory
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
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


int** multiply_matrices(int** matrix1, int** matrix2, int size) {
    // Allocate memory for the result matrix
    int** result = (int**)malloc(size * sizeof(int*));
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        result[i] = (int*)malloc(size * sizeof(int));
        if (result[i] == NULL) {
            // If allocation fails, free any previously allocated memory
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
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
        free(matrixProduct->matrixA[i]);
        free(matrixProduct->matrixB[i]);
        free(matrixProduct->matrixProductAB[i]);
    }
    free(matrixProduct->matrixA);
    free(matrixProduct->matrixB);
    free(matrixProduct->matrixProductAB);
    free(matrixProduct);
}

int main() {

    int testCount = 1;
    int modValue = 1000;
    int offsetValue = 500;
    int *randomSizes = (int*)malloc(testCount * sizeof(int));
    MatrixProduct** matrixProducts = (MatrixProduct**)malloc(testCount * sizeof(MatrixProduct*));
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
    free(randomSizes);
    free(matrixProducts);


    return 0;
}