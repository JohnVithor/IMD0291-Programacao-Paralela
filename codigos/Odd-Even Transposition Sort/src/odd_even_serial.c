#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol

void print_vector(double *vector, int n){
    printf("%lf", vector[0]);
    for (int i = 1; i < n; ++i) {
        printf(" %lf", vector[i]);
    }
}

void odd_even_sort(double *a, int n) {
    int phase, i, temp;
    for (phase = 0; phase < n; ++phase) {
        if(phase % 2 == 0) {
            for (i = 1; i < n; i += 2) {
                if (a[i-1] > a[i]) {
                    temp = a[i];
                    a[i] = a[i-1];
                    a[i-1] = temp;
                }
            }
        } else {
            for (i = 1; i < n-1; i += 2) {
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

double convert_str_double(char* str) {
    char *p;
    errno = 0;
    double conv = strtod(str, &p);

    if (errno != 0 || *p != '\0') {
        printf("%s não é um número!\n", str);
        exit(-1);
    }
    return (double) conv;
}

int main( int argc, char **argv ) {

    if (argc < 3) {
        printf("É necessário informar 2 argumentos na seguinte ordem:\n");
        printf("1º Tamanho do vetor\n2º Elementos do vetor\n");
        return -1;
    }

    int n = convert_str_int(argv[1]);

    if(argc - n != 2){
        printf("O número de elementos do vetor é diferente do numero de elementos esperados.\nNumero de elementos esperados: %d\nNúmero de elementos informados: %d\n", n, argc - 2);
        return -1;
    }

    double *a = NULL;
    
    a = calloc(n,sizeof(double));
    for (int i = 0; i < n; ++i) {
        a[i] = convert_str_double(argv[i+2]);
    }

    odd_even_sort(a, n);

    print_vector(a, n);

    free(a);

    return 0;
}  /* main */