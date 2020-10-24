#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol
#include <time.h>

void print_and_validade_order(int *vector, int n){
    printf("%d", vector[0]);
    int less = vector[0];
    for (int i = 1; i < n; ++i) {
        printf(", %d", vector[i]);
        if(less > vector[i]){
            exit(-1);
        }
        less = vector[i];
    }
}

void odd_even_sort(int *a, int n) {
    int temp = 0;
    for (int phase = 0; phase < n; ++phase) {
        if(phase % 2 == 0) {
            for (int i = 1; i < n; i += 2) {
                if (a[i-1] > a[i]) {
                    temp = a[i];
                    a[i] = a[i-1];
                    a[i-1] = temp;
                }
            }
        } else {
            for (int i = 1; i < n-1; i += 2) {
                if (a[i] > a[i+1]) {
                    temp = a[i];
                    a[i] = a[i+1];
                    a[i+1] = temp;
                }
            }
        }
    }
    
}

int convert_str_int(char* str) {
    char *p;
    errno = 0;
    long conv = strtol(str, &p, 10);

    if (errno != 0 || *p != '\0' || conv > INT_MAX) {
        printf("%s não é um número!\n", str);
        exit(-1);
    }
    return (int) conv;
}

int main( int argc, char **argv ) {

if (argc != 4) {
        printf("É necessário informar 3 argumentos na seguinte ordem:\n");
        printf("1º Seed para gerar os números pseudo-aleatórios\n2º Tamanho do vetor\n3º 1 se o resultado da ordenação deve ser exibido e 0 caso contrário\n");
        return -1;
    }

    unsigned int seed = convert_str_int(argv[1]);
    int n = convert_str_int(argv[2]);
    int print_result = convert_str_int(argv[3]);

    srand(seed);
    int *a = malloc(n*sizeof(int));
    
    for (int i = 0; i < n; ++i) {
        a[i] = rand();
    }
    
    clock_t t = clock();
    odd_even_sort(a, n);
    t = clock() - t; 
    
    if (print_result == 1){
        printf("{\"Vetor\": [");
        print_and_validade_order(a, n);
        printf("], \"time\": %.10lf}\n", ((double)t) / CLOCKS_PER_SEC);
    } else {
        printf("{\"time\": %.10lf}\n", ((double)t) / CLOCKS_PER_SEC);
    }

    free(a);

    return 0;
}  /* main */