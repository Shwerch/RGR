#pragma once

#include "parser.hpp"

#include <cstdint>
#include <vector>

std::vector<uint8_t> getKey(const Arguments &args);
void saveKey(const Arguments &args, const std::vector<uint8_t> &key);

std::vector<uint8_t> getInputData(const Arguments &args);
void writeOutputData(const Arguments &args, const std::vector<uint8_t> &data);
