#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol
#include <mpi.h>     // For MPI functions, etc

void print_and_validade_order(int *vector, int n){
    printf("%d", vector[0]);
    int less = vector[0];
    for (int i = 1; i < n; ++i) {
        printf(", %d", vector[i]);
        if(less > vector[i]){
            exit(-1);
        }
        less = vector[i];
    }
}

void sort_groups(int *a, int *b, int n, int*aux) {
    int i = 0, j = 0, c = 0;

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

    int k = 0;
    int *left = NULL;

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

    for(i = 0; i < n; ++i) {
        a[i] = aux[i];
        b[i] = aux[i+n];
    }
}

void odd_even_sort(int *a, int n) {
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

int main( int argc, char **argv ) {

    if (argc != 4) {
        printf("É necessário informar 3 argumentos na seguinte ordem:\n");
        printf("1º Seed para gerar os números pseudo-aleatórios\n2º Tamanho do vetor\n3º 1 se o resultado da ordenação deve ser exibido e 0 caso contrário\n");
        return -1;
    }

    unsigned int seed = convert_str_int(argv[1]);
    int n = convert_str_int(argv[2]);
    int print_result = convert_str_int(argv[3]);

    int *a = NULL;
    
    int my_rank = 0, comm_sz = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_size( MPI_COMM_WORLD , &comm_sz);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);


    if(n % comm_sz != 0) {
        if(my_rank == 0) {
            printf("É necessário que o número a serem ordenados sejam divididos igualmente entre os processos solicitados\n");
        }
        MPI_Finalize();

        return 0;
    }

    if(my_rank == 0) {
        srand(seed);
        a = malloc(n*sizeof(int));

        for (int i = 0; i < n; ++i) {
            a[i] = rand();
        }
    }

    int local_n = n / comm_sz;
    int *local_a = malloc(local_n*sizeof(int));//a + my_rank*local_n;
    int *local_b = malloc(local_n*sizeof(int));
    int *aux = malloc(2*n*sizeof(int));
    
    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    MPI_Scatter(a, local_n, MPI_INT, local_a, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    odd_even_sort(local_a, local_n);
    
    for(int i = 0; i < comm_sz; ++i) {
        if(my_rank % 2 == i % 2) {
            if(my_rank != comm_sz-1){
                MPI_Recv(local_b, local_n, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(local_a, local_n, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
                sort_groups(local_a, local_b, local_n, aux);
            }            
        } else {
            if(my_rank != 0){
                MPI_Send(local_a, local_n, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD);
                MPI_Recv(local_b, local_n, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sort_groups(local_b, local_a, local_n, aux);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Gather(local_a, local_n, MPI_INT, a, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    double finish = MPI_Wtime();

    double local_time = finish-start;
    double final_time = 0;

    MPI_Reduce(&local_time, &final_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 

    if(my_rank == 0) {
        if (print_result == 1){
            printf("{\"Vetor\": [");
            print_and_validade_order(a, n);
            printf("], \"time\": %.10lf}\n", final_time);
        } else {
            printf("{\"time\": %.10lf}\n", final_time);
        }
        free(a);
    } 

    free(local_a);
    free(local_b);
    free(aux);

    MPI_Finalize();

    return 0;
}  /* main */