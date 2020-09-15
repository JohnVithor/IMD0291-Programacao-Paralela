#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol

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

int main(int argc, char **argv){

    if (argc != 2)
    {
        printf("É necessário informar apenas 1 argumento o número de termos:\n");
        return -1;
    }

    double number_of_terms = convert_str_long(argv[1]);

    double pi = BaileyBorweinPlouffe(0, number_of_terms);

    printf("Valor Calculado de PI foi: %.100lf\n", pi);

    return 0;
} /* main */