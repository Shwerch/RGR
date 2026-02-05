#pragma once

#include "parser.hpp"

#include <cstdint>
#include <vector>

std::vector<uint8_t> get_key(const Arguments &args);
void save_key(const Arguments &args, const std::vector<uint8_t> &key);

std::vector<uint8_t> get_input_data(const Arguments &args);
void write_output_data(const Arguments &args, const std::vector<uint8_t> &data);
