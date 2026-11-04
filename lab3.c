#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "lab3.h"

#define GET_BIT(arr, bit)   (((arr)[(bit) / 32] >> ((bit) % 32)) & 1U)
#define XOR_BIT(arr, bit)   ((arr)[(bit) / 32] ^= (1U << ((bit) % 32)))
#define CLEAR_BIT(arr, bit) ((arr)[(bit) / 32] &= ~(1U << ((bit) % 32)))



void init_zero(struct BigInt* a) {
    memset(a->words, 0, sizeof(a->words));
}

void init_one(struct BigInt* a) {
    init_zero(a);
    a->words[0] = 1;
}

void init_const(struct BigInt* a, uint32_t c) {
    init_zero(a);
    a->words[0] = c;
}

void print_hex(struct BigInt* a) {
    int start_printing = 0;

    for (int i = MAX_WORDS - 1; i >= 0; i--) {
        if (a->words[i] != 0) {
            start_printing = 1;
        }
        if (start_printing) {
            printf("%08x", a->words[i]); 
        }
    }
    if (!start_printing) {
        printf("0");
    }
    printf("\n");
}

void set_hex(struct BigInt* a, const char* hex_str) {
    init_zero(a);

    int len = strlen(hex_str);
    int word_idx = 0; 

    for (int i = len; i > 0 && word_idx < MAX_WORDS; i -= 8) {
        char buffer[9] = {0}; 
        
        int start = i - 8;
        int copy_len = 8;

        if (start < 0) {
            copy_len = i;
            start = 0; }

        strncpy(buffer, &hex_str[start], copy_len);
        a->words[word_idx] = (uint32_t)strtoul(buffer, NULL, 16);
        word_idx++;
    }
}

void get_rand(struct BigInt* a, const int d) {
    init_zero(a);
    int words_count = d / 32;
    for (int i = 0; i < words_count; i++) {
        uint32_t part1 = rand();
        uint32_t part2 = rand();
        uint32_t part3 = rand();
        a->words[i] = (part1 << 17) | (part2 << 2) | (part3 & 0b11);
    }
}

int bit_length(struct BigInt* a) {
    for (int i = MAX_WORDS - 1; i >= 0; i--) {
        if (a->words[i] != 0) {
            int bits = oldest_one(a->words[i]);
            return (i * 32) + bits;
        }
    }
    return 0;
}

int oldest_one(uint32_t a) {
    int bit_pos = 1;
    while (a > 1) {
        a = a >> 1;
        bit_pos++;
    }
    return bit_pos;
}

int long_cmp(struct BigInt* a, struct BigInt* b) {
    int i = MAX_WORDS - 1;
    while (a->words[i] == b->words[i]) {
        i--;  
        if (i == -1)
            return 0;
    } if (a->words[i] > b->words[i]) {
        return 1;
    } else {
        return -1;
    }
}


// void fast_reduction_443(struct BigInt* a) {
//     // Максимальний степінь після множення двох поліномів степеня 442 дорівнює 884
//     for (int i = 884; i >= 443; i--) {
//         if (GET_BIT(a->words, i)) {
//             // Обнуляємо поточний старший біт
//             CLEAR_BIT(a->words, i);
            
//             // Математично додаємо (через XOR) залишок генератора
//             XOR_BIT(a->words, i - 443 + 28);
//             XOR_BIT(a->words, i - 443 + 3);
//             XOR_BIT(a->words, i - 443 + 1);
//             XOR_BIT(a->words, i - 443 + 0);
//         }
//     }
//     for (int i = 14; i < MAX_WORDS; i++) {
//         a->words[i] = 0;
//     }
// }

