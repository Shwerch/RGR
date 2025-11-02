#include "rava.h"
#include "../utils/pkcs7.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

constexpr size_t BLOCK_SIZE = 8;
constexpr size_t WORD_SIZE = 32;
constexpr size_t KEY_SIZE = 32;
constexpr size_t ROUNDS = 32;
constexpr uint8_t ROTATION_A = 7;
constexpr uint8_t ROTATION_B = 2;
constexpr uint8_t ROTATION_KEY = 3;

uint32_t rotate_left(uint32_t value, uint8_t shift) {
    shift &= 31;
    return (value << shift) | (value >> (32 - shift));
}

uint32_t rotate_right(uint32_t value, uint8_t shift) {
    shift &= 31;
    return (value >> shift) | (value << (32 - shift));
}

void bytes_to_words(const uint8_t* bytes, uint32_t& left, uint32_t& right) {
    left = static_cast<uint32_t>(bytes[0]) |
           (static_cast<uint32_t>(bytes[1]) << 8) |
           (static_cast<uint32_t>(bytes[2]) << 16) |
           (static_cast<uint32_t>(bytes[3]) << 24);

    right = static_cast<uint32_t>(bytes[4]) |
            (static_cast<uint32_t>(bytes[5]) << 8) |
            (static_cast<uint32_t>(bytes[6]) << 16) |
            (static_cast<uint32_t>(bytes[7]) << 24);
}

void words_to_bytes(uint32_t left, uint32_t right, uint8_t* bytes) {
    bytes[0] = static_cast<uint8_t>(left & 0xFF);
    bytes[1] = static_cast<uint8_t>((left >> 8) & 0xFF);
    bytes[2] = static_cast<uint8_t>((left >> 16) & 0xFF);
    bytes[3] = static_cast<uint8_t>((left >> 24) & 0xFF);

    bytes[4] = static_cast<uint8_t>(right & 0xFF);
    bytes[5] = static_cast<uint8_t>((right >> 8) & 0xFF);
    bytes[6] = static_cast<uint8_t>((right >> 16) & 0xFF);
    bytes[7] = static_cast<uint8_t>((right >> 24) & 0xFF);
}

void generate_round_keys(const std::vector<uint8_t>& key, std::vector<uint32_t>& round_keys) {
    std::vector<uint32_t> key_words(8);

    for (size_t i = 0; i < 8; ++i) {
        key_words[i] = static_cast<uint32_t>(key[i * 4]) |
                       (static_cast<uint32_t>(key[i * 4 + 1]) << 8) |
                       (static_cast<uint32_t>(key[i * 4 + 2]) << 16) |
                       (static_cast<uint32_t>(key[i * 4 + 3]) << 24);
    }

    round_keys.resize(ROUNDS);
    round_keys[0] = key_words[0];

    for (size_t i = 1; i < ROUNDS; ++i) {
        size_t key_idx = i % 8;
        key_words[key_idx] = rotate_left(key_words[key_idx], ROTATION_KEY) + round_keys[i - 1];
        round_keys[i] = key_words[key_idx] ^ static_cast<uint32_t>(i);
    }
}

void encrypt_block(uint32_t& left, uint32_t& right, const std::vector<uint32_t>& round_keys) {
    for (size_t i = 0; i < ROUNDS; ++i) {
        left = rotate_right(left, ROTATION_B);
        left = left + right;
        left = left ^ round_keys[i];
        right = rotate_left(right, ROTATION_A);
        right = right ^ left;
    }
}

void decrypt_block(uint32_t& left, uint32_t& right, const std::vector<uint32_t>& round_keys) {
    for (int i = ROUNDS - 1; i >= 0; --i) {
        right = right ^ left;
        right = rotate_right(right, ROTATION_A);
        left = left ^ round_keys[i];
        left = left - right;
        left = rotate_left(left, ROTATION_B);
    }
}

uint8_t* vector_to_raw(const std::vector<uint8_t>& vec) {
    uint8_t* raw_data = new uint8_t[vec.size()];
    std::copy_n(vec.data(), vec.size(), raw_data);
    return raw_data;
}

extern "C" {
    EXPORT uint8_t* encrypt(const uint8_t* plaintext_ptr, const size_t size, const uint8_t* key_ptr, size_t* out_size) {
        try {
            if (!plaintext_ptr || !key_ptr || !out_size) {
                std::cerr << "null pointer provided to encrypt function" << std::endl;
                return nullptr;
            }

            std::vector<uint8_t> plaintext(plaintext_ptr, plaintext_ptr + size);
            std::vector<uint8_t> key(key_ptr, key_ptr + KEY_SIZE);

            std::vector<uint8_t> padded = pkcs7_pad(plaintext, BLOCK_SIZE);
            std::vector<uint32_t> round_keys;
            generate_round_keys(key, round_keys);

            std::vector<uint8_t> ciphertext(padded.size());

            for (size_t i = 0; i < padded.size(); i += BLOCK_SIZE) {
                uint32_t left, right;
                bytes_to_words(&padded[i], left, right);
                encrypt_block(left, right, round_keys);
                words_to_bytes(left, right, &ciphertext[i]);
            }

            *out_size = ciphertext.size();
            return vector_to_raw(ciphertext);

        } catch (const std::exception& e) {
            std::cerr << "Error during encryption: " << e.what() << std::endl;
            return nullptr;
        }
    }

    EXPORT uint8_t* decrypt(const uint8_t* ciphertext_ptr, const size_t size, const uint8_t* key_ptr, size_t* out_size) {
        try {
            if (!ciphertext_ptr || !key_ptr || !out_size) {
                std::cerr << "null pointer provided to decrypt function" << std::endl;
                return nullptr;
            }

            if (size % BLOCK_SIZE != 0) {
                std::cerr << "ciphertext size must be multiple of block size" << std::endl;
                return nullptr;
            }

            std::vector<uint8_t> ciphertext(ciphertext_ptr, ciphertext_ptr + size);
            std::vector<uint8_t> key(key_ptr, key_ptr + KEY_SIZE);

            std::vector<uint32_t> round_keys;
            generate_round_keys(key, round_keys);

            std::vector<uint8_t> decrypted(ciphertext.size());

            for (size_t i = 0; i < ciphertext.size(); i += BLOCK_SIZE) {
                uint32_t left, right;
                bytes_to_words(&ciphertext[i], left, right);
                decrypt_block(left, right, round_keys);
                words_to_bytes(left, right, &decrypted[i]);
            }

            std::vector<uint8_t> plaintext = pkcs7_unpad(decrypted, BLOCK_SIZE);
            *out_size = plaintext.size();
            return vector_to_raw(plaintext);

        } catch (const std::exception& e) {
            std::cerr << "Error during decryption: " << e.what() << std::endl;
            return nullptr;
        }
    }
}