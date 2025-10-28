#include "hex_utils.h"
#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <format>

std::vector<uint8_t> parse_hex_to_vec(const std::string& hex_str, size_t N) {
    if (hex_str.length() < N * 2)
        throw std::runtime_error("Hex string too short");
    if (hex_str.size() % 2 != 0)
        throw std::runtime_error("Hex string length must be even");
    std::vector<uint8_t> result;
    result.reserve(N);
    for (size_t i = 2; i < N * 2; i += 2) {
        result.push_back(std::stoul(hex_str.substr(i, 2), nullptr, 16));
    }
    return result;
}

std::vector<uint8_t> parse_hex_all(const std::string &hex_str) {
    return parse_hex_to_vec(hex_str, hex_str.size() / 2);
}

std::string vector_to_hex_str(const std::vector<uint8_t>& bytes) {
    std::string result;
    result.reserve(bytes.size() * 2);
    for (uint8_t b : bytes) {
        result += std::format("{:02x}", b);
    }
    return result;
}
