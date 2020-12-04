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

ValueType threads_number;

void print_order(UnsignedVector vector, long size){
    printf("%u", vector[0]);
    for (int i = 1; i < size; ++i) {
        printf(", %u", vector[i]);
    }
}

void print_and_validade_order(UnsignedVector vector, long size){
    printf("%u", vector[0]);
    int less = vector[0];
    for (int i = 1; i < size; ++i) {
        printf(", %u", vector[i]);
        if(less > vector[i]){
            printf("\nNot sorted!\n");
            exit(-1);
        }
        less = vector[i];
    }
}

void validade_order(UnsignedVector vector, long size){
    int less = vector[0];
    for (int i = 1; i < size; ++i) {
        if(less > vector[i]){
            printf("Not sorted!\n");
            exit(-1);
        }
        less = vector[i];
    }
}

void merge_vector(UnsignedVector vector, ValueType left, ValueType mid, ValueType right) {
	ValueType size_A = right - left + 1;
	ValueType size_L = mid - left + 1;
	ValueType size_R = size_A - size_L;

    ValueType* L_aux = malloc(size_L*sizeof(ValueType));
	ValueType* R_aux = malloc(size_R*sizeof(ValueType));

	memcpy(L_aux, vector + left		, sizeof(ValueType)*size_L);
    memcpy(R_aux, vector + (mid + 1), sizeof(ValueType)*size_R);

	ValueType i = 0;
	ValueType j = 0;
	ValueType k = left;

    while(i < size_L && j < size_R) {
        vector[k++] = (L_aux[i] < R_aux[j]) ? L_aux[i++] : R_aux[j++];
    }

	if(i < size_L) {
        memcpy(vector + k, L_aux + i, sizeof(ValueType)*(size_L-i));
	}
	else {
        memcpy(vector + k, R_aux + j, sizeof(ValueType)*(size_R-j));
	}

	free(L_aux);
	free(R_aux);
}

void merge_sort_vector_internal(UnsignedVector vector, ValueType left, ValueType right) {
	if(right > left) {
		ValueType mid = left + (right - left) / 2;
        merge_sort_vector_internal(vector, left, mid);
        merge_sort_vector_internal(vector, mid + 1, right);
        merge_vector(vector, left, mid, right);
	}
}

void merge_vectors(UnsignedVector vector, UnsignedVector limits, ValueType size) {
    if(size < 2){
        return;
    }
    if(size == 2){
        merge_vector(vector, limits[0], limits[(size/2)]-1, limits[size]-1);
        return;
    }
    if(size == 3){
        merge_vector(vector, limits[0], limits[(size/2)]-1, limits[size-1]-1);
        merge_vector(vector, limits[0], limits[size-1]-1, limits[size]-1);
        return;
    }
    if(size % 2 == 0) {
        #pragma omp parallel sections num_threads(threads_number) default(none) shared(vector, limits, size)
        {
            #pragma omp section
            {
                merge_vectors(vector, limits, (size/2) );
            }
            #pragma omp section
            {
                merge_vectors(vector, limits + (size/2), (size/2));
            }           
        }
        merge_vector(vector, limits[0], limits[(size/2)]-1, limits[size]-1);
    } else {
        #pragma omp parallel sections num_threads(threads_number) default(none) shared(vector, limits, size)
        {
            #pragma omp section
            {
                merge_vectors(vector, limits, (size/2) );
            }
            #pragma omp section
            {
                merge_vectors(vector, limits + (size/2), (size/2));
            }
        }
        merge_vector(vector, limits[0], limits[(size/2)]-1, limits[size-1]-1);
        merge_vector(vector, limits[0], limits[size-1]-1, limits[size]-1);
    }
}

void merge_sort_vector(UnsignedVector vector, ValueType left, ValueType right) {
    ValueType distance = (right - left) / threads_number;
    UnsignedVector limits = malloc((threads_number+1)*sizeof(double));

    for (ValueType i = 0; i < threads_number; ++i) {
        limits[i] = (left + i*distance);
    }
    limits[threads_number] = right;

    #pragma omp parallel num_threads(threads_number) default(none) shared(vector, limits, threads_number)
    {
        #pragma omp for schedule(guided)
        for (ValueType j = 0; j < threads_number; ++j) {
            merge_sort_vector_internal(vector, limits[j], limits[j+1]-1);
        }
    }
    merge_vectors(vector, limits, threads_number);

    free(limits);
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

	if (argc != 5) {
        printf("É necessário informar 3 argumentos na seguinte ordem:\n");
        printf("1º Seed para gerar os números pseudo-aleatórios\n2º Tamanho do vetor\n3º 1 se o resultado da ordenação deve ser exibido e 0 caso contrário\n");
        return -1;
    }

    threads_number = convert_str_long(argv[1]);
    ValueType seed = convert_str_long(argv[2]);
    ValueType size = convert_str_long(argv[3]);
    int print_result = convert_str_long(argv[4]);

    srand(seed);
    UnsignedVector vector = malloc(size*sizeof(ValueType));
    
    for (int i = 0; i < size; ++i) {
        vector[i] = rand();
    }

    double t = omp_get_wtime();
    merge_sort_vector(vector, 0, size);
    t = omp_get_wtime() - t; 
    
    if (print_result == 1){
        printf("{\"Vetor\": [");
        print_order(vector, size);
        printf("], \"time\": %.10lf}\n", t);
        validade_order(vector, size);
    } else {
        validade_order(vector, size);
        printf("%.10lf\n", t);
    }

    free(vector);

    return 0;
}  /* main */