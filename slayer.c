#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#define DEBUG

// declare Plaintext, Secret Key and ciphertext for PIPO-64/128
const uint64_t plaintext = 0x098552F61E270026;
const uint64_t k1 = 0x6DC416DD779428D2;
const uint64_t k0 = 0x7E1D20AD2E152297;
const uint64_t ciphertext = 0x6B6B2981AD5D0327;

uint8_t secand (uint8_t a, uint8_t b, uint8_t a1, uint8_t b1, uint8_t mask) {
    // declare variables
    uint8_t r4, r5; // temp variables
    uint8_t a0 = a ^ a1;
    uint8_t b0 = b ^ b1;
    
    r4 = b0;    // step 01
    r4 &= a0;   // step 02
    r4 ^= mask; // step 03

    r5 = a0;    // step 04
    r5 &= b1;   // step 05
    r4 ^= r5;   // step 06

    r5 = a1;    // step 07
    r5 &= b0;   // step 08
    r4 ^= r5;   // step 09

    r5 = a1;    // step 10
    r5 &= b1;   // step 11
    r4 ^= r5;   // step 12

    return r4;
}

uint8_t secor (uint8_t a, uint8_t b, uint8_t a1, uint8_t b1, uint8_t c) {
    // declare variables
    uint8_t r4, r5, r6; // temp variables
    uint8_t a0 = a ^ a1;
    uint8_t b0 = b ^ b1;

    r4 = c;  // step 1
    r4 ^= b1;   // step 2
    r5 = b0;    // step 3
    r5 |= a1;   // step 4
    r5 ^= r4;   // step 5

    r4 = b0;    // step 6
    r4 &= a0;   // step 7
    r6 = a0;    // step 8
    r6 |= b1;   // step 9
    r4 ^= r6;   // step 10
    r4 ^= r5;   // step 11

    return r4;
}

void slayer (uint8_t *X, uint8_t m0, uint8_t m1, uint8_t m2) {
    ///////////
    // S-Box //
    ///////////

    // declare helping variables
    uint8_t T[3] = {0, };
    uint8_t temp;
    uint8_t c = (m0 | m1);
    printf("c = %02X\n", c);

// S5_1
    temp = secand(X[7], X[6], m0, m1, m2);
    printf("04: SecAND(%02X, %02X) = %02X\n", X[7], X[6], temp);
    X[5] = X[5] ^ temp;
    printf("05: X[5] = %02X\n", X[5]);

    temp = secand(X[3], X[5], m2, m0, m1);
    printf("06: SecAND(%02X, %02X) = %02X\n", X[3], X[5], temp);
    X[4] = X[4] ^ temp;
    printf("07: X[4] = %02X\n", X[4]);
    X[7] = X[7] ^ X[4];
    printf("08: X[7] = %02X\n", X[7]);
    X[6] = X[6] ^ X[3];
    printf("09: X[6] = %02X\n", X[6]);

    temp = secor(X[5], X[4], m0, m2, c);
    printf("10: SecOR(%02X, %02X) = %02X\n", X[5], X[4], temp);
    X[3] = X[3] ^ temp;
    printf("11: X[3] = %02X\n", X[3]);
    X[5] = X[5] ^ X[7];
    printf("12: X[5] = %02X\n", X[5]);

    temp = secand(X[5], X[6], m2, m0, m1);
    printf("13: SecAND(%02X, %02X) = %02X\n", X[5], X[6], temp);
    X[4] = X[4] ^ temp;
    printf("14: X[4] = %02X\n", X[4]);

// S3
    temp = secand(X[1], X[0], m1, m0, m2);
    printf("15: SecAND(%02X, %02X) = %02X\n", X[1], X[0], temp);
    X[2] = X[2] ^ temp;
    printf("16: X[2] = %02X\n", X[2]);

    temp = secor(X[1], X[2], m1, m0, c);
    printf("17: SecOR(%02X, %02X) = %02X\n", X[1], X[2], temp);
    X[0] = X[0] ^ temp;
    printf("18: X[0] = %02X\n", X[0]);

    temp = secor(X[2], X[0], m0, m2, c);
    printf("19: SecOR(%02X, %02X) = %02X\n", X[2], X[0], temp);
    X[1] = X[1] ^ temp;
    printf("20: X[1] = %02X\n", X[1]);
    X[2] = X[2];
    printf("21: X[2] = %02X\n", X[2]);

// Extend XOR
    X[7] = X[7] ^ X[1];
    printf("22: X[7] = %02X\n", X[7]);
    X[3] = X[3] ^ X[2];
    printf("23: X[3] = %02X\n", X[3]);
    X[4] = X[4] ^ X[0];
    printf("24: X[4] = %02X\n", X[4]);

// S5_2
    T[0] = X[7];
    printf("25: T[0] = %02X\n", T[0]);
    T[1] = X[3];
    printf("26: T[1] = %02X\n", T[1]);
    T[2] = X[4];
    printf("27: T[2] = %02X\n", T[2]);

    temp = secand(X[5], T[0], m2, m0, m1);
    printf("28: SecAND(%02X, %02X) = %02X\n", X[5], T[0], temp);
    X[6] = X[6] ^ temp;
    printf("29: X[6] = %02X\n", X[6]);

    T[0] = T[0] ^ X[6];
    printf("30: T[0] = %02X\n", T[0]);

    temp = secor(T[2], T[1], m1, m2, c);
    printf("31: SecOR(%02X, %02X) = %02X\n", T[2], T[1], temp);
    X[6] = X[6] ^ temp;
    printf("32: X[6] = %02X\n", X[6]);

    T[1] = X[5] ^ m1;
    printf("33: T[1] = %02X\n", T[1]);
    
    temp = secor(X[6], T[2], m0, m1, c);
    printf("34: SecOR(%02X, %02X) = %02X\n", X[6], T[2], temp);
    X[5] = X[5] ^ temp;
    printf("35: X[5] = %02X\n", X[5]);

    temp = secand(T[1], T[0], m0, m1, m2);
    printf("36: SecAND(%02X, %02X) = %02X\n", T[1], T[0], temp);
    T[2] = T[2] ^ temp;
    printf("37: T[2] = %02X\n", T[2]);

// truncate XOR and swap
    X[2] = X[2] ^ T[0];
    printf("38: X[2] = %02X\n", X[2]);
    T[0] = X[1] ^ T[2];
    printf("39: T[0] = %02X\n", T[0]);
    X[1] = X[0] ^ T[1];
    printf("40: X[1] = %02X\n", X[1]);
    X[0] = X[7] ^ m1;
    printf("41: X[0] = %02X\n", X[0]);
    X[7] = T[0];
    printf("42: X[7] = %02X\n", X[7]);
    T[1] = X[3];
    printf("43: T[1] = %02X\n", T[1]);
    X[3] = X[6];
    printf("44: X[3] = %02X\n", X[3]);
    X[6] = T[1];
    printf("45: X[6] = %02X\n", X[6]);
    T[2] = X[4];
    printf("46: T[2] = %02X\n", T[2]);
    X[4] = X[5];
    printf("47: X[4] = %02X\n", X[4]);
    X[5] = T[2];
    printf("48: X[5] = %02X\n", X[5]);
}

void add_mask (uint8_t *x, uint8_t *m) {
    x[0] ^= m[1];
    x[1] ^= m[0];
    x[2] ^= m[0];
    x[3] ^= m[0];
    x[4] ^= m[1];
    x[5] ^= m[0];
    x[6] ^= m[0];
    x[7] ^= m[1];
}


int main () {

    // init var
    uint8_t mrk[8];
    srand(time(NULL));

    // mask initialization
    uint8_t m[10];
    m[0] = 0xAE; //rand() % 256;
    m[1] = 0x6A; //rand() % 256;
    m[2] = m[0] ^ m[1];
    m[3] = ((m[1] >> 1) | (m[1] << 7)) ^ m[1];
    m[4] = ((m[2] >> 4) | (m[2] << 4)) ^ m[1];
    m[5] = ((m[0] >> 5) | (m[0] << 3)) ^ m[2];
    m[6] = ((m[1] >> 2) | (m[1] << 6)) ^ m[0];
    m[7] = ((m[1] >> 3) | (m[1] << 5)) ^ m[1];
    m[8] = ((m[2] >> 7) | (m[2] << 1)) ^ m[1];
    m[9] = ((m[1] >> 6) | (m[1] << 2)) ^ m[0];
    
#ifdef DEBUG
    printf("m0 = 0x%02X\n", m[0]);
    printf("m1 = 0x%02X\n", m[1]);
    printf("m2 = 0x%02X\n", m[2]);
    printf("m3 = 0x%02X\n", m[3]);
    printf("m4 = 0x%02X\n", m[4]);
    printf("m5 = 0x%02X\n", m[5]);
    printf("m6 = 0x%02X\n", m[6]);
    printf("m7 = 0x%02X\n", m[7]);
    printf("m8 = 0x%02X\n", m[8]);
    printf("m9 = 0x%02X\n", m[9]);
#endif

    // split 64-bit Input into 8 bytes --> MSB X[7] ... X[0] LSB
    uint8_t X[8];
    X[0] = plaintext >> 0;
    X[1] = plaintext >> 8;
    X[2] = plaintext >> 16;
    X[3] = plaintext >> 24;
    X[4] = plaintext >> 32;
    X[5] = plaintext >> 40;
    X[6] = plaintext >> 48;
    X[7] = plaintext >> 56;

    // split key k0 and k1 into 8 bytes
    uint8_t k_0[8];
    k_0[0] = k0 >> 0;
    k_0[1] = k0 >> 8;
    k_0[2] = k0 >> 16;
    k_0[3] = k0 >> 24;
    k_0[4] = k0 >> 32;
    k_0[5] = k0 >> 40;
    k_0[6] = k0 >> 48;
    k_0[7] = k0 >> 56;

    uint8_t k_1[8];
    k_1[0] = k1 >> 0;
    k_1[1] = k1 >> 8;
    k_1[2] = k1 >> 16;
    k_1[3] = k1 >> 24;
    k_1[4] = k1 >> 32;
    k_1[5] = k1 >> 40;
    k_1[6] = k1 >> 48;
    k_1[7] = k1 >> 56;

#ifdef DEBUG
    printf("\nRound 0:");
    printf("\n  Plaintext:    %02X %02X %02X %02X   %02X %02X %02X %02X", X[7], X[6], X[5], X[4], X[3], X[2], X[1], X[0]);
#endif

    // Add Mask
    add_mask(X, m);

#ifdef DEBUG
    printf("\n  ADDED MASK:   %02X %02X %02X %02X   %02X %02X %02X %02X", X[7], X[6], X[5], X[4], X[3], X[2], X[1], X[0]);
#endif

    // XOR masked Whitening Key
    X[0] ^= k_0[0] ^ m[2];
    X[1] ^= k_0[1] ^ m[2];
    X[2] ^= k_0[2] ^ m[2];
    X[3] ^= k_0[3] ^ m[1];
    X[4] ^= k_0[4] ^ m[2];
    X[5] ^= k_0[5] ^ m[2];
    X[6] ^= k_0[6] ^ m[2];
    X[7] ^= k_0[7] ^ m[2];
    

#ifdef DEBUG
    printf("\n  masked RK:    %02X %02X %02X %02X   %02X %02X %02X %02X", k_0[7] ^ m[2], k_0[6] ^ m[2], k_0[5] ^ m[2], k_0[4] ^ m[2], k_0[3] ^ m[1], k_0[2] ^ m[2], k_0[1] ^ m[2], k_0[0] ^ m[2]);
    printf("\n  X:            %02X %02X %02X %02X   %02X %02X %02X %02X\n\n", X[7], X[6], X[5], X[4], X[3], X[2], X[1], X[0]);
#endif

    // S-Boxes            
    slayer(X, m[0], m[1], m[2]);

#ifdef DEBUG
    printf("\n\n After S-Box:  %02X %02X %02X %02X   %02X %02X %02X %02X\n", X[7], X[6], X[5], X[4], X[3], X[2], X[1], X[0]);
#endif

}