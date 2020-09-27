#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol
#include <mpi.h>     // For MPI functions, etc
#include <time.h>
#include <math.h>

double function(double i){
    return sin(i) + sin(2*i);
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

    double local_n = n / comm_sz;
    double local_a = a + my_rank * local_n * h;
    double local_b = local_a + local_n * h;

    clock_t t = clock(); 

    double local_integral = calculate_trap(local_a, local_b, local_n, h, func_ptr);

    double total_integral = local_integral;

    MPI_Reduce(&local_integral, &total_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    t = clock() - t; 

    if(my_rank == 0) {
        printf("{\"Integral\": %.20lf, \"time\": %.10lf}", total_integral, ((double)t) / CLOCKS_PER_SEC);
    }
    MPI_Finalize();

    return 0;
}  /* main */
