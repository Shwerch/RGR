#include "ciphers/ngea/ngea.hpp"
#include "utils/rand_utils.hpp"

#include <cstring>
#include <new>

static inline uint32_t rotl(uint32_t x, int n) { return (x << n) | (x >> (32 - n)); }

static inline uint32_t load32_le(const uint8_t *p) {
	return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

static void QuarterRound(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
	*a += *b;
	*d ^= *a;
	*d = rotl(*d, 23);
	*c += *d;
	*b ^= *c;
	*b = rotl(*b, 19);
	*a += *b;
	*d ^= *a;
	*d = rotl(*d, 13);
	*c += *d;
	*b ^= *c;
	*b = rotl(*b, 7);
}

static void ngea_process_block(const uint8_t *key_ptr, uint32_t counter, const uint8_t *nonce_ptr,
							   const uint8_t *input_block_ptr, uint8_t *output_block_ptr,
							   size_t block_size) {
	uint32_t initial_state[16];
	uint32_t working_state[16];

	initial_state[0] = 0x61707865;
	initial_state[1] = 0x3320646e;
	initial_state[2] = 0x79622d32;
	initial_state[3] = 0x6b206574;

	for (int i = 0; i < 8; ++i) {
		initial_state[4 + i] = load32_le(key_ptr + i * 4);
	}
	initial_state[12] = counter;
	initial_state[13] = load32_le(nonce_ptr);
	initial_state[14] = load32_le(nonce_ptr + 4);
	initial_state[15] = load32_le(nonce_ptr + 8);

	memcpy(working_state, initial_state, sizeof(initial_state));

	for (int i = 0; i < 10; ++i) {
		QuarterRound(&working_state[0], &working_state[4], &working_state[8], &working_state[12]);
		QuarterRound(&working_state[1], &working_state[5], &working_state[9], &working_state[13]);
		QuarterRound(&working_state[2], &working_state[6], &working_state[10], &working_state[14]);
		QuarterRound(&working_state[3], &working_state[7], &working_state[11], &working_state[15]);

		QuarterRound(&working_state[0], &working_state[5], &working_state[10], &working_state[15]);
		QuarterRound(&working_state[1], &working_state[6], &working_state[11], &working_state[12]);
		QuarterRound(&working_state[2], &working_state[7], &working_state[8], &working_state[13]);
		QuarterRound(&working_state[3], &working_state[4], &working_state[9], &working_state[14]);
	}

	uint8_t keystream_block[64];
	uint32_t *keystream_block_32 = reinterpret_cast<uint32_t *>(keystream_block);

	for (int i = 0; i < 16; ++i) {
		keystream_block_32[i] = working_state[i] + initial_state[i];
	}

	for (size_t i = 0; i < block_size; ++i) {
		output_block_ptr[i] = input_block_ptr[i] ^ keystream_block[i];
	}
}

void release_memory(uint8_t *ptr) { delete[] ptr; }

uint64_t encrypt(const uint8_t *plaintext_ptr, const size_t size, const uint8_t *key_ptr,
				 uint8_t **ciphertext_ptr, size_t *ciphertext_size, Deleter *deleter_ptr) {
	try {
		if (!plaintext_ptr || !key_ptr || !ciphertext_ptr || !ciphertext_size || !deleter_ptr) {
			return 1;
		}

		constexpr size_t nonce_size = 12;
		constexpr size_t block_size = 64;

		*ciphertext_size = nonce_size + size;
		*ciphertext_ptr = new (std::nothrow) uint8_t[*ciphertext_size];
		*deleter_ptr = release_memory;

		uint8_t nonce[nonce_size];
		random_array(nonce);
		memcpy(*ciphertext_ptr, nonce, nonce_size);

		uint8_t *out_ptr = *ciphertext_ptr + nonce_size;
		const uint8_t *in_ptr = plaintext_ptr;
		size_t remaining_size = size;
		uint32_t counter = 0;

		while (remaining_size >= block_size) {
			ngea_process_block(key_ptr, counter, nonce, in_ptr, out_ptr, block_size);
			in_ptr += block_size;
			out_ptr += block_size;
			remaining_size -= block_size;
			counter++;
		}

		if (remaining_size > 0) {
			ngea_process_block(key_ptr, counter, nonce, in_ptr, out_ptr, remaining_size);
		}
	} catch (...) {
		return 2;
	}

	return 0;
}

uint64_t decrypt(const uint8_t *ciphertext_ptr, const size_t size, const uint8_t *key_ptr,
				 uint8_t **plaintext_ptr, size_t *plaintext_size, Deleter *deleter_ptr) {
	try {
		constexpr size_t nonce_size = 12;
		constexpr size_t block_size = 64;

		if (!ciphertext_ptr || size < nonce_size || !key_ptr || !plaintext_ptr || !plaintext_size ||
			!deleter_ptr) {
			return 1;
		}

		*plaintext_size = size - nonce_size;
		*plaintext_ptr = new (std::nothrow) uint8_t[*plaintext_size];
		*deleter_ptr = release_memory;

		const uint8_t *nonce = ciphertext_ptr;

		uint8_t *out_ptr = *plaintext_ptr;
		const uint8_t *in_ptr = ciphertext_ptr + nonce_size;
		size_t remaining_size = *plaintext_size;
		uint32_t counter = 0;

		while (remaining_size >= block_size) {
			ngea_process_block(key_ptr, counter, nonce, in_ptr, out_ptr, block_size);
			in_ptr += block_size;
			out_ptr += block_size;
			remaining_size -= block_size;
			counter++;
		}

		if (remaining_size > 0) {
			ngea_process_block(key_ptr, counter, nonce, in_ptr, out_ptr, remaining_size);
		}
	} catch (...) {
		return 2;
	}

	return 0;
}