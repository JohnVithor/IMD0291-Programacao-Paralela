#include <stdio.h>
#include <errno.h>  // for errno
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol

double calculate_pi(long number_of_terms){
    double pi = 0;
    double divisor = 1;
    for (long i = 1; i <= number_of_terms; ++i) {
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

int main(int argc, char **argv){

    if (argc != 2)
    {
        printf("É necessário informar apenas 1 argumento o número de termos:\n");
        return -1;
    }

    double number_of_terms = convert_str_long(argv[1]);

    double pi = calculate_pi(number_of_terms);

    printf("Valor Calculado de PI foi: %lf\n", pi);

    return 0;
} /* main */