void reduction(struct BigInt* a) {
    int len = bit_length(a);
    int word_len = len / 32;
    for (int i = word_len; i >= 14; i--) {
        uint32_t w = a->words[i];
        if (w == 0) continue;

        a->words[i] = 0;
        uint64_t prod = (uint64_t)w ^ 
                        ((uint64_t)w << 1) ^ 
                        ((uint64_t)w << 3) ^ 
                        ((uint64_t)w << 28);
        // Зсув на 443 біти назад відповідає позиції: (i - 14) слів + 5 бітів
        int j = i - 14;
        uint64_t shifted = prod << 5;

        a->words[j] ^= (uint32_t)(shifted & 0xFFFFFFFF);
        a->words[j + 1] ^= (uint32_t)(shifted >> 32);
        // рівно один 59-й біт стане бітом 0 у words[j + 2]
        uint32_t overflow_bit = (uint32_t)(prod >> 59);
        if (overflow_bit) {
            a->words[j + 2] ^= overflow_bit;
        }
    }

    // Редукція 5 зайвих бітів (степені 443..447) у words[13]
    uint32_t upper_bits = a->words[13] >> 27; 
    if (upper_bits != 0) {
        a->words[13] &= 0x07FFFFFF; 

        uint64_t prod = (uint64_t)upper_bits ^ 
                        ((uint64_t)upper_bits << 1) ^ 
                        ((uint64_t)upper_bits << 3) ^ 
                        ((uint64_t)upper_bits << 28);

        a->words[0] ^= (uint32_t)(prod & 0xFFFFFFFF);
        a->words[1] ^= (uint32_t)(prod >> 32);
    }
}

void poly_add(struct BigInt* a, struct BigInt* b, struct BigInt* c) {
    init_zero(c);
    int d1 = bit_length(a);
    int d2 = bit_length(b);
    int max_bits = (d1 > d2) ? d1 : d2;
    int words = (max_bits + 31) / 32;
    for (int i = 0; i < words; i++) {
        c->words[i] = a->words[i] ^ b->words[i];
    }
}

uint64_t mul32(uint32_t a, uint32_t b) {
    uint64_t res = 0;
    uint64_t b64 = b;
    for (int i = 0; i < 32; i++) {
        if ((a >> i) & 1U) {
            res ^= (b64 << i);
        }
    }
    return res;
}

void poly_mul(struct BigInt* a, struct BigInt* b, struct BigInt* c) {
    init_zero(c);
    struct BigInt temp = {0};

    for (int i = 0; i < 14; i++) {
        if (a->words[i] == 0) continue;
        for (int j = 0; j < 14; j++) {
            if (b->words[j] == 0) continue;

            uint64_t prod = mul32(a->words[i], b->words[j]);
            temp.words[i + j]     ^= (uint32_t)(prod);
            temp.words[i + j + 1] ^= (uint32_t)(prod >> 32);
        }
    }

    reduction(&temp);
    *c = temp;
}

void poly_square(struct BigInt* a, struct BigInt* c) {
    struct BigInt temp = {0};
    int len = bit_length(a);
    int word_len = (len + 31) / 32;
    for (int i = 0; i < word_len; i++) {
        if (a->words[i] == 0) continue;
        uint64_t temp64 = 0;
        for (int j = 0; j < 32; j++) {
           int bit = (a->words[i] >> j) & 1U;
           temp64 ^= ((uint64_t)bit << (j * 2));
        }
        temp.words[2 * i] = (uint32_t)temp64;
        temp.words[2 * i + 1] = (uint32_t)(temp64 >> 32);
    }
    reduction(&temp);
    *c = temp;
}

void find_trace(struct BigInt* a, struct BigInt* c) {
    struct BigInt temp = *a;
    struct BigInt add = *a;
    struct BigInt sum;
    for (int i = 1; i < 443; i++) {
        poly_square(&temp, &temp);
        poly_add(&add, &temp, &sum);
        add = sum;
    }
    *c = add;
}

void poly_power(struct BigInt* a, struct BigInt* d, struct BigInt* c) {
    init_one(c);
    struct BigInt temp, local_a;
    local_a = *a;

    int m = bit_length(d);
    for (int i = 0; i < m; i++) {
        int word_idx = i / 32; 
        int bit_idx = i % 32; 
        if ((d->words[word_idx] & (1U << bit_idx)) != 0) {
            poly_mul(c, &local_a, &temp);
            *c = temp;
        } 
        if (i != m - 1) {
            poly_square(&local_a, &temp);
            local_a = temp;
        }
    }
}

void poly_conv(struct BigInt* a, struct BigInt* c) {
    struct BigInt d = {0};
    d.words[0] = 0xFFFFFFFE;
    for (int i = 1; i < 13; i++) {
        d.words[i] = 0xFFFFFFFF;
    } d.words[13] = 0x07FFFFFF;
    poly_power(a, &d, c);
}



