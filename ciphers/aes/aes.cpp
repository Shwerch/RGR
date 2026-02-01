#include "aes.hpp"
#include "rand_utils.hpp"

#include <cstring>

const uint8_t sbox[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

const uint8_t rcon[11] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

struct AesContext {
	uint8_t round_keys[240];
};

void key_expansion(const uint8_t *key, AesContext &ctx) {
	unsigned i, j, k;
	uint8_t tempa[4];

	for (i = 0; i < 32; ++i) {
		ctx.round_keys[i] = key[i];
	}

	for (i = 8; i < 60; ++i) {
		{
			k = (i - 1) * 4;
			tempa[0] = ctx.round_keys[k + 0];
			tempa[1] = ctx.round_keys[k + 1];
			tempa[2] = ctx.round_keys[k + 2];
			tempa[3] = ctx.round_keys[k + 3];
		}

		if (i % 8 == 0) {
			{
				const uint8_t u8tmp = tempa[0];
				tempa[0] = tempa[1];
				tempa[1] = tempa[2];
				tempa[2] = tempa[3];
				tempa[3] = u8tmp;
			}
			{
				tempa[0] = sbox[tempa[0]];
				tempa[1] = sbox[tempa[1]];
				tempa[2] = sbox[tempa[2]];
				tempa[3] = sbox[tempa[3]];
			}
			tempa[0] = tempa[0] ^ rcon[i / 8];
		} else if (i % 8 == 4) {
			{
				tempa[0] = sbox[tempa[0]];
				tempa[1] = sbox[tempa[1]];
				tempa[2] = sbox[tempa[2]];
				tempa[3] = sbox[tempa[3]];
			}
		}

		j = i * 4;
		k = (i - 8) * 4;
		ctx.round_keys[j + 0] = ctx.round_keys[k + 0] ^ tempa[0];
		ctx.round_keys[j + 1] = ctx.round_keys[k + 1] ^ tempa[1];
		ctx.round_keys[j + 2] = ctx.round_keys[k + 2] ^ tempa[2];
		ctx.round_keys[j + 3] = ctx.round_keys[k + 3] ^ tempa[3];
	}
}

void add_round_key(uint8_t round, uint8_t *state, const uint8_t *round_keys) {
	for (uint8_t i = 0; i < 16; ++i) {
		state[i] ^= round_keys[round * 16 + i];
	}
}

void sub_bytes(uint8_t *state) {
	for (uint8_t i = 0; i < 16; ++i) {
		state[i] = sbox[state[i]];
	}
}

void shift_rows(uint8_t *state) {
	uint8_t temp[4];

	temp[0] = state[1];
	state[1] = state[5];
	state[5] = state[9];
	state[9] = state[13];
	state[13] = temp[0];

	temp[0] = state[2];
	temp[1] = state[6];
	state[2] = state[10];
	state[6] = state[14];
	state[10] = temp[0];
	state[14] = temp[1];

	temp[0] = state[15];
	state[15] = state[11];
	state[11] = state[7];
	state[7] = state[3];
	state[3] = temp[0];
}

uint8_t galois_mul(uint8_t a, uint8_t b) {
	uint8_t p = 0;
	for (uint8_t counter = 0; counter < 8; counter++) {
		if ((b & 1) != 0) {
			p ^= a;
		}
		bool hi_bit_set = (a & 0x80) != 0;
		a <<= 1;
		if (hi_bit_set) {
			a ^= 0x1b;
		}
		b >>= 1;
	}
	return p;
}

void mix_columns(uint8_t *state) {
	uint8_t column[4];
	uint8_t cpy[4];
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			column[j] = state[i * 4 + j];
			cpy[j] = column[j];
		}
		column[0] = galois_mul(cpy[0], 2) ^ galois_mul(cpy[1], 3) ^ cpy[2] ^ cpy[3];
		column[1] = cpy[0] ^ galois_mul(cpy[1], 2) ^ galois_mul(cpy[2], 3) ^ cpy[3];
		column[2] = cpy[0] ^ cpy[1] ^ galois_mul(cpy[2], 2) ^ galois_mul(cpy[3], 3);
		column[3] = galois_mul(cpy[0], 3) ^ cpy[1] ^ cpy[2] ^ galois_mul(cpy[3], 2);
		for (int j = 0; j < 4; ++j) {
			state[i * 4 + j] = column[j];
		}
	}
}

void aes_cipher(const uint8_t *input, uint8_t *output, const AesContext &ctx) {
	uint8_t state[16];
	for (int i = 0; i < 16; ++i) {
		state[i] = input[i];
	}

	add_round_key(0, state, ctx.round_keys);

	for (uint8_t round = 1; round < 14; ++round) {
		sub_bytes(state);
		shift_rows(state);
		mix_columns(state);
		add_round_key(round, state, ctx.round_keys);
	}

	sub_bytes(state);
	shift_rows(state);
	add_round_key(14, state, ctx.round_keys);

	for (int i = 0; i < 16; ++i) {
		output[i] = state[i];
	}
}

void increment_iv(uint8_t *iv) {
	for (int i = 15; i >= 0; --i) {
		iv[i]++;
		if (iv[i] != 0) {
			break;
		}
	}
}

void ctr_process(const uint8_t *in, uint8_t *out, size_t len, const uint8_t *key, uint8_t *iv) {
	AesContext ctx;
	key_expansion(key, ctx);

	uint8_t keystream[16];
	uint8_t curr_iv[16];
	std::memcpy(curr_iv, iv, 16);

	size_t i = 0;
	while (i < len) {
		aes_cipher(curr_iv, keystream, ctx);
		increment_iv(curr_iv);

		size_t block_len = (len - i) < 16 ? (len - i) : 16;
		for (size_t j = 0; j < block_len; ++j) {
			out[i + j] = in[i + j] ^ keystream[j];
		}
		i += block_len;
	}
}

void release_memory(uint8_t *ptr) { delete[] ptr; }

uint64_t encrypt(const uint8_t *plaintext_ptr, const size_t size, const uint8_t *key_ptr,
				 uint8_t **ciphertext_ptr, size_t *ciphertext_size, Deleter *deleter_ptr) {
	if (!plaintext_ptr || !key_ptr || !ciphertext_ptr || !ciphertext_size || !deleter_ptr) {
		return 1;
	}

	try {
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
	if (!ciphertext_ptr || !key_ptr || !plaintext_ptr || !plaintext_size || !deleter_ptr ||
		size < 16) {
		return 1;
	}

	try {
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
