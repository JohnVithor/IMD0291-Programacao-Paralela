#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol
#include <mpi.h>     // For MPI functions, etc
#include <math.h>

double BaileyBorweinPlouffe(long initial_k, long final_k){
    double pi = 0.0;
    for (long n = initial_k; n < final_k; ++n) {
        pi += (1.0 / pow(16,n)) * ((4.0 / (8.0 * n + 1.0)) - (2.0 / (8.0 * n + 4.0)) - (1.0 / (8.0 * n + 5.0)) - (1.0 / (8.0 * n + 6.0)));
    }
    return pi;
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

int main( int argc, char **argv ) {

    if (argc != 2)
    {
        printf("É necessário informar apenas 1 argumento o número de termos:\n");
        return -1;
    }

    int my_rank, comm_sz;
    double number_of_terms = convert_str_long(argv[1]);

    MPI_Init(NULL, NULL);
    MPI_Comm_size( MPI_COMM_WORLD , &comm_sz);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);

    long qtd_terms_per_core = number_of_terms / comm_sz;
    long local_initial_term = my_rank * qtd_terms_per_core;
    long local_final_term = (my_rank+1) * qtd_terms_per_core;

    double local_pi = BaileyBorweinPlouffe(local_initial_term, local_final_term);

    if(my_rank != 0) {
        MPI_Send(&local_pi, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        double total_pi = local_pi;
        for (int proc = 1; proc < comm_sz; ++proc) {
            MPI_Recv(&local_pi, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_pi += local_pi;
        }
        printf("Valor Calculado de PI foi de: %.100lf\n", total_pi);
    }
    MPI_Finalize();

    return 0;
}  /* main */
