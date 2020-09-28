#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>

int count_insides(long seed, long counter){
    srand(seed);
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
    return 4.0 * (float) in / (float) counter;
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
        printf("É necessário informar 2 argumentos a seed inicial para o sorteio dos numeros e a quantidade de sorteios:\n");
        return -1;
    }

    long seed = convert_str_long(argv[1]);
    long counter = convert_str_long(argv[2]);

    clock_t t = clock();
    
    long in = count_insides(seed, counter);
    double pi = calculate_pi(in, counter);

    t = clock() - t; 

    printf("{\"PI\": %.50lf, \"time\": %.10lf}", pi, ((double)t) / CLOCKS_PER_SEC);

    return 0;
} /* main */