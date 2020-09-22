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

    double times[10]; 
    double pis[10]; 

    for (long i = 0; i < 10; ++i) {
        long local_data[2] = {0,0};

        clock_t t = clock(); 

        count_in_out(seed, local_counter, local_data);

        if(my_rank != 0) {
            MPI_Send(local_data, 2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        } else {
            long total_in = local_data[0];
            long total_out = local_data[1];
            for (int proc = 1; proc < comm_sz; ++proc) {
                MPI_Recv(&local_data, 2, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                total_in += local_data[0];
                total_out += local_data[1];
            }
            pis[i] = calculate_pi(total_in, total_out);
        }
        
        t = clock() - t; 

        times[i] = ((double)t) / CLOCKS_PER_SEC;
    }

    if(my_rank == 0) {
        double initial_pi = pis[0];
        double mean_time = times[0];

        for (int i = 1; i < 10; ++i) {
            if (initial_pi != pis[i]){
                printf("O valor calculado para PI na repetição %d foi diferente das anteriores: %.20lf", i, pis[i]);
            }
            mean_time += times[i];
        }

        mean_time = mean_time/10.0;

        printf("{\"PI\": %.50lf, \"time\": %.10lf}", initial_pi, mean_time);

    }
    MPI_Finalize();

    return 0;
}  /* main */
