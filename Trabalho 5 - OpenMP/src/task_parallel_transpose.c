#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>
#include <omp.h>

int numThreads = 1;

void freeMatrix(double** matrix, long lins){
    for (long i = 0; i < lins; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

double** allocMatrix(long lins, long cols){
    double** matrix = malloc(lins*sizeof(double*));
    for (long i = 0; i < lins; ++i) {
        matrix[i] = malloc(cols*sizeof(double));
    }
    return matrix;
}

void printMatrix(double** matrix, long lins, long cols){
    for (long i = 0; i < lins; ++i) {
        for (long j = 0; j < cols; ++j)
            printf("%lf ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

void fillMatrix(double** matrix, long lins, long cols, long seed){
    srand(seed);
    for (long i = 0; i < lins; ++i) {
        for (long j = 0; j < cols; ++j) {
            matrix[i][j]= (double) rand() / INT_MAX;
        }
    }
}

void transpose_matrix(double** matrix, double** transposed, long lins, long cols){
    #pragma omp parallel num_threads(numThreads) default(none) \
    shared(matrix, transposed, lins, cols)
    {
        #pragma omp single
        {
            for (long i = 0; i < lins; ++i) {
                #pragma omp task
                {
                    for (long j = 0; j < cols; ++j) {
                        transposed[j][i] = matrix[i][j];
                    }
                }
            }
        }
    }
}

void multiply_row(double* linA, double** B, double* result, long colsB, long size){
    for (long j = 0; j < colsB; ++j) {
        result[j] = 0;
        for (long k = 0; k < size; ++k){
            result[j] += linA[k] * B[j][k];
        }
    }
}

void multiply_matrix(double** A, double** B, double** result, long linsA, long colsB, long size){
    #pragma omp parallel num_threads(numThreads) default(none) \
    shared(linsA, numThreads, A, B, result, colsB, size)
    {
        #pragma omp single
        {
            for (long i = 0; i < linsA; ++i) {
                #pragma omp task
                    multiply_row(A[i], B, result[i], colsB, size);
            }
        }
    }
}


long convert_str_long(char *str){
    char *p;
    errno = 0;
    long conv = strtol(str, &p, 10);

    if (errno != 0 || *p != '\0')
    {
        printf("%s não é um número!\n", str);
        exit(-1);
    }
    return (long)conv;
}

int main(int argc, char **argv){

    if (argc != 9) {
        printf("É necessário informar os seguintes argumentos:\nO número de threads a serem usadas\nSe as matrizes devem ser exibidas\nSeed para gerar a matriz A\nSeed para gerar a matriz B\nNúmero de linhas de A\nNúmero de colunas de A\nNúmero de linhas de B\nNúmero de colunas de B\n");
        return -1;
    }

    numThreads = convert_str_long(argv[1]);

    int show_matrix = convert_str_long(argv[2]);

    long seedA = convert_str_long(argv[3]);
    long seedB = convert_str_long(argv[4]);

    long linsA = convert_str_long(argv[5]);
    long colsA = convert_str_long(argv[6]);

    long linsB = convert_str_long(argv[7]);
    long colsB = convert_str_long(argv[8]);

    if(colsA != linsB){
        printf("Número de colunas de A é diferente do número de linhas de B, multiplicação não é possivel.\n");
        return -1;
    }

    double t = omp_get_wtime();

    double** A = allocMatrix(linsA, colsA);
    double** B = allocMatrix(linsB, colsB);
    double** BT = allocMatrix(colsB, linsB);
    double** R = allocMatrix(linsA, colsB);

    fillMatrix(A, linsA, colsA, seedA);
    fillMatrix(B, linsB, colsB, seedB);

    transpose_matrix(B, BT, linsB, colsB);
    multiply_matrix(A, BT, R, linsA, colsB, colsA);

    t = omp_get_wtime() - t; 

    printf("%.10lf\n", t);

    if(show_matrix == 1){
        printMatrix(A, linsA, colsA);
        printMatrix(B, linsB, colsB);
        printMatrix(R, linsA, colsB);
    }
    
    freeMatrix(A, linsA);
    freeMatrix(B, linsB);
    freeMatrix(BT, colsB);
    freeMatrix(R, linsA);

    return 0;
} /* main */