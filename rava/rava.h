#pragma once
#include <vector>
#include <cstdint>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

extern "C" {
    EXPORT std::vector<uint8_t> encrypt(const std::vector<uint8_t> &plaintext,
                                        const std::vector<uint8_t> &key);
    EXPORT std::vector<uint8_t> decrypt(const std::vector<uint8_t> &ciphertext,
                                        const std::vector<uint8_t> &key);
}