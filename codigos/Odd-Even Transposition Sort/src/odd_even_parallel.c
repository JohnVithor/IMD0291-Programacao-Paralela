#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol
#include <mpi.h>     // For MPI functions, etc

void print_vector(double *vector, int n){
    printf("%lf", vector[0]);
    for (int i = 1; i < n; ++i) {
        printf(" %lf", vector[i]);
    }
}

void sort_groups(double *a, double *b, int n) {
    double *aux = malloc(2*n*sizeof(double));
    int i = 0, j = 0, c = 0;

    // Start the merge of values
    while(c < 2*n && i < n && j < n) {
        if (a[i] < b[j]) {
            aux[c] = a[i];
            ++i;
        } else {
            aux[c] = b[j];
            ++j;
        }
        ++c;
    }

    // Handle the leftorvers in some of the vectors
    int k = 0;
    double *left = NULL;

    if (i >= n) {
        k = j;
        left = b;
    } else {
        k = i;
        left = a;
    }
    for (; k < n; ++k){
        aux[c] = left[k];
        ++c;
    }

    // Put back in order
    for(i = 0; i < n; ++i) {
        a[i] = aux[i];
        b[i] = aux[i+n];
    }
    free(aux);
}

void odd_even_sort(double *a, int n) {
    int phase = 0, i = 0, temp = 0;
    for (phase = 0; phase < n; ++phase) {
        if(phase % 2 == 0) {
            for (i = 1; i < n; i += 2) {
                if (a[i-1] > a[i]) {
                    temp = a[i-1];
                    a[i-1] = a[i];
                    a[i] = temp;
                }
            }
        } else {
            for (i = 1; i < n-1; i += 2) {
                if (a[i] > a[i+1]) {
                    temp = a[i];
                    a[i] = a[i+1];
                    a[i+1] = temp;
                }
            }
        }
    }
}

int convert_str_int(char* str) {
    char *p = NULL;
    errno = 0;
    long conv = strtol(str, &p, 10);

    if (errno != 0 || *p != '\0' || conv > INT_MAX) {
        printf("%s não é um número!\n", str);
        exit(-1);
    }
    return (int) conv;
}

double convert_str_double(char* str) {
    char *p = NULL;
    errno = 0;
    double conv = strtod(str, &p);

    if (errno != 0 || *p != '\0') {
        printf("%s não é um número!\n", str);
        exit(-1);
    }
    return (double) conv;
}

int main( int argc, char **argv ) {

    if (argc < 3) {
        printf("É necessário informar 2 argumentos na seguinte ordem:\n");
        printf("1º Tamanho do vetor\n2º Elementos do vetor\n");
        return -1;
    }

    int n = convert_str_int(argv[1]);

    if(argc - n != 2){
        printf("O número de elementos do vetor é diferente do numero de elementos esperados.\nNumero de elementos esperados:%d numero de elementos informados:%d\n", n, argc - 2);
        return -1;
    }


    double *a = NULL;
    int my_rank = 0, comm_sz = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_size( MPI_COMM_WORLD , &comm_sz);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);

    a = malloc(n*sizeof(double));

    int local_n = n / comm_sz;
    double *local_a = a + my_rank*local_n;
    double *local_b = malloc(local_n*sizeof(double));
    

    for (int i = 0; i < n; ++i) {
        a[i] = convert_str_double(argv[i+2]);
    }
    
    odd_even_sort(local_a, local_n);
    
    for(int i = 0; i < comm_sz; ++i) {
        if(my_rank % 2 == i % 2) {
            
            if(my_rank != comm_sz-1){
                MPI_Recv(local_b, local_n, MPI_DOUBLE, my_rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sort_groups(local_a, local_b, local_n);
                MPI_Send(local_b, local_n, MPI_DOUBLE, my_rank+1, 0, MPI_COMM_WORLD);
            }            
        } else {
            if(my_rank != 0){
                MPI_Send(local_a, local_n, MPI_DOUBLE, my_rank-1, 0, MPI_COMM_WORLD);
                MPI_Recv(local_a, local_n, MPI_DOUBLE, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        // barrier
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Gather(local_a, local_n, MPI_DOUBLE, a, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Finalize();

    if(my_rank == 0) {
        print_vector(a, n);
    } 

    free(a);
    free(local_b);

    return 0;
}  /* main */