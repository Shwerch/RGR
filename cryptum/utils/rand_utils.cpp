#include "rand_utils.h"
#include <random>

std::vector<uint8_t> random_vector(size_t num_bytes) {
    std::vector<uint8_t> random_bytes(num_bytes);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (size_t i = 0; i < num_bytes; ++i) {
        random_bytes[i] = static_cast<uint8_t>(dis(gen));
    }
    
    return random_bytes;
}

template<size_t N>
void random_array(uint8_t (&arr)[N]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (size_t i = 0; i < N; ++i) {
        arr[i] = static_cast<uint8_t>(dis(gen));
    }
}

template void random_array<8>(uint8_t (&)[8]);
template void random_array<16>(uint8_t (&)[16]);
template void random_array<32>(uint8_t (&)[32]);