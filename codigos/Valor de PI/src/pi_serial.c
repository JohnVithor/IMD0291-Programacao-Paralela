#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>

void count_in_out(long seed, long counter, long* in, long* out){
    srand(seed);
    *in = 0;
    *out = 0;
    double d, x, y;
    while (--counter > 0) {
        x = (double) rand() / (double)INT_MAX;
        y = (double) rand() / (double)INT_MAX;
        d = x*x + y*y;
        (d <= 1.0) ? ++*in : ++*out;
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

int main(int argc, char **argv){

    if (argc != 3)
    {
        printf("É necessário informar 2 argumentos a seed inicial para o sorteio dos numeros e a quantidade de sorteios:\n");
        return -1;
    }

    long seed = convert_str_long(argv[1]);
    long counter = convert_str_long(argv[2]);
    double times[10]; 
    double pis[10]; 

    for (long i = 0; i < 10; ++i) {
        clock_t t = clock(); 
        long in = 0;
        long out = 0;
        count_in_out(seed, counter, &in, &out);
        pis[i] = calculate_pi(in, out);
        t = clock() - t; 
        times[i] = ((double)t) / CLOCKS_PER_SEC;
    }
    
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

    return 0;
} /* main */