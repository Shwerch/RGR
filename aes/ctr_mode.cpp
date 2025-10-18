#include "constants.h"
#include <cstring>
#include <vector>

namespace aes {
    extern void encrypt_block(uint8_t state[BLOCK_SIZE], uint8_t round_keys[][BLOCK_SIZE]);
    extern void key_expansion(const uint8_t key[KEY_SIZE], uint8_t round_keys[][BLOCK_SIZE]);

    void increment_counter(uint8_t counter[BLOCK_SIZE]) {
        for (int i = BLOCK_SIZE - 1; i >= 0; --i) {
            counter[i] = static_cast<uint8_t>(counter[i] + 1);
            if (counter[i] != 0) {
                break;
            }
        }
    }

    void iv_to_counter(const uint8_t iv[IV_SIZE], uint8_t counter[BLOCK_SIZE]) {
        std::memcpy(counter, iv, BLOCK_SIZE);
    }

    std::vector<uint8_t> ctr_process(const std::vector<uint8_t> &data, 
                                      const uint8_t key[KEY_SIZE], 
                                      const uint8_t iv[IV_SIZE]) {
        uint8_t round_keys[NR + 1][BLOCK_SIZE];
        key_expansion(key, round_keys);

        uint8_t counter[BLOCK_SIZE];
        iv_to_counter(iv, counter);

        std::vector<uint8_t> result;
        result.reserve(data.size());

        for (size_t offset = 0; offset < data.size(); offset += BLOCK_SIZE) {
            uint8_t encrypted_counter[BLOCK_SIZE];
            std::memcpy(encrypted_counter, counter, BLOCK_SIZE);
            encrypt_block(encrypted_counter, round_keys);

            size_t chunk_size = std::min(BLOCK_SIZE, data.size() - offset);
            for (size_t i = 0; i < chunk_size; ++i) {
                result.push_back(data[offset + i] ^ encrypted_counter[i]);
            }

            increment_counter(counter);
        }

        return result;
    }
}