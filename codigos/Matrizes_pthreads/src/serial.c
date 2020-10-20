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


void multiply_row(double* linA, double** B, double* result, long colsB, long size){
    for (long j = 0; j < colsB; ++j) {
        result[j] = 0;
        for (long k = 0; k < size; ++k){
            result[j] += linA[k] * B[k][j];
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

    if (argc != 8) {
        printf("É necessário informar os seguintes argumentos:\nSe as matrizes devem ser exibidas\nSeed para gerar a matriz A\nSeed para gerar a matriz B\nNúmero de linhas de A\nNúmero de colunas de A\n Número de linhas de B\nNúmero de colunas de B\n");
        return -1;
    }

    int show_matrix = convert_str_long(argv[1]);

    long seedA = convert_str_long(argv[2]);
    long seedB = convert_str_long(argv[3]);

    long linsA = convert_str_long(argv[4]);
    long colsA = convert_str_long(argv[5]);

    long linsB = convert_str_long(argv[6]);
    long colsB = convert_str_long(argv[7]);

    if(colsA != linsB){
        printf("Número de colunas de A é diferente do número de linhas de B, multiplicação não é possivel.\n");
        return -1;
    }

    clock_t t = clock();

    double** A = allocMatrix(linsA, colsA);
    double** B = allocMatrix(linsB, colsB);
    double** R = allocMatrix(linsA, colsB);

    fillMatrix(A, linsA, colsA, seedA);
    fillMatrix(B, linsB, colsB, seedB);

    multiply_matrix(A, B, R, linsA, colsB, colsA);

    t = clock() - t; 

    printf("{\"time\": %.10lf}\n", ((double)t) / CLOCKS_PER_SEC);

    if(show_matrix == 1){
        printMatrix(A, linsA, colsA);
        printMatrix(B, linsB, colsB);
        printMatrix(R, linsA, colsB);
    }
    
    freeMatrix(A, linsA);
    freeMatrix(B, linsB);
    freeMatrix(R, linsA);

    return 0;
} /* main */