#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>

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

void transpose_square_matrix(double**A, long size){
    for (long i = 0; i < size; ++i) {
        for (long j = i; j < size; ++j) {
            double aux = A[j][i];
            A[j][i] = A[i][j];
            A[i][j] = aux;
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
    for (long i = 0; i < linsA; ++i) {
        multiply_row(A[i], B, result[i], colsB, size);
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

    if (argc != 5) {
        printf("É necessário informar os seguintes argumentos:\nSe as matrizes devem ser exibidas\nSeed para gerar a matriz A\nSeed para gerar a matriz B\nOrdem da matriz quadrada\n");
        return -1;
    }

    int show_matrix = convert_str_long(argv[1]);

    long seedA = convert_str_long(argv[2]);
    long seedB = convert_str_long(argv[3]);

    long ordem = convert_str_long(argv[4]);

    clock_t t = clock();

    double** A = allocMatrix(ordem, ordem);
    double** B = allocMatrix(ordem, ordem);
    double** R = allocMatrix(ordem, ordem);

    fillMatrix(A, ordem, ordem, seedA);
    fillMatrix(B, ordem, ordem, seedB);

    transpose_square_matrix(B, ordem);
    multiply_matrix(A, B, R, ordem, ordem, ordem);

    t = clock() - t; 

    printf("%.10lf\n", ((double)t) / CLOCKS_PER_SEC);

    if(show_matrix == 1){
        printMatrix(A, ordem, ordem);
        printMatrix(B, ordem, ordem);
        printMatrix(R, ordem, ordem);
    }
    
    freeMatrix(A, ordem);
    freeMatrix(B, ordem);
    freeMatrix(R, ordem);

    return 0;
} /* main */