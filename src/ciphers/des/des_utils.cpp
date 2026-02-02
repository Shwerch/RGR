#include "ciphers/des/des_utils.hpp"

uint64_t bytes_to_uint64(const uint8_t *data) {
	uint64_t val = 0;
	for (int i = 0; i < 8; ++i) {
		val = (val << 8) | data[i];
	}
	return val;
}

void uint64_to_bytes(uint64_t val, uint8_t *data) {
	for (int i = 7; i >= 0; --i) {
		data[i] = static_cast<uint8_t>(val & 0xFF);
		val >>= 8;
	}
}

uint64_t permute(uint64_t input, const uint8_t *table, int n) {
	uint64_t output = 0;
	for (int i = 0; i < n; ++i) {
		if ((input >> (64 - table[i])) & 1) {
			output |= (1ULL << (n - 1 - i));
		}
	}
	return output;
}

uint64_t permute_56(uint64_t input, const uint8_t *table) {
	uint64_t output = 0;
	for (int i = 0; i < 56; ++i) {
		if ((input >> (64 - table[i])) & 1) {
			output |= (1ULL << (55 - i));
		}
	}
	return output;
}

uint64_t permute_48(uint64_t input, const uint8_t *table) {
	uint64_t output = 0;
	for (int i = 0; i < 48; ++i) {
		if ((input >> (56 - table[i])) & 1) {
			output |= (1ULL << (47 - i));
		}
	}
	return output;
}

void generate_subkeys(uint64_t key, uint64_t *subkeys) {
	uint64_t permuted_key = permute_56(key, PC1);
	uint32_t c = (uint32_t)((permuted_key >> 28) & 0xFFFFFFF);
	uint32_t d = (uint32_t)(permuted_key & 0xFFFFFFF);

	for (int i = 0; i < 16; ++i) {
		uint32_t shift = SHIFTS[i];
		c = ((c << shift) | (c >> (28 - shift))) & 0xFFFFFFF;
		d = ((d << shift) | (d >> (28 - shift))) & 0xFFFFFFF;
		uint64_t cd = ((uint64_t)c << 28) | d;
		subkeys[i] = permute_48(cd, PC2);
	}
}

uint32_t f_function(uint32_t r, uint64_t k) {
	uint64_t extended_r = 0;
	for (int i = 0; i < 48; ++i) {
		if ((r >> (32 - E[i])) & 1) {
			extended_r |= (1ULL << (47 - i));
		}
	}
	uint64_t xor_val = extended_r ^ k;
	uint32_t output = 0;
	for (int i = 0; i < 8; ++i) {
		int row = ((xor_val >> (47 - i * 6)) & 1) * 2 + ((xor_val >> (42 - i * 6)) & 1);
		int col = (xor_val >> (43 - i * 6)) & 0xF;
		output |= ((uint32_t)SBOX[i][row][col] << (28 - i * 4));
	}
	uint32_t permuted_output = 0;
	for (int i = 0; i < 32; ++i) {
		if ((output >> (32 - P[i])) & 1) {
			permuted_output |= (1U << (31 - i));
		}
	}
	return permuted_output;
}

uint64_t process_block(uint64_t block, const uint64_t *subkeys, bool decrypt) {
	block = permute(block, IP, 64);
	uint32_t left = (uint32_t)(block >> 32);
	uint32_t right = (uint32_t)(block & 0xFFFFFFFF);

	for (int i = 0; i < 16; ++i) {
		uint32_t prev_left = left;
		left = right;
		uint64_t key = decrypt ? subkeys[15 - i] : subkeys[i];
		right = prev_left ^ f_function(right, key);
	}

	uint64_t final_block = ((uint64_t)right << 32) | left;
	return permute(final_block, FP, 64);
}

void release_memory(uint8_t *ptr) { delete[] ptr; }