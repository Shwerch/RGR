#pragma once
#include <vector>
#include <string>
#include <cstdint>

std::vector<uint8_t> parse_hex_to_vec(const std::string_view& hex_str);
std::string vector_to_hex_str(const std::vector<uint8_t> &bytes);