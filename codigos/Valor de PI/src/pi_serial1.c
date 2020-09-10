#include <stdio.h>
#include <errno.h>  // for errno
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol

double calculate_pi(long root, long number_of_terms){
    srand(root);

    int in = 0, out = 0;
    double  x, y;

    while (--number_of_terms > 0) {
        x = (double) rand() / (double)INT_MAX;
        y = (double) rand() / (double)INT_MAX;
        double d = x*x + y*y;
        ( d <= 1.0 ) ? ++in : ++out;
    }

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

int main(int argc, char **argv){

    if (argc != 3)
    {
        printf("É necessário informar 2 argumento a raiz aleatória e o número de termos:\n");
        return -1;
    }

    double root = convert_str_long(argv[1]);
    double number_of_terms = convert_str_long(argv[2]);

    double pi = calculate_pi(root, number_of_terms);

    printf("Valor Calculado de PI foi: %lf\n", pi);

    return 0;
} /* main */