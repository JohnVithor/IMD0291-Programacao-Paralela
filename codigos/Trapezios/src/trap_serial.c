#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol

double get_double(double i){
    return i*i + i;
}

double calculate_trap(double a, double b, double n, double (*func_ptr)(double i)) {
    double h = (b - a) / n;  
    printf("O h utilizado foi de: %lf\n", h);
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
    printf("Valor Calculado: %lf\n",calculate_trap(convert_str_double(argv[1]), convert_str_double(argv[2]), convert_str_double(argv[3]), func_ptr));
    return 0;
}  /* main */