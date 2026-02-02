#include "ciphers/aes/aes_utils.hpp"

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