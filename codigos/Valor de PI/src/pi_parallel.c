#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol
#include <mpi.h>     // For MPI functions, etc

double calculate_pi(long initial_term, long final_term, double divisor){
    double pi = 0;
    for (long i = initial_term; i <= final_term; ++i) {
        if (i % 2 != 0) {
            pi += 4 / divisor;
        }
        else {
            pi -= 4 / divisor;
        }
        divisor += 2;
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
    long local_divisor = 1 + my_rank*2;



    double local_pi = calculate_pi(local_initial_term, local_final_term, local_divisor);
    //printf("Proc %d of %d > Eu calculei: a=%lf, b=%lf, n=%lf, resultado=%lf\n", my_rank+1, comm_sz, local_a, local_b, local_n, local_integral);

    if(my_rank != 0) {
        MPI_Send(&local_integral, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        double total_integral = local_integral;
        for (int proc = 1; proc < comm_sz; ++proc) {
            MPI_Recv(&local_integral, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_integral += local_integral;
        }
        printf("O h utilizado foi de: %lf\n", h);
        printf("Valor Calculado: %lf\n", total_integral);
    }
    MPI_Finalize();

    // printf("Valor Calculado: %lf\n",);
    return 0;
}  /* main */
