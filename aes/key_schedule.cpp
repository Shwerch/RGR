#include "constants.h"
#include <cstring>

namespace aes {
    void sub_word(uint8_t word[4]) {
        for (size_t i = 0; i < 4; ++i) {
            word[i] = S_BOX[word[i]];
        }
    }

    void rot_word(uint8_t word[4]) {
        uint8_t temp = word[0];
        word[0] = word[1];
        word[1] = word[2];
        word[2] = word[3];
        word[3] = temp;
    }

    void key_expansion(const uint8_t key[KEY_SIZE], uint8_t round_keys[][BLOCK_SIZE]) {
        uint8_t w[NB * (NR + 1)][4];

        for (size_t i = 0; i < NK; ++i) {
            w[i][0] = key[4 * i];
            w[i][1] = key[4 * i + 1];
            w[i][2] = key[4 * i + 2];
            w[i][3] = key[4 * i + 3];
        }

        for (size_t i = NK; i < NB * (NR + 1); ++i) {
            std::memcpy(w[i], w[i - 1], 4);

            if (i % NK == 0) {
                rot_word(w[i]);
                sub_word(w[i]);
                w[i][0] ^= R_CON[i / NK];
            } else if (i % NK == 4) {
                sub_word(w[i]);
            }

            w[i][0] ^= w[i - NK][0];
            w[i][1] ^= w[i - NK][1];
            w[i][2] ^= w[i - NK][2];
            w[i][3] ^= w[i - NK][3];
        }

        for (size_t round = 0; round <= NR; ++round) {
            for (size_t col = 0; col < NB; ++col) {
                const size_t word_idx = round * NB + col;
                round_keys[round][col * 4] = w[word_idx][0];
                round_keys[round][col * 4 + 1] = w[word_idx][1];
                round_keys[round][col * 4 + 2] = w[word_idx][2];
                round_keys[round][col * 4 + 3] = w[word_idx][3];
            }
        }
    }
}