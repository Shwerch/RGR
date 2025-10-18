#pragma once
#include <vector>
#include <cstdint>

std::vector<uint8_t> random_vector(size_t num_bytes);

template<size_t N>
void random_array(uint8_t (&arr)[N]);