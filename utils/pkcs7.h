#pragma once

#include <cstdint>
#include <vector>

std::vector<uint8_t> pkcs7_pad(const std::vector<uint8_t>& data, size_t block_size);
std::vector<uint8_t> pkcs7_unpad(const std::vector<uint8_t>& data, size_t block_size);
