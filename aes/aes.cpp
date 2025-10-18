#include "aes.h"

#include <iostream>
#include <ostream>
#include <vector>

#include "../utils/exception.h"

std::vector<uint8_t> raw_to_vector(const uint8_t* ptr, const size_t size) {
    if (ptr == nullptr || size == 0) {
        throw EXCEPTION("Empty plaintext");
    }
    return {ptr, ptr + size};
}
uint8_t* vector_to_dynamic_raw(const std::vector<uint8_t>& vec) {
    if (vec.empty()) {
        throw EXCEPTION("Empty ciphertext");
    }
    auto* raw_data = new uint8_t[vec.size()];
    std::copy_n(vec.data(), vec.size(), raw_data);
    return raw_data;
}

extern "C" {
    uint8_t* encrypt(const uint8_t* plaintext_ptr, const size_t size, const uint8_t* key_ptr) {
        const auto plaintext = raw_to_vector(plaintext_ptr, size);
        const auto key = raw_to_vector(key_ptr, 32);
        const auto& ciphertext(plaintext);
        return vector_to_dynamic_raw(ciphertext);
    }

    uint8_t* decrypt(const uint8_t* ciphertext_ptr, const size_t size, const uint8_t* key_ptr) {
        return nullptr;
    }
}
