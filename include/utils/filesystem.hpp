#pragma once

#include <cstdint>
#include <string>
#include <vector>

std::vector<uint8_t> read_file(const std::string &path);
void write_file(const std::string &path, const std::vector<uint8_t> &data);

std::vector<uint8_t> read_from_stdin();
void write_to_stdout(const std::vector<uint8_t> &data);
