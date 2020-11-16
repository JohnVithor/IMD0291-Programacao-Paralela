#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol

#define __USE_POSIX199309 1
#include <time.h>
#include <pthread.h>

int thread_count;

double *array;
long size;

long *result;
long bins;
double *limits;

long local_size;

typedef struct {
    long min;
    long max;
} MinMaxPair;

typedef struct {
   MinMaxPair pair;
   long rank;
} Thread_data;

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

MinMaxPair getMinMaxIdxThread(void* thread_data) {
    Thread_data *my_data = (Thread_data*) thread_data;
    long start = my_data->rank * local_size;
    //long end = (my_rank + 1) * local_size;
    my_data->pair = getMinMaxIdx(array+start , local_size);
}

MinMaxPair getMinMaxIdxParallel(double* array, long size, pthread_t* threads_handles){
    MinMaxPair* possible_max_min = malloc(thread_count*sizeof(MinMaxPair));
    for (long thread = 0; thread < thread_count; ++thread) {
        pthread_create(&threads_handles[thread], NULL, getMinMaxIdxThread, (void*) (possible_max_min+thread));
    }

    for (long thread = 0; thread < thread_count; ++thread) {
        pthread_join(threads_handles[thread], NULL);
    }
    MinMaxPair pair = possible_max_min[0];
    for (size_t i = 1; i < thread_count; ++i) {
        if(possible_max_min[0].min < pair.min){
            pair.min = possible_max_min[0].min;
        }
        if(possible_max_min[0].max < pair.max){
            pair.max = possible_max_min[0].max;
        }
    }
    
    free(possible_max_min);
    return pair;
}


void fillArray(double* array, long size, long seed){
    srand(seed);
    for (long i = 0; i < size; ++i) {
        array[i] = (double) rand() / INT_MAX;
    }
}

double* histogram(pthread_t* threads_handles) {
    MinMaxPair pair = getMinMaxIdxParallel(array, size, threads_handles);
    // printf("min: %.25lf, max:%.25lf\n", array[pair.min], array[pair.max]);
    double distance = (array[pair.max] - array[pair.min]) / bins;
    limits = malloc((bins+1)*sizeof(double));
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

    if (argc != 6) {
        printf("É necessário informar os seguintes argumentos:\n");
        return -1;
    }

    thread_count = convert_str_long(argv[1]);
    long show_data = convert_str_long(argv[2]);
    long seed = convert_str_long(argv[3]);
    size = convert_str_long(argv[4]);
    bins = convert_str_long(argv[5]);

    local_size = size / thread_count;

    array = malloc(size*sizeof(double));
    result = calloc(bins, sizeof(long));
    fillArray(array, size, seed);

    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    pthread_t* threads_handles;

    threads_handles = malloc(thread_count*sizeof(pthread_t));

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("%.10lf\n", elapsed);

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

    free(threads_handles);

    free(array);
    free(result);
    free(limits);

    return 0;
} /* main */