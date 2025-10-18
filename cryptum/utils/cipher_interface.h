#pragma once
#include <vector>
#include <string>
#include <cstdint>

struct CipherInterface {
    std::vector<uint8_t> (*encrypt)(const std::vector<uint8_t> &plaintext, 
                                     const std::vector<uint8_t> &key);
    std::vector<uint8_t> (*decrypt)(const std::vector<uint8_t> &ciphertext, 
                                     const std::vector<uint8_t> &key);
    size_t key_size;
    std::string name;
};