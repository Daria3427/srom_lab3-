#include <stdio.h>
#include <x86intrin.h> 
#include <windows.h>
#include "lab3.h"
#define ITERATIONS 1000

#define MEASURE_TICKS(func_call, iterations, name) do { \
    unsigned long long start_tick, end_tick; \
    start_tick = __rdtsc(); \
    for (int i = 0; i < iterations; i++) { \
        func_call; \
    } \
    end_tick = __rdtsc(); \
    printf("%-20s : %llu тактів\n", name, (end_tick - start_tick) / iterations); \
} while(0)


int main() {
    SetConsoleOutputCP(CP_UTF8);
    struct BigInt a, b, d, sum, mul, sq, pow, cov, trace, mul2;
    char num1[] = "7ddd53ff91904c10303829339aa48ebc1a3ef34e9038bcb51fc4dc3ef4b3cdacd0e888aeacc3bfa4bdfb212ab85dec3352d568f69671144";
    char num2[] = "197cb37e67d7f12d730fcd9938677ba977d02112d732edd6ae4fdc0108a30c607c3d5c0717ab6accb35ec606cb6c7ef6cb38a98d7fdcddf";
    char num3[] = "ba0b1f9163ce9ff57f43b7a3a69a8dca03580d7b71d8f56413";
    char num4[] = "";

    set_hex(&a, num1);
    printf("А = ");
    print_hex(&a);
    set_hex(&b, num2);
    printf("B = ");
    print_hex(&b);
    set_hex(&d, num3);
    printf("N = ");
    print_hex(&d);
    poly_add(&a, &b, &sum);
    printf("Сума = ");
    print_hex(&sum);
    poly_mul(&a, &b, &mul);
    printf("Добуток = ");
    print_hex(&mul);
    poly_square(&a, &sq);
    printf("Квадрат = ");
    print_hex(&sq);
    poly_conv(&a, &cov);
    printf("Обернений = ");
    print_hex(&cov);
    poly_mul(&a, &cov, &mul2);
    printf("Добуток елемента та його оберненого = ");
    print_hex(&mul2);
    poly_power(&a, &d, &pow);
    printf("Степінь = ");
    print_hex(&pow);
    find_trace(&a, &trace);
    printf("Слід = ");
    print_hex(&trace);







    



    // struct BigInt a, b, c, res1, res2, res3, temp1, temp2;
    // char num1[] = "7ddd53ff91904c10303829339aa48ebc1a3ef34e9038bcb51fc4dc3ef4b3cdacd0e888aeacc3bfa4bdfb212ab85dec3352d568f69671144";
    // char num2[] = "197cb37e67d7f12d730fcd9938677ba977d02112d732edd6ae4fdc0108a30c607c3d5c0717ab6accb35ec606cb6c7ef6cb38a98d7fdcddf";
    // char num3[] = "d0e888aeacc3bfa4bdfb212ab85dec3352d568f69671144d6ae4fdc0108a30c60d0e888aeacc3bfa4bdfb212ab85dec3352d58f6b77c51a";
    // char num4[] = "";

    
    // set_hex(&a, num1);
    // set_hex(&b, num2);
    // set_hex(&c, num3);


    // printf("\nТотожність №1:\n");
    
    // poly_add(&a, &b, &temp1);
    // poly_mul(&temp1, &c, &res1);
    // printf("Результат 1-ої частини: ");
    // print_hex(&res1);

    // init_zero(&temp1);
    // poly_mul(&a, &c, &temp1);
    // poly_mul(&b, &c, &temp2);
    // poly_add(&temp1, &temp2, &res2);
    // printf("Результат 2-ої частини: ");
    // print_hex(&res2);

    // printf("\nТотожність №2:\n");

    // struct BigInt d = {0};
    // d.words[0] = 0xFFFFFFFF;
    // for (int i = 1; i < 13; i++) {
    //     d.words[i] = 0xFFFFFFFF;
    // } d.words[13] = 0x07FFFFFF;
    // poly_power(&a, &d, &res3);
    // printf("Результат: ");
    // print_hex(&res3);


    // MEASURE_TICKS(reduction(&a), ITERATIONS, "reduction");
    // MEASURE_TICKS(poly_add(&a, &b, &c), ITERATIONS, "poly_add");
    // MEASURE_TICKS(poly_mul(&a, &b, &c), ITERATIONS, "poly_mul");
    // MEASURE_TICKS(poly_square(&a, &c), ITERATIONS, "poly_square");
    // MEASURE_TICKS(find_trace(&a, &c), ITERATIONS, "find_trace");
    // MEASURE_TICKS(poly_power(&a, &b, &c), ITERATIONS, "poly_power");
    // MEASURE_TICKS(poly_conv(&a, &c), ITERATIONS, "poly_conv");



    return 0;
}