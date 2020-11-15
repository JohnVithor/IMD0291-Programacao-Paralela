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

void histogram(double* array, long size, long* result, long bins) {
    MinMaxPair minMaxPair = getMinMaxIdx(array, size);
    for (long i = 0; i < size; ++i) {

    }    
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

    if (argc != 0) {
        printf("É necessário informar os seguintes argumentos:");
        return -1;
    }

    int show_matrix = convert_str_long(argv[1]);


    clock_t t = clock();


    t = clock() - t; 

    return 0;
} /* main */