#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// declare Secret Key, plaintext and ciphertext
const uint64_t k1 = 0x6DC416DD779428D2;
const uint64_t k0 = 0x7E1D20AD2E152297;
const uint64_t plaintext = 0x098552F61E270026;
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

uint8_t sbox_8 (uint8_t var) {
    // split 8-bit Input into 8 bits --> MSB x[7] ... x[0] LSB
    uint8_t x[8];
    x[0] = (var >> 0) & 0x01;
    x[1] = (var >> 1) & 0x01;
    x[2] = (var >> 2) & 0x01;
    x[3] = (var >> 3) & 0x01;
    x[4] = (var >> 4) & 0x01;
    x[5] = (var >> 5) & 0x01;
    x[6] = (var >> 6) & 0x01;
    x[7] = (var >> 7) & 0x01;

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
    x[2] = x[2] ^ 1;

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

    uint8_t output = (x[7] << 7) ^ (x[6] << 6) ^ (x[5] << 5) ^ (x[4] << 4) ^ (x[3] << 3) ^ (x[2] << 2) ^ (x[1] << 1) ^ x[0]; 
    return output;
}

int main () {
    
    // split 64-bit Input into 8 bytes --> MSB X[7] ... X[0] LSB
    uint8_t X[8];
    X[0] = (plaintext >> 0) & 0xFF;
    X[1] = (plaintext >> 8) & 0xFF;
    X[2] = (plaintext >> 16) & 0xFF;
    X[3] = (plaintext >> 24) & 0xFF;
    X[4] = (plaintext >> 32) & 0xFF;
    X[5] = (plaintext >> 40) & 0xFF;
    X[6] = (plaintext >> 48) & 0xFF;
    X[7] = (plaintext >> 56) & 0xFF;

    // split key k0 and k1 into 8 bytes
    uint8_t k_0[8];
    k_0[0] = (k0 >> 0) & 0xFF;
    k_0[1] = (k0 >> 8) & 0xFF;
    k_0[2] = (k0 >> 16) & 0xFF;
    k_0[3] = (k0 >> 24) & 0xFF;
    k_0[4] = (k0 >> 32) & 0xFF;
    k_0[5] = (k0 >> 40) & 0xFF;
    k_0[6] = (k0 >> 48) & 0xFF;
    k_0[7] = (k0 >> 56) & 0xFF;

    uint8_t k_1[8];
    k_1[0] = (k1 >> 0) & 0xFF;
    k_1[1] = (k1 >> 8) & 0xFF;
    k_1[2] = (k1 >> 16) & 0xFF;
    k_1[3] = (k1 >> 24) & 0xFF;
    k_1[4] = (k1 >> 32) & 0xFF;
    k_1[5] = (k1 >> 40) & 0xFF;
    k_1[6] = (k1 >> 48) & 0xFF;
    k_1[7] = (k1 >> 56) & 0xFF;

    // XOR Whitening Key
    for (int i = 0; i < 8; i++) {
        X[i] ^= k_0[i];
    }

    // 13 Rounds of S-Layer --> R-layer --> XOR Round Key
    for (int i = 1; i <= 13; i++) {
     
        // 8 S-Boxes            
        X[0] = sbox_8(X[0]);
        X[1] = sbox_8(X[1]);
        X[2] = sbox_8(X[2]);
        X[3] = sbox_8(X[3]);
        X[4] = sbox_8(X[4]);
        X[5] = sbox_8(X[5]);
        X[6] = sbox_8(X[6]);
        X[7] = sbox_8(X[7]);


        // R-Layer
        rlayer(X);


        // Add Round Key XOR round constant
        if (i % 2 == 0) {
            // Use key k0
            X[0] ^= (k_0[0] ^ i);
            X[1] ^= k_0[1];
            X[2] ^= k_0[2];
            X[3] ^= k_0[3];
            X[4] ^= k_0[4];
            X[5] ^= k_0[5];
            X[6] ^= k_0[6];
            X[7] ^= k_0[7];
        }
        else {
            // use key k1
            X[0] ^= (k_1[0] ^ i);
            X[1] ^= k_1[1];
            X[2] ^= k_1[2];
            X[3] ^= k_1[3];
            X[4] ^= k_1[4];
            X[5] ^= k_1[5];
            X[6] ^= k_1[6];
            X[7] ^= k_1[7];
        }

    }

    // Print Output after Round 13
    uint64_t result = (uint64_t)(X[0]) | (uint64_t)(X[1]) << 8 | (uint64_t)(X[2]) << 16 | (uint64_t)(X[3]) << 24 | (uint64_t)(X[4]) << 32 | (uint64_t)(X[5]) << 40 | (uint64_t)(X[6]) << 48 | (uint64_t)(X[7]) << 56;
    printf("Result = 0x%llX\n", result);

}