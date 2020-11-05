#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol

#define __USE_POSIX199309 1
#include <time.h>
#include <pthread.h>

int thread_count;

int linsA;
int colsA;
int linsB;
int colsB;

double** A;
double** B;
double** BT;
double** R;

void multiply_row(double* linA, double** BT, double* result, long colsB, long size){
    for (long j = 0; j < colsB; ++j) {
        result[j] = 0;
        for (long k = 0; k < size; ++k){
            result[j] += linA[k] * BT[j][k];
        }
    }
}

void *Pth_mat_vect(void* rank){
    long my_rank = (long) rank;
    long local_linsA = linsA / thread_count;
    long my_first_row = my_rank * local_linsA;
    long my_last_row = (my_rank + 1) * local_linsA;

    for (long i = my_first_row; i < my_last_row; ++i) {
        multiply_row(A[i], BT, R[i], colsB, linsB);    
    }
    return NULL;
}

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

void transpose_line_matrix(double** matrix, double** transposed, long i, long cols){
    for (long j = 0; j < cols; ++j) {
        transposed[j][i] = matrix[i][j];
    }
}

void *transpose_matrix(void* rank){
    long my_rank = (long) rank;
    long local_linsA = linsA / thread_count;
    long my_first_row = my_rank * local_linsA;
    long my_last_row = (my_rank + 1) * local_linsA;
    for (long i = my_first_row; i < my_last_row; ++i) {
        transpose_line_matrix(B, BT, i, colsB);
    }
    return NULL;
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
        printf("É necessário informar os seguintes argumentos:\nNúmero de threads a serem usadas\nSe as matrizes devem ser exibidas\nSeed para gerar a matriz A\nSeed para gerar a matriz B\nNúmero de linhas de A\nNúmero de colunas de A\nNúmero de linhas de B\nNúmero de colunas de B\n");
        return -1;
    }

    thread_count = convert_str_long(argv[1]);
    int show_matrix = convert_str_long(argv[2]);
    long seedA = convert_str_long(argv[3]);
    long seedB = convert_str_long(argv[4]);

    linsA = convert_str_long(argv[5]);
    colsA = convert_str_long(argv[6]);

    linsB = convert_str_long(argv[7]);
    colsB = convert_str_long(argv[8]);

    if (linsA % thread_count != 0 && linsB % thread_count != 0){
        printf("É necessário que o numero de linhas das matriz sejam divididas igualmente entre a quantidade de threads solicitada\n");
        return 1;
    }

    struct timespec start, finish;
    double elapsed;

    if(colsA != linsB){
        printf("Número de colunas de A é diferente do número de linhas de B, multiplicação não é possivel.\n");
        return -1;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    pthread_t* threads_handles;

    threads_handles = malloc(thread_count*sizeof(pthread_t));

    A = allocMatrix(linsA, colsA);
    B = allocMatrix(linsB, colsB);
    BT = allocMatrix(colsB, linsB);
    R = allocMatrix(linsA, colsB);

    fillMatrix(A, linsA, colsA, seedA);
    fillMatrix(B, linsB, colsB, seedB);

    for (long thread = 0; thread < thread_count; ++thread) {
        pthread_create(&threads_handles[thread], NULL, transpose_matrix, (void*) thread);
    }

    for (long thread = 0; thread < thread_count; ++thread) {
        pthread_join(threads_handles[thread], NULL);
    }

    for (long thread = 0; thread < thread_count; ++thread) {
        pthread_create(&threads_handles[thread], NULL, Pth_mat_vect, (void*) thread);
    }

    for (long thread = 0; thread < thread_count; ++thread) {
        pthread_join(threads_handles[thread], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("{\"time\": %.10lf}\n", elapsed);

    if(show_matrix == 1){
        printMatrix(A, linsA, colsA);
        printMatrix(B, linsB, colsB);
        printMatrix(R, linsA, colsB);
    }

    free(threads_handles);

    freeMatrix(A, linsA);
    freeMatrix(B, linsB);
    freeMatrix(R, linsA);

    return 0;
} /* main */