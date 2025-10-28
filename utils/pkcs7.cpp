#include "pkcs7.h"

#include <stdexcept>

std::vector<uint8_t> pkcs7_pad(const std::vector<uint8_t>& data, size_t block_size) {
    if (block_size == 0 || block_size > 255)
        throw std::runtime_error("invalid block size");
    size_t rem = data.size() % block_size;
    uint8_t pad_len = static_cast<uint8_t>(rem == 0 ? block_size : block_size - rem);
    std::vector<uint8_t> out(data);
    out.insert(out.end(), pad_len, pad_len);
    return out;
}

std::vector<uint8_t> pkcs7_unpad(const std::vector<uint8_t>& data, size_t block_size) {
    if (block_size == 0 || data.empty() || data.size() % block_size != 0)
        throw std::runtime_error("invalid data or block size");
    uint8_t pad_len = data.back();
    if (pad_len == 0 || pad_len > block_size || pad_len > data.size())
        throw std::runtime_error("invalid padding");
    for (size_t i = data.size() - pad_len; i < data.size(); ++i)
        if (data[i] != pad_len) throw std::runtime_error("invalid padding");
    return {data.begin(), data.end() - pad_len};
}
