#include "ciphers/ngea/ngea.hpp"
#include "ciphers/ngea/ngea_utils.hpp"
#include "utils/rand_utils.hpp"

uint64_t encrypt(const uint8_t *plaintext_ptr, const size_t size, const uint8_t *key_ptr,
				 uint8_t **ciphertext_ptr, size_t *ciphertext_size, Deleter *deleter_ptr) {
	try {
		constexpr size_t nonce_size = 12;
		constexpr size_t block_size = 64;

		if (!plaintext_ptr || !key_ptr || !ciphertext_ptr || !ciphertext_size || !deleter_ptr) {
			return 1;
		}

		*ciphertext_size = nonce_size + size;
		*ciphertext_ptr = new uint8_t[*ciphertext_size];
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
		*plaintext_ptr = new uint8_t[*plaintext_size];
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