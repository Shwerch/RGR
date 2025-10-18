#pragma once
#include <vector>
#include <cstdint>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

extern "C" {
EXPORT uint8_t* encrypt(const uint8_t* plaintext_ptr, size_t size, const uint8_t* key_ptr);
EXPORT uint8_t* decrypt(const uint8_t* ciphertext_ptr, size_t size, const uint8_t* key_ptr);
}
