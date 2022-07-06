#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define DEBUG

// declare Plaintext, Secret Key and ciphertext for PIPO-64/128
const uint64_t plaintext = 0x098552F61E270026;
const uint64_t k1 = 0x6DC416DD779428D2;
const uint64_t k0 = 0x7E1D20AD2E152297;
const uint64_t ciphertext = 0x6B6B2981AD5D0327;

void rlayer (uint8_t array[]) {
    array[1] = ((array[1] << 7)) | ((array[1] >> 1));
    array[2] = ((array[2] << 4)) | ((array[2] >> 4));
    array[3] = ((array[3] << 3)) | ((array[3] >> 5));
    array[4] = ((array[4] << 6)) | ((array[4] >> 2));
    array[5] = ((array[5] << 5)) | ((array[5] >> 3));
    array[6] = ((array[6] << 1)) | ((array[6] >> 7));
    array[7] = ((array[7] << 2)) | ((array[7] >> 6));
}

void sbox_8 (uint8_t *x) {
    ///////////
    // S-Box //
    ///////////

    // declare helping variables
    uint8_t t[3] = {};

    // S5_1
    x[5] ^= (x[7] & x[6]);
    x[4] ^= (x[3] & x[5]);
    x[7] ^= x[4];
    x[6] ^= x[3];
    x[3] ^= (x[4] | x[5]);
    x[5] ^= x[7];
    x[4] ^= (x[5] & x[6]);

    // S3
    x[2] ^= (x[1] & x[0]);
    x[0] ^= (x[2] | x[1]);
    x[1] ^= (x[2] | x[0]);
    x[2] = ~x[2];

    // Extend XOR
    x[7] ^= x[1];
    x[3] ^= x[2];
    x[4] ^= x[0];

    // S5_2
    t[0] = x[7];
    t[1] = x[3];
    t[2] = x[4];

    x[6] ^= (t[0] & x[5]);
    t[0] ^= x[6];
    x[6] ^= (t[2] | t[1]);
    t[1] ^= x[5];
    x[5] ^= (x[6] | t[2]);
    t[2] ^= (t[1] & t[0]);

    // truncate XOR and swap
    x[2] ^= t[0];
    t[0] = x[1] ^ t[2];
    x[1] = x[0] ^ t[1];

    x[0] = x[7];
    x[7] = t[0];

    t[1] = x[3];
    x[3] = x[6];
    x[6] = t[1];

    t[2] = x[4];
    x[4] = x[5];
    x[5] = t[2];

}

void pipo128 (uint64_t plain) {
    // split 64-bit Input into 8 bytes --> MSB X[7] ... X[0] LSB
    uint8_t X[8];
    X[0] = plain >> 0;
    X[1] = plain >> 8;
    X[2] = plain >> 16;
    X[3] = plain >> 24;
    X[4] = plain >> 32;
    X[5] = plain >> 40;
    X[6] = plain >> 48;
    X[7] = plain >> 56;

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
    printf("Round 0:");
    printf("\n  Plaintext:    %02X %02X %02X %02X   %02X %02X %02X %02X", X[7], X[6], X[5], X[4], X[3], X[2], X[1], X[0]);
#endif

    // XOR Whitening Key
    for (int i = 0; i < 8; i++) {
        X[i] ^= k_0[i];
    }

#ifdef DEBUG
    printf("\n  RK:           %02X %02X %02X %02X   %02X %02X %02X %02X", k_0[7], k_0[6], k_0[5], k_0[4], k_0[3], k_0[2], k_0[1], k_0[0]);
    printf("\n  X:            %02X %02X %02X %02X   %02X %02X %02X %02X\n", X[7], X[6], X[5], X[4], X[3], X[2], X[1], X[0]);
#endif

    // 13 Rounds of S-Layer --> R-layer --> XOR Round Key
    for (int i = 1; i <= 13; i++) {

#ifdef DEBUG
            printf("\nRound %d:", i);        
            printf("\n  X:            %02X %02X %02X %02X   %02X %02X %02X %02X\n", X[7], X[6], X[5], X[4], X[3], X[2], X[1], X[0]);
#endif

        // S-Boxes            
        sbox_8(X);

#ifdef DEBUG
            printf("  After S-Box:  %02X %02X %02X %02X   %02X %02X %02X %02X\n", X[7], X[6], X[5], X[4], X[3], X[2], X[1], X[0]);
#endif

        // R-Layer
        rlayer(X);

#ifdef DEBUG        
            printf("  After R-Layer:%02X %02X %02X %02X   %02X %02X %02X %02X\n", X[7], X[6], X[5], X[4], X[3], X[2], X[1], X[0]);        
#endif

        // Add Round Key XOR round constant
        if (i % 2 == 0) {
            // round key 
            uint8_t rk[8];
            rk[0] = (k_0[0] ^ i);
            rk[1] = k_0[1];
            rk[2] = k_0[2];
            rk[3] = k_0[3];
            rk[4] = k_0[4];
            rk[5] = k_0[5];
            rk[6] = k_0[6];
            rk[7] = k_0[7]; 

#ifdef DEBUG        
            printf("  RK:           %02X %02X %02X %02X   %02X %02X %02X %02X\n", rk[7], rk[6], rk[5], rk[4], rk[3], rk[2], rk[1], rk[0]);
#endif

            // Use key k0
            X[0] ^= rk[0];
            X[1] ^= rk[1];
            X[2] ^= rk[2];
            X[3] ^= rk[3];
            X[4] ^= rk[4];
            X[5] ^= rk[5];
            X[6] ^= rk[6];
            X[7] ^= rk[7];

#ifdef DEBUG        
            printf("  X:            %02X %02X %02X %02X   %02X %02X %02X %02X\n", X[7], X[6], X[5], X[4], X[3], X[2], X[1], X[0]);
#endif

        }
        else {
            // round key 
            uint8_t rk[8];
            rk[0] = (k_1[0] ^ i);
            rk[1] = k_1[1];
            rk[2] = k_1[2];
            rk[3] = k_1[3];
            rk[4] = k_1[4];
            rk[5] = k_1[5];
            rk[6] = k_1[6];
            rk[7] = k_1[7]; 

#ifdef DEBUG      
            printf("  RK:           %02X %02X %02X %02X   %02X %02X %02X %02X\n", rk[7], rk[6], rk[5], rk[4], rk[3], rk[2], rk[1], rk[0]);
#endif
            // use key k1
            X[0] ^= (k_1[0] ^ i);
            X[1] ^= k_1[1];
            X[2] ^= k_1[2];
            X[3] ^= k_1[3];
            X[4] ^= k_1[4];
            X[5] ^= k_1[5];
            X[6] ^= k_1[6];
            X[7] ^= k_1[7];

#ifdef DEBUG        
            printf("  X:            %02X %02X %02X %02X   %02X %02X %02X %02X\n", X[7], X[6], X[5], X[4], X[3], X[2], X[1], X[0]);
#endif

        }
    }

    // Print Output after Round 13
    uint64_t result = (uint64_t)(X[0]) | (uint64_t)(X[1]) << 8 | (uint64_t)(X[2]) << 16 | (uint64_t)(X[3]) << 24 | (uint64_t)(X[4]) << 32 | (uint64_t)(X[5]) << 40 | (uint64_t)(X[6]) << 48 | (uint64_t)(X[7]) << 56;
    printf("\nResult PIPO-64/128 = 0x%llX\n", result);
}

int main () {
    
    pipo128(plaintext);

}