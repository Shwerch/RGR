#pragma once

#include <cstdint>
#include <cstddef>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

extern "C" {
    typedef void (*Deleter)(uint8_t*);
    EXPORT bool encrypt(
        const uint8_t* plaintext_ptr,
        const size_t size,
        const uint8_t* key_ptr,
        uint8_t** ciphertext_ptr,
        size_t* ciphertext_size,
        Deleter* deleter_ptr
    );
    EXPORT bool decrypt(
        const uint8_t* ciphertext_ptr,
        const size_t size,
        const uint8_t* key_ptr,
        uint8_t** plaintext_ptr,
        size_t* plaintext_size,
        Deleter* deleter_ptr
    );
}
