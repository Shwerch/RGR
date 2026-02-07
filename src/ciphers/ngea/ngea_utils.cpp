#include "ciphers/ngea/ngea_utils.hpp"

#include <bit>
#include <cstdint>

void cryptify(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
	*a += *b;
	*d ^= *a;
	*d = std::rotl(*d, 23);
	*c += *d;
	*b ^= *c;
	*b = std::rotl(*b, 19);
	*a += *b;
	*d ^= *a;
	*d = std::rotl(*d, 13);
	*c += *d;
	*b ^= *c;
	*b = std::rotl(*b, 7);
}

void cryptify_helper(uint32_t *working_state, uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
	cryptify(&working_state[a], &working_state[b], &working_state[c], &working_state[d]);
}

void ngea_process_block(const uint8_t *key_ptr, uint32_t counter, const uint8_t *nonce_ptr,
						const uint8_t *input_block_ptr, uint8_t *output_block_ptr,
						size_t block_size) {
	uint32_t initial_state[16];
	uint32_t working_state[16];

	initial_state[0] = 0x6e6f654e;
	initial_state[1] = 0x656e6547;
	initial_state[2] = 0x43736973;
	initial_state[3] = 0x74707972;

	for (int i = 0; i < 8; ++i) {
		initial_state[4 + i] = load32_le(key_ptr + i * 4);
	}
	initial_state[12] = counter;
	initial_state[13] = load32_le(nonce_ptr);
	initial_state[14] = load32_le(nonce_ptr + 4);
	initial_state[15] = load32_le(nonce_ptr + 8);

	memcpy(working_state, initial_state, sizeof(initial_state));

	for (int i = 0; i < 10; ++i) {
		cryptify_helper(working_state, 0, 4, 8, 12);
		cryptify_helper(working_state, 1, 5, 9, 13);
		cryptify_helper(working_state, 2, 6, 10, 14);
		cryptify_helper(working_state, 3, 7, 11, 15);

		cryptify_helper(working_state, 0, 5, 10, 15);
		cryptify_helper(working_state, 1, 6, 11, 12);
		cryptify_helper(working_state, 2, 7, 8, 13);
		cryptify_helper(working_state, 3, 4, 9, 14);
	}

	uint8_t keystream_bytes[64];

	for (int i = 0; i < 16; ++i) {
		uint32_t z = working_state[i] + initial_state[i];
		store32_le(&keystream_bytes[i * 4], z);
	}

	for (size_t i = 0; i < block_size; ++i) {
		output_block_ptr[i] = input_block_ptr[i] ^ keystream_bytes[i];
	}
}

void release_memory(uint8_t *ptr) { delete[] ptr; }