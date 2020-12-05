#include <stdio.h>
#include <string.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>
#include <omp.h>

typedef unsigned int ValueType;
typedef unsigned int* UnsignedVector;

UnsignedVector aux;

void print_and_validade_order(UnsignedVector vector, long size){
    printf("%u", vector[0]);
    int less = vector[0];
    for (int i = 1; i < size; ++i) {
        printf(", %u", vector[i]);
        if(less > vector[i]){
            exit(-1);
        }
        less = vector[i];
    }
}

void merge_vector(UnsignedVector vector, ValueType left, ValueType mid, ValueType right) {
    ValueType size_A = right - left + 1;
    ValueType size_L = mid - left + 1;
    ValueType size_R = size_A - size_L;

    memcpy(aux+left, vector + left, sizeof(ValueType)*size_L);
    memcpy(aux+(mid+1), vector + (mid + 1), sizeof(ValueType)*size_R);

    ValueType i = left;
    ValueType j = mid;
    ValueType k = left;

    while(i < mid && j < right) {
        vector[k++] = (aux[i] < aux[j]) ? aux[i++] : aux[j++];
    }

    if(i < mid) {
        memcpy(vector + k, aux + i, sizeof(ValueType)*(mid-i));
    }
    else {
        memcpy(vector + k, aux + j, sizeof(ValueType)*(right-j));
    }
}

void merge_sort_vector_internal (UnsignedVector vector, ValueType left, ValueType right) {
    if(right > left) {
        ValueType mid = left + (right - left) / 2;
        merge_sort_vector_internal(vector, left, mid);
        merge_sort_vector_internal(vector, mid + 1, right);
        merge_vector(vector, left, mid, right);
    }
}

void merge_sort_vector(UnsignedVector vector, ValueType left, ValueType right) {
    --right;
    merge_sort_vector_internal(vector, left, right);
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

int main( int argc, char **argv ) {

    if (argc != 4) {
        printf("É necessário informar 3 argumentos na seguinte ordem:\n");
        printf("1º Seed para gerar os números pseudo-aleatórios\n2º Tamanho do vetor\n3º 1 se o resultado da ordenação deve ser exibido e 0 caso contrário\n");
        return -1;
    }

    unsigned int seed = convert_str_long(argv[1]);
    unsigned int size = convert_str_long(argv[2]);
    int print_result = convert_str_long(argv[3]);

    srand(seed);
    UnsignedVector vector = malloc(size*sizeof(ValueType));
    aux = malloc(size*sizeof(ValueType));
    
    for (int i = 0; i < size; ++i) {
        vector[i] = rand();
    }
    
    clock_t t = clock();
    merge_sort_vector(vector, 0, size);
    t = clock() - t; 
    
    if (print_result == 1){
        printf("{\"Vetor\": [");
        print_and_validade_order(vector, size);
        printf("], \"time\": %.10lf}\n", ((double)t) / CLOCKS_PER_SEC);
    } else {
        printf("%.10lf\n", ((double)t) / CLOCKS_PER_SEC);
    }

    free(vector);
    free(aux);

    return 0;
}  /* main */