#include "constants.h"
#include <cstring>

namespace aes {
    void sub_bytes(uint8_t state[BLOCK_SIZE]) {
        for (size_t i = 0; i < BLOCK_SIZE; ++i) {
            state[i] = S_BOX[state[i]];
        }
    }

    void shift_rows(uint8_t state[BLOCK_SIZE]) {
        uint8_t temp[BLOCK_SIZE];
        std::memcpy(temp, state, BLOCK_SIZE);

        state[1] = temp[5];
        state[5] = temp[9];
        state[9] = temp[13];
        state[13] = temp[1];

        state[2] = temp[10];
        state[6] = temp[14];
        state[10] = temp[2];
        state[14] = temp[6];

        state[3] = temp[15];
        state[7] = temp[3];
        state[11] = temp[7];
        state[15] = temp[11];
    }

    uint8_t gmul(uint8_t a, uint8_t b) {
        uint8_t result = 0;

        for (int i = 0; i < 8; ++i) {
            if (b & 1) {
                result ^= a;
            }

            bool hi_bit_set = a & 0x80;
            a <<= 1;

            if (hi_bit_set) {
                a ^= 0x1b;
            }

            b >>= 1;
        }

        return result;
    }

    void mix_columns(uint8_t state[BLOCK_SIZE]) {
        for (size_t i = 0; i < NB; ++i) {
            size_t col_start = i * 4;
            uint8_t s0 = state[col_start];
            uint8_t s1 = state[col_start + 1];
            uint8_t s2 = state[col_start + 2];
            uint8_t s3 = state[col_start + 3];

            state[col_start] = gmul(0x02, s0) ^ gmul(0x03, s1) ^ s2 ^ s3;
            state[col_start + 1] = s0 ^ gmul(0x02, s1) ^ gmul(0x03, s2) ^ s3;
            state[col_start + 2] = s0 ^ s1 ^ gmul(0x02, s2) ^ gmul(0x03, s3);
            state[col_start + 3] = gmul(0x03, s0) ^ s1 ^ s2 ^ gmul(0x02, s3);
        }
    }

    void add_round_key(uint8_t state[BLOCK_SIZE], const uint8_t round_key[BLOCK_SIZE]) {
        for (size_t i = 0; i < BLOCK_SIZE; ++i) {
            state[i] ^= round_key[i];
        }
    }

    void encrypt_block(uint8_t state[BLOCK_SIZE], uint8_t round_keys[][BLOCK_SIZE]) {
        add_round_key(state, round_keys[0]);

        for (size_t round = 1; round < NR; ++round) {
            sub_bytes(state);
            shift_rows(state);
            mix_columns(state);
            add_round_key(state, round_keys[round]);
        }

        sub_bytes(state);
        shift_rows(state);
        add_round_key(state, round_keys[NR]);
    }
}