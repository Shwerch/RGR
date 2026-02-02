#pragma once

#include <cstdint>
#include <vector>

std::vector<uint8_t> random_vector(size_t num_bytes);

template <size_t N> void random_array(uint8_t (&random_bytes)[N]);
