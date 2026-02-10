#include "ciphers/des/des.hpp"
#include "ciphers/des/des_utils.hpp"

#include <cstring>

uint64_t encrypt(const uint8_t *plaintext_ptr, const size_t size, const uint8_t *key_ptr,
				 uint8_t **ciphertext_ptr, size_t *ciphertext_size, Deleter *deleter_ptr, uint8_t *fixed_iv) {
	try {
		if (!plaintext_ptr || !key_ptr || !ciphertext_ptr || !ciphertext_size || !deleter_ptr) {
			return 1;
		}

		size_t padded_len = (size / 8 + 1) * 8;
		uint8_t *output = new uint8_t[padded_len];

		uint64_t subkeys[16];
		uint64_t key_val = bytes_to_uint64(key_ptr);
		generate_subkeys(key_val, subkeys);

		for (size_t i = 0; i < size / 8; ++i) {
			uint64_t block = bytes_to_uint64(plaintext_ptr + i * 8);
			uint64_t encrypted_block = process_block(block, subkeys, false);
			uint64_to_bytes(encrypted_block, output + i * 8);
		}

		uint8_t last_block[8];
		size_t remaining = size % 8;
		std::memcpy(last_block, plaintext_ptr + (size / 8) * 8, remaining);
		uint8_t padding_val = static_cast<uint8_t>(8 - remaining);
		for (size_t i = remaining; i < 8; ++i) {
			last_block[i] = padding_val;
		}

		uint64_t final_block_val = bytes_to_uint64(last_block);
		uint64_t encrypted_final = process_block(final_block_val, subkeys, false);
		uint64_to_bytes(encrypted_final, output + (size / 8) * 8);

		*ciphertext_ptr = output;
		*ciphertext_size = padded_len;
		*deleter_ptr = release_memory;
	} catch (...) {
		return 2;
	}
	return 0;
}

uint64_t decrypt(const uint8_t *ciphertext_ptr, const size_t size, const uint8_t *key_ptr,
				 uint8_t **plaintext_ptr, size_t *plaintext_size, Deleter *deleter_ptr, uint8_t *fixed_iv) {
	try {
		if (!ciphertext_ptr || !key_ptr || !plaintext_ptr || !plaintext_size || !deleter_ptr) {
			return 1;
		}
		if (size == 0 || size % 8 != 0) {
			return 1;
		}
		uint8_t *output = new uint8_t[size];

		uint64_t subkeys[16];
		uint64_t key_val = bytes_to_uint64(key_ptr);
		generate_subkeys(key_val, subkeys);

		for (size_t i = 0; i < size / 8; ++i) {
			uint64_t block = bytes_to_uint64(ciphertext_ptr + i * 8);
			uint64_t decrypted_block = process_block(block, subkeys, true);
			uint64_to_bytes(decrypted_block, output + i * 8);
		}

		uint8_t padding_val = output[size - 1];
		if (padding_val == 0 || padding_val > 8) {
			delete[] output;
			return 2;
		}

		for (size_t i = 0; i < padding_val; ++i) {
			if (output[size - 1 - i] != padding_val) {
				delete[] output;
				return 2;
			}
		}

		size_t real_size = size - padding_val;
		uint8_t *final_output = new uint8_t[real_size];
		std::memcpy(final_output, output, real_size);
		delete[] output;

		*plaintext_ptr = final_output;
		*plaintext_size = real_size;
		*deleter_ptr = release_memory;
	} catch (...) {
		return 2;
	}
	return 0;
}
