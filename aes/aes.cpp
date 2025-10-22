#include "aes.h"

#include <iostream>
#include <ostream>
#include <vector>

uint8_t* vector_to_raw(const std::vector<uint8_t>& vec) {
    auto* raw_data = new uint8_t[vec.size()];
    std::copy_n(vec.data(), vec.size(), raw_data);
    return raw_data;
}

extern "C" {
    uint8_t* encrypt(const uint8_t* plaintext_ptr, const size_t size, const uint8_t* key_ptr) {
        const std::vector<uint8_t> plaintext = {plaintext_ptr, plaintext_ptr + size};
        const std::vector<uint8_t> key = {key_ptr, key_ptr + 32};
        const auto& ciphertext(plaintext);
        return vector_to_raw(ciphertext);
    }

    uint8_t* decrypt(const uint8_t* ciphertext_ptr, const size_t size, const uint8_t* key_ptr) {
        return nullptr;
    }
}
