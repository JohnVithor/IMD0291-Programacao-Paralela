#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol

double get_double(double i){
    return 3*i*i*i + 4*i*i - 7*i + 8;
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

    double (*func_ptr)(double i);
    func_ptr = get_double;

    double a = convert_str_double(argv[1]);
    double b = convert_str_double(argv[2]); 
    double n = convert_str_double(argv[3]);
    double h = (b - a) / n;

    double local_integral = calculate_trap(a, b, n, h, func_ptr);

    printf("O h utilizado foi de: %lf\n", h);
    printf("Valor Calculado: %lf\n", local_integral);

    return 0;
}  /* main */