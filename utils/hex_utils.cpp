#include "hex_utils.h"
#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <format>
#include <algorithm>

constexpr bool is_hex_digit(char c) noexcept {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

std::vector<uint8_t> parse_hex_to_vec(const std::string_view& hex_str) {
    std::string_view data = hex_str;
    if (hex_str.starts_with("0x") || hex_str.starts_with("0X")) {
        data = hex_str.substr(2);
    }
    if (data.length() < 2)
        throw std::runtime_error("hex string too short");
    if (data.size() % 2 != 0)
        throw std::runtime_error("hex string length must be even");
    if (!std::ranges::all_of(data, is_hex_digit))
        throw std::runtime_error("invalid hex characters");

    const std::size_t num_bytes = data.size() / 2;
    std::vector<uint8_t> result;
    result.reserve(num_bytes);
    for (std::size_t i = 0; i < num_bytes; ++i) {
        const char* start_ptr = data.data() + i * 2;
        unsigned int byte_value;

        auto [ptr, ec] = std::from_chars(start_ptr, start_ptr + 2, byte_value, 16);
        if (ec != std::errc() || ptr != start_ptr + 2)
            throw std::runtime_error("invalid hex characters in std::from_chars parsing");

        result.push_back(static_cast<uint8_t>(byte_value));
    }
    return result;
}

std::string vector_to_hex_str(const std::vector<uint8_t>& bytes) {
    std::string result;
    result.reserve(bytes.size() * 2);
    for (uint8_t b : bytes) {
        result += std::format("{:02x}", b);
    }
    return result;
}
