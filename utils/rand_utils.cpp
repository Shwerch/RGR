#include "rand_utils.hpp"

#include <random>

std::vector<uint8_t> random_vector(size_t num_bytes) {
	std::vector<uint8_t> random_bytes(num_bytes);
	std::random_device rd;
	for (size_t i = 0; i < num_bytes; i++) {
		random_bytes.at(i) = static_cast<uint8_t>(rd() & 0xFF);
	}
	return random_bytes;
}

template <size_t N> void random_array(uint8_t (&random_bytes)[N]) {
	std::random_device rd;
	for (size_t i = 0; i < N; i++) {
		random_bytes[i] = static_cast<uint8_t>(rd() & 0xFF);
	}
}

template void random_array(uint8_t (&)[8]);
template void random_array(uint8_t (&)[12]);
template void random_array(uint8_t (&)[16]);
template void random_array(uint8_t (&)[32]);
