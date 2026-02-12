#include "ciphers/aes/aes.hpp"
#include "ciphers/aes/aes_utils.hpp"
#include "utils/rand_utils.hpp"
#include <cstring>

uint64_t encrypt(const uint8_t *plaintext_ptr, const size_t size, const uint8_t *key_ptr,
				 uint8_t **ciphertext_ptr, size_t *ciphertext_size, Deleter *deleter_ptr) {
	try {
		if (!plaintext_ptr || !key_ptr || !ciphertext_ptr || !ciphertext_size || !deleter_ptr) {
			return 1;
		}

		size_t total_size = 16 + size;
		uint8_t *buffer = new uint8_t[total_size];

		uint8_t iv[16];
		random_array(iv);

		std::memcpy(buffer, iv, 16);

		ctr_process(plaintext_ptr, buffer + 16, size, key_ptr, iv);

		*ciphertext_ptr = buffer;
		*ciphertext_size = total_size;
		*deleter_ptr = release_memory;
	} catch (...) {
		return 2;
	}
	return 0;
}

uint64_t decrypt(const uint8_t *ciphertext_ptr, const size_t size, const uint8_t *key_ptr,
				 uint8_t **plaintext_ptr, size_t *plaintext_size, Deleter *deleter_ptr) {
	try {
		if (!ciphertext_ptr || !key_ptr || !plaintext_ptr || !plaintext_size || !deleter_ptr ||
			size < 16) {
			return 1;
		}

		size_t actual_size = size - 16;
		uint8_t *buffer = new uint8_t[actual_size];

		uint8_t iv[16];
		std::memcpy(iv, ciphertext_ptr, 16);

		ctr_process(ciphertext_ptr + 16, buffer, actual_size, key_ptr, iv);

		*plaintext_ptr = buffer;
		*plaintext_size = actual_size;
		*deleter_ptr = release_memory;
	} catch (...) {
		return 2;
	}
	return 0;
}
