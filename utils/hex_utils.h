#pragma once
#include <vector>
#include <string>
#include <cstdint>

std::vector<uint8_t> parse_hex_to_vec(const std::string &hex_str, size_t N);
std::vector<uint8_t> parse_hex_all(const std::string &hex_str);
std::string vector_to_hex_str(const std::vector<uint8_t> &bytes);