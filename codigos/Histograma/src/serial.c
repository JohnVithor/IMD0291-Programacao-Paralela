#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>

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

MinMaxPair getMinMaxIdx(double* array, long size){
    MinMaxPair result;
    result.min = 0;
    result.max = 0;
    for (long i = 1; i < size; ++i) {
        if (array[i] < array[result.min]){
            result.min = i;
        }
        if (array[i] > array[result.max]){
            result.max = i;
        }
    }
    return result;
}


void fillArray(double* array, long size, long seed){
    srand(seed);
    for (long i = 0; i < size; ++i) {
        array[i] = (double) rand() / INT_MAX;
    }
}

double* histogram(double* array, long size, long* result, long bins) {
    MinMaxPair pair = getMinMaxIdx(array, size);
    // printf("min: %.25lf, max:%.25lf\n", array[pair.min], array[pair.max]);
    double distance = (array[pair.max] - array[pair.min]) / bins;
    double *limits = malloc((bins+1)*sizeof(double));
    for (long i = 0; i < bins; ++i) {
        limits[i] = array[pair.min] + i*distance;
    }
    limits[bins] = array[pair.max];
    for (long i = 0; i < size; ++i) {
        // printf("%.16lf <= %.16lf: %d\n", array[i], array[i], array[i] <= array[i]);
        // printf("%.16lf <= %.16lf: %d\n", array[i], limits[bins], array[i] <= limits[bins]);
        for (long j = 0; j < bins; ++j) {
            if(array[i] >= limits[j] && array[i] <= limits[j+1]) {
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

    if (argc != 5) {
        printf("É necessário informar os seguintes argumentos:");
        return -1;
    }

    long show_data = convert_str_long(argv[1]);
    long seed = convert_str_long(argv[2]);
    long size = convert_str_long(argv[3]);
    long bins = convert_str_long(argv[4]);

    double *array = malloc(size*sizeof(double));
    long *result = calloc(bins, sizeof(long));
    fillArray(array, size, seed);
    
    clock_t t = clock();

    double* limits = histogram(array, size, result, bins);

    t = clock() - t; 
    printf("%.10lf\n", ((double)t) / CLOCKS_PER_SEC);
    if(show_data > 0){
        if(show_data > 1) {
            printf("No array: ");
            printArrayD(array, size);
        }
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
    free(array);
    free(result);
    free(limits);

    return 0;
} /* main */