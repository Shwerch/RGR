#pragma once

#include <cstdint>
#include <string>
#include <vector>

std::vector<uint8_t> readFile(const std::string &path);
void writeFile(const std::string &path, const std::vector<uint8_t> &data);

std::vector<uint8_t> readFromStdin();
void writeToStdout(const std::vector<uint8_t> &data);
