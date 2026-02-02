#pragma once

#include <cstdint>
#include <cstring>

inline uint32_t rotl(uint32_t x, int n);

inline uint32_t load32_le(const uint8_t *p);

void QuarterRound(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);

void ngea_process_block(const uint8_t *key_ptr, uint32_t counter, const uint8_t *nonce_ptr,
						const uint8_t *input_block_ptr, uint8_t *output_block_ptr,
						size_t block_size);

void release_memory(uint8_t *ptr);