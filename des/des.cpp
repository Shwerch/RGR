#include "des.h"


#include <iostream>
#include <vector>
#include <cstdint>
#include <array>
#include <algorithm>

#include "../utils/pkcs7.h"

constexpr int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

constexpr int FP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

constexpr int E[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

constexpr int P[32] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

constexpr int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

constexpr int PC2[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 46, 54,
    29, 36, 43, 51, 25, 38,
    50, 47, 53, 49, 42, 30,
    24, 34, 32, 40, 44, 48
};

constexpr int SHIFTS[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

constexpr int S[8][4][16] = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

uint64_t permute(uint64_t input, const int* table, int n) {
    uint64_t output = 0;
    for (int i = 0; i < n; i++) {
        output <<= 1;
        output |= (input >> (64 - table[i])) & 1;
    }
    return output;
}

uint32_t permute32(uint32_t input, const int* table, int n) {
    uint32_t output = 0;
    for (int i = 0; i < n; i++) {
        output <<= 1;
        output |= (input >> (32 - table[i])) & 1;
    }
    return output;
}

uint64_t permute56(uint64_t input, const int* table, int n) {
    uint64_t output = 0;
    for (int i = 0; i < n; i++) {
        output <<= 1;
        output |= (input >> (64 - table[i])) & 1;
    }
    return output;
}

uint64_t permute48_from_56(uint64_t input, const int* table, int n) {
    uint64_t output = 0;
    for (int i = 0; i < n; i++) {
        output <<= 1;
        output |= (input >> (56 - table[i])) & 1;
    }
    return output;
}

uint64_t expand(uint32_t input) {
    uint64_t output = 0;
    for (int i : E) {
        output <<= 1;
        output |= (input >> (32 - i)) & 1;
    }
    return output;
}

uint32_t sbox_substitution(uint64_t input) {
    uint32_t output = 0;
    for (int i = 0; i < 8; i++) {
        int block = (input >> (42 - i * 6)) & 0x3F;
        int row = ((block & 0x20) >> 4) | (block & 0x01);
        int col = (block >> 1) & 0x0F;
        output <<= 4;
        output |= S[i][row][col];
    }
    return output;
}

uint32_t left_rotate28(uint32_t value, int shift) {
    return ((value << shift) | (value >> (28 - shift))) & 0x0FFFFFFF;
}

void generate_subkeys(uint64_t key, std::array<uint64_t, 16>& subkeys) {
    uint64_t permuted_key = permute56(key, PC1, 56);
    uint32_t c = (permuted_key >> 28) & 0x0FFFFFFF;
    uint32_t d = permuted_key & 0x0FFFFFFF;

    for (int i = 0; i < 16; i++) {
        c = left_rotate28(c, SHIFTS[i]);
        d = left_rotate28(d, SHIFTS[i]);
        uint64_t cd = (static_cast<uint64_t>(c) << 28) | d;
        subkeys[i] = permute48_from_56(cd, PC2, 48);
    }
}

uint32_t feistel(uint32_t r, uint64_t subkey) {
    uint64_t expanded = expand(r);
    uint64_t xored = expanded ^ subkey;
    uint32_t substituted = sbox_substitution(xored);
    return permute32(substituted, P, 32);
}

uint64_t des_block(uint64_t block, const std::array<uint64_t, 16>& subkeys, bool encrypt) {
    uint64_t permuted = permute(block, IP, 64);
    uint32_t l = (permuted >> 32) & 0xFFFFFFFF;
    uint32_t r = permuted & 0xFFFFFFFF;

    for (int i = 0; i < 16; i++) {
        int key_index = encrypt ? i : 15 - i;
        uint32_t temp = r;
        r = l ^ feistel(r, subkeys[key_index]);
        l = temp;
    }

    uint64_t combined = (static_cast<uint64_t>(r) << 32) | l;
    return permute(combined, FP, 64);
}

std::vector<uint8_t> des_process(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key, bool encrypt) {
    if (key.size() != 8) {
        std::cerr << "Key must be 8 bytes" << std::endl;
        return {};
    }

    uint64_t key64 = 0;
    for (int i = 0; i < 8; i++) {
        key64 = (key64 << 8) | key[i];
    }

    std::array<uint64_t, 16> subkeys;
    generate_subkeys(key64, subkeys);

    std::vector<uint8_t> result;
    result.reserve(data.size());

    for (size_t i = 0; i < data.size(); i += 8) {
        uint64_t block = 0;
        for (size_t j = 0; j < 8 && i + j < data.size(); j++) {
            block = (block << 8) | data[i + j];
        }

        uint64_t processed = des_block(block, subkeys, encrypt);

        for (int j = 7; j >= 0; j--) {
            result.push_back((processed >> (j * 8)) & 0xFF);
        }
    }

    return result;
}

uint8_t* vector_to_raw(const std::vector<uint8_t>& vec) {
    auto* raw_data = new uint8_t[vec.size()];
    std::copy_n(vec.data(), vec.size(), raw_data);
    return raw_data;
}

extern "C" {
    uint8_t* encrypt(const uint8_t* plaintext_ptr, const size_t size, const uint8_t* key_ptr, size_t* out_size) {
        try {
            std::vector<uint8_t> plaintext = {plaintext_ptr, plaintext_ptr + size};
            std::vector<uint8_t> key = {key_ptr, key_ptr + 8};

            std::vector<uint8_t> padded = pkcs7_pad(plaintext, 8);
            std::vector<uint8_t> encrypted = des_process(padded, key, true);

            *out_size = encrypted.size();
            return vector_to_raw(encrypted);
        } catch (const std::exception& e) {
            std::cerr << "Encryption error: " << e.what() << std::endl;
            *out_size = 0;
            return nullptr;
        }
    }

    uint8_t* decrypt(const uint8_t* ciphertext_ptr, const size_t size, const uint8_t* key_ptr, size_t* out_size) {
        try {
            std::vector<uint8_t> ciphertext = {ciphertext_ptr, ciphertext_ptr + size};
            std::vector<uint8_t> key = {key_ptr, key_ptr + 8};

            if (ciphertext.size() % 8 != 0) {
                std::cerr << "Ciphertext size must be multiple of 8" << std::endl;
                *out_size = 0;
                return nullptr;
            }

            std::vector<uint8_t> decrypted = des_process(ciphertext, key, false);
            std::vector<uint8_t> unpadded = pkcs7_unpad(decrypted, 8);

            *out_size = unpadded.size();
            return vector_to_raw(unpadded);
        } catch (const std::exception& e) {
            std::cerr << "Decryption error: " << e.what() << std::endl;
            *out_size = 0;
            return nullptr;
        }
    }
}