#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

// declare Plaintext, Secret Key and ciphertext for PIPO-64/128
const uint64_t plaintext = 0x098552F61E270026;
const uint64_t k1 = 0x6DC416DD779428D2;
const uint64_t k0 = 0x7E1D20AD2E152297;
const uint64_t ciphertext = 0x6B6B2981AD5D0327;


extern void rlayer (uint8_t *X);
extern void slayer (uint8_t *X, uint8_t *m, uint8_t *T, uint8_t c);

void add_mask (uint8_t *x, const uint8_t *m) {
    x[0] ^= m[1];
    x[1] ^= m[0];
    x[2] ^= m[0];
    x[3] ^= m[0];
    x[4] ^= m[1];
    x[5] ^= m[0];
    x[6] ^= m[0];
    x[7] ^= m[1];
}

void remove_mask (uint8_t *x, uint8_t *m) {
    x[0] ^= m[0];
    x[1] ^= m[1];
    x[2] ^= m[1];
    x[3] ^= m[2];
    x[4] ^= m[0];
    x[5] ^= m[1];
    x[6] ^= m[1];
    x[7] ^= m[0];
}


int main () {

    //srand(time(NULL));

    // mask initialization
    uint8_t m[10];
    m[0] = rand() % 256;
    m[1] = rand() % 256;
    m[2] = m[0] ^ m[1];
    m[3] = ((m[1] >> 1) | (m[1] << 7)) ^ m[1];
    m[4] = ((m[2] >> 4) | (m[2] << 4)) ^ m[1];
    m[5] = ((m[0] >> 5) | (m[0] << 3)) ^ m[2];
    m[6] = ((m[1] >> 2) | (m[1] << 6)) ^ m[0];
    m[7] = ((m[1] >> 3) | (m[1] << 5)) ^ m[1];
    m[8] = ((m[2] >> 7) | (m[2] << 1)) ^ m[1];
    m[9] = ((m[1] >> 6) | (m[1] << 2)) ^ m[0];

    // Preparation of Slayer
    uint8_t T[3] = {0};
    uint8_t c = (m[0] | m[1]);

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


    // Add Mask
    add_mask(X, m);

    uint8_t *round_key = (uint8_t *)&k0;
    // XOR masked Whitening Key
    X[0] ^= round_key[0] ^ m[2];
    X[1] ^= round_key[1] ^ m[2];
    X[2] ^= round_key[2] ^ m[2];
    X[3] ^= round_key[3] ^ m[1];
    X[4] ^= round_key[4] ^ m[2];
    X[5] ^= round_key[5] ^ m[2];
    X[6] ^= round_key[6] ^ m[2];
    X[7] ^= round_key[7] ^ m[2];

    // 12 Rounds of S-Layer --> R-layer --> XOR Round Key
    for (int i = 1; i <= 13; i++) {

        // S-Boxes
    	slayer(X, m, T, c);

        // R-Layer
        rlayer(X);

        // Add Round Key XOR round constant
        round_key = ((i % 2) == 0) ? (uint8_t *)&k0 : (uint8_t *)&k1;
        X[0] ^= round_key[0] ^ m[1] ^ i;
		for (int j = 1; j < 8; j++) {
			X[j] ^= round_key[j] ^ m[j + 2];
		}
    }

    // remove mask
    remove_mask(X, m);

    // Test variable to check the output after Round 13
    uint64_t result = (uint64_t)(X[0]) | (uint64_t)(X[1]) << 8 | (uint64_t)(X[2]) << 16 | (uint64_t)(X[3]) << 24 | (uint64_t)(X[4]) << 32 | (uint64_t)(X[5]) << 40 | (uint64_t)(X[6]) << 48 | (uint64_t)(X[7]) << 56;

}
