#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// declare Plaintext
const uint64_t plaintext = 0x098552F61E270026;

// declare Secret Key and ciphertext for PIPO-64/128
const uint64_t k1_128 = 0x6DC416DD779428D2;
const uint64_t k0_128 = 0x7E1D20AD2E152297;
const uint64_t plaintext_128 = 0x098552F61E270026;
const uint64_t ciphertext_128 = 0x6B6B2981AD5D0327;

// declare Secret Key and ciphertext for PIPO-64/128
const uint64_t k3_256 = 0x009A3AA476A96DB5;
const uint64_t k2_256 = 0x54A7120626D15633;
const uint64_t k1_256 = 0x6DC416DD779428D2;
const uint64_t k0_256 = 0x7E1D20AD2E152297;
const uint64_t ciphertext_256 = 0x816DAE6FB6523889;

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
    uint8_t k_0_128[8];
    k_0_128[0] = k0_128 >> 0;
    k_0_128[1] = k0_128 >> 8;
    k_0_128[2] = k0_128 >> 16;
    k_0_128[3] = k0_128 >> 24;
    k_0_128[4] = k0_128 >> 32;
    k_0_128[5] = k0_128 >> 40;
    k_0_128[6] = k0_128 >> 48;
    k_0_128[7] = k0_128 >> 56;

    uint8_t k_1_128[8];
    k_1_128[0] = k1_128 >> 0;
    k_1_128[1] = k1_128 >> 8;
    k_1_128[2] = k1_128 >> 16;
    k_1_128[3] = k1_128 >> 24;
    k_1_128[4] = k1_128 >> 32;
    k_1_128[5] = k1_128 >> 40;
    k_1_128[6] = k1_128 >> 48;
    k_1_128[7] = k1_128 >> 56;

    // XOR Whitening Key
    for (int i = 0; i < 8; i++) {
        X[i] ^= k_0_128[i];
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
            X[0] ^= (k_0_128[0] ^ i);
            X[1] ^= k_0_128[1];
            X[2] ^= k_0_128[2];
            X[3] ^= k_0_128[3];
            X[4] ^= k_0_128[4];
            X[5] ^= k_0_128[5];
            X[6] ^= k_0_128[6];
            X[7] ^= k_0_128[7];
        }
        else {
            // use key k1
            X[0] ^= (k_1_128[0] ^ i);
            X[1] ^= k_1_128[1];
            X[2] ^= k_1_128[2];
            X[3] ^= k_1_128[3];
            X[4] ^= k_1_128[4];
            X[5] ^= k_1_128[5];
            X[6] ^= k_1_128[6];
            X[7] ^= k_1_128[7];
        }
    }

    // Print Output after Round 13
    uint64_t result = (uint64_t)(X[0]) | (uint64_t)(X[1]) << 8 | (uint64_t)(X[2]) << 16 | (uint64_t)(X[3]) << 24 | (uint64_t)(X[4]) << 32 | (uint64_t)(X[5]) << 40 | (uint64_t)(X[6]) << 48 | (uint64_t)(X[7]) << 56;
    printf("Result PIPO-64/128 = 0x%llX\n", result);
}

void pipo256 (uint64_t plain) {
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

    // split keys into 8 bytes
    uint8_t k_0_256[8];
    k_0_256[0] = k0_256 >> 0;
    k_0_256[1] = k0_256 >> 8;
    k_0_256[2] = k0_256 >> 16;
    k_0_256[3] = k0_256 >> 24;
    k_0_256[4] = k0_256 >> 32;
    k_0_256[5] = k0_256 >> 40;
    k_0_256[6] = k0_256 >> 48;
    k_0_256[7] = k0_256 >> 56;

    uint8_t k_1_256[8];
    k_1_256[0] = k1_256 >> 0;
    k_1_256[1] = k1_256 >> 8;
    k_1_256[2] = k1_256 >> 16;
    k_1_256[3] = k1_256 >> 24;
    k_1_256[4] = k1_256 >> 32;
    k_1_256[5] = k1_256 >> 40;
    k_1_256[6] = k1_256 >> 48;
    k_1_256[7] = k1_256 >> 56;

    uint8_t k_2_256[8];
    k_2_256[0] = k2_256 >> 0;
    k_2_256[1] = k2_256 >> 8;
    k_2_256[2] = k2_256 >> 16;
    k_2_256[3] = k2_256 >> 24;
    k_2_256[4] = k2_256 >> 32;
    k_2_256[5] = k2_256 >> 40;
    k_2_256[6] = k2_256 >> 48;
    k_2_256[7] = k2_256 >> 56;

    uint8_t k_3_256[8];
    k_3_256[0] = k3_256 >> 0;
    k_3_256[1] = k3_256 >> 8;
    k_3_256[2] = k3_256 >> 16;
    k_3_256[3] = k3_256 >> 24;
    k_3_256[4] = k3_256 >> 32;
    k_3_256[5] = k3_256 >> 40;
    k_3_256[6] = k3_256 >> 48;
    k_3_256[7] = k3_256 >> 56;

    // XOR Whitening Key
    for (int i = 0; i < 8; i++) {
        X[i] ^= k_0_256[i];
    }

    // 13 Rounds of S-Layer --> R-layer --> XOR Round Key
    for (int i = 1; i <= 17; i++) {
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
        if (i % 4 == 0) {
            // Use key k0
            X[0] ^= (k_0_256[0] ^ i);
            X[1] ^= k_0_256[1];
            X[2] ^= k_0_256[2];
            X[3] ^= k_0_256[3];
            X[4] ^= k_0_256[4];
            X[5] ^= k_0_256[5];
            X[6] ^= k_0_256[6];
            X[7] ^= k_0_256[7];
        }
        else if (i % 4 == 1) {
            // use key k1
            X[0] ^= (k_1_256[0] ^ i);
            X[1] ^= k_1_256[1];
            X[2] ^= k_1_256[2];
            X[3] ^= k_1_256[3];
            X[4] ^= k_1_256[4];
            X[5] ^= k_1_256[5];
            X[6] ^= k_1_256[6];
            X[7] ^= k_1_256[7];
        }
        else if (i % 4 == 2) {
            // use key k2
            X[0] ^= (k_2_256[0] ^ i);
            X[1] ^= k_2_256[1];
            X[2] ^= k_2_256[2];
            X[3] ^= k_2_256[3];
            X[4] ^= k_2_256[4];
            X[5] ^= k_2_256[5];
            X[6] ^= k_2_256[6];
            X[7] ^= k_2_256[7];
        }
        else  {
            // use key k3
            X[0] ^= (k_3_256[0] ^ i);
            X[1] ^= k_3_256[1];
            X[2] ^= k_3_256[2];
            X[3] ^= k_3_256[3];
            X[4] ^= k_3_256[4];
            X[5] ^= k_3_256[5];
            X[6] ^= k_3_256[6];
            X[7] ^= k_3_256[7];
        }
    }

    // Print Output after Round 13
    uint64_t result = (uint64_t)(X[0]) | (uint64_t)(X[1]) << 8 | (uint64_t)(X[2]) << 16 | (uint64_t)(X[3]) << 24 | (uint64_t)(X[4]) << 32 | (uint64_t)(X[5]) << 40 | (uint64_t)(X[6]) << 48 | (uint64_t)(X[7]) << 56;
    printf("Result PIPO-64/256 = 0x%llX\n", result);
}

int main () {
    
    pipo128(plaintext);
    pipo256(plaintext);

}