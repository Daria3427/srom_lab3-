#ifndef LAB3_H
#define LAB3_H
#include <stdint.h>
#define MAX_WORDS 28

struct BigInt {
    uint32_t words[MAX_WORDS]; 
};


void init_zero(struct BigInt* a);
void init_one(struct BigInt* a);
void init_const(struct BigInt* a, uint32_t c);
void print_hex(struct BigInt* a);
void set_hex(struct BigInt* dest, const char* hex_str);
void get_rand(struct BigInt* a, const int d);
int bit_length(struct BigInt* a);
int oldest_one(uint32_t a);
int long_cmp(struct BigInt* a, struct BigInt* b);
void reduction(struct BigInt* a);
void poly_add(struct BigInt* a, struct BigInt* b, struct BigInt* c);
uint64_t mul32(uint32_t a, uint32_t b);
void poly_mul(struct BigInt* a, struct BigInt* b, struct BigInt* c);
void poly_square(struct BigInt* a, struct BigInt* c);
void find_trace(struct BigInt* a, struct BigInt* c);
void poly_power(struct BigInt* a, struct BigInt* d, struct BigInt* c);
void poly_conv(struct BigInt* a, struct BigInt* c);




#endif