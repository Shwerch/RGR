#pragma once
#include <cstdint>

namespace des {
    constexpr size_t KEY_SIZE = 8;
    constexpr size_t IV_SIZE = 8;
    constexpr size_t BLOCK_SIZE = 8;

    extern const size_t IP[64];
    extern const size_t IP_INV[64];
    extern const size_t E[48];
    extern const size_t P[32];
    extern const uint8_t S_BOXES[8][4][16];
    extern const size_t PC1[56];
    extern const size_t PC2[48];
    extern const size_t SHIFTS[16];
}