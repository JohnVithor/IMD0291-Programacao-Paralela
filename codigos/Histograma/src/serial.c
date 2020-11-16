#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>

#define M_PI 3.14159265358979323846

double rand_gen() {
   // return a uniformly distributed random value
   return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}
double normalRandom(double sigma, double mi) {
   // return a normally distributed random value
   return (cos(2*M_PI*rand_gen())*sqrt(-2.*log(rand_gen()))*sigma) + mi;
}

typedef struct{
    long min;
    long max;
} MinMaxPair;

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

MinMaxPair getMinMaxNormal(double sigma, double mi){
    MinMaxPair result;
    result.min = mi - (4 * sigma);
    result.min = mi + (4 * sigma);
    return result;
}


void fillArray(double* array, long size, long seed){
    srand(seed);
    for (long i = 0; i < size; ++i) {
        array[i] = normalRandom(0.5, 0);
    }
}

double* histogram(long size, long* result, long bins, double sigma, double mi) {
    double min = mi - (4 * sigma);
    double max = mi + (4 * sigma);
    // printf("min: %.25lf, max:%.25lf\n", array[pair.min], array[pair.max]);
    double distance = (max - min) / bins;
    double *limits = malloc((bins+1)*sizeof(double));
    for (long i = 0; i < bins; ++i) {
        limits[i] = min + i*distance;
    }
    limits[bins] = max;
    for (long i = 0; i < size; ++i) {
        // printf("%.16lf <= %.16lf: %d\n", array[i], array[i], array[i] <= array[i]);
        // printf("%.16lf <= %.16lf: %d\n", array[i], limits[bins], array[i] <= limits[bins]);
        double item = normalRandom(sigma, mi);
        for (long j = 0; j < bins; ++j) {
            if(item >= limits[j] && item <= limits[j+1]) {
                // printf("%ld - %lf está no intervalo %ld: [%lf, %lf].\n", i, array[i], j, limits[j], limits[j+1]);
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
        printf("É necessário informar os seguintes argumentos:");
        return -1;
    }

    long show_data = convert_str_long(argv[1]);
    long seed = convert_str_long(argv[2]);
    srand(seed);
    long size = convert_str_long(argv[3]);
    long bins = convert_str_long(argv[4]);

    double sigma = convert_str_long(argv[5]);
    double mi = convert_str_long(argv[6]);

    long *result = calloc(bins, sizeof(long));
    
    clock_t t = clock();

    double* limits = histogram(size, result, bins, sigma, mi);

    t = clock() - t; 
    printf("%.10lf\n", ((double)t) / CLOCKS_PER_SEC);
    if(show_data > 0){
        printf("Temos:\n");
        printf("%ld itens no intervalo [%lf, %lf].\n", result[0], limits[0], limits[1]);
        for (long i = 1; i < bins; ++i) {
            printf("%ld itens no intervalo ]%lf, %lf].\n", result[i], limits[i], limits[i+1]);
        }
        // long sum = 0;
        // for (long i = 0; i < bins; ++i) {
        //     sum += result[i];
        // }
        // if(sum == size){
        //     printf("OK\n");
        // }
        
    }
    free(result);
    free(limits);

    return 0;
} /* main */