#pragma once
#include <vector>
#include <string>
#include <cstdint>

std::vector<uint8_t> read_bytes(const std::string &path, size_t size);
std::vector<uint8_t> read_all_bytes(const std::string &path);
void write_bytes(const std::string &path, const std::vector<uint8_t> &data);