#include "aes.h"
#include "constants.h"
#include "../cryptum/utils/exception.h"
#include <random>
#include <cstring>

namespace aes {
    extern std::vector<uint8_t> ctr_process(const std::vector<uint8_t> &data,
                                             const uint8_t key[KEY_SIZE],
                                             const uint8_t iv[IV_SIZE]);
}

extern "C" {
    std::vector<uint8_t> encrypt(const std::vector<uint8_t> &plaintext,
                                 const std::vector<uint8_t> &key) {
        if (plaintext.empty()) {
            throw EXCEPTION("Error AES encrypt: empty plaintext");
        }
        if (key.size() != aes::KEY_SIZE) {
            throw EXCEPTION("Error AES encrypt: key must have length 32 bytes");
        }

        uint8_t iv[aes::IV_SIZE];
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        for (size_t i = 0; i < aes::IV_SIZE; ++i) {
            iv[i] = static_cast<uint8_t>(dis(gen));
        }

        std::vector<uint8_t> result;
        result.reserve(aes::IV_SIZE + plaintext.size());
        result.insert(result.end(), iv, iv + aes::IV_SIZE);

        uint8_t key_arr[aes::KEY_SIZE];
        std::memcpy(key_arr, key.data(), aes::KEY_SIZE);

        std::vector<uint8_t> ciphertext = aes::ctr_process(plaintext, key_arr, iv);
        result.insert(result.end(), ciphertext.begin(), ciphertext.end());

        return result;
    }

    std::vector<uint8_t> decrypt(const std::vector<uint8_t> &ciphertext,
                                 const std::vector<uint8_t> &key) {
        if (ciphertext.size() < aes::IV_SIZE) {
            throw EXCEPTION("Error AES decrypt: ciphertext too short");
        }
        if (key.size() != aes::KEY_SIZE) {
            throw EXCEPTION("Error AES decrypt: key must have length 32 bytes");
        }

        uint8_t iv[aes::IV_SIZE];
        std::memcpy(iv, ciphertext.data(), aes::IV_SIZE);

        const std::vector<uint8_t> encrypted_data(ciphertext.begin() + aes::IV_SIZE,
                                            ciphertext.end());

        uint8_t key_arr[aes::KEY_SIZE];
        std::memcpy(key_arr, key.data(), aes::KEY_SIZE);

        return aes::ctr_process(encrypted_data, key_arr, iv);
    }
}
