#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol
#include <mpi.h>     // For MPI functions, etc
#include <math.h>
#include <time.h>

#define M_PI 3.14159265358979323846

double function(double x){
    return 10 * ( (x * x) - (10 * cos(2 * M_PI * x)));
}

double calculate_trap(double a, double b, int n, double h, double (*func_ptr)(double i)) {  
    double area_total = ((*func_ptr)(a) + (*func_ptr)(b)) / 2;
    
    for (int i = 1; i < n; ++i) {
        double x_i = a + i * h;
        area_total += (*func_ptr)(x_i);
    }
    return h * area_total;

}

double convert_str_double(char* str) {
    char *p;
    errno = 0;
    double conv = strtod(str, &p);

    if (errno != 0 || *p != '\0' || conv > INT_MAX) {
        printf("%s não é um número!\n", str);
        exit(-1);
    }
    return (double) conv;
}

int main( int argc, char **argv ) {

    if (argc != 4) {
        printf("É necessário informar 3 argumentos na seguinte ordem:\n");
        printf("1º Inicio do intervalo\n2º Fim do intervalo\n3º Número de trapézios a serem usados\n");
        return -1;
    }

    int my_rank, comm_sz;

    double (*func_ptr)(double i);
    func_ptr = function;

    double a = convert_str_double(argv[1]);
    double b = convert_str_double(argv[2]); 
    double n = convert_str_double(argv[3]);
    double h = (b - a) / n;

    MPI_Init(NULL, NULL);
    MPI_Comm_size( MPI_COMM_WORLD , &comm_sz);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);

    clock_t t = clock(); 

    double local_n = n / comm_sz;
    double local_a = a + my_rank * local_n * h;
    double local_b = local_a + local_n * h;
    double local_integral = calculate_trap(local_a, local_b, local_n, h, func_ptr);
    //printf("Proc %d of %d > Eu calculei: a=%lf, b=%lf, n=%lf, resultado=%lf\n", my_rank+1, comm_sz, local_a, local_b, local_n, local_integral);

    if(my_rank != 0) {
        MPI_Send(&local_integral, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        double total_integral = local_integral;
        for (int proc = 1; proc < comm_sz; ++proc) {
            MPI_Recv(&local_integral, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_integral += local_integral;
        }
        t = clock() - t; 

        printf("{\"Integral\": %.20lf, \"time\": %.10lf}", total_integral, ((double)t) / CLOCKS_PER_SEC);
    }
    MPI_Finalize();

    // printf("Valor Calculado: %lf\n",);
    return 0;
}  /* main */
