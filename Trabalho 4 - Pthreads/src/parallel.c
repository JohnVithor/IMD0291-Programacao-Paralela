#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#define __USE_POSIX199309 1
#include <time.h>
#include <pthread.h>
# include <stdint.h>

#include "ziggurat.c"

uint32_t kn[128];
float fn[128];
float wn[128];

int thread_count;

long size;
long *result;
long bins;
double *limits;
long seed;
unsigned int state;
double sigma;
double mi;
long local_size;

pthread_mutex_t* locks; 
pthread_mutex_t lock_r; 

double rand_gen(unsigned int* state){
    // return a uniformly distributed random value
    return ( (double)(rand_r(state)) + 1. )/( (double)(RAND_MAX) + 1. );
}

double truncNormalRandom(unsigned int* state) {
    // return a normally distributed random value
    //double value = ltqnorm(rand_gen(state));
    double value = (double) r4_nor((uint32_t*) state, kn, fn, wn);
    value = ((value * sigma) + mi) / 6;
    if (value < mi - sigma) {
        return mi - sigma;
    }
    if (value > mi + sigma) {
        return mi + sigma;
    }
    return value;
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

void *histogram_thread(void* rank){
    long my_rank = (long) rank;
    long my_start = my_rank * local_size;
    long my_end = (my_rank + 1) * local_size;
    if(my_rank == thread_count-1){
        my_end = size;
    }

    long *local_result = calloc(bins, sizeof(long));
    long counter = 0;
    long batch_size = (local_size / bins);

    double* my_data = malloc(batch_size*sizeof(double));

    pthread_mutex_lock(&lock_r); 
    for (long j = 0; j < batch_size; ++j) {
        my_data[j] = truncNormalRandom(&state);
    }
    pthread_mutex_unlock(&lock_r);

    for (long i = my_start; i < my_end; ++i) {
        double item = my_data[i % batch_size];
        for (long j = 0; j < bins; ++j) {
            if(item >= limits[j] && item <= limits[j+1]) {
                ++local_result[j];
                ++counter;
                break;
            }
        }
        if(counter == batch_size) {
            for (int j = 0; j < bins; ++j) {
                if(local_result[j] > 0) {
                    pthread_mutex_lock(locks+j); 
                    result[j] += local_result[j];
                    pthread_mutex_unlock(locks+j); 
                    local_result[j] = 0;
                }
            }
            pthread_mutex_lock(&lock_r); 
            for (long j = i; j < i + batch_size; ++j) {
                my_data[j - i] = truncNormalRandom(&state);
            }
            pthread_mutex_unlock(&lock_r);
            counter = 0;
        }
    }

    for (int j = 0; j < bins; ++j) {
        if(local_result[j] > 0) {
            pthread_mutex_lock(&(locks[j])); 
            result[j] += local_result[j];
            pthread_mutex_unlock(&(locks[j])); 
        }
    }

    free(local_result);
    free(my_data);
    return NULL;
}

double* histogram(pthread_t* threads_handles) {
    double min = mi - sigma;
    double max = mi + sigma;
    double distance = (max - min) / bins;
    limits = malloc((bins+1)*sizeof(double));

    for (long i = 0; i < bins; ++i) {
        limits[i] = min + i*distance;
    }
    limits[bins] = max;

    for (long thread = 0; thread < thread_count; ++thread) {
        pthread_create(&threads_handles[thread], NULL, histogram_thread, (void*) thread);
    }

    for (long thread = 0; thread < thread_count; ++thread) {
        pthread_join(threads_handles[thread], NULL);
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

    if (argc != 9) {
        printf("É necessário informar os seguintes argumentos:\n");
        printf("Quantas threads devem ser criadas\n");
        printf("Se devememos mostrar o resultado final do histograma (0 ou 1)\n");
        printf("Qual a seed a ser utilizada na geração dos números\n");
        printf("Qual o número de números a serem gerados\n");
        printf("Qual o número minimo de intervalos a serem usados\n");
        printf("Qual o número maximo de intervalos a serem usados\n");
        printf("Qual o range que será usado na geração dos números\n");
        printf("Qual será  valor central do qual os números serão gerados\n");
        return -1;
    }

    thread_count = convert_str_long(argv[1]);
    long show_data = convert_str_long(argv[2]);
    seed = convert_str_long(argv[3]);
    size = convert_str_long(argv[4]);
    long min_bins = convert_str_long(argv[5]);
    long max_bins = convert_str_long(argv[6]);

    srand(seed);
    bins = (rand() % (max_bins - min_bins + 1)) + min_bins; 
    r4_nor_setup(kn, fn, wn);
    sigma = convert_str_long(argv[7]);
    mi = convert_str_long(argv[8]);

    state = (seed+1) * (sigma+1) * (mi+1);

    local_size = size / thread_count;

    result = calloc(bins, sizeof(long));

    struct timespec start, finish;
    double elapsed;

    pthread_t* threads_handles;

    threads_handles = malloc(thread_count*sizeof(pthread_t));
    locks = malloc(bins*sizeof(pthread_mutex_t));

    for (long i = 0; i < bins; ++i) {
        pthread_mutex_init( locks+i, NULL);
    }
    pthread_mutex_init( &lock_r, NULL);

    clock_gettime(CLOCK_MONOTONIC, &start);

    double* limits = histogram(threads_handles);

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    if(show_data == 1){
        printf("[%lf, %lf];", limits[0], limits[1]);
        for (long i = 1; i < bins; ++i) {
            printf("]%lf, %lf];", limits[i], limits[i+1]);
        }
        printf("\n%ld;", result[0]);
        for (long i = 1; i < bins; ++i) {
            printf("%ld;", result[i]);
        }
    } else {
        printf("%.10lf\n", elapsed);
    }
    free(result);
    free(limits);
    free(threads_handles);

    for (long i = 0; i < bins; ++i) {
        pthread_mutex_destroy( locks+i);
    }
    pthread_mutex_destroy( &lock_r);

    return 0;
} /* main */
