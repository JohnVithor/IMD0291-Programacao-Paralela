#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>
#include <mpi.h>    // For MPI functions, etc

void count_in_out(long seed, long counter, long* data){
    srand(seed);
    data[0] = 0;
    data[1] = 0;
    double d, x, y;
    while (--counter > 0) {
        x = (double) rand() / (double)INT_MAX;
        y = (double) rand() / (double)INT_MAX;
        d = x*x + y*y;
        (d <= 1.0) ? ++(data[0]) : ++(data[1]);
    }
}

double calculate_pi(long in, long out) {
    return 4.0 * (float) in / (float) (in + out);
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

    if (argc != 3)
    {
        printf("É necessário informar 2 argumentos a seed inicial para o sorteio dos numeros e a quantidade de sorteios:\n");
        return -1;
    }

    int my_rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_size( MPI_COMM_WORLD , &comm_sz);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);

    long seed = convert_str_long(argv[1]);
    long counter = convert_str_long(argv[2]);

    long local_counter = counter/comm_sz;

    double pi = 0;

    long local_in_out[2] = {0,0};
    long total_in_out[2] = {0,0};

    clock_t t = clock(); 

    count_in_out(seed, local_counter, local_in_out);

    MPI_Reduce(local_in_out, total_in_out, 2, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    pi = calculate_pi(total_in_out[0], total_in_out[1]);

    t = clock() - t; 

    if(my_rank == 0) {
        printf("{\"PI\": %.50lf, \"time\": %.10lf}", pi, ((double)t) / CLOCKS_PER_SEC);
    }
    MPI_Finalize();

    return 0;
}  /* main */
