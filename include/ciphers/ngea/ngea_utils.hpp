#pragma once

#include <cstdint>
#include <cstring>

inline uint32_t load32_le(const uint8_t *p) {
	return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

inline void store32_le(uint8_t *p, uint32_t x) {
	p[0] = x & 0xff;
	p[1] = (x >> 8) & 0xff;
	p[2] = (x >> 16) & 0xff;
	p[3] = (x >> 24) & 0xff;
}

void cryptify(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);

void cryptify_helper(uint32_t *working_state, uint8_t a, uint8_t b, uint8_t c, uint8_t d);

void ngea_process_block(const uint8_t *key_ptr, uint32_t counter, const uint8_t *nonce_ptr,
						const uint8_t *input_block_ptr, uint8_t *output_block_ptr,
						size_t block_size);

void release_memory(uint8_t *ptr);