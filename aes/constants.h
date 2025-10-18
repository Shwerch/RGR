#pragma once
#include <cstdint>

namespace aes {
    constexpr size_t KEY_SIZE = 32;
    constexpr size_t IV_SIZE = 32;
    constexpr size_t BLOCK_SIZE = 16;
    constexpr size_t NK = 8;
    constexpr size_t NR = 14;
    constexpr size_t NB = 4;

    extern const uint8_t R_CON[11];
    extern const uint8_t S_BOX[256];
}
