#pragma once

#include <cstdint>
#include <cstring>

void cryptify(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);

void cryptify_helper(uint32_t (*working_state)[16], uint8_t a, uint8_t b, uint8_t c, uint8_t d);

void ngea_process_block(const uint8_t *key_ptr, uint32_t counter, const uint8_t *nonce_ptr,
						const uint8_t *input_block_ptr, uint8_t *output_block_ptr,
						size_t block_size);

void release_memory(uint8_t *ptr);