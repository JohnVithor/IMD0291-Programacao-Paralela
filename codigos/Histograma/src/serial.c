#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>
#include "ltqnorm.c"

double rand_gen(unsigned int *state){
    // return a uniformly distributed random value
    return ( (double)(rand_r(state)) + 1. )/( (double)(RAND_MAX) + 1. );
}

double normalRandom(double sigma, double mi, unsigned int* state) {
    // return a normally distributed random value
    return (ltqnorm(rand_gen(state))*sigma) + mi;
}

void printArrayL(long* array, long size){
    for (long i = 0; i < size; ++i) {
        printf("%ld ", array[i]);
    }
    printf("\n");
}

void printArrayD(double* array, long size){
    for (long i = 0; i < size; ++i) {
        printf("%lf ", array[i]);
    }
    printf("\n");
}

double* histogram(long size, long* result, long bins, double sigma, double mi, unsigned int state) {
    double min = mi - (6 * sigma);
    double max = mi + (6 * sigma);
    double distance = (max - min) / bins;
    double *limits = malloc((bins+1)*sizeof(double));

    for (long i = 0; i < bins; ++i) {
        limits[i] = min + i*distance;
    }
    limits[bins] = max;

    for (long i = 0; i < size; ++i) {
        double item = normalRandom(sigma, mi, &state);
        for (long j = 0; j < bins; ++j) {
            if(item >= limits[j] && item <= limits[j+1]) {
                ++result[j];
                break;
            }
        }
    }
    return limits;
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

    if (argc != 7) {
        printf("É necessário informar os seguintes argumentos:\n");
        printf("Se devememos mostrar o resultado final do histograma (0 ou 1)\n");
        printf("Qual a seed a ser utilizada na geração dos números\n");
        printf("Qual o número de números a serem gerados\n");
        printf("Qual o range que será usado na geração dos números\n");
        printf("Qual será  valor central do qual os números serão gerados\n");
        return -1;
    }

    long show_data = convert_str_long(argv[1]);
    long seed = convert_str_long(argv[2]);
    long size = convert_str_long(argv[3]);
    long bins = convert_str_long(argv[4]);

    double sigma = convert_str_long(argv[5]);
    double mi = convert_str_long(argv[6]);
    unsigned int state = (seed+1) * (sigma+1) * (mi+1);
    long *result = calloc(bins, sizeof(long));
    
    clock_t t = clock();

    double* limits = histogram(size, result, bins, sigma/4, mi, state);

    t = clock() - t; 
    printf("%.10lf\n", ((double)t) / CLOCKS_PER_SEC);
    if(show_data > 0){
        printf("Temos:\n");
        printf("%ld itens no intervalo [%lf, %lf].\n", result[0], limits[0], limits[1]);
        for (long i = 1; i < bins; ++i) {
            printf("%ld itens no intervalo ]%lf, %lf].\n", result[i], limits[i], limits[i+1]);
        }
    }
    free(result);
    free(limits);

    return 0;
} /* main */