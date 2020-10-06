#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>
#include <mpi.h>    // For MPI functions, etc

long count_insides(long seed, long counter){
    double x, y;
    long inside = 0;
    while (counter > 0) {
        x = (double) rand() / (double)INT_MAX;
        y = (double) rand() / (double)INT_MAX;
        if (x*x + y*y <= 1.0){
            ++inside;
        }
        --counter;
    }
    return inside;
}

double calculate_pi(long in, long counter) {
    return 4.0 * (double) in / (double) counter;
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
    long seed = convert_str_long(argv[1]);
    long counter = convert_str_long(argv[2]);
    
    MPI_Init(NULL, NULL);
    MPI_Comm_size( MPI_COMM_WORLD , &comm_sz);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);

    long local_counter = counter/comm_sz;
    srand(seed + my_rank);

    double start = MPI_Wtime();

    long local_insides = count_insides(seed, local_counter);

    if(my_rank != 0) {
        MPI_Send(&local_insides, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        long total_insides = local_insides;
        for (int proc = 1; proc < comm_sz; ++proc) {
            MPI_Recv(&local_insides, 1, MPI_LONG, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_insides += local_insides;
        }
        double pi  = calculate_pi(total_insides, counter);
        
        double finish = MPI_Wtime();
        double final_time = finish-start;
        printf("{\"PI\": %.50lf, \"time\": %.10lf}", pi, final_time);
    }
    
    MPI_Finalize();

    return 0;
}  /* main */
