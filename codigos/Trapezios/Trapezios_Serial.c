#include <stdio.h>

int get_double(int i){
    return 2 * i;
}

int calculate_trap(int a, int b, int n, int h, int (*func_ptr)(int i)) {
    h = (b - a) / n;
    int area_total = ((*func_ptr)(a) + (*func_ptr)(b)) / 2;
    
    for (int i = 1; i < n; ++i) {
        int x_i = a * i * h;
        area_total += (*func_ptr)(x_i);
    }
    return h * area_total;

}

int main(void) {
    int (*func_ptr)(int i);
    func_ptr = get_double;
    printf("Valor: %d",calculate_trap(2, 12, 2, 6, func_ptr));
    return 0;
}  /* main